/*
 * Enemy.cpp : エネミー
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

// エネミー
static struct Enemy enemys[ENEMY_N];

// エネミーデータ
static const struct EnemyData enemyDatas[ENEMY_TYPE_SIZE]  = {
  // 　　　　　     Lif Spd Hit Atk Spl  Cnd                 Rect               Ptn    Size
  /* 　　　　　 */ {  0,  0,  0,  0,  0, CONDITION_NULL,    { 0,  0,  0,  0, }, 0x00, { 0,  0, }, }, 
  /* オーク　　 */ {  4,  8,  4,  3,  0, CONDITION_NULL,    {-4, -4, +3, +3, }, 0x40, { 8,  8, }, },    // Lv1(Z1), Physical, Approach, Very Slow
  /* ゴーレム　 */ { 16,  6,  4,  7,  0, CONDITION_NULL,    {-4, -4, +3, +3, }, 0x42, { 8,  8, }, },    // Lv3(Z3), Physical, Random, Slow
  /* スケルトン */ {  3,  6,  6,  2,  0, CONDITION_NULL,    {-4, -4, +3, +3, }, 0x44, { 8,  8, }, },    // Lv1(Z1), Physical, Random, Slow
  /* リッチ　　 */ { 18,  6,  4,  8,  0, CONDITION_CONFUSE, {-4, -4, +3, +3, }, 0x46, { 8,  8, }, },    // Lv4(B3), Confuse, Random, Slow
  /* ゴースト　 */ {  9,  8,  4,  4,  0, CONDITION_NULL,    {-4, -4, +3, +3, }, 0x48, { 8,  8, }, },    // Lv2(B1), Physical, Hide, Very Slow
  /* ローグ　　 */ { 10,  4,  6,  5,  0, CONDITION_NULL,    {-4, -4, +3, +3, }, 0x4a, { 8,  8, }, },    // Lv2(Z2), Physical, Approach, Normal
  /* ナイト　　 */ { 24,  6,  3,  9,  0, CONDITION_NULL,    {-4, -4, +3, +3, }, 0x4c, { 8,  8, }, },    // Lv4(Z4), Physical, Approach, Slow
  /* メイジ　　 */ { 12,  0,  0,  0,  7, CONDITION_NULL,    {-4, -4, +3, +3, }, 0x4e, { 8,  8, }, },    // Lv3(Z3), Magic, Warp
  /* バット　　 */ {  2,  2,  5,  2,  0, CONDITION_NULL,    {-4, -4, +3, +3, }, 0x50, { 8,  8, }, },    // Lv1(Z1), Physical, Random, Very Fast
  /* パイソン　 */ { 15,  6,  4,  6,  0, CONDITION_POISON,  {-4, -4, +3, +3, }, 0x52, { 8,  8, }, },    // Lv3(Z3), Poison, Random, Slow
  /* スパイダー */ {  8,  6,  5,  4,  0, CONDITION_SLOW,    {-4, -4, +3, +3, }, 0x54, { 8,  8, }, },    // Lv2(B1), Slow, Random, Slow
  /* ゾーン　　 */ { 12,  0,  0,  0,  6, CONDITION_NULL,    {-4, -4, +3, +3, }, 0x56, { 8,  8, }, },    // Lv2(B2), Magic, Immobile
  /* スクイッド */ { 15,  8,  3,  6,  0, CONDITION_SLEEP,   {-4, -4, +3, +3, }, 0x58, { 8,  8, }, },    // Lv3(B2), Sleep, Approach, Slow
  /* ゲイザー　 */ { 18,  8,  3,  8,  0, CONDITION_BLIND,   {-4, -4, +3, +3, }, 0x5a, { 8,  8, }, },    // Lv4(B3), Blind, Approach, Very Slow
  /* リザード　 */ { 18,  6,  4,  8,  0, CONDITION_NULL,    {-4, -4, +3, +3, }, 0x5c, { 8,  8, }, },    // Lv3(B3), Physical, Approach, Slow
  /* デーモン　 */ { 20,  6,  4,  8, 10, CONDITION_NULL,    {-4, -4, +3, +3, }, 0x5e, { 8,  8, }, },    // Lv4(B3), Magic, Random, Slow
  /* ガーゴイル */ { 50,  6,  3,  4,  0, CONDITION_NULL,    {-8, -8, +7, +7, }, 0x60, {16, 16, }, },    // Lv1(B1), Physical, Approach, Slow
  /* リーパー　 */ { 80,  0,  0,  0, 10, CONDITION_NULL,    {-8, -8, +7, +7, }, 0x64, {16, 16, }, },    // Lv2(B2), Physical, Approach, Very Slow
  /* シャドウ　 */ {120,  0,  0,  0, 15, CONDITION_NULL,    {-8, -8, +7, +7, }, 0x68, {16, 16, }, },    // Lv3(B3), Magic, Warp
  /* ドラゴン　 */ {200,  6,  2,  9, 12, CONDITION_NULL,    {-8, -8, +7, +7, }, 0x6c, {16, 16, }, },    // Lv4(B4), Magic, Approach, Slow
  /* クリスタル */ {350,  0,  0,  1,  0, CONDITION_NULL,    {-4, -4, +3, +3, }, 0x80, { 8,  8, }, },    // Lv5(--), Physical, Immobile
};

//          Atk Def Spd Hit : 1-Hit : 4-Lif    Atk  : 1-Hit
// Zako Lv1   1   1   3   6 :    18 :     4   3(+2) :    36
// Boss Lv1   2   2   3   6 :    18 :     8   4(+2) :    36
// Zako Lv2   3   3   3   6 :    18 :    10   5(+2) :    36
// Boss Lv2   3   3   3   6 :    18 :    12   6(+3) :    36
// Zako Lv3   4   4   2   6 :    12 :    16   7(+3) :    24
// Boss Lv3   5   5   2   6 :    12 :    20   8(+3) :    24
// Zako Lv4   6   6   2   5 :    10 :    24   9(+3) :    20
// Boss Lv4   7   7   2   5 :    10 :    28  10(+3) :    20

// Very Slow 8
//      Slow 6
//    Normal 4
//      Fast 3
// Very Fast 2

// 移動
static const int8_t enemyMoves[]  = {
   0, -1, 
   0, +1, 
  -1,  0, 
  +1,  0, 
};

// スペル
static const int8_t enemyCasts_1[]  = {
   0, -7, 
   0,  6, 
  -7,  0,
   6,  0, 
};
static const int8_t enemyCasts_2[]  = {
   -4, -11,   4, -11, 
    4,  10,  -4,  10, 
  -11,   4, -11,  -4, 
   10,  -4,  10,   4, 
};


/*
 * 内部関数の宣言
 */
static void EnemyOrc(struct Enemy *enemy);
static void EnemyGolem(struct Enemy *enemy);
static void EnemySkelton(struct Enemy *enemy);
static void EnemyLiche(struct Enemy *enemy);
static void EnemyGhost(struct Enemy *enemy);
static void EnemyRogue(struct Enemy *enemy);
static void EnemyKnight(struct Enemy *enemy);
static void EnemyMage(struct Enemy *enemy);
static void EnemyBat(struct Enemy *enemy);
static void EnemyPython(struct Enemy *enemy);
static void EnemySpider(struct Enemy *enemy);
static void EnemyZorn(struct Enemy *enemy);
static void EnemySquid(struct Enemy *enemy);
static void EnemyGazer(struct Enemy *enemy);
static void EnemyLizard(struct Enemy *enemy);
static void EnemyDaemon(struct Enemy *enemy);
static void EnemyGargoyle(struct Enemy *enemy);
static void EnemyReaper(struct Enemy *enemy);
static void EnemyShadow(struct Enemy *enemy);
static void EnemyDragon(struct Enemy *enemy);
static void EnemyCrystal(struct Enemy *enemy);
static void EnemyStay(struct Enemy *enemy);
static void EnemyMove(struct Enemy *enemy, bool approach);
static bool EnemyWarp(struct Enemy *enemy);
static void EnemyDead(struct Enemy *enemy);
static bool EnemyIsCollision(struct Enemy *enemy, int8_t left, int8_t top, int8_t right, int8_t bottom);
static int8_t EnemyGetPlayerDirection(struct Enemy *enemy);
static int8_t EnemyGetTurnDirection(struct Enemy *enemy);
static void EnemyCast(struct Enemy *enemy, int8_t direction, int8_t attack, int8_t cast);


/*
 * エネミーを初期化する
 */
void EnemyInitialize(void)
{
  // エネミーの初期化
  for (int8_t i = 0; i < ENEMY_N; i++) {
    enemys[i].type = ENEMY_TYPE_NULL;
  }
}

/*
 * エネミーを更新する
 */
void EnemyUpdate(void)
{
  // エネミーの走査
  for (int8_t i = 0; i < ENEMY_N; i++) {

    // エネミーの存在
    if (enemys[i].type != ENEMY_TYPE_NULL) {
      
      // 状態別の処理
      switch (enemys[i].state & 0xf0) {
    
      // 待機
      case ENEMY_STATE_STAY:
        EnemyStay(&enemys[i]);
        break;

      // プレイ
      case ENEMY_STATE_PLAY:
        {
          // 種類別の処理
          switch (enemys[i].type) {
            
          // オーク
          case ENEMY_TYPE_ORC:
            EnemyOrc(&enemys[i]);
            break;
        
          // ゴーレム
          case ENEMY_TYPE_GOLEM:
            EnemyGolem(&enemys[i]);
            break;
        
          // スケルトン
          case ENEMY_TYPE_SKELTON:
            EnemySkelton(&enemys[i]);
            break;
        
          // リッチ
          case ENEMY_TYPE_LICH:
            EnemyLiche(&enemys[i]);
            break;
        
          // ゴースト
          case ENEMY_TYPE_GHOST:
            EnemyGhost(&enemys[i]);
            break;
        
          // ローグ
          case ENEMY_TYPE_ROGUE:
            EnemyRogue(&enemys[i]);
            break;
        
          // ナイト
          case ENEMY_TYPE_KNIGHT:
            EnemyKnight(&enemys[i]);
            break;
        
          // メイジ
          case ENEMY_TYPE_MAGE:
            EnemyMage(&enemys[i]);
            break;
        
          // バット
          case ENEMY_TYPE_BAT:
            EnemyBat(&enemys[i]);
            break;
        
          // パイソン
          case ENEMY_TYPE_PYTHON:
            EnemyPython(&enemys[i]);
            break;
        
          // スパイダー
          case ENEMY_TYPE_SPIDER:
            EnemySpider(&enemys[i]);
            break;
        
          // ゾーン
          case ENEMY_TYPE_ZORN:
            EnemyZorn(&enemys[i]);
            break;
        
          // スクイッド
          case ENEMY_TYPE_SQUID:
            EnemySquid(&enemys[i]);
            break;
        
          // ゲイザー
          case ENEMY_TYPE_GAZER:
            EnemyGazer(&enemys[i]);
            break;
        
          // リザード
          case ENEMY_TYPE_LIZARD:
            EnemyLizard(&enemys[i]);
            break;
        
          // デーモン
          case ENEMY_TYPE_DAEMON:
            EnemyDaemon(&enemys[i]);
            break;
        
          // ガーゴイル
          case ENEMY_TYPE_GARGOYLE:
            EnemyGargoyle(&enemys[i]);
            break;
        
          // リーパー
          case ENEMY_TYPE_REAPER:
            EnemyReaper(&enemys[i]);
            break;
        
          // シャドウ
          case ENEMY_TYPE_SHADOW:
            EnemyShadow(&enemys[i]);
            break;
        
          // ドラゴン
          case ENEMY_TYPE_DRAGON:
            EnemyDragon(&enemys[i]);
            break;
        
          // クリスタル
          case ENEMY_TYPE_CRYSTAL:
            EnemyCrystal(&enemys[i]);
            break;
        
          // その他
          default:
            break;
          }
  
          // ダメージの更新
          if (enemys[i].damage > 0) {
            --enemys[i].damage;
          }
        }
        break;

      // 死亡
      case ENEMY_STATE_DEAD:
        EnemyDead(&enemys[i]);
        break;
        
      // その他
      default:
        break;
      }
    }
  }
}

/*
 * エネミーを描画する
 */
void EnemyRender(void)
{
  // エネミーの走査
  for (int8_t i = 0; i < ENEMY_N; i++) {

    // エネミーの存在
    if (enemys[i].type != ENEMY_TYPE_NULL) {

      // データの取得
      struct EnemyData *data = (EnemyData *)&enemyDatas[enemys[i].type];

      // パターンの描画
      int8_t width = (int8_t)pgm_read_byte(&data->size.x);
      int8_t height = (int8_t)pgm_read_byte(&data->size.y);

      // ダメージの描画
      if (enemys[i].damage > 0) {
        for (int8_t y = 0; y < height; y += 8) {
          AppReversePattern(
            pgm_read_byte(&data->pattern) + y * (0x10 / 0x08) + ((enemys[i].animation >> 4) & 0x01) * (width / 8), 
            enemys[i].position.x + (int8_t)pgm_read_byte(&data->rect.left), 
            enemys[i].position.y + (int8_t)pgm_read_byte(&data->rect.top) + y, 
            width, 
            SYSTEM_VIDEO_FLIP_NORMAL
          );
        }

      // 通常の描画
      } else if ((enemys[i].flag & ENEMY_FLAG_HIDE) == 0) {
        for (int8_t y = 0; y < height; y += 8) {
          AppMaskPattern(
            pgm_read_byte(&data->pattern) + y * (0x10 / 0x08) + ((enemys[i].animation >> 4) & 0x01) * (width / 8), 
            enemys[i].mask, 
            enemys[i].position.x + (int8_t)pgm_read_byte(&data->rect.left), 
            enemys[i].position.y + (int8_t)pgm_read_byte(&data->rect.top) + y, 
            width, 
            SYSTEM_VIDEO_FLIP_NORMAL
          );
        }
      }
    }
  }
}

/*
 * ルームにエネミーを配置する
 */
void EnemyEncount(void)
{
  // エネミーの解放
  for (int8_t i = 0; i < ENEMY_N; i++) {
    enemys[i].type = ENEMY_TYPE_NULL;
  }

  // ルームの取得
  int8_t room = PlayerGetRoom();

  // 種類の取得
  int8_t type = DungeonGetEnemyType(room);
  if (type != ENEMY_TYPE_NULL) {

    // データの取得
    struct EnemyData *data = (EnemyData *)&enemyDatas[type];

    // エネミーの配置
    int8_t rest = DungeonGetEnemyRest(room);
    for (int8_t i = 0; i < rest; i++) {

      // 種類の設定
      enemys[i].type = type;
      
      // 状態の設定
      enemys[i].state = ENEMY_STATE_PLAY;

      // フラグの設定
      enemys[i].flag = ENEMY_FLAG_NULL;

      // 位置の設定
      DungeonGetEnemyLocation(i, rest, &enemys[i].position.x, &enemys[i].position.y);

      // 向きの設定
      enemys[i].direction = ENEMY_DIRECTION_DOWN;

      // 体力の設定
      enemys[i].life = (int16_t)pgm_read_word(&data->life);

      // 速度の設定
      enemys[i].speed = 0;

      // ヒットの設定
      enemys[i].hit = 0;

      // ダメージの設定
      enemys[i].damage = 0;

      // アニメーションの設定
      enemys[i].animation = SystemGetRandom();

      // マスクの設定
      enemys[i].mask = 0xff;
    }
  }
}

/*
 * オークを更新する
 */
static void EnemyOrc(struct Enemy *enemy)
{
  // 初期化
  if ((enemy->state & 0x0f) == 0x00) {

    // パラメータの設定
    enemy->params[ENEMY_PARAM_0] = 0x08;
    enemy->params[ENEMY_PARAM_1] = 0x0f;

    // 状態の更新／EnemyMove() で更新される
    // ++enemys->state;
  }

  // プレイヤに接近
  EnemyMove(enemy, true);
}

/*
 * ゴーレムを更新する
 */
static void EnemyGolem(struct Enemy *enemy)
{
  // 初期化
  if ((enemy->state & 0x0f) == 0x00) {

    // パラメータの設定
    enemy->params[ENEMY_PARAM_0] = 0x08;
    enemy->params[ENEMY_PARAM_1] = 0x1f;

    // 状態の更新／EnemyMove() で更新される
    // ++enemys->state;
  }

  // ランダムに移動
  EnemyMove(enemy, false);
}

/*
 * スケルトンを更新する
 */
static void EnemySkelton(struct Enemy *enemy)
{
  // 初期化
  if ((enemy->state & 0x0f) == 0x00) {

    // パラメータの設定
    enemy->params[ENEMY_PARAM_0] = 0x08;
    enemy->params[ENEMY_PARAM_1] = 0x1f;

    // 状態の更新／EnemyMove() で更新される
    // ++enemys->state;
  }

  // ランダムに移動
  EnemyMove(enemy, false);
}

/*
 * リッチを更新する
 */
static void EnemyLiche(struct Enemy *enemy)
{
  // 初期化
  if ((enemy->state & 0x0f) == 0x00) {

    // パラメータの設定
    enemy->params[ENEMY_PARAM_0] = 0x08;
    enemy->params[ENEMY_PARAM_1] = 0x1f;

    // 状態の更新／EnemyMove() で更新される
    // ++enemys->state;
  }

  // ランダムに移動
  EnemyMove(enemy, false);
}

/*
 * ゴーストを更新する
 */
static void EnemyGhost(struct Enemy *enemy)
{
  // 初期化
  if ((enemy->state & 0x0f) == 0x00) {

    // パラメータの設定
    enemy->params[ENEMY_PARAM_0] = 0x08;
    enemy->params[ENEMY_PARAM_1] = 0x1f;

    // 状態の更新／EnemyMove() で更新される
    // ++enemys->state;
  }

  // ランダムに移動
  EnemyMove(enemy, false);

  // 一定時間隠れる
  if (enemy->animation < 0xd0 && !PlayerIsItem(ITEM_CANDLE)) {
    enemy->flag |= ENEMY_FLAG_HIDE;
  } else {
    enemy->flag &= ~ENEMY_FLAG_HIDE;
  }
}

/*
 * ローグを更新する
 */
static void EnemyRogue(struct Enemy *enemy)
{
  // 初期化
  if ((enemy->state & 0x0f) == 0x00) {

    // パラメータの設定
    enemy->params[ENEMY_PARAM_0] = 0x08;
    enemy->params[ENEMY_PARAM_1] = 0x0f;

    // 状態の更新／EnemyMove() で更新される
    // ++enemys->state;
  }

  // プレイヤに接近
  EnemyMove(enemy, true);
}

/*
 * ナイトを更新する
 */
static void EnemyKnight(struct Enemy *enemy)
{
  // 初期化
  if ((enemy->state & 0x0f) == 0x00) {

    // パラメータの設定
    enemy->params[ENEMY_PARAM_0] = 0x08;
    enemy->params[ENEMY_PARAM_1] = 0x0f;

    // 状態の更新／EnemyMove() で更新される
    // ++enemys->state;
  }

  // プレイヤに接近
  EnemyMove(enemy, true);
}

/*
 * メイジを更新する
 */
static void EnemyMage(struct Enemy *enemy)
{
  // 初期化
  if ((enemy->state & 0x0f) == 0x00) {

    // パラメータの設定
    enemy->params[ENEMY_PARAM_0] = (SystemGetRandom() & 0x3f) + 0x60;
    enemy->params[ENEMY_PARAM_1] = enemy->params[ENEMY_PARAM_0] - 0x20;

    // 状態の更新／EnemyMove() で更新される
    // ++enemys->state;
  }

  // ワープ
  if (EnemyWarp(enemy)) {

    // スペルを撃つ
    if (PlayerIsLive()) {
      EnemyCast(enemy, EnemyGetPlayerDirection(enemy), (int8_t)pgm_read_byte(&enemyDatas[enemy->type].spell), ENEMY_CAST_1);
    }
  }
}

/*
 * バットを更新する
 */
static void EnemyBat(struct Enemy *enemy)
{
  // 初期化
  if ((enemy->state & 0x0f) == 0x00) {

    // パラメータの設定
    enemy->params[ENEMY_PARAM_0] = 0x04;
    enemy->params[ENEMY_PARAM_1] = 0x0f;

    // 状態の更新／EnemyMove() で更新される
    // ++enemys->state;
  }

  // ランダムに移動
  EnemyMove(enemy, false);
}

/*
 * パイソンを更新する
 */
static void EnemyPython(struct Enemy *enemy)
{
  // 初期化
  if ((enemy->state & 0x0f) == 0x00) {

    // パラメータの設定
    enemy->params[ENEMY_PARAM_0] = 0x08;
    enemy->params[ENEMY_PARAM_1] = 0x1f;

    // 状態の更新／EnemyMove() で更新される
    // ++enemys->state;
  }

  // プレイヤに接近
  EnemyMove(enemy, false);
}

/*
 * スパイダーを更新する
 */
static void EnemySpider(struct Enemy *enemy)
{
  // 初期化
  if ((enemy->state & 0x0f) == 0x00) {

    // パラメータの設定
    enemy->params[ENEMY_PARAM_0] = 0x08;
    enemy->params[ENEMY_PARAM_1] = 0x1f;

    // 状態の更新／EnemyMove() で更新される
    // ++enemys->state;
  }

  // ランダムに移動
  EnemyMove(enemy, false);
}

/*
 * ゾーンを更新する
 */
static void EnemyZorn(struct Enemy *enemy)
{
  // 初期化
  if ((enemy->state & 0x0f) == 0x00) {

    // パラメータの設定
    enemy->params[ENEMY_PARAM_3] = (SystemGetRandom() & 0x3f) + 0x40;

    // 状態の更新
    ++enemy->state;
  }

  // スペルを撃つ
  if (enemy->params[ENEMY_PARAM_3] > 0) {
    --enemy->params[ENEMY_PARAM_3];
  } else if (PlayerIsLive()) {
    EnemyCast(enemy, EnemyGetPlayerDirection(enemy), (int8_t)pgm_read_byte(&enemyDatas[enemy->type].spell), ENEMY_CAST_1);
    enemy->state &= 0xf0;
  }

  // アニメーションの更新
  ++enemy->animation;
}

/*
 * スクイッドを更新する
 */
static void EnemySquid(struct Enemy *enemy)
{
  // 初期化
  if ((enemy->state & 0x0f) == 0x00) {

    // パラメータの設定
    enemy->params[ENEMY_PARAM_0] = 0x08;
    enemy->params[ENEMY_PARAM_1] = 0x0f;

    // 状態の更新／EnemyMove() で更新される
    // ++enemys->state;
  }

  // プレイヤに接近
  EnemyMove(enemy, true);
}

/*
 * ゲイザーを更新する
 */
static void EnemyGazer(struct Enemy *enemy)
{
  // 初期化
  if ((enemy->state & 0x0f) == 0x00) {

    // パラメータの設定
    enemy->params[ENEMY_PARAM_0] = 0x08;
    enemy->params[ENEMY_PARAM_1] = 0x0f;

    // 状態の更新／EnemyMove() で更新される
    // ++enemys->state;
  }

  // プレイヤに接近
  EnemyMove(enemy, true);
}

/*
 * リザードを更新する
 */
static void EnemyLizard(struct Enemy *enemy)
{
  // 初期化
  if ((enemy->state & 0x0f) == 0x00) {

    // パラメータの設定
    enemy->params[ENEMY_PARAM_0] = 0x08;
    enemy->params[ENEMY_PARAM_1] = 0x0f;

    // 状態の更新／EnemyMove() で更新される
    // ++enemys->state;
  }

  // ランダムに移動
  EnemyMove(enemy, true);
}

/*
 * デーモンを更新する
 */
static void EnemyDaemon(struct Enemy *enemy)
{
  // 初期化
  if ((enemy->state & 0x0f) == 0x00) {

    // パラメータの設定
    enemy->params[ENEMY_PARAM_0] = 0x08;
    enemy->params[ENEMY_PARAM_1] = 0x1f;
    enemy->params[ENEMY_PARAM_3] = (SystemGetRandom() & 0x3f) + 0x60;

    // 状態の更新／EnemyMove() で更新される
    // ++enemys->state;
  }

  // ランダムに移動
  EnemyMove(enemy, false);
  
  // スペルを撃つ
  if (enemy->params[ENEMY_PARAM_3] > 0) {
    --enemy->params[ENEMY_PARAM_3];
  } else if (PlayerIsLive()) {
    EnemyCast(enemy, enemy->direction, (int8_t)pgm_read_byte(&enemyDatas[enemy->type].spell), ENEMY_CAST_1);
    enemy->params[ENEMY_PARAM_3] = (SystemGetRandom() & 0x3f) + 0x60;
  }
  
}

/*
 * ガーゴイルを更新する
 */
static void EnemyGargoyle(struct Enemy *enemy)
{
  // 初期化
  if ((enemy->state & 0x0f) == 0x00) {

    // パラメータの設定
    enemy->params[ENEMY_PARAM_0] = 0x08;
    enemy->params[ENEMY_PARAM_1] = 0x0f;

    // 状態の更新／EnemyMove() で更新される
    // ++enemys->state;
  }

  // プレイヤに接近
  EnemyMove(enemy, true);
}

/*
 * リーパーを更新する
 */
static void EnemyReaper(struct Enemy *enemy)
{
  // 初期化
  if ((enemy->state & 0x0f) == 0x00) {

    // パラメータの設定
    enemy->params[ENEMY_PARAM_3] = (SystemGetRandom() & 0x3f) + (SystemGetRandom() & 0x1f) + 0x20;

    // 状態の更新
    ++enemys->state;
  }

  // スペルを撃つ
  if (enemy->params[ENEMY_PARAM_3] > 0) {
    --enemy->params[ENEMY_PARAM_3];
  } else if (PlayerIsLive()) {
    EnemyCast(enemy, EnemyGetPlayerDirection(enemy), (int8_t)pgm_read_byte(&enemyDatas[enemy->type].spell), ENEMY_CAST_2);
    enemy->state &= 0xf0;
  }

  // アニメーションの更新
  ++enemy->animation;
}

/*
 * シャドウを更新する
 */
static void EnemyShadow(struct Enemy *enemy)
{
  // 初期化
  if ((enemy->state & 0x0f) == 0x00) {

    // パラメータの設定
    enemy->params[ENEMY_PARAM_0] = (SystemGetRandom() & 0x1f) + 0x30;
    enemy->params[ENEMY_PARAM_1] = enemy->params[ENEMY_PARAM_0] - 0x20;

    // 状態の更新／EnemyMove() で更新される
    // ++enemys->state;
  }

  // ワープ
  if (EnemyWarp(enemy)) {

    // スペルを撃つ
    if (PlayerIsLive()) {
      int8_t px, py;
      PlayerGetPosition(&px, &py);
      int8_t spell = (int8_t)pgm_read_byte(&enemyDatas[enemy->type].spell);
      EnemyCast(enemy, py < enemy->position.y ? ENEMY_DIRECTION_UP : ENEMY_DIRECTION_DOWN, spell, ENEMY_CAST_2);
      EnemyCast(enemy, px < enemy->position.x ? ENEMY_DIRECTION_LEFT : ENEMY_DIRECTION_RIGHT, spell, ENEMY_CAST_2);
    }
  }
}

/*
 * ドラゴンを更新する
 */
static void EnemyDragon(struct Enemy *enemy)
{
  // 初期化
  if ((enemy->state & 0x0f) == 0x00) {

    // パラメータの設定
    enemy->params[ENEMY_PARAM_0] = 0x10;
    enemy->params[ENEMY_PARAM_1] = 0x0f;
    enemy->params[ENEMY_PARAM_3] = 0x60;

    // 状態の更新／EnemyMove() で更新される
    // ++enemys->state;
  }

  // プレイヤに接近
  EnemyMove(enemy, true);
  
  // スペルを撃つ
  if (enemy->params[ENEMY_PARAM_3] > 0) {
    --enemy->params[ENEMY_PARAM_3];
  } else if (PlayerIsLive()) {
    EnemyCast(enemy, EnemyGetPlayerDirection(enemy), (int8_t)pgm_read_byte(&enemyDatas[enemy->type].spell), ENEMY_CAST_2);
    enemy->params[ENEMY_PARAM_3] = (SystemGetRandom() & 0x1f) + 0x60;
  }
}

/*
 * クリスタルを更新する
 */
static void EnemyCrystal(struct Enemy *enemy)
{
  // データの取得
  struct EnemyData *data = (EnemyData *)&enemyDatas[enemy->type];
  
  // 初期化
  if ((enemy->state & 0x0f) == 0x00) {

    // パラメータの設定
    enemy->params[ENEMY_PARAM_3] = 0x0b;

    // アニメーションの設定
    enemy->animation = 0;

    // 状態の更新
    ++enemys->state;
  }

  // クリスタルを持っている
  if (PlayerIsItem(ITEM_CRYSTAL)) {

    // プレイヤへの攻撃
    if (PlayerIsLive()) {
      if (--enemy->params[ENEMY_PARAM_3] == 0) {
        PlayerIsHit(
          enemy->position.x + (int8_t)pgm_read_byte(&data->rect.left) - 0x01, 
          enemy->position.y + (int8_t)pgm_read_byte(&data->rect.top) - 0x01, 
          enemy->position.x + (int8_t)pgm_read_byte(&data->rect.right) + 0x01, 
          enemy->position.y + (int8_t)pgm_read_byte(&data->rect.bottom) + 0x01, 
          (int8_t)pgm_read_byte(&data->attack), 
          (int8_t)pgm_read_byte(&data->condition)
        );
        enemy->params[ENEMY_PARAM_3] = 0x0b;
      }
    }

    // アニメーションの更新
    ++enemy->animation;

  // クリスタルを持っていない
  } else {
    
    // プレイヤの回復
    int16_t life = (int16_t)pgm_read_word(&data->life);
    if (enemy->life < life) {
      enemy->life = life;
      PlayerHeal(1);
    }
  }
}

/*
 * エネミーが待機する
 */
static void EnemyStay(struct Enemy *enemy)
{
  // 初期化
  if ((enemy->state & 0x0f) == 0x00) {

    // ダメージの設定
    enemy->damage = 0;

    // 状態の更新
    ++enemy->state;
  }

  // アニメーションの更新
  ++enemy->animation;
}

/*
 * エネミーが移動する
 */
static void EnemyMove(struct Enemy *enemy, bool approach)
{
  // データの取得
  struct EnemyData *data = (EnemyData *)&enemyDatas[enemy->type];
  
  // 初期化
  if ((enemy->state & 0x0f) == 0x00) {

    // 向きの設定
    enemy->direction = approach ? EnemyGetPlayerDirection(enemy) : ((SystemGetRandom() >> 3) & 0x03);

    // 移動量の設定
    enemy->params[ENEMY_PARAM_2] = (SystemGetRandom() & enemy->params[ENEMY_PARAM_1]) + enemy->params[ENEMY_PARAM_0];

    // 状態の更新
    ++enemy->state;
  }

  // 移動
  if (++enemy->speed > (int8_t)pgm_read_byte(&data->speed)) {

    // 移動量の取得
    int8_t mx = (int8_t)pgm_read_byte(&enemyMoves[enemy->direction * 0x02 + 0x00]);
    int8_t my = (int8_t)pgm_read_byte(&enemyMoves[enemy->direction * 0x02 + 0x01]);

    // 矩形の取得
    int8_t left = enemy->position.x + (int8_t)pgm_read_byte(&data->rect.left) + mx;
    int8_t top = enemy->position.y + (int8_t)pgm_read_byte(&data->rect.top) + my;
    int8_t right = enemy->position.x + (int8_t)pgm_read_byte(&data->rect.right) + mx;
    int8_t bottom = enemy->position.y + (int8_t)pgm_read_byte(&data->rect.bottom) + my;

    // 壁や他のエネミーとのコリジョン判定
    if (
      !DungeonIsCollision(left, top, right, bottom) && 
      !EnemyIsCollision(enemy, left, top, right, bottom)
    ) {
      
      // 攻撃力の取得
      int8_t attack = 0;
      if (++enemy->hit >= (int8_t)pgm_read_byte(&data->hit)) {
        attack = (int8_t)pgm_read_byte(&data->attack);
        enemy->hit = 0;
      }

      // プレイヤとのヒット判定
      if (!PlayerIsHit(left, top, right, bottom, attack, (int8_t)pgm_read_byte(&data->condition))) {
        enemy->position.x += mx;
        enemy->position.y += my;
      }

    // 方向転換
    } else {
      enemy->direction = EnemyGetTurnDirection(enemy);
    }

    // 移動量の設定
    if (--enemy->params[ENEMY_PARAM_2] <= 0) {
      uint8_t rate = SystemGetRandom(); 
      if ((approach && rate < 0xf0) || (!approach && rate < 0x40)) {
        enemy->direction = EnemyGetPlayerDirection(enemy);
      } else {
        enemy->direction = (SystemGetRandom() >> 3) & 0x03;
      }
      enemy->params[ENEMY_PARAM_2] = (SystemGetRandom() & enemy->params[ENEMY_PARAM_1]) + enemy->params[ENEMY_PARAM_0];
    }

    // 速度の設定
    enemy->speed = 0;
  }

  // アニメーションの更新
  ++enemy->animation;
}

/*
 * エネミーがワープする
 */
static bool EnemyWarp(struct Enemy *enemy)
{
  // 結果の初期化
  bool result = false;
  
  // データの取得
  struct EnemyData *data = (EnemyData *)&enemyDatas[enemy->type];
  
  // 初期化
  if ((enemy->state & 0x0f) == 0x00) {

      // 退避
      enemy->position.x = -0x40;
      enemy->params[ENEMY_PARAM_2] = (SystemGetRandom() & 0x3f) + 0x01;
    
    // 状態の更新
    ++enemy->state;
  }

  // 退避
  if ((enemy->state & 0x0f) == 0x01) {

    // 退避の完了
    if (--enemy->params[ENEMY_PARAM_2] == 0) {
      
      // 位置の取得
      if (pgm_read_byte(&data->size.x) <= 0x08) {
        uint8_t p[8] = {0xff, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0xff, };
        {
          int8_t px, py;
          PlayerGetPosition(&px, &py);
          p[(py + PLAYER_RECT_TOP) >> 3] |= (0x80 >> ((px + PLAYER_RECT_LEFT) >> 3)) | (0x80 >> ((px + PLAYER_RECT_RIGHT) >> 3));
          p[(py + PLAYER_RECT_BOTTOM) >> 3] |= (0x80 >> ((px + PLAYER_RECT_LEFT) >> 3)) | (0x80 >> ((px + PLAYER_RECT_RIGHT) >> 3));
        }
        {
          for (int8_t i = 0; i < ENEMY_N; i++) {
            if (enemys[i].type != ENEMY_TYPE_NULL) {
              p[enemys[i].position.y >> 3] |= (0x80 >> (enemys[i].position.x >> 3));
            }
          }
        }
        {
          uint8_t count = ((SystemGetRandom() >> 1) & 0x3f) + 0x01;
          int8_t x = 0;
          int8_t y = 0;
          do {
            do {
              if (++x >= 8) {
                if (++y >= 8) {
                  y = 0;
                }
                x = 0;
              }
            } while ((p[y] & (0x80 >> x)) != 0);
          } while (--count > 0);
          enemy->position.x = x * 0x08 + 0x04;
          enemy->position.y = y * 0x08 + 0x04;
        }
      } else {
        int8_t px, py;
        PlayerGetPosition(&px, &py);
        do {
          enemy->position.x = ((SystemGetRandom() >> 1) & 0x1f) + (SystemGetRandom() & 0x07) + 0x0c;
          enemy->position.y = ((SystemGetRandom() >> 1) & 0x1f) + (SystemGetRandom() & 0x07) + 0x0c;
        } while (abs(enemy->position.x - px) < 0x10 && abs(enemy->position.y - py) < 0x10);
      }
        
      // パラメータの設定
      enemy->params[ENEMY_PARAM_2] = 0x30;
  
      // 状態の更新
      ++enemy->state;
    }

    // 常に隠れる
    enemy->flag |= ENEMY_FLAG_HIDE;
  }

  // 出現
  if ((enemy->state & 0x0f) == 0x02) {

    // 出現の完了
    if (--enemy->params[ENEMY_PARAM_2] == 0) {
      enemy->params[ENEMY_PARAM_2] = enemy->params[ENEMY_PARAM_0];
      ++enemy->state;
    }

    // 点滅
    if ((enemy->params[ENEMY_PARAM_2] & 0x04) != 0) {
      enemy->flag |= ENEMY_FLAG_HIDE;
    } else {
      enemy->flag &= ~ENEMY_FLAG_HIDE;
    }
  }

  // 待機
  if ((enemy->state & 0x0f) == 0x03) {
    
    // 待機の完了
    if (--enemy->params[ENEMY_PARAM_2] == 0) {
      enemy->params[ENEMY_PARAM_2] = 0x30;
      ++enemy->state;

    // 攻撃タイミング
    } else if (enemy->params[ENEMY_PARAM_2] == enemy->params[ENEMY_PARAM_1]) {
      result = true;
    }

    // 常に現れる
    enemy->flag &= ~ENEMY_FLAG_HIDE;
  }

  // 消滅
  if ((enemy->state & 0x0f) == 0x04) {

    // 消滅の完了
    if (--enemy->params[ENEMY_PARAM_2] == 0) {
      enemy->position.x = -0x40;
      enemy->params[ENEMY_PARAM_2] = 0x20;
      enemy->state &= 0xf0;
    }

    // 点滅
    if ((enemy->params[ENEMY_PARAM_2] & 0x04) != 0) {
      enemy->flag |= ENEMY_FLAG_HIDE;
    } else {
      enemy->flag &= ~ENEMY_FLAG_HIDE;
    }
  }

  // アニメーションの更新
  ++enemy->animation;

  // 終了
  return result;
}

/*
 * エネミーが死亡する
 */
static void EnemyDead(struct Enemy *enemy)
{
  // 初期化
  if ((enemy->state & 0x0f) == 0x00) {

    // フラグの設定
    enemy->flag &= ~(ENEMY_FLAG_HIDE);

    // アニメーションの設定
    enemy->animation = ENEMY_DEAD;

    // 状態の更新
    ++enemy->state;
  }

  // ダメージの更新
  if (enemy->damage > 0) {
    --enemy->damage;

  // アニメーションの更新
  } else if (++enemy->animation >= ENEMY_DEAD) {

    // マスクの更新
    enemy->mask = (enemy->mask >> 1) & 0x77;

    // 死亡の継続
    if (enemy->mask > 0x00) {
      
      // アニメーションの設定
      enemy->animation = 0;
      
    // 死亡の完了
    } else {

      // ルームの取得
      int8_t room = PlayerGetRoom();

      // エネミーの減少
      if (DungeonKillEnemy(room) == 0) {
        
        // アイテムの出現
        DungeonFindItem(room);
      }

      // エネミーの削除
      enemy->type = ENEMY_TYPE_NULL;
    }
  }
}

/*
 * エネミーとのコリジョンを判定する
 */
static bool EnemyIsCollision(struct Enemy *enemy, int8_t left, int8_t top, int8_t right, int8_t bottom)
{
  bool result = false;
  for (int8_t i = 0; i < ENEMY_N; i++) {
    if (&enemys[i] != enemy && enemys[i].type != ENEMY_TYPE_NULL && enemys[i].life > 0) {
      struct EnemyData *data = (EnemyData *)&enemyDatas[enemys[i].type];
      if (
        left <= enemys[i].position.x + (int8_t)pgm_read_byte(&data->rect.right) && 
        top <= enemys[i].position.y + (int8_t)pgm_read_byte(&data->rect.bottom) && 
        right >= enemys[i].position.x + (int8_t)pgm_read_byte(&data->rect.left) && 
        bottom >= enemys[i].position.y + (int8_t)pgm_read_byte(&data->rect.top)
      ) {
        result = true;
      }
    }
  }
  return result;
}
bool EnemyIsCollision(int8_t left, int8_t top, int8_t right, int8_t bottom)
{
  return EnemyIsHit(left, top, right, bottom, 0);
}

/*
 * エネミーとのヒットを判定する
 */
bool EnemyIsHit(int8_t left, int8_t top, int8_t right, int8_t bottom, int8_t attack)
{
  bool result = false;
  for (int8_t i = 0; i < ENEMY_N; i++) {
    if (enemys[i].type != ENEMY_TYPE_NULL) {
      struct EnemyData *data = (EnemyData *)&enemyDatas[enemys[i].type];
      if (
        left <= enemys[i].position.x + (int8_t)pgm_read_byte(&data->rect.right) && 
        top <= enemys[i].position.y + (int8_t)pgm_read_byte(&data->rect.bottom) && 
        right >= enemys[i].position.x + (int8_t)pgm_read_byte(&data->rect.left) && 
        bottom >= enemys[i].position.y + (int8_t)pgm_read_byte(&data->rect.top)
      ) {
        if (attack > 0 && enemys[i].life > 0) {
          if (enemys[i].life > attack) {
            enemys[i].life -= attack;
          } else {
            enemys[i].life = 0;
            enemys[i].state = ENEMY_STATE_DEAD;
          }
          enemys[i].damage = ENEMY_DAMAGE;
        }
        result = true;
      }
    }
  }
  return result;
}

/*
 * プレイヤの方向を取得する
 */
static int8_t EnemyGetPlayerDirection(struct Enemy *enemy)
{
  // 向きの初期化
  int8_t direction = ENEMY_DIRECTION_UP;
    
  // プレイヤの存在
  if (PlayerIsLive()) {
    
    // プレイヤの位置の取得
    int8_t x, y;
    PlayerGetPosition(&x, &y);
  
    // 方向の取得
    x = x - enemy->position.x;
    y = y - enemy->position.y;
    if (abs(y) > abs(x)) {
      direction = y < 0 ? ENEMY_DIRECTION_UP : ENEMY_DIRECTION_DOWN;
    } else {
      direction = x < 0 ? ENEMY_DIRECTION_LEFT : ENEMY_DIRECTION_RIGHT;
    }

  // ランダムな方向
  } else {
    direction = (SystemGetRandom() >> 1) & 0x03;
  }

  // 終了
  return direction;
}

/*
 * 現在向いている方向以外の方向を取得する
 */
static int8_t EnemyGetTurnDirection(struct Enemy *enemy)
{
  int8_t direction = ENEMY_DIRECTION_UP;
  do {
    direction = SystemGetRandom() & 0x03;
  } while (direction == enemy->direction);
  return direction;
}

/*
 * スペルを撃つ
 */
static void EnemyCast(struct Enemy *enemy, int8_t direction, int8_t attack, int8_t cast)
{
  if (cast == ENEMY_CAST_1) {
    int8_t *p = (int8_t *)&enemyCasts_1[direction * 0x02];
    SpellCast(enemy->position.x + (int8_t)pgm_read_byte(p + 0x00), enemy->position.y + (int8_t)pgm_read_byte(p + 0x01), direction, attack);
  } else if (cast == ENEMY_CAST_2) {
    int8_t *p = (int8_t *)&enemyCasts_2[direction * 0x04];
    SpellCast(enemy->position.x + (int8_t)pgm_read_byte(p + 0x00), enemy->position.y + (int8_t)pgm_read_byte(p + 0x01), direction, attack);
    SpellCast(enemy->position.x + (int8_t)pgm_read_byte(p + 0x02), enemy->position.y + (int8_t)pgm_read_byte(p + 0x03), direction, attack);
  } else {
    int8_t *p = (int8_t *)&enemyCasts_2[direction * 0x04 + (SystemGetRandom() & 0x02)];
    SpellCast(enemy->position.x + (int8_t)pgm_read_byte(p + 0x00), enemy->position.y + (int8_t)pgm_read_byte(p + 0x01), direction, attack);
  }
}
