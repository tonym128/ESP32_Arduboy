/*
 * Game.cpp : ゲーム
 */

/*
 * 参照ファイル
 */
#include "System.h"
#include "App.h"
#include "Game.h"
#include "Dungeon.h"
#include "Player.h"
#include "Enemy.h"
#include "Spell.h"


/*
 * 変数の定義
 */

// 状態
static uint8_t gameState;

// フレーム
static uint16_t gameFrame;

// 文字列
static const char gameStringLife[]  = "LIFE";
static const char gameStringYouAreDead[]  = "YOU ARE DEAD";
static const char gameStringYouReturned[]  = "YOU RETURNED";
static const char gameStringFromThe[]  = "FROM THE";
static const char gameStringLabyrinth[]  = "LABYRINTH";

// パターン
static const uint8_t gameStatusFramePatterns[]  = {
  0xb0, 0xb1, 0xb1, 0xb1, 0xb1, 0xb1, 0xb1, 0xb2, 
  0xb3, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xb4, 
  0xb3, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xb4, 
  0xb8, 0xb9, 0xb9, 0xb9, 0xb9, 0xb9, 0xb9, 0xba, 
  0xb3, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xb4, 
  0xb3, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xb4, 
  0xb3, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xb4, 
  0xb5, 0xb6, 0xb6, 0xb6, 0xb6, 0xb6, 0xb6, 0xb7, 
};

// サウンド
static const char gameSoundClear[]  = 
  "T5L3"
  "O3A-1B-1"
  "O4C-6O3B-1A-1B-5G-5A-8R5"
  ;


/*
 * 内部関数の宣言
 */
static void GamePlay(void);
static void GameOver(void);
static void GameClear(void);
static void GameLoop(void);
static void GameDraw(void);
static void GameDrawString(const char *string, int8_t y);


/*
 * ゲームを初期化する
 */
void GameInitialize(void)
{
  // ダンジョンの初期化
  DungeonInitialize();

  // プレイヤの初期化
  PlayerInitialize();

  // エネミーの初期化
  EnemyInitialize();

  // スペルの初期化
  SpellInitialize();

  // フレームの初期化
  gameFrame = 0;
  
  // 状態の設定
  gameState = GAME_STATE_PLAY;

  // アプリケーションの更新
  AppSetState(APP_STATE_GAME_UPDATE);
}

/*
 * ゲームを更新する
 */
void GameUpdate(void)
{
  // 状態別の処理
  switch (gameState & 0xf0) {

  // ゲームプレイ
  case GAME_STATE_PLAY:
    GamePlay();
    break;

  // ゲームオーバー
  case GAME_STATE_OVER:
    GameOver();
    break;

  // ゲームクリア
  case GAME_STATE_CLEAR:
    GameClear();
    break;

  // その他
  default:
    break;
  }
}

/*
 * ゲームをプレイする
 */
static void GamePlay(void)
{
  // 初期化
  if ((gameState & 0x0f) == 0x00) {

    // ルームに入る
    DungeonEnterRoom(PlayerGetRoom());

    // エネミーの配置
    EnemyEncount();

    // スペルの削除
    SpellKill();

    // 状態の更新
    ++gameState;
  }

  // 更新
  {
    // ゲームの処理
    GameLoop();
  }

  // 描画
  {
    // 描画の開始
    SystemClearVideo(BLACK);

    // ゲームの描画
    GameDraw();

    // 描画の完了
    SystemDisplayVideo();
  }

  // クリア
  if (PlayerIsItem(ITEM_EXIT)) {

    // 状態の更新
    gameState = GAME_STATE_CLEAR;
    
  // ゲームオーバー
  } else if (!PlayerIsLive()) {

    // 状態の更新
    gameState = GAME_STATE_OVER;
    
  // 部屋の移動
  } else if (PlayerExitRoom()) {

    // 状態の更新
    gameState = GAME_STATE_PLAY;
  }
}

/*
 * ゲームオーバーになる
 */
static void GameOver(void)
{
  // 初期化
  if ((gameState & 0x0f) == 0x00) {

    // サウンドの停止
    SystemStopSound();

    // フレームの設定
    gameFrame = 0x0200;

    // 状態の更新
    ++gameState;
  }

  // 更新
  {
    // ゲームの処理
    GameLoop();

    // フレームの更新
    if (gameFrame > 0) {
      --gameFrame;
    }
  }

  // 描画
  {
    // 描画の開始
    SystemClearVideo(BLACK);

    // ゲームの描画
    GameDraw();

    // ゲームオーバーの描画
    GameDrawString(gameStringYouAreDead, 0x1d);

    // 描画の完了
    SystemDisplayVideo();
  }

  // タイトル
  if (gameFrame == 0 || SystemIsInputEdge(A_BUTTON)) {
    
    // アプリケーションの更新
    AppSetState(APP_STATE_TITLE_INITIALIZE);
  }
}

/*
 * ゲームをクリアする
 */
static void GameClear(void)
{
  // 初期化
  if ((gameState & 0x0f) == 0x00) {

    // サウンドの再生
    SystemRequestSound(gameSoundClear, false);

    // フレームの設定
    gameFrame = 0x0200;

    // 状態の更新
    ++gameState;
  }

  // 更新
  {
    // ゲームの処理
    GameLoop();

    // フレームの更新
    if (gameFrame > 0) {
      --gameFrame;
    }
  }

  // 描画
  {
    // 描画の開始
    SystemClearVideo(BLACK);

    // ゲームの描画
    GameDraw();

    // ゲームクリアの描画
    GameDrawString(gameStringYouReturned, 0x16);
    GameDrawString(gameStringFromThe, 0x1d);
    GameDrawString(gameStringLabyrinth, 0x24);

    // 描画の完了
    SystemDisplayVideo();
  }

  // タイトル
  if (gameFrame == 0 || SystemIsInputEdge(A_BUTTON)) {
    
    // アプリケーションの更新
    AppSetState(APP_STATE_TITLE_INITIALIZE);
  }
}

/*
 * ゲームループを処理する
 */
static void GameLoop(void)
{
  // ダンジョンの更新
  DungeonUpdate();

  // プレイヤの更新
  PlayerUpdate();

  // エネミーの更新
  EnemyUpdate();

  // スペルの更新
  SpellUpdate();
}

/*
 * ゲームを描画する
 */
static void GameDraw(void)
{
  // ダンジョンの描画
  DungeonRender();

  // プレイヤの描画
  PlayerRender();

  // エネミーの描画
  EnemyRender();

  // スペルの描画
  SpellRender();

  // 暗闇の描画
  {
    if (PlayerIsCondition(CONDITION_BLIND)) {
      int8_t px, py;
      PlayerGetPosition(&px, &py);
      int8_t y = -(0x08 - (py & 0x07));
      while (y < py - 0x10) {
        SystemClipByte(0xff, 0x00, y, 0x40);
        y += 0x08;
      }
      for (int8_t i = 0; i < 4; i++) {
        if (px > 0x10) {
          SystemClipByte(0xff, 0x00, y, px - 0x10);
        }
        AppClipPattern(0x00a0 + i * 0x04, px - 0x10, y, 0x20, SYSTEM_VIDEO_FLIP_NORMAL);
        if (px < 0x40 - 0x10) {
          SystemClipByte(0xff, px + 0x10, y, 0x40 - (px + 0x10));
        }
        y += 0x08;
      }
      while (y < 0x40) {
        SystemClipByte(0xff, 0x00, y, 0x40);
        y += 0x08;
      }
    }
  }
  
  // 枠の描画
  {
    uint8_t *p = (uint8_t *)gameStatusFramePatterns;
    for (int8_t y = 0; y < 8; y++) {
      for (int8_t x = 0; x < 8; x++) {
        AppDrawPattern((int16_t)pgm_read_byte(p), x * 0x08 + 0x40, y * 0x08, 0x08, SYSTEM_VIDEO_FLIP_NORMAL);
        ++p;
      }
    }
  }

  // 体力の描画
  {
    int8_t life = PlayerGetLife();
    AppDrawString(gameStringLife, 0x46, 0x06);
    AppDrawPattern(0x0088, 0x46, 0x0c, 0x34, SYSTEM_VIDEO_FLIP_NORMAL);
    if (life > 0) {
      if (life >= PLAYER_LIFE / 4 || (gameFrame & 0x02) == 0) {
        SystemDrawByte(0x0f, 0x47, 0x0d, life);
      }
    }
    // AppDrawNumber(life, 0x72, 0x06, 0x02);
  }

  // コンディションの描画
  {
    int8_t x = 0x46;
    for (int8_t i = CONDITION_NULL + 1; i < CONDITION_SIZE; i++) {
      if (PlayerIsCondition(i)) {
        AppDrawPattern(0x0038 + (i - 1), x, 0x12, 0x06, SYSTEM_VIDEO_FLIP_NORMAL);
        x += 0x07;
      }
    }
  }

  // 攻撃力の描画
  {
    int8_t x = 0x45;
    int8_t n = PlayerGetAttack();
    for (int8_t i = 0; i < n; i++) {
        AppDrawPattern(0x0090 + (ITEM_SWORD - 1), x, 0x21, 0x07, SYSTEM_VIDEO_FLIP_NORMAL);
        x += 0x07;
    }
  }

  // 防御力の描画
  {
    int8_t x = 0x45;
    int8_t n = PlayerGetDefense();
    for (int8_t i = 0; i < n; i++) {
        AppDrawPattern(0x0090 + (ITEM_SHIELD - 1), x, 0x28, 0x07, SYSTEM_VIDEO_FLIP_NORMAL);
        x += 0x07;
    }
  }

  // アイテムの描画
  {
    int8_t x = 0x45;
    for (int8_t i = ITEM_BOOTS; i <= ITEM_CRYSTAL; i++) {
      if (PlayerIsItem(i)) {
        AppDrawPattern(0x0090 + (i - 1), x, 0x2f, 0x07, SYSTEM_VIDEO_FLIP_NORMAL);
        x += 0x07;
      }
    }
  }
}

/*
 * 文字列をセンタリングして描画する
 */
static void GameDrawString(const char *string, int8_t y)
{
  int8_t x = 0;
  {
    int8_t length = 0;
    const char *s = string;
    char c;
    while ((c = pgm_read_byte(s)) != '\0') {
      ++length;
      ++s;
    }
    x = (0x40 - length * 4) / 2;
  }
  AppClipString(string, x - 1, y + 0);
  AppClipString(string, x + 1, y + 0);
  AppClipString(string, x + 0, y - 1);
  AppClipString(string, x + 0, y + 1);
  AppDrawString(string, x, y);
}
