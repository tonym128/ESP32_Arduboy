/*
 * System.cpp : システムライブラリ
 */

/*
 * 参照ファイル
 */
#include "System.h"


/*
 * 変数の定義
 */

// Arduboy ライブラリ
static Arduboy2 arduboy;

// キー入力
static struct Input input;

// ビデオ
static struct Video video;

// サウンド
static const uint8_t soundOctaves[] PROGMEM = {
  0x00, // O0
  0x0c, // O1
  0x18, // O2
  0x24, // O3
  0x30, // O4
  0x3c, // O5
  0x48, // O6
  0x54, // O7
  0x60, // O8
  0x6c, // O9
};
static const uint8_t soundLengths[] PROGMEM = {
  0x01, // L0: 32分音符
  0x02, // L1: 16分音符
  0x03, // L2: 付点16分音符
  0x04, // L3: 8分音符
  0x06, // L4: 付点8分音符
  0x08, // L5: 4分音符
  0x0c, // L6: 付点4分音符
  0x10, // L7: 2分音符
  0x18, // L8: 付点2分音符
  0x20, // L9: 全音符
};
static const uint8_t soundNotes[] PROGMEM = {
  0x09, // 'A'
  0x0b, // 'B'
  0x00, // 'C'
  0x02, // 'D'
  0x04, // 'E'
  0x05, // 'F'
  0x07, // 'G'
};
static const uint16_t soundFrequencys[] PROGMEM = {
//    C     C#      D     D#      E      F     F#      G     G#      A     A#      B
     16,    17,    18,    19,    21,    22,    23,    24,    26,    28,    29,    31, // O0
     33,    35,    37,    39,    41,    44,    46,    49,    52,    55,    58,    62, // O1
     65,    69,    73,    78,    82,    87,    92,    98,   104,   110,   117,   123, // O2
    131,   139,   147,   156,   165,   175,   185,   196,   208,   220,   233,   247, // O3
    262,   277,   294,   311,   330,   349,   370,   392,   415,   440,   466,   494, // O4
    523,   554,   587,   622,   659,   698,   740,   784,   831,   880,   932,   988, // O5
   1047,  1109,  1175,  1245,  1319,  1397,  1480,  1568,  1661,  1760,  1865,  1976, // O6
   2093,  2217,  2349,  2489,  2637,  2794,  2960,  3136,  3322,  3520,  3729,  3951, // O7
   4186,  4435,  4699,  4978,  5274,  5588,  5920,  6272,  6645,  7040,  7459,  7902, // O8
   8372,  8870,  9397,  9956, 10548, 11175, 11840, 12544, 13290, 14080, 14917, 15804, // O9
};
static struct Sound sound;

// 乱数
static uint8_t randomNumber;

// シリアル
#if SYSTEM_DEBUG
static uint8_t serialIndex;
static uint8_t serialString[SYSTEM_SERIAL_STRING_LENGTH];
#endif

// 画面キャプチャ
#if SYSTEM_DEBUG
static const char captureString[] PROGMEM = "capture";
static const char captureHeader[] PROGMEM = 
  "begin 644 capture.bmp\n"
  "M0DT^!````````#X````H````@````$`````!``$`````````````````````\n"
  "M`````````````````/___P";
static const char captureFooter[] PROGMEM = 
  "\n`\nend\n";
#endif


/*
 * 内部関数の宣言
 */
static void SystemInitializeInput(void);
static void SystemUpdateInput(void);
static void SystemInitializeVideo(void);
static void SystemUpdateVideo(void);
static void SystemInitializeSound(void);
static void SystemUpdateSound(void);
static void SystemSetSoundDefault(void);
#if SYSTEM_DEBUG
static void SystemInitializeDebug(void);
static void SystemUpdateDebug(void);
static bool SystemIsSerialString(const char *string);
static void SystemCaptureScreen(void);
#endif


/*
 * システムを初期化する
 */
void SystemInitialize(uint8_t frameRate)
{
  // Arduboy ライブラリの初期化
#if SYSTEM_DEBUG
  arduboy.boot();
  arduboy.blank();
  arduboy.flashlight();
  arduboy.audio.begin();
#else
  arduboy.begin();
#endif

  // FPS の設定
  arduboy.setFrameRate(frameRate);

  // キー入力の初期化
  SystemInitializeInput();

  // ビデオの初期化
  SystemInitializeVideo();

  // サウンドの初期化
  SystemInitializeSound();

  // 乱数の初期化
  randomNumber = 0x13;

  // デバッグの初期化
#if SYSTEM_DEBUG
  SystemInitializeDebug();
#endif
}

/*
 * システムの更新する
 */
bool SystemUpdate(void)
{
  // フレームの更新
  if (!arduboy.nextFrame()) {
    return false;
  }

  // キー入力の更新
  SystemUpdateInput();

  // ビデオの更新
  SystemUpdateVideo();

  // サウンドの更新
  SystemUpdateSound();

  // デバッグの更新
#if SYSTEM_DEBUG
  SystemUpdateDebug();
#endif

  // 終了
  return true;
}

/*
 * Arduboy を取得する
 */
Arduboy2 &SystemGetArduboy(void)
{
  return arduboy;
}

/*
 * キー入力を初期化する
 */
static void SystemInitializeInput(void)
{
  input.edge = 0x00;
  input.push = 0x00;
}

/*
 * キー入力を更新する
 */
static void SystemUpdateInput(void)
{
  uint8_t buttons = arduboy.buttonsState();
  input.edge = (buttons ^ input.push) & buttons;
  input.push = buttons;
}

/*
 * ボタンが押された瞬間かを取得する
 */
bool SystemIsInputEdge(uint8_t button)
{
  return (input.edge & button) != 0x00 ? true : false;
}

/*
 * ボタンが押されているかを取得する
 */
bool SystemIsInputPush(uint8_t button)
{
  return (input.push & button) != 0x00 ? true : false;
}

/*
 * ビデオを初期化する
 */
static void SystemInitializeVideo(void)
{
  video.buffer = arduboy.getBuffer();
}

/*
 * ビデオを更新する
 */
static void SystemUpdateVideo(void)
{
}

/*
 * ビデオバッファを取得する
 */
uint8_t *SystemGetVideoBuffer(void)
{
  return video.buffer;
}

/*
 * ビデオをクリアする
 */
void SystemClearVideo(uint8_t color)
{
  arduboy.fillScreen(color);
}

/*
 * ビデオを表示する
 */
void SystemDisplayVideo(void)
{
  arduboy.display();
}

/*
 * パターンを描画する
 */
void SystemDrawPattern(const uint8_t *pattern, int8_t x, int8_t y, uint8_t width, int8_t flip)
{
  // 表示領域の判定
  if (x + width > 0x00 && y > -0x08 && y < HEIGHT) {

    // パターンの反転
    if (flip < 0x00) {
      pattern = pattern + width - 0x01;
    }

    // 水平方向のクリッピング
    if (x < 0x00) {
      if (flip < 0x00) {
        pattern = pattern + x;
      } else {
        pattern = pattern - x;
      }
      width = width + x;
      x = 0x00;
    }
    if (x + width >= WIDTH) {
      width = WIDTH - x;
    }

    // 上方向にクリッピングしての描画
    if (y < 0x00) {
      uint8_t *b = video.buffer + x;
      int8_t s = -y;
      while (width-- > 0x00) {
        *b++ |= pgm_read_byte(pattern) >> s;
        pattern = pattern + flip;
      }

    // 下方向にクリッピングしての描画
    } else if (y > HEIGHT - 0x08) {
      uint8_t *b = video.buffer + ((HEIGHT >> 3) - 0x01) * WIDTH + x;
      int8_t s = 0x08 - (HEIGHT - y);
      while (width-- > 0x00) {
        *b++ |= pgm_read_byte(pattern) << s;
        pattern = pattern + flip;
      }

    // 垂直方向にクリッピングなしの描画
    } else {
      int8_t s_0 = y & 0x07;
      int8_t s_1 = 0x08 - s_0;
      if (s_0 == 0x00) {
        uint8_t *b = video.buffer + (y >> 3) * WIDTH + x;
        while (width--) {
          *b++ |= pgm_read_byte(pattern);
          pattern = pattern + flip;
        }
      } else {
        uint8_t *b_0 = video.buffer + (y >> 3) * WIDTH + x;
        uint8_t *b_1 = b_0 + WIDTH;
        while (width--) {
          uint8_t p = pgm_read_byte(pattern);
          *b_0++ |= p << s_0;
          *b_1++ |= p >> s_1;
          pattern = pattern + flip;
        }
      }
    }
  }
}

/*
 * パターンで切り抜く
 */
void SystemClipPattern(const uint8_t *pattern, int8_t x, int8_t y, uint8_t width, int8_t flip)
{
  // 表示領域の判定
  if (x + width > 0x00 && y > -0x08 && y < HEIGHT) {

    // パターンの反転
    if (flip < 0x00) {
      pattern = pattern + width - 0x01;
    }

    // 水平方向のクリッピング
    if (x < 0x00) {
      if (flip < 0x00) {
        pattern = pattern + x;
      } else {
        pattern = pattern - x;
      }
      width = width + x;
      x = 0x00;
    }
    if (x + width >= WIDTH) {
      width = WIDTH - x;
    }

    // 上方向にクリッピングしての描画
    if (y < 0x00) {
      uint8_t *b = video.buffer + x;
      int8_t s = -y;
      while (width-- > 0x00) {
        *b++ &= ~(pgm_read_byte(pattern) >> s);
        pattern = pattern + flip;
      }

    // 下方向にクリッピングしての描画
    } else if (y > HEIGHT - 0x08) {
      uint8_t *b = video.buffer + ((HEIGHT >> 3) - 0x01) * WIDTH + x;
      int8_t s = 0x08 - (HEIGHT - y);
      while (width-- > 0x00) {
        *b++ &= ~(pgm_read_byte(pattern) << s);
        pattern = pattern + flip;
      }

    // 垂直方向にクリッピングなしの描画
    } else {
      int8_t s_0 = y & 0x07;
      int8_t s_1 = 0x08 - s_0;
      if (s_0 == 0x00) {
        uint8_t *b = video.buffer + (y >> 3) * WIDTH + x;
        while (width--) {
          *b++ &= ~pgm_read_byte(pattern);
          pattern = pattern + flip;
        }
      } else {
        uint8_t *b_0 = video.buffer + (y >> 3) * WIDTH + x;
        uint8_t *b_1 = b_0 + WIDTH;
        while (width--) {
          uint8_t p = pgm_read_byte(pattern);
          *b_0++ &= ~(p << s_0);
          *b_1++ &= ~(p >> s_1);
          pattern = pattern + flip;
        }
      }
    }
  }
}

/*
 * パターンをマスクして描画する
 */
void SystemMaskPattern(const uint8_t *pattern, uint8_t mask, int8_t x, int8_t y, uint8_t width, int8_t flip)
{
  // 表示領域の判定
  if (x + width > 0x00 && y > -0x08 && y < HEIGHT) {

    // パターンの反転
    if (flip < 0x00) {
      pattern = pattern + width - 0x01;
    }

    // 水平方向のクリッピング
    if (x < 0x00) {
      if (flip < 0x00) {
        pattern = pattern + x;
      } else {
        pattern = pattern - x;
      }
      width = width + x;
      x = 0x00;
    }
    if (x + width >= WIDTH) {
      width = WIDTH - x;
    }

    // 上方向にクリッピングしての描画
    if (y < 0x00) {
      uint8_t *b = video.buffer + x;
      int8_t s = -y;
      while (width-- > 0x00) {
        *b++ |= (pgm_read_byte(pattern) & mask) >> s;
        pattern = pattern + flip;
      }

    // 下方向にクリッピングしての描画
    } else if (y > HEIGHT - 0x08) {
      uint8_t *b = video.buffer + ((HEIGHT >> 3) - 0x01) * WIDTH + x;
      int8_t s = 0x08 - (HEIGHT - y);
      while (width-- > 0x00) {
        *b++ |= (pgm_read_byte(pattern) & mask) << s;
        pattern = pattern + flip;
      }

    // 垂直方向にクリッピングなしの描画
    } else {
      int8_t s_0 = y & 0x07;
      int8_t s_1 = 0x08 - s_0;
      if (s_0 == 0x00) {
        uint8_t *b = video.buffer + (y >> 3) * WIDTH + x;
        while (width--) {
          *b++ |= pgm_read_byte(pattern) & mask;
          pattern = pattern + flip;
        }
      } else {
        uint8_t *b_0 = video.buffer + (y >> 3) * WIDTH + x;
        uint8_t *b_1 = b_0 + WIDTH;
        while (width--) {
          uint8_t p = pgm_read_byte(pattern) & mask;
          *b_0++ |= p << s_0;
          *b_1++ |= p >> s_1;
          pattern = pattern + flip;
        }
      }
    }
  }
}

/*
 * バイトパターンで描画する
 */
void SystemDrawByte(uint8_t draw, int8_t x, int8_t y, uint8_t width)
{
  // 表示領域の判定
  if (x + width > 0x00 && y > -0x08 && y < HEIGHT) {

    // 水平方向のクリッピング
    if (x < 0x00) {
      width = width + x;
      x = 0x00;
    }
    if (x + width >= WIDTH) {
      width = WIDTH - x;
    }

    // 上方向にクリッピングしての描画
    if (y < 0x00) {
      uint8_t *b = video.buffer + x;
      draw >>= -y;
      while (width-- > 0x00) {
        *b++ |= draw;
      }

    // 下方向にクリッピングしての描画
    } else if (y > HEIGHT - 0x08) {
      uint8_t *b = video.buffer + ((HEIGHT >> 3) - 0x01) * WIDTH + x;
      draw <<= 0x08 - (HEIGHT - y);
      while (width-- > 0x00) {
        *b++ |= draw;
      }

    // 垂直方向にクリッピングなしの描画
    } else {
      int8_t s_0 = y & 0x07;
      int8_t s_1 = 0x08 - s_0;
      if (s_0 == 0x00) {
        uint8_t *b = video.buffer + (y >> 3) * WIDTH + x;
        while (width--) {
          *b++ |= draw;
        }
      } else {
        uint8_t *b_0 = video.buffer + (y >> 3) * WIDTH + x;
        uint8_t *b_1 = b_0 + WIDTH;
        uint8_t d_0 = draw << s_0;
        uint8_t d_1 = draw >> s_1;
        while (width--) {
          *b_0++ |= d_0;
          *b_1++ |= d_1;
        }
      }
    }
  }
}

/*
 * バイトパターンで切り抜く
 */
void SystemClipByte(uint8_t clip, int8_t x, int8_t y, uint8_t width)
{
  // 表示領域の判定
  if (x + width > 0x00 && y > -0x08 && y < HEIGHT) {

    // 水平方向のクリッピング
    if (x < 0x00) {
      width = width + x;
      x = 0x00;
    }
    if (x + width >= WIDTH) {
      width = WIDTH - x;
    }

    // 上方向にクリッピングしての描画
    if (y < 0x00) {
      uint8_t *b = video.buffer + x;
      clip = ~(clip >> -y);
      while (width-- > 0x00) {
        *b++ &= clip;
      }

    // 下方向にクリッピングしての描画
    } else if (y > HEIGHT - 0x08) {
      uint8_t *b = video.buffer + ((HEIGHT >> 3) - 0x01) * WIDTH + x;
      clip = ~(clip << (0x08 - (HEIGHT - y)));
      while (width-- > 0x00) {
        *b++ &= clip;
      }

    // 垂直方向にクリッピングなしの描画
    } else {
      int8_t s_0 = y & 0x07;
      int8_t s_1 = 0x08 - s_0;
      if (s_0 == 0x00) {
        uint8_t *b = video.buffer + (y >> 3) * WIDTH + x;
        clip = ~clip;
        while (width--) {
          *b++ &= clip;
        }
      } else {
        uint8_t *b_0 = video.buffer + (y >> 3) * WIDTH + x;
        uint8_t *b_1 = b_0 + WIDTH;
        uint8_t c_0 = ~(clip << s_0);
        uint8_t c_1 = ~(clip >> s_1);
        while (width--) {
          *b_0++ &= c_0;
          *b_1++ &= c_1;
        }
      }
    }
  }
}

/*
 * サウンドを初期化する
 */
static void SystemInitializeSound(void)
{
  sound.head = NULL;
  sound.play = NULL;
}

/*
 * サウンドを更新する
 */
static void SystemUpdateSound(void)
{
  // 再生データの存在
  if (sound.play != NULL) {

    // 音符／休符を再生中
    if (sound.frame > 0x0000) {
      --sound.frame;

    // 音符の区切り
    } else if (sound.rest > 0x00) {
      noTone(PIN_SPEAKER_1);
      --sound.rest;

    // MML を解析
    } else {
      bool flag = false;
      while (!flag) {
        char c = pgm_read_byte(sound.play++);
        if (c == '\0') {
          if (sound.loop) {
            SystemSetSoundDefault();
            sound.play = sound.head;
          } else {
            sound.head = NULL;
            sound.play = NULL;
            flag = true;
          }
        } else if (c == 'T') {
          sound.tempo = pgm_read_byte(sound.play++) - '0';
        } else if (c == 'O') {
          uint8_t octave = pgm_read_byte(sound.play++) - '0';
          sound.octave = pgm_read_byte(soundOctaves + octave);
        } else if (c == 'L') {
          sound.length = pgm_read_byte(sound.play++) - '0';
        } else if ('A' <= c && c <= 'G') {
          {
            uint8_t note = pgm_read_byte(soundNotes + c - 'A') + sound.octave;
            c = pgm_read_byte(sound.play);
            if (c == '+') {
              ++note;
              ++sound.play;
            } else if (c == '-') {
              --note;
              ++sound.play;
            }
            uint16_t freq = pgm_read_word(soundFrequencys + note);
            if (arduboy.audio.enabled()) {
              tone(PIN_SPEAKER_1, freq);
            }
          }
          {
            uint8_t length = sound.length;
            c = pgm_read_byte(sound.play);
            if ('0' <= c && c <= '9') {
              length = c - '0';
              ++sound.play;
            }
            sound.frame = (uint16_t)pgm_read_byte(soundLengths + length) * sound.tempo;
            c = pgm_read_byte(sound.play);
            if (c == '&') {
              ++sound.play;
            } else if (sound.frame > 0x01) {
              --sound.frame;
              sound.rest = 0x01;
            }
          }
          flag = true;
        } else if (c == 'R') {
          {
            noTone(PIN_SPEAKER_1);
          }
          {
            uint8_t length = sound.length;
            c = pgm_read_byte(sound.play);
            if ('0' <= c && c <= '9') {
              length = c - '0';
              ++sound.play;
            }
            sound.frame = (uint16_t)pgm_read_byte(soundLengths + length) * sound.tempo;
          }
          flag = true;
        }
      }
    }
  }
}

/*
 * サウンドの許可状態を判定する
 */
bool SystemIsSoundEnable(void)
{
  return arduboy.audio.enabled();
}

/*
 * サウンドの許可状態を設定する
 */
void SystemSwitchSound(void)
{
  if (!arduboy.audio.enabled()) {
    arduboy.audio.on();
    if (sound.head == NULL) {
      noTone(PIN_SPEAKER_1);
    }
  } else {
    arduboy.audio.off();
    noTone(PIN_SPEAKER_1);
  }
  arduboy.audio.saveOnOff();
}
void SystemSwitchSound(bool enable)
{
  if (enable) {
    arduboy.audio.on();
    if (sound.head == NULL) {
      noTone(PIN_SPEAKER_1);
    }
  } else {
    arduboy.audio.off();
    noTone(PIN_SPEAKER_1);
  }
  arduboy.audio.saveOnOff();
}

/*
 * サウンドの再生をリクエストする
 */
void SystemRequestSound(const char *mml, bool loop)
{
  sound.head = mml;
  sound.play = mml;
  sound.loop = loop;
  SystemSetSoundDefault();
}

/*
 * サウンドを停止する
 */
void SystemStopSound(void)
{
  noTone(PIN_SPEAKER_1);
  sound.head = NULL;
  sound.play = NULL;
}

/*
 * サウンドを演奏しているかどうかを判定する
 */
bool SystemIsSoundPlaying(void)
{
  return sound.play != NULL ? true : false;
}

/*
 * サウンドのデフォルト値を設定する
 */
static void SystemSetSoundDefault(void)
{
  sound.tempo = 0x01;
  sound.octave = 0x30;
  sound.length = 0x08;
  sound.rest = 0x00;
  sound.frame = 0x0000;
}

/*
 * 乱数を取得する
 */
uint8_t SystemGetRandom(void)
{
  uint8_t r = (randomNumber ^ 0xaa) + 0x49;
  randomNumber = r;
  return (r >> 3) | (r << 5);
}

/*
 * デバッグ機能を初期化する
 */
#if SYSTEM_DEBUG
static void SystemInitializeDebug(void)
{
  // シリアルの初期化
  Serial.begin(9600);
  serialIndex = 0x00;
}
#endif

/*
 * デバッグ機能を更新する
 */
#if SYSTEM_DEBUG
static void SystemUpdateDebug(void)
{
  // シリアルの監視
  int8_t recieved = Serial.available();
  if (recieved > 0x00) {
    int8_t c;
    while ((c = Serial.read()) >= 0x00) {
      if (c == '.') {
        if (SystemIsSerialString(captureString)) {
          SystemCaptureScreen();
        }
        serialIndex = 0x00;
      } else {
        serialString[serialIndex] = c;
        if (++serialIndex >= SYSTEM_SERIAL_STRING_LENGTH) {
          serialIndex = 0x00;
        }
      }
    }
  }
}
#endif

/*
 * シリアルの文字列を比較する
 */
#if SYSTEM_DEBUG
static bool SystemIsSerialString(const char *string)
{
  bool result = false;
  int8_t i = 0x00;
  while (i < SYSTEM_SERIAL_STRING_LENGTH) {
    char c = pgm_read_byte(string + i);
    if (c == '\0') {
      result = true;
      break;
    }
    if (c != serialString[i]) {
      break;
    }
    ++i;
  }
  return result;
}
#endif

/*
 * 画面のキャプチャを uuencode した .bmp ファイルで出力する
 */
#if SYSTEM_DEBUG
static void SystemCaptureScreen(void)
{
  // ヘッダの出力
  {
    const char *s = captureHeader;
    char c;
    while ((c = pgm_read_byte(s++)) != '\0') {
      Serial.write(c);
    }
  }

  // イメージの出力
  {
    int16_t databits = 0x0080 * 0x0040;
    uint8_t x = 0x00;
    uint8_t y = 0x3f;
    uint8_t encodechar = 0x00;
    uint8_t encodebits = 0x02;
    uint8_t encodeline = 0x16;
    while (databits > 0x0000) {
      while (encodebits > 0x00) {
        encodechar <<= 1;
        if (databits > 0x0000) {
          int n = 0x0080 * 0x0040 - databits;
          encodechar |= arduboy.getPixel(x, y);
          if (++x >= 0x80) {
            x = 0x00;
            --y;
          }
          --databits;
        }
        --encodebits;
      }
      if (encodechar != 0x00) {
        encodechar += ' ';
      } else {
        encodechar = '`';
      }
      Serial.write(encodechar);
      if (++encodeline >= 0x3c) {
        if (databits > 0x0000) {
          Serial.write('\n');
          encodeline = 0x00;
          if (databits >= 0x002d * 0x0008) {
              Serial.write('M');
          } else {
              Serial.write(' ' + (databits + 0x07) / 0x08);
          }
        }
      };
      encodechar = 0x00;
      encodebits = 0x06;
    }
  }

  // フッタの出力
  {
    const char *s = captureFooter;
    char c;
    while ((c = pgm_read_byte(s++)) != '\0') {
      Serial.write(c);
    }
  }
}
#endif

