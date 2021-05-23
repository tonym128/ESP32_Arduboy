/*
 * Game.cpp : ゲーム
 */

/*
 * 参照ファイル
 */
#include "System.h"
#include "App.h"
#include "Game.h"
#include "Field.h"
#include "Player.h"
#include "Shot.h"
#include "Enemy.h"
#include "Bullet.h"


/*
 * 変数の定義
 */

// 状態
static uint8_t gameState;

// フレーム
static uint16_t gameFrame;

// スコア
static uint16_t gameScore;

// 距離
static const int8_t gameDistanceYs[] PROGMEM = {
  0x1f, 0x1e, 0x1d, 0x1c, 0x1b, 0x1a, 0x19, 0x18, 
  0x17, 0x16, 0x15, 0x15, 0x14, 0x13, 0x12, 0x12, 
  0x11, 0x11, 0x10, 0x10, 0x0f, 0x0f, 0x0e, 0x0e, 
  0x0d, 0x0d, 0x0c, 0x0c, 0x0c, 0x0b, 0x0b, 0x0b, 
  0x0a, 0x0a, 0x0a, 0x0a, 0x09, 0x09, 0x09, 0x09, 
  0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 
};
static const int8_t gameDistanceScales[] PROGMEM = {
  0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 
  0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 
  0x02, 0x02, 0x02, 0x02, 0x01, 0x01, 0x01, 0x01, 
  0x01, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
};

// 描画
static uint16_t gameDraws[GAME_DRAW_N];

// 文字列
static const char gameStringScore[] PROGMEM = "SCORE-    00";
static const char gameStringHiScore[] PROGMEM = "HIGH SCORE";
static const char gameString00[] PROGMEM = "00";

// サウンド
static const char gameSoundStart[] PROGMEM = 
  "T3"
  "L1O3E-3E-E-O4E-3O3E-E-RE-3E-O4E-3O3E-E-E-3E-E-O4E-3O3E-E-"
  "L1O4E-3E-E-R3E-3E-E-4R5"
  ;
static const char gameSoundPlay[] PROGMEM = 
  "T3"
  "L3O4G5F5E-O3B-O4C1E-4GGF1FE-1&E-5RC"
  "L4O4FGC3FGC3FG1&G7F3G3"
  "L3O4A-7RA-B-O5CE-5C5E-CG-1FE-1&E-9R9"
  "L3O4G5F5E-O3B-O4C1E-4GGF1FE-1&E-5RC"
  "L4O4FGC3FGC3FG1&G7F3G3"
  "L3O4A-7RA-B-O5CE-5C5E-CG-1FE-1&E-9R9"
  "L3O5CO4BO5CO4BO5CO4BO5CO4BO5C4E-1&E-5RC1E-1F1G-E-1"
  "L1O4RB-4&B-5R3GB-O5CE-3O4G-RE-4&E-5R3CO3B-O4CE-4"
  "L3O5CO4BO5CO4BO5CO4BO5CO4BO5C4E-1&E-5RC1E-1G-1O6C3O5B-&B-8R5R9"
  "L3O5CO4BO5CO4BO5CO4BO5CO4BO5C4E-1&E-5RC1E-1F1G-E-1"
  "L1O4RB-4&B-5R3GB-O5CE-3O4G-RE-4&E-5R3CO3B-O4CE-4"
  "L3O5CO4BO5CO4BO5CO4BO5CO4BO5C4E-1&E-5RC1E-1G-1O6C3O5B-&B-8R5R9"
  ;


/*
 * 内部関数の宣言
 */
static void GameStart(void);
static void GamePlay(void);
static void GameOver(void);
static void GameHiScore(void);
static void GameHit(void);
static void GameResetDraw(void);
static void GameDraw(void);
static void GameDrawStatus(void);


/*
 * ゲームを初期化する
 */
void GameInitialize(void)
{
  // フィールドの初期化
  FieldInitialize();

  // プレイヤの初期化
  PlayerInitialize();

  // 自弾の初期化
  ShotInitialize();

  // エネミーの初期化
  EnemyInitialize();

  // 敵弾の初期化
  BulletInitialize();

  // スコアの初期化
  gameScore = 0x0000;
  
  // 状態の設定
  gameState = GAME_STATE_START;

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

  // ゲームスタート
  case GAME_STATE_START:
    GameStart();
    break;

  // ゲームプレイ
  case GAME_STATE_PLAY:
    GamePlay();
    break;

  // ゲームオーバー
  case GAME_STATE_OVER:
    GameOver();
    break;

  // ハイスコア
  case GAME_STATE_HISCORE:
    GameHiScore();
    break;

  // その他
  default:
    break;
  }
}

/*
 * ゲームを開始する
 */
static void GameStart(void)
{
  // 初期化
  if ((gameState & 0x0f) == 0x00) {

    // サウンドの再生
    SystemRequestSound(gameSoundStart, false);

    // 状態の更新
    ++gameState;
  }

  // 更新
  {
    // 描画順のリセット
    GameResetDraw();

    // フィールドの更新
    FieldUpdate();

    // プレイヤの更新
    PlayerUpdate();

    // 自弾の更新
    ShotUpdate();
  }

  // 描画
  {
    // 描画の開始
    SystemClearVideo(BLACK);

    // フィールドの描画
    FieldRender();

    // 登録順の描画
    GameDraw();

    // プレイヤの描画
    PlayerRender();

    // ステータスの描画
    GameDrawStatus();

    // スタートの描画
    AppClipPattern(0x0158, 0x0178, 0x20, 0x1c, 0x40, SYSTEM_VIDEO_FLIP_NORMAL);

    // 描画の完了
    SystemDisplayVideo();
  }

  // サウンドの監視
  if (!SystemIsSoundPlaying()) {
    
    // 状態の更新
    gameState = GAME_STATE_PLAY;
  }
}

/*
 * ゲームをプレイする
 */
static void GamePlay(void)
{
  // 初期化
  if ((gameState & 0x0f) == 0x00) {

    // サウンドの再生
    SystemRequestSound(gameSoundPlay, true);

    // 状態の更新
    ++gameState;
  }

  // 更新
  {
    // 描画順のリセット
    GameResetDraw();

    // ヒット判定
    GameHit();

    // フィールドの更新
    FieldUpdate();

    // プレイヤの更新
    PlayerUpdate();

    // 自弾の更新
    ShotUpdate();

    // エネミーの更新
    EnemyUpdate();

    // 敵弾の更新
    BulletUpdate();
  }

  // 描画
  {
    // 描画の開始
    SystemClearVideo(BLACK);

    // フィールドの描画
    FieldRender();

    // 登録順の描画
    GameDraw();

    // プレイヤの描画
    PlayerRender();

    // ステータスの描画
    GameDrawStatus();

    // 描画の完了
    SystemDisplayVideo();
  }

  // プレイヤの監視
  if (PlayerIsDead()) {

    // 状態の更新
    gameState = GAME_STATE_OVER;
  }
}

/*
 * ゲームオーバーになる
 */
static void GameOver(void)
{
  // 初期化
  if ((gameState & 0x0f) == 0x00) {

    // フレームの設定
    gameFrame = 0x0008 * 0x0040;

    // サウンドの停止
    SystemStopSound();

    // 状態の更新
    ++gameState;
  }

  // 更新
  {
    // 描画順のリセット
    GameResetDraw();

    // フィールドの更新
    FieldUpdate();

    // プレイヤの更新
    PlayerUpdate();

    // 自弾の更新
    ShotUpdate();

    // エネミーの更新
    EnemyUpdate();

    // 敵弾の更新
    BulletUpdate();

    // フレームの更新
    if (gameFrame > 0x0000) {
      --gameFrame;
    }
  }

  // 描画
  {
    // 描画の開始
    SystemClearVideo(BLACK);

    // フィールドの描画
    FieldRender();

    // 登録順の描画
    GameDraw();

    // プレイヤの描画
    PlayerRender();

    // ステータスの描画
    GameDrawStatus();

    // ゲームオーバーの描画
    AppClipPattern(0x0168, 0x0178, 0x20, 0x1c, 0x40, SYSTEM_VIDEO_FLIP_NORMAL);

    // 描画の完了
    SystemDisplayVideo();
  }

  // フレームとキー入力の監視
  if (gameFrame == 0x0000 || SystemIsInputEdge(A_BUTTON)) {

    // ハイスコアの更新
    if (gameScore > AppGetScore()) {
      AppSetScore(gameScore);

      // 状態の更新
      gameState = GAME_STATE_HISCORE;

    // そのまま終了
    } else {
  
      // アプリケーションの更新
      AppSetState(APP_STATE_TITLE_INITIALIZE);
    }
  }
}

/*
 * ハイスコアを更新した
 */
static void GameHiScore(void)
{
  // 初期化
  if ((gameState & 0x0f) == 0x00) {

    // フレームの設定
    gameFrame = 0x0008 * 0x0040;

    // 状態の更新
    ++gameState;
  }

  // 更新
  {
    // フレームの更新
    if (gameFrame > 0x0000) {
      --gameFrame;
    }
  }

  // 描画
  {
    // 描画の開始
    SystemClearVideo(BLACK);

    // ハイスコアの描画
    AppDrawString(gameStringHiScore, 0x2c, 0x18);
    AppDrawNumber(gameScore, 0x34, 0x22, 0x04);
    AppDrawString(gameString00, 0x44, 0x22);

    // 描画の完了
    SystemDisplayVideo();
  }

  // フレームとキー入力の監視
  if (gameFrame == 0x0000 || SystemIsInputEdge(A_BUTTON)) {

    // アプリケーションの更新
    AppSetState(APP_STATE_TITLE_INITIALIZE);
  }
}

/*
 * ヒット判定を行う
 */
static void GameHit(void)
{
  // 矩形の確保
  int8_t rect[GAME_RECT_SIZE];
  int8_t z;
    
  // 自弾との判定
  for (int8_t i = 0x00; i < SHOT_N; i++) {
    if (ShotGetRect(i, rect, &z)) {
      uint8_t result = EnemyHit(rect, z);
      if (result == ENEMY_HIT_DAMAGE) {
        ShotSetState(i, SHOT_STATE_NULL);
      } else if (result == ENEMY_HIT_INVINCIBLE) {
        ShotSetState(i, SHOT_STATE_REBOUND);
      }
    }
  }
  
  // プレイヤとの判定
  {
    if (PlayerGetRect(rect, &z)) {
      if (EnemyHit(rect, z) != ENEMY_HIT_NULL || BulletHit(rect, z)) {
        PlayerDamage();
      }          
    }
  }
}

/*
 * 距離における描画位置を取得する
 */
void GameGetDrawPosition(int8_t positionX, int8_t positionY, int8_t positionZ, int8_t *drawX, int8_t *drawY, int8_t *drawScale, int8_t *drawHeight)
{
  int8_t y = pgm_read_byte(gameDistanceYs + positionZ);
  if (drawX != NULL) {
    *drawX = (int16_t)positionX * y >> 5;
  }
  if (drawY != NULL) {
    *drawY = (int16_t)positionY * y >> 5;
  }
  if (drawScale != NULL) {
    *drawScale = pgm_read_byte(gameDistanceScales + positionZ);
  }
  if (drawHeight != NULL) {
    *drawHeight = y << 1;
  }
}

/*
 * 描画を登録する
 */
void GameEntryDraw(uint16_t draw, int8_t index, int8_t z)
{
  int8_t i = 0x00;
  while (z < (gameDraws[i] & 0x00ff) && i < GAME_DRAW_N - 0x01) {
    ++i;
  }
  for (int8_t j = GAME_DRAW_N - 0x01; j > i; j--) {
    gameDraws[j] = gameDraws[j - 0x01];
  }
  gameDraws[i] = draw | ((uint16_t)index << 8) | z;
}

/*
 * 描画順をリセットする
 */
static void GameResetDraw(void)
{
  for (int8_t i = 0x00; i < GAME_DRAW_N; i++) {
    gameDraws[i] = 0x0000;
  }
}

/*
 * 登録された順に描画する
 */
static void GameDraw(void)
{
  // ソートされた順に描画
  for (int8_t i = 0x00; i < GAME_DRAW_N; i++) {
    uint8_t index = (uint8_t)((gameDraws[i] >> 8) & 0x3f);

    // 種類別の描画
    switch (gameDraws[i] & 0xc000) {

    // 自弾の描画
    case GAME_DRAW_SHOT:
      ShotDraw(index);
      break;

    // エネミーの描画
    case GAME_DRAW_ENEMY:
      EnemyDraw(index);
      break;

    // 敵弾の描画
    case GAME_DRAW_BULLET:
      BulletDraw(index);
      break;

    // その他
    default:
      break;
    }
  }
}

/*
 * ステータスを描画する
 */
static void GameDrawStatus(void)
{
  // 体力の描画
  {
    uint8_t life = PlayerGetLife();
    uint8_t i = 0x00;
    while (i < life) {
      AppDrawPattern(0x11, i << 3, 0x00, 0x08, SYSTEM_VIDEO_FLIP_NORMAL);
      ++i;
    }
    while (i < PLAYER_LIFE) {
      AppClipPattern(0x10, 0x11, i << 3, 0x00, 0x08, SYSTEM_VIDEO_FLIP_NORMAL);
      ++i;
    }
  }

  // スコアの描画
  {
    AppDrawString(gameStringScore, 0x50, 0x00);
    AppDrawNumber(gameScore, 0x68, 0x00, 0x04);
  }
}

/*
 * スコアを加算する
 */
void GameAddScore(uint16_t score)
{
  if (APP_SCORE_MAX - gameScore < score) {
    gameScore = APP_SCORE_MAX;
  } else {
    gameScore += score;
  }
}

