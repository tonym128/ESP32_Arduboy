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

// フレーム
static uint8_t titleFrame;

// ブリンク
static uint8_t titleBlink;

// 文字列
static const char titleStringScore[] PROGMEM = "SCORE-    00";

// サウンド
static const char titleSoundOption[] PROGMEM = "T1O4B1";


/*
 * 内部関数の宣言
 */


/*
 * タイトルを初期化する
 */
void TitleInitialize(void)
{
  // フレームの設定
  titleFrame = 0x00;

  // ブリンクの設定
  titleBlink = 0x00;
  
  // 状態の設定
  titleState = TITLE_STATE_STAY;

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
      // フレームの更新
      ++titleFrame;
      
      // ブリンクの更新
      if ((titleFrame & 0x1f) == 0x00) {
        ++titleBlink;
      }
      
      // A ボタンで開始
      if (SystemIsInputEdge(A_BUTTON)) {
        titleBlink = 0x20;
        titleState = TITLE_STATE_START;

      // B ボタンでサウンドの切り替え
      } else if (SystemIsInputEdge(B_BUTTON)) {
        SystemSwitchSound();
        if (SystemIsSoundEnable()) {
          SystemRequestSound(titleSoundOption, false);
        }
      }
    }
    break;

  // スタート
  case TITLE_STATE_START:
    {
      // フレームの更新
      ++titleFrame;
      
      // ブリンクの更新
      if ((titleFrame & 0x01) == 0x00) {
        --titleBlink;
      }
      if (titleBlink == 0x00) {
        
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

    // 店員の描画
    for (int8_t i = 0x00; i < 0x08; i++) {
      AppDrawPattern(0x0188 + i * 0x10, 0x40, i * 0x08, 0x40, SYSTEM_VIDEO_FLIP_NORMAL);
    }

    // スコアの描画
    {
      AppDrawString(titleStringScore, 0x00, 0x00);
      AppDrawNumber(AppGetScore(), 0x18, 0x00, 0x04);
    }
    
    // ロゴの描画
    {
      int8_t y = 0x0c + (((int16_t)0x0006 * AppGetSin(titleFrame)) >> 8);
      for (int8_t i = 0x00; i < 0x04; i++) {
        AppDrawPattern(0x0185 + i * 0x10, 0x0c, y + i * 0x08, 0x18, SYSTEM_VIDEO_FLIP_NORMAL);
        AppDrawPattern(0x01c5 + i * 0x10, 0x24, y + i * 0x08, 0x10, SYSTEM_VIDEO_FLIP_NORMAL);
      }
    }

    // スタートの描画
    if ((titleBlink & 0x01) == 0x00) {
      AppClipPattern(0x0008, 0x0018, 0x20, 0x30, 0x40, SYSTEM_VIDEO_FLIP_NORMAL);
    }

    // サウンドの描画
    if (SystemIsSoundEnable()) {
      AppDrawPattern(0x0005, 0x01, 0x38, 0x18, SYSTEM_VIDEO_FLIP_NORMAL);
    } else {
      AppDrawPattern(0x0002, 0x01, 0x38, 0x18, SYSTEM_VIDEO_FLIP_NORMAL);
    }

    // 描画の完了
    SystemDisplayVideo();
  }
}


