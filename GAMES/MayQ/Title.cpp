/*
 * Title.cpp : タイトル
 */

/*
 * 参照ファイル
 */
#include "System.h"
#include "App.h"
#include "Title.h"
#include "pattern.h"


/*
 * 変数の定義
 */

// 状態
static uint8_t titleState;

// ページ
static int8_t titlePage;

// 点滅
static uint16_t titleBlink;

// レイアウト
static const uint8_t titleLayoutObjects[] PROGMEM = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x32, 0x00, 
  0x33, 0x08, 0x0c, 0x08, 0x08, 0x04, 0x14, 0x0d, 
  0x80, 0x08, 0x16, 0x08, 0x08, 0x05, 0x14, 0x17, 
  0x9a, 0x08, 0x20, 0x08, 0x08, 0x06, 0x14, 0x21, 
};
static const uint8_t titleLayoutMonsters_1[] PROGMEM = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x30, 0x00, 
  0x40, 0x08, 0x0c, 0x08, 0x08, 0x07, 0x14, 0x0d, 
  0x42, 0x48, 0x0c, 0x08, 0x08, 0x08, 0x54, 0x0d, 
  0x44, 0x08, 0x16, 0x08, 0x08, 0x09, 0x14, 0x17, 
  0x46, 0x48, 0x16, 0x08, 0x08, 0x0a, 0x54, 0x17, 
  0x48, 0x08, 0x20, 0x08, 0x08, 0x0b, 0x14, 0x21, 
  0x4a, 0x48, 0x20, 0x08, 0x08, 0x0c, 0x54, 0x21, 
  0x4c, 0x08, 0x2a, 0x08, 0x08, 0x0d, 0x14, 0x2b, 
  0x4e, 0x48, 0x2a, 0x08, 0x08, 0x0e, 0x54, 0x2b, 
};
static const uint8_t titleLayoutMonsters_2[] PROGMEM = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x30, 0x00, 
  0x50, 0x08, 0x0c, 0x08, 0x08, 0x0f, 0x14, 0x0d, 
  0x52, 0x48, 0x0c, 0x08, 0x08, 0x10, 0x54, 0x0d, 
  0x54, 0x08, 0x16, 0x08, 0x08, 0x11, 0x14, 0x17, 
  0x56, 0x48, 0x16, 0x08, 0x08, 0x12, 0x54, 0x17, 
  0x58, 0x08, 0x20, 0x08, 0x08, 0x13, 0x14, 0x21, 
  0x5a, 0x48, 0x20, 0x08, 0x08, 0x14, 0x54, 0x21, 
  0x5c, 0x08, 0x2a, 0x08, 0x08, 0x15, 0x14, 0x2b, 
  0x5e, 0x48, 0x2a, 0x08, 0x08, 0x16, 0x54, 0x2b, 
};
static const uint8_t titleLayoutMonsters_3[] PROGMEM = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x30, 0x00, 
  0x60, 0x08, 0x0c, 0x10, 0x10, 0x17, 0x1c, 0x0d, 
  0x64, 0x48, 0x0c, 0x10, 0x10, 0x18, 0x5c, 0x0d, 
  0x68, 0x08, 0x1e, 0x10, 0x10, 0x19, 0x1c, 0x1f, 
  0x6c, 0x48, 0x1e, 0x10, 0x10, 0x1a, 0x5c, 0x1f, 
};
static const uint8_t titleLayoutItems_1[] PROGMEM = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x36, 0x00, 
  0x90, 0x08, 0x0c, 0x08, 0x08, 0x1b, 0x14, 0x0d, 
  0x91, 0x48, 0x0c, 0x08, 0x08, 0x1c, 0x54, 0x0d, 
  0x92, 0x08, 0x16, 0x08, 0x08, 0x1d, 0x14, 0x17, 
  0x93, 0x48, 0x16, 0x08, 0x08, 0x1e, 0x54, 0x17, 
  0x94, 0x08, 0x20, 0x08, 0x08, 0x1f, 0x14, 0x21, 
};
static const uint8_t titleLayoutItems_2[] PROGMEM = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x36, 0x00, 
  0x95, 0x08, 0x0c, 0x08, 0x08, 0x20, 0x14, 0x0d, 
  0x96, 0x48, 0x0c, 0x08, 0x08, 0x21, 0x54, 0x0d, 
  0x97, 0x08, 0x16, 0x08, 0x08, 0x22, 0x14, 0x17, 
  0x98, 0x48, 0x16, 0x08, 0x08, 0x23, 0x54, 0x17, 
  0x99, 0x08, 0x20, 0x08, 0x08, 0x05, 0x14, 0x21, 
};
static const uint8_t titleLayoutConditions[] PROGMEM = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x2c, 0x00, 
  0x38, 0x08, 0x0c, 0x08, 0x08, 0x24, 0x14, 0x0d, 
  0x39, 0x48, 0x0c, 0x08, 0x08, 0x25, 0x54, 0x0d, 
  0x3a, 0x08, 0x16, 0x08, 0x08, 0x26, 0x14, 0x17, 
  0x3b, 0x48, 0x16, 0x08, 0x08, 0x27, 0x54, 0x17, 
  0x3c, 0x08, 0x20, 0x08, 0x08, 0x28, 0x14, 0x21, 
};

// 文字列
static const char titleStrings[][12] PROGMEM = {
// 0123456789AB
  "OBJECTS", 
  "MONSTERS", 
  "ITEMS", 
  "CONDITIONS", 
  "YOU", 
  "CRYSTAL", 
  "EXIT", 
  "ORC", 
  "GOLEM", 
  "SKELTON", 
  "LICH", 
  "GHOST", 
  "ROGUE", 
  "KNIGHT", 
  "MAGE", 
  "BAT", 
  "PYTHON", 
  "SPIDER", 
  "ZORN", 
  "SQUID", 
  "GAZER", 
  "LIZARD", 
  "DAEMON", 
  "GARGOYLE", 
  "REAPER", 
  "SHADOW", 
  "DRAGON", 
  "SWORD", 
  "SHIELD", 
  "POTION", 
  "BOOTS", 
  "RING", 
  "HAMMER", 
  "CANDLE", 
  "MIRROR", 
  "KEY", 
  "SLOW", 
  "SLEEP", 
  "BLIND", 
  "CONFUSE", 
  "POISON", 
};

// サウンド
static const char titleSoundOption[] PROGMEM = "T1O4B1";
static const char titleSoundStart[] PROGMEM = 
  "T3L3"
  "O3A-4E-1E-7A-5B-8RA-1B-1"
  "O4C-7RC-O3B-A-B-7G-7"
  "O3A-4E-1E-7A-5B-8RA-1B-1"
  "O4C-6O3B-1A-1B-5G-5A-8R5"
  "O3A-4E-1E-7A-5B-8RA-1B-1"
  "O4C-7RC-O3B-A-B-7G-7"
  "O3A-4E-1E-7A-5B-8RA-1B-1"
  "O4C-6O3B-1A-1B-5G-5A-8R5"
  "O4C-C-4C-C-1D-7DD4DD1D-7C-C-4C-C-1O3A-7"
  "O3B-7E-7"
  "O4C-C-4C-C-1D-7DD4DD1D-7C-C-4C-C-1O3A-7"
  "O4C-7O3B-7"
  ;

/*
 * 内部関数の宣言
 */
static void TitleDrawLayout(const uint8_t *layout, int8_t size);


/*
 * タイトルを初期化する
 */
void TitleInitialize(void)
{
  // 状態の設定
  titleState = TITLE_STATE_STAY;

  // ページの設定
  titlePage = 0;

  // 点滅の設定
  titleBlink = 0x0000;

  // アプリケーションの更新
  AppSetState(APP_STATE_TITLE_UPDATE);
}

/*
 * タイトルを更新する
 */
void TitleUpdate(void)
{
  // 状態別の処理
  switch (titleState & 0xf0) {

  // 待機
  case TITLE_STATE_STAY:
    {
      // 点滅の更新
      ++titleBlink;
      
      // A ボタンで開始
      if (SystemIsInputEdge(A_BUTTON) && titlePage == TITLE_PAGE_LOGO) {
        SystemRequestSound(titleSoundStart, true);
        titleBlink = 0x0000;
        titleState = TITLE_STATE_START;

      // B ボタンでサウンドの切り替え
      } else if (SystemIsInputEdge(B_BUTTON)) {
        SystemSwitchSound();
        if (SystemIsSoundEnable()) {
          SystemRequestSound(titleSoundOption, false);
        }

      // ←→で改ページ
      } else if (SystemIsInputEdge(LEFT_BUTTON)) {
        if (--titlePage < 0) {
          titlePage = TITLE_PAGE_SIZE - 1;
        }
      } else if (SystemIsInputEdge(RIGHT_BUTTON)) {
        if (++titlePage >= TITLE_PAGE_SIZE) {
          titlePage = 0;
        }
      }
    }
    break;

  // スタート
  case TITLE_STATE_START:
    {
      // 点滅の更新
      titleBlink += 0x0008;
      if (titleBlink >= 0x0300) {
        
        // アプリケーションの更新
        AppSetState(APP_STATE_GAME_INITIALIZE);
      }
    }
    break;
    
  // その他
  default:
    break;
  }
  
  // 描画
  {
    // 描画の開始
    SystemClearVideo(BLACK);

    // ページ別の処理
    switch (titlePage) {

    // ロゴ
    case TITLE_PAGE_LOGO:
      {
        // ロゴの描画
        AppDrawPattern(0x00c0, 0x20, 0x0c, 0x40, SYSTEM_VIDEO_FLIP_NORMAL);
        AppDrawPattern(0x00d0, 0x20, 0x14, 0x40, SYSTEM_VIDEO_FLIP_NORMAL);
        AppDrawPattern(0x00e0, 0x20, 0x1c, 0x40, SYSTEM_VIDEO_FLIP_NORMAL);
    
        // A ボタンの描画
        if ((titleBlink & 0x0020) == 0) {
          AppDrawPattern(0x0008, 0x20, 0x2c, 0x40, SYSTEM_VIDEO_FLIP_NORMAL);
        }
      }
      break;

    // オブジェクト
    case TITLE_PAGE_OBJECT:
      {
        TitleDrawLayout(titleLayoutObjects, sizeof (titleLayoutObjects) / 0x08);
      }
      break;

    // モンスター１
    case TITLE_PAGE_MONSTER_1:
      {
        TitleDrawLayout(titleLayoutMonsters_1, sizeof (titleLayoutMonsters_1) / 0x08);
      }
      break;

    // モンスター２
    case TITLE_PAGE_MONSTER_2:
      {
        TitleDrawLayout(titleLayoutMonsters_2, sizeof (titleLayoutMonsters_2) / 0x08);
      }
      break;

    // モンスター３
    case TITLE_PAGE_MONSTER_3:
      {
        TitleDrawLayout(titleLayoutMonsters_3, sizeof (titleLayoutMonsters_3) / 0x08);
      }
      break;

    // アイテム１
    case TITLE_PAGE_ITEM_1:
      {
        TitleDrawLayout(titleLayoutItems_1, sizeof (titleLayoutItems_1) / 0x08);
      }
      break;

    // アイテム２
    case TITLE_PAGE_ITEM_2:
      {
        TitleDrawLayout(titleLayoutItems_2, sizeof (titleLayoutItems_2) / 0x08);
      }
      break;

    // コンディション
    case TITLE_PAGE_CONDITION:
      {
        TitleDrawLayout(titleLayoutConditions, sizeof (titleLayoutConditions) / 0x08);
      }
      break;

    // その他
    default:
      break;
    }
    
    // サウンドの描画
    if (SystemIsSoundEnable()) {
      AppDrawPattern(0x0005, 0x01, 0x38, 0x18, SYSTEM_VIDEO_FLIP_NORMAL);
    } else {
      AppDrawPattern(0x0002, 0x01, 0x38, 0x18, SYSTEM_VIDEO_FLIP_NORMAL);
    }

    // 改ページの描画
    AppDrawPattern(0x003e, 0x72, 0x38, 0x0e, SYSTEM_VIDEO_FLIP_NORMAL);

    // 描画の完了
    SystemDisplayVideo();
  }
}

/*
 * レイアウトを描画する
 */
static void TitleDrawLayout(const uint8_t *layout, int8_t size)
{
  uint8_t pattern, string;
  int8_t px, py, pw, ph, sx, sy;
  while (size-- > 0) {
    pattern = pgm_read_byte(layout++);
    px = (int8_t)pgm_read_byte(layout++);
    py = (int8_t)pgm_read_byte(layout++);
    pw = (int8_t)pgm_read_byte(layout++);
    ph = (int8_t)pgm_read_byte(layout++);
    string = pgm_read_byte(layout++);
    sx = (int8_t)pgm_read_byte(layout++);
    sy = (int8_t)pgm_read_byte(layout++);
    if (pattern != 0x00) {
      while (ph > 0x00) {
        AppDrawPattern((uint16_t)pattern, px, py, pw, SYSTEM_VIDEO_FLIP_NORMAL);
        pattern += 0x10;
        py += 0x08;
        ph -= 0x08;
      }
    }
    AppDrawString(titleStrings[string], sx, sy);
  }
}

 
