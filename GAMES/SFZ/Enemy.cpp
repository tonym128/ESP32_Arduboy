/*
 * Enemy.cpp : エネミー
 */

/*
 * 参照ファイル
 */
#include "System.h"
#include "App.h"
#include "Game.h"
#include "Player.h"
#include "Enemy.h"
#include "Bullet.h"


/*
 * 変数の定義
 */

// エネミー
static struct Enemy enemys[ENEMY_N];

// ジェネレータ
static struct EnemyGenerator enemyGenerator;
static const uint8_t enemyGenerateTypes[] PROGMEM = {
  ENEMY_TYPE_ONYANMA, 
  ENEMY_TYPE_ONYANMA, 
  ENEMY_TYPE_ONYANMA, 
  ENEMY_TYPE_JURII, 
  ENEMY_TYPE_JURII, 
  ENEMY_TYPE_DORARINFURAS, 
  ENEMY_TYPE_DORARINFURAS, 
  ENEMY_TYPE_POLE, 
};

// パターン
static const uint8_t enemyPolePatterns[] PROGMEM = {
  0x66, 0xfc, 0x08, 0x00, 
  0x65, 0xfc, 0x08, 0x00, 
  0x63, 0xf8, 0x10, 0x00, 
  0x60, 0xf4, 0x18, 0x00, 
  
};
static const uint8_t enemyDorarinfurasPatterns[] PROGMEM = {
  0x8f, 0xf8, 0xfc, 0x08, 0x9f, 0x00, 0xfc, 0x08, 
  0x8d, 0xf8, 0xf8, 0x10, 0x9d, 0xf8, 0x00, 0x10, 
  0x8a, 0xf4, 0xf8, 0x18, 0x9a, 0xf4, 0x00, 0x18, 
  0x87, 0xf4, 0xf8, 0x18, 0x97, 0xf4, 0x00, 0x18, 
};
static const uint8_t enemyJuriiPatterns[] PROGMEM = {
  0xc7, 0xfc, 0xfc, 0x08, 0xc7, 0xfc, 0xfc, 0x08, 
  0xc5, 0xf8, 0xf8, 0x10, 0xd5, 0xf8, 0x00, 0x10, 
  0xc3, 0xf8, 0xf8, 0x10, 0xd3, 0xf8, 0x00, 0x10, 
  0xc0, 0xf4, 0xf8, 0x18, 0xd0, 0xf4, 0x00, 0x18, 
};
static const uint8_t enemyOnyanmaPatterns[] PROGMEM = {
  0xcf, 0xf8, 0xfc, 0x08, 0xdf, 0x00, 0xfc, 0x08, 
  0xcd, 0xf8, 0xf8, 0x10, 0xdd, 0xf8, 0x00, 0x10, 
  0xcb, 0xf8, 0xf8, 0x10, 0xdb, 0xf8, 0x00, 0x10, 
  0xc8, 0xf4, 0xf8, 0x18, 0xd8, 0xf4, 0x00, 0x18, 
};
static const uint8_t enemyBombPatterns[] PROGMEM = {
  0x01, 0x6f, 0x7f, 0xfc, 0xfc, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x01, 0x6f, 0x7f, 0xfc, 0xfc, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x02, 0x6b, 0x6d, 0xf8, 0xf8, 0x10, 0x7b, 0x7d, 0xf8, 0x00, 0x10, 
  0x02, 0x67, 0x69, 0xf8, 0xf8, 0x10, 0x77, 0x79, 0xf8, 0x00, 0x10, 
};

// 矩形
static const uint8_t enemyRects[] PROGMEM = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0xfe, 0x01, 0x00, 0x00, 0xfd, 0x02, 0x00, 0x00, 0xf9, 0x06, 0x00, 0x00, 0xf7, 0x08, 0x00, 0x00, // 柱
  0xfc, 0x04, 0xfd, 0x02, 0xfa, 0x06, 0xfc, 0x04, 0xf7, 0x09, 0xfa, 0x05, 0xf5, 0x0b, 0xf8, 0x07, // ドラリンフラー
  0xfd, 0x02, 0xfd, 0x02, 0xfb, 0x05, 0xfb, 0x05, 0xfa, 0x06, 0xfa, 0x06, 0xf7, 0x09, 0xf8, 0x07, // ジェリイ
  0xfb, 0x04, 0xfe, 0x02, 0xfa, 0x05, 0xfc, 0x03, 0xf8, 0x07, 0xfa, 0x05, 0xf6, 0x0a, 0xf8, 0x07, // オニャンマ
  0xfa, 0x05, 0x01, 0x09, 0xfa, 0x05, 0x01, 0x09, 0xfa, 0x05, 0x01, 0x09, 0xfa, 0x05, 0x01, 0x09, // スタンパロン（コア）
  0xf2, 0x0d, 0xf0, 0x0f, 0xf2, 0x0d, 0xf0, 0x0f, 0xf2, 0x0d, 0xf0, 0x0f, 0xf2, 0x0d, 0xf0, 0x0f, // スタンパロン（ボディ）
};

// 体力
static const uint8_t enemyLifes[] PROGMEM = {
  0x00, 
  ENEMY_LIFE_INVINCIBLE, 
  0x01, 
  0x01, 
  0x01, 
  0x20, 
  ENEMY_LIFE_INVINCIBLE, 
};

// スコア
static const uint8_t enemyScores[] PROGMEM = {
  0x00, 
  0x00, 
  0x02, 
  0x01, 
  0x01, 
  0x32, 
  0x00, 
};


/*
 * 内部関数の宣言
 */
static struct Enemy *EnemyGetEmpty(void);
static void EnemyCalcRect(struct Enemy *enemy, int8_t *rect);
static void EnemyDrawZako(struct Enemy *enemy, uint8_t *p);
static void EnemyGeneratePole(void);
static void EnemyUpdatePole(struct Enemy *enemy);
static void EnemyDrawPole(struct Enemy *enemy);
static void EnemyGenerateDorarinfuras(void);
static void EnemyUpdateDorarinfuras(struct Enemy *enemy);
static void EnemyDrawDorarinfuras(struct Enemy *enemy);
static void EnemyGenerateJurii(void);
static void EnemyUpdateJurii(struct Enemy *enemy);
static void EnemyDrawJurii(struct Enemy *enemy);
static void EnemyGenerateOnyanma(void);
static void EnemyUpdateOnyanma(struct Enemy *enemy);
static void EnemyDrawOnyanma(struct Enemy *enemy);
static void EnemyGenerateStamperon(void);
static void EnemyUpdateStamperonCore(struct Enemy *enemy);
static void EnemyUpdateStamperonBody(struct Enemy *enemy);
static void EnemyDrawStamperonCore(struct Enemy *enemy);
static void EnemyDrawStamperonBody(struct Enemy *enemy);
static void EnemyGenerateBomb(struct Enemy *enemy);
static void EnemyGenerateBomb(int8_t x, int8_t y, int8_t z, int8_t scale);
static void EnemyUpdateBomb(struct Enemy *enemy);
static void EnemyDrawBomb(struct Enemy *enemy);


/*
 * エネミーを初期化する
 */
void EnemyInitialize(void)
{
  // エネミーの取得
  struct Enemy *enemy = enemys;
  
  // エネミーの初期化
  for (int8_t i = 0x00; i < ENEMY_N; i++) {

    // 参照の設定
    enemy->index = i;

    // 種類の設定
    enemy->type = ENEMY_TYPE_NULL;

    // 次へ
    ++enemy;
  }

  // ジェネレータの初期化
  {
    enemyGenerator.type = ENEMY_TYPE_NULL;
    enemyGenerator.count = 0x01;
    enemyGenerator.fire = 0x60;
    enemyGenerator.frame = ENEMY_GENERATOR_FRAME;
    enemyGenerator.boss = false;
  }
}

/*
 * エネミーを更新する
 */
void EnemyUpdate(void)
{
  // ジェネレータの更新
  {
    // プレイヤの存在
    if (PlayerIsLive()) {

      // エネミーの数の取得
      int8_t count = EnemyGetCount();

      // ボスを倒したらレベルアップ
      if (enemyGenerator.boss) {
        if (count == 0x00) {
          if (enemyGenerator.count < ENEMY_N - 0x04) {
            ++enemyGenerator.count;
          }
          if (enemyGenerator.fire > 0x08) {
            enemyGenerator.fire -= 0x08;
          }
          enemyGenerator.frame = ENEMY_GENERATOR_FRAME;
          enemyGenerator.boss = false;
        }

      // ボスの生成
      } else if (enemyGenerator.frame == 0x00) {
        if (count == 0x00) {
          enemyGenerator.type = ENEMY_TYPE_STAMPERON_CORE;
          enemyGenerator.boss = true;
        }

      // ザコの生成
      } else {
        if (count < enemyGenerator.count) {
          enemyGenerator.type = pgm_read_byte(enemyGenerateTypes + (SystemGetRandom() & 0x07));
        }
        --enemyGenerator.frame;
      }
      /*
      if (SystemIsInputEdge(B_BUTTON)) {
        // enemyGenerator.type = ENEMY_TYPE_POLE;
        // enemyGenerator.type = ENEMY_TYPE_DORARINFURAS;
        // enemyGenerator.type = ENEMY_TYPE_JURII;
        // enemyGenerator.type = ENEMY_TYPE_ONYANMA;
        // enemyGenerator.type = ENEMY_TYPE_STAMPERON_CORE;
      }
      */

      // 種類別の処理
      switch (enemyGenerator.type) {
    
      // 柱
      case ENEMY_TYPE_POLE:
        EnemyGeneratePole();
        break;

      // ドラリンフラー
      case ENEMY_TYPE_DORARINFURAS:
        EnemyGenerateDorarinfuras();
        break;

      // ジェリイ
      case ENEMY_TYPE_JURII:
        EnemyGenerateJurii();
        break;

      // オニャンマ
      case ENEMY_TYPE_ONYANMA:
        EnemyGenerateOnyanma();
        break;

      // スタンパロン
      case ENEMY_TYPE_STAMPERON_CORE:
      case ENEMY_TYPE_STAMPERON_BODY:
        EnemyGenerateStamperon();
        break;

      // 爆発
      case ENEMY_TYPE_BOMB:
        break;
  
      // その他
      default:
        break;
      }

      // エネミーの生成の完了
      enemyGenerator.type = ENEMY_TYPE_NULL;
    }
  }
  
  // エネミーの更新
  {
    struct Enemy *enemy = enemys;
    for (int8_t i = 0x00; i < ENEMY_N; i++) {
  
      // 種類別の処理
      switch (enemy->type) {
  
      // 柱
      case ENEMY_TYPE_POLE:
        EnemyUpdatePole(enemy);
        break;
        
      // ドラリンフラー
      case ENEMY_TYPE_DORARINFURAS:
        EnemyUpdateDorarinfuras(enemy);
        break;
        
      // ジェリイ
      case ENEMY_TYPE_JURII:
        EnemyUpdateJurii(enemy);
        break;
        
      // オニャンマ
      case ENEMY_TYPE_ONYANMA:
        EnemyUpdateOnyanma(enemy);
        break;
        
      // スタンパロン（コア）
      case ENEMY_TYPE_STAMPERON_CORE:
        EnemyUpdateStamperonCore(enemy);
        break;
        
      // スタンパロン（ボディ）
      case ENEMY_TYPE_STAMPERON_BODY:
        EnemyUpdateStamperonBody(enemy);
        break;
        
      // 爆発
      case ENEMY_TYPE_BOMB:
        EnemyUpdateBomb(enemy);
        break;
        
      // その他
      default:
        break;
      }

      // 次へ
      ++enemy;
    }
  }
}

/*
 * エネミーを描画する
 */
void EnemyRender(void)
{
}
void EnemyDraw(int8_t index)
{
  // エネミーの取得
  struct Enemy *enemy = &enemys[index];
  
  // 種類別の処理
  switch (enemy->type) {

  // 柱
  case ENEMY_TYPE_POLE:
    EnemyDrawPole(enemy);
    break;
    
  // 柱
  case ENEMY_TYPE_DORARINFURAS:
    EnemyDrawDorarinfuras(enemy);
    break;
    
  // ジェリイ
  case ENEMY_TYPE_JURII:
    EnemyDrawJurii(enemy);
    break;
    
  // オニャンマ
  case ENEMY_TYPE_ONYANMA:
    EnemyDrawOnyanma(enemy);
    break;
    
  // スタンパロン（コア）
  case ENEMY_TYPE_STAMPERON_CORE:
    EnemyDrawStamperonCore(enemy);
    break;
    
  // スタンパロン（ボディ）
  case ENEMY_TYPE_STAMPERON_BODY:
    EnemyDrawStamperonBody(enemy);
    break;
    
  // 爆発
  case ENEMY_TYPE_BOMB:
    EnemyDrawBomb(enemy);
    break;
    
  // その他
  default:
    break;
  }
}

/*
 * エネミーとのヒット判定を行う
 */
uint8_t EnemyHit(int8_t *rect, int8_t z)
{
  uint8_t result = ENEMY_HIT_NULL;
  int8_t er[GAME_RECT_SIZE];
  for (int8_t i = 0x00; i < ENEMY_N; i++) {
    if (enemys[i].type != ENEMY_TYPE_NULL && enemys[i].life > 0x00) {
      if (GAME_DISTANCE_NEAR <= enemys[i].positionZ && enemys[i].positionZ <= GAME_DISTANCE_FAR) {
        if (z <= enemys[i].positionZ && enemys[i].positionZ - ENEMY_HIT_DISTANCE <= z) {
          EnemyCalcRect(&enemys[i], er);
          if (
            rect[GAME_RECT_LEFT] > er[GAME_RECT_RIGHT] || 
            rect[GAME_RECT_RIGHT] < er[GAME_RECT_LEFT] || 
            rect[GAME_RECT_TOP] > er[GAME_RECT_BOTTOM] || 
            rect[GAME_RECT_BOTTOM] < er[GAME_RECT_TOP]
          ) {
            ;
          } else {
            if (enemys[i].life != ENEMY_LIFE_INVINCIBLE) {
              if (--enemys[i].life == 0x00) {
                GameAddScore(pgm_read_byte(enemyScores + enemys[i].type));
                EnemyGenerateBomb(&enemys[i]);
              }
              result = ENEMY_HIT_DAMAGE;
            } else if (result == ENEMY_HIT_NULL) {
              result = ENEMY_HIT_INVINCIBLE;
            }
          }
        }
      }
    }
  }
  return result;
}

/*
 * エネミーの数を取得する
 */
int8_t EnemyGetCount(void)
{
  int8_t count = 0x00;
  for (int8_t i = 0x00; i < ENEMY_N; i++) {
    if (enemys[i].type != ENEMY_TYPE_NULL) {
      ++count;
    }
  }
  return count;
}
/*
 * 空のエネミーを取得する
 */
static struct Enemy *EnemyGetEmpty(void)
{
  struct Enemy *enemy = NULL;
  for (int8_t i = 0x00; i < ENEMY_N; i++) {
    if (enemys[i].type == ENEMY_TYPE_NULL) {
      enemy = &enemys[i];
      break;
    }
  }
  return enemy;
}

/*
 * 矩形を研鑽する
 */
static void EnemyCalcRect(struct Enemy *enemy, int8_t *rect)
{
  uint8_t *r = (uint8_t *)(enemyRects + (enemy->type << 4) + (enemy->drawScale << 2));
  if (enemy->type == ENEMY_TYPE_POLE) {
    rect[GAME_RECT_LEFT] = enemy->drawX + (int8_t)pgm_read_byte(r + 0x00);
    rect[GAME_RECT_RIGHT] = enemy->drawX + (int8_t)pgm_read_byte(r + 0x01);
    rect[GAME_RECT_TOP] = enemy->drawY - enemy->drawHeight + 0x01;
    rect[GAME_RECT_BOTTOM] = enemy->drawY - 0x01;
  } else {
    rect[GAME_RECT_LEFT] = enemy->drawX + (int8_t)pgm_read_byte(r + 0x00);
    rect[GAME_RECT_RIGHT] = enemy->drawX + (int8_t)pgm_read_byte(r + 0x01);
    rect[GAME_RECT_TOP] = enemy->drawY + (int8_t)pgm_read_byte(r + 0x02);
    rect[GAME_RECT_BOTTOM] = enemy->drawY + (int8_t)pgm_read_byte(r + 0x03);
  }
}

/*
 * ザコを描画する
 */
static void EnemyDrawZako(struct Enemy *enemy, uint8_t *p)
{
  int16_t x = (int16_t)enemy->drawX + GAME_VIEW_OX;
  int16_t y = (int16_t)enemy->drawY + GAME_VIEW_OY;
  p += enemy->drawScale << 3;
  for (int8_t i = 0x00; i < 0x02; i++) {
    uint16_t pattern = pgm_read_byte(p + 0x00);
    AppClipPattern(
      pattern + 0x0000, 
      pattern + 0x0020, 
      x + (int8_t)pgm_read_byte(p + 0x01), 
      y + (int8_t)pgm_read_byte(p + 0x02), 
      pgm_read_byte(p + 0x03), 
      SYSTEM_VIDEO_FLIP_NORMAL
    );
    p += 0x04;
  }
}
/*
 * 柱を生成する
 */
static void EnemyGeneratePole(void)
{
  // 生成数の取得
  int8_t count = (SystemGetRandom() & 0x03) + 0x01;

  // エネミーの生成
  for (int8_t i = 0; i < count; i++) {
    struct Enemy *enemy = EnemyGetEmpty();
    if (enemy == NULL) {
      break;
    }
    int8_t x = SystemGetRandom() & 0x07;
    if (x == 0x07) {
      x = 0x00;
    }
    enemy->type = ENEMY_TYPE_POLE;
    enemy->positionX = (x - 0x03) * 0x16;
    enemy->positionY = 0x20;
    enemy->positionZ = GAME_DISTANCE_FAR + 0x01;
    enemy->life = pgm_read_byte(enemyLifes + enemy->type);
    enemy->params[ENEMY_PARAM_0] = i * 0x20 + 0x01;
    enemy->params[ENEMY_PARAM_1] = 0x00;
    enemy->state = ENEMY_STATE_NULL;
  }
}

/*
 * 柱を更新する
 */
static void EnemyUpdatePole(struct Enemy * enemy)
{
  // 状態別の処理
  switch (enemy->state) {

  // 0x00: 待機
  case 0x00:
    {
      if (--enemy->params[ENEMY_PARAM_0] == 0x00) {
        ++enemy->state;
      }
    }
    break;

  // 0x01: 移動
  case 0x01:
    {
      ++enemy->params[ENEMY_PARAM_1];
      if ((enemy->params[ENEMY_PARAM_1] & 0x01) == 0x00) {
        enemy->positionZ -= 0x01;
        if (enemy->positionZ < GAME_DISTANCE_NEAR) {
          enemy->type = ENEMY_TYPE_NULL;
        }
      }
    }
    break;

  // その他
  default:
    break;
  }

  // エネミーの存在
  if (enemy->type != ENEMY_TYPE_NULL) {

    // 画面上の存在
    if (enemy->positionZ <= GAME_DISTANCE_FAR) {
      
      // 描画の設定
      GameGetDrawPosition(enemy->positionX, enemy->positionY, enemy->positionZ, &enemy->drawX, &enemy->drawY, &enemy->drawScale, &enemy->drawHeight);
      GameEntryDraw(GAME_DRAW_ENEMY, enemy->index, enemy->positionZ);
    }
  }
}

/*
 * 柱を描画する
 */
static void EnemyDrawPole(struct Enemy *enemy)
{
  uint8_t *p = (uint8_t *)(enemyPolePatterns + (enemy->drawScale << 2));
  uint16_t pattern = pgm_read_byte(p + 0x00);
  int16_t x = (int16_t)enemy->drawX + (int8_t)pgm_read_byte(p + 0x01) + GAME_VIEW_OX;
  int16_t y = (int16_t)enemy->drawY + GAME_VIEW_OY;
  int8_t width = pgm_read_byte(p + 0x02);
  int8_t height = enemy->drawHeight;
  AppClipPattern(pattern + 0x0020, pattern + 0x0050, x, y, width, SYSTEM_VIDEO_FLIP_NORMAL);
  while (height >= 0x08) {
    y -= 0x08;
    AppClipPattern(pattern + 0x0010, pattern + 0x0040, x, y, width, SYSTEM_VIDEO_FLIP_NORMAL);
    height -= 0x08;
  }
  if (height > 0x00) {
    y -= height;
    AppClipPattern(pattern + 0x0010, pattern + 0x0040, x, y, width, SYSTEM_VIDEO_FLIP_NORMAL);
  }
  y -= 0x08;
  AppClipPattern(pattern + 0x0000, pattern + 0x0030, x, y, width, SYSTEM_VIDEO_FLIP_NORMAL);
}

/*
 * ドラリンフラーを生成する
 */
static void EnemyGenerateDorarinfuras(void)
{
  // 生成数の取得
  int8_t count = (SystemGetRandom() & 0x01) + 0x03;

  // 向きの取得
  int8_t direction = (SystemGetRandom() & 0x02) - 0x01;

  // エネミーの生成
  for (int8_t i = 0; i < count; i++) {
    struct Enemy *enemy = EnemyGetEmpty();
    if (enemy == NULL) {
      break;
    }
    enemy->type = ENEMY_TYPE_DORARINFURAS;
    enemy->positionX = -0x4c * direction;
    enemy->positionY = 0x18;
    enemy->positionZ = ENEMY_HIT_DISTANCE + 0x01;
    enemy->life = pgm_read_byte(enemyLifes + enemy->type);
    enemy->fire = (SystemGetRandom() & 0x3f) + 0x40;
    enemy->params[ENEMY_PARAM_0] = i * 0x10 + 0x01;
    enemy->params[ENEMY_PARAM_1] = 0x00;
    enemy->params[ENEMY_PARAM_2] = direction;
    enemy->state = ENEMY_STATE_NULL;
  }
}

/*
 * ドラリンフラーを更新する
 */
static void EnemyUpdateDorarinfuras(struct Enemy * enemy)
{
  // 状態別の処理
  switch (enemy->state) {

  // 0x00: 待機
  case 0x00:
    {
      if (--enemy->params[ENEMY_PARAM_0] == 0x00) {
        ++enemy->state;
      }
    }
    break;

  // 0x01: 登場
  case 0x01:
    {
      enemy->positionX += 0x02 * enemy->params[ENEMY_PARAM_2];
      if (
        (enemy->params[ENEMY_PARAM_2] > 0x00 && enemy->positionX >= 0x00) || 
        (enemy->params[ENEMY_PARAM_2] < 0x00 && enemy->positionX <= 0x00)
      ) {
        ++enemy->state;
      }
    }
    break;

  // 0x02: 円運動
  case 0x02:
    {
      // 移動
      int16_t s = AppGetSin(enemy->params[ENEMY_PARAM_1]);
      int16_t c = AppGetCos(enemy->params[ENEMY_PARAM_1]);
      ++enemy->params[ENEMY_PARAM_1];
      enemy->positionX = (s * enemy->params[ENEMY_PARAM_2] * 0x40) >> 8;
      enemy->positionY = (c * 0x18) >> 8;
      enemy->positionZ = ((c * -0x10) >> 8) + (ENEMY_HIT_DISTANCE + 0x11);
      if (enemy->params[ENEMY_PARAM_1] == 0x00) {
        ++enemy->state;
      }
      
      // 発射
      if (--enemy->fire == 0x00) {
        BulletEntry(enemy->positionX, enemy->positionY, enemy->positionZ);
        enemy->fire = (SystemGetRandom() & 0x3f) + enemyGenerator.fire;
      }
    }
    break;

  // 0x03: 退場
  case 0x03:
    {
      enemy->positionX += 0x02 * enemy->params[ENEMY_PARAM_2];
      if (enemy->positionX < -0x4c || 0x4c < enemy->positionX) {
        enemy->type = ENEMY_TYPE_NULL;
      }
    }
    break;

  // その他
  default:
    break;
  }
  
  // エネミーの存在
  if (enemy->type != ENEMY_TYPE_NULL) {

    // 画面上の存在
    if (enemy->positionZ <= GAME_DISTANCE_FAR) {
      
      // 描画の設定
      GameGetDrawPosition(enemy->positionX, enemy->positionY, enemy->positionZ, NULL, &enemy->drawY, &enemy->drawScale, NULL);
      enemy->drawX = enemy->positionX;
      GameEntryDraw(GAME_DRAW_ENEMY, enemy->index, enemy->positionZ);
    }
  }
}

/*
 * ドラリンフラーを描画する
 */
static void EnemyDrawDorarinfuras(struct Enemy *enemy)
{
  EnemyDrawZako(enemy, (uint8_t *)enemyDorarinfurasPatterns);
}

/*
 * ジェリイを生成する
 */
static void EnemyGenerateJurii(void)
{
  // 生成数の取得
  int8_t count = (SystemGetRandom() & 0x01) + 0x03;

  // エネミーの生成
  for (int8_t i = 0; i < count; i++) {
    struct Enemy *enemy = EnemyGetEmpty();
    if (enemy == NULL) {
      break;
    }
    enemy->type = ENEMY_TYPE_JURII;
    enemy->positionX = (SystemGetRandom() % 0x60) - 0x30;
    enemy->positionY = 0x18;
    enemy->positionZ = GAME_DISTANCE_FAR + 0x01;
    enemy->life = pgm_read_byte(enemyLifes + enemy->type);
    enemy->fire = (SystemGetRandom() & 0x3f) + enemyGenerator.fire;
    enemy->params[ENEMY_PARAM_0] = i * 0x20 + 0x01;
    enemy->params[ENEMY_PARAM_1] = 0x00;
    enemy->state = ENEMY_STATE_NULL;
  }
}

/*
 * ジェリイを更新する
 */
static void EnemyUpdateJurii(struct Enemy * enemy)
{
  // 状態別の処理
  switch (enemy->state) {

  // 0x00: 待機
  case 0x00:
    {
      if (--enemy->params[ENEMY_PARAM_0] == 0x00) {
        ++enemy->state;
      }
    }
    break;

  // 0x01: 移動
  case 0x01:
    {
      // 移動
      ++enemy->params[ENEMY_PARAM_1];
      enemy->positionY = 0x18 - (((int16_t)0x0030 * AppGetSin((enemy->params[ENEMY_PARAM_1] << 2) & 0x7f)) >> 8);
      if ((enemy->params[ENEMY_PARAM_1] & 0x03) == 0x00) {
        enemy->positionZ -= 0x01;
        if (enemy->positionZ < GAME_DISTANCE_NEAR) {
          enemy->type = ENEMY_TYPE_NULL;
        }
      }
      
      // 発射
      if (--enemy->fire == 0x00) {
        BulletEntry(enemy->positionX, enemy->positionY, enemy->positionZ);
        enemy->fire = (SystemGetRandom() & 0x3f) + enemyGenerator.fire;
      }
    }
    break;

  // その他
  default:
    break;
  }
  
  // エネミーの存在
  if (enemy->type != ENEMY_TYPE_NULL) {

    // 画面上の存在
    if (enemy->positionZ <= GAME_DISTANCE_FAR) {
      
      // 描画の設定
      GameGetDrawPosition(enemy->positionX, enemy->positionY, enemy->positionZ, NULL, &enemy->drawY, &enemy->drawScale, NULL);
      enemy->drawX = enemy->positionX;
      GameEntryDraw(GAME_DRAW_ENEMY, enemy->index, enemy->positionZ);
    }
  }
}

/*
 * ジェリイを描画する
 */
static void EnemyDrawJurii(struct Enemy *enemy)
{
  EnemyDrawZako(enemy, (uint8_t *)enemyJuriiPatterns);
}

/*
 * オニャンマを生成する
 */
static void EnemyGenerateOnyanma(void)
{
  // 生成数の取得
  int8_t count = (SystemGetRandom() & 0x01) + 0x02;

  // エネミーの生成
  for (int8_t i = 0; i < count; i++) {
    struct Enemy *enemy = EnemyGetEmpty();
    if (enemy == NULL) {
      break;
    }
    enemy->type = ENEMY_TYPE_ONYANMA;
    enemy->positionX = (SystemGetRandom() % 0x60) - 0x30;
    enemy->positionY = 0x00;
    enemy->positionZ = GAME_DISTANCE_FAR + 0x01;
    enemy->life = pgm_read_byte(enemyLifes + enemy->type);
    enemy->fire = (SystemGetRandom() & 0x3f) + enemyGenerator.fire;
    enemy->params[ENEMY_PARAM_0] = i * 0x30 + 0x01;
    enemy->params[ENEMY_PARAM_1] = 0x00;
    enemy->params[ENEMY_PARAM_2] = (SystemGetRandom() & 0x02) - 0x01;
    enemy->params[ENEMY_PARAM_3] = (SystemGetRandom() & 0x0f) - 0x08;
    enemy->state = ENEMY_STATE_NULL;
  }
}

/*
 * オニャンマを更新する
 */
static void EnemyUpdateOnyanma(struct Enemy * enemy)
{
  // 状態別の処理
  switch (enemy->state) {

  // 0x00: 待機
  case 0x00:
    {
      if (--enemy->params[ENEMY_PARAM_0] == 0x00) {
        ++enemy->state;
      }
    }
    break;

  // 0x01: 移動
  case 0x01:
    {
      // 移動
      ++enemy->params[ENEMY_PARAM_1];
      if ((enemy->params[ENEMY_PARAM_1] & 0x01) == 0x00) {
        enemy->positionX += enemy->params[ENEMY_PARAM_2];
        if (enemy->positionX <= -0x38 || 0x38 <= enemy->positionX) {
          enemy->params[ENEMY_PARAM_2] = -enemy->params[ENEMY_PARAM_2];
        }
      }
      enemy->positionY = enemy->params[ENEMY_PARAM_3] + (((int16_t)0x0010 * AppGetSin(enemy->params[ENEMY_PARAM_1] << 2)) >> 8);
      if ((enemy->params[ENEMY_PARAM_1] & 0x03) == 0x00) {
        enemy->positionZ -= 0x01;
        if (enemy->positionZ < GAME_DISTANCE_NEAR) {
          enemy->type = ENEMY_TYPE_NULL;
        }
      }
      
      // 発射
      if (--enemy->fire == 0x00) {
        BulletEntry(enemy->positionX, enemy->positionY, enemy->positionZ);
        enemy->fire = (SystemGetRandom() & 0x3f) + enemyGenerator.fire;
      }
    }
    break;

  // その他
  default:
    break;
  }
  
  // エネミーの存在
  if (enemy->type != ENEMY_TYPE_NULL) {

    // 画面上の存在
    if (enemy->positionZ <= GAME_DISTANCE_FAR) {
      
      // 描画の設定
      GameGetDrawPosition(enemy->positionX, enemy->positionY, enemy->positionZ, NULL, &enemy->drawY, &enemy->drawScale, NULL);
      enemy->drawX = enemy->positionX;
      GameEntryDraw(GAME_DRAW_ENEMY, enemy->index, enemy->positionZ);
    }
  }
}

/*
 * オニャンマを描画する
 */
static void EnemyDrawOnyanma(struct Enemy *enemy)
{
  EnemyDrawZako(enemy, (uint8_t *)enemyOnyanmaPatterns);
}

/*
 * スタンパロンを生成する
 */
static void EnemyGenerateStamperon(void)
{
  // コアの生成
  struct Enemy *core = EnemyGetEmpty();
  if (core != NULL) {
    core->type = ENEMY_TYPE_STAMPERON_CORE;
    core->positionX = 0x00;
    core->positionY = -0x80;
    core->positionZ = (GAME_DISTANCE_FAR + GAME_DISTANCE_NEAR) >> 1;
    core->life = pgm_read_byte(enemyLifes + core->type);
    core->fire = (SystemGetRandom() & 0x1f) + (enemyGenerator.fire >> 1);
    core->params[ENEMY_PARAM_0] = 0x00;
    core->params[ENEMY_PARAM_1] = (SystemGetRandom() & 0x02) - 0x01;
    core->params[ENEMY_PARAM_2] = 0x00;
    core->state = ENEMY_STATE_NULL;
  }

  // ボディの生成
  struct Enemy *body = EnemyGetEmpty();
  if (body != NULL) {
    body->type = ENEMY_TYPE_STAMPERON_BODY;
    body->positionX = core->positionX;
    body->positionY = core->positionY;
    body->positionZ = core->positionZ;
    body->life = pgm_read_byte(enemyLifes + body->type);
    body->fire = 0x00;
    body->params[ENEMY_PARAM_0] = core->index;
    body->params[ENEMY_PARAM_1] = 0xc0;
    body->state = ENEMY_STATE_NULL;
  } else if (core != NULL) {
    core->type = ENEMY_TYPE_NULL;
  }
}

/*
 * スタンパロンを更新する
 */
static void EnemyUpdateStamperonCore(struct Enemy * enemy)
{
  // 状態別の処理
  switch (enemy->state) {

  // 0x00: 登場
  case 0x00:
    {
      ++enemy->positionY;
      if (enemy->positionY >= 0x00) {
        ++enemy->state;
      }
    }
    break;

  // 0x01: 移動
  case 0x01:
    {
      // 移動
      ++enemy->params[ENEMY_PARAM_0];
      if ((enemy->params[ENEMY_PARAM_0] & 0x01) == 0x00) {
        enemy->positionX += enemy->params[ENEMY_PARAM_1];
        if (enemy->positionX < -0x30 || 0x30 < enemy->positionX) {
          enemy->params[ENEMY_PARAM_1] = -enemy->params[ENEMY_PARAM_1];
        }
      }
      enemy->params[ENEMY_PARAM_2] += 0x04;
      enemy->positionY = ((int16_t)0x0010 * AppGetSin(enemy->params[ENEMY_PARAM_2])) >> 8;
      
      // 発射
      if (--enemy->fire == 0x00) {
        BulletEntry(enemy->positionX, enemy->positionY + 0x04, enemy->positionZ);
        enemy->fire = (SystemGetRandom() & 0x0f) + (enemyGenerator.fire >> 1);
      }
    }
    break;

  // その他
  default:
    break;
  }
  
  // エネミーの存在
  if (enemy->type != ENEMY_TYPE_NULL) {

    // 描画の設定
    GameGetDrawPosition(enemy->positionX, enemy->positionY, enemy->positionZ, NULL, &enemy->drawY, NULL, NULL);
    enemy->drawX = enemy->positionX;
    enemy->drawScale = 0x03;
    GameEntryDraw(GAME_DRAW_ENEMY, enemy->index, enemy->positionZ);
  }
}
static void EnemyUpdateStamperonBody(struct Enemy * enemy)
{
  // コアの取得
  struct Enemy *core = &enemys[enemy->params[ENEMY_PARAM_0]];

  // コアの存在
  if (core->type == ENEMY_TYPE_STAMPERON_CORE) {
  
    // コアの追従
    enemy->positionX = core->positionX;
    enemy->positionY = core->positionY;
    enemy->positionZ = core->positionZ;
    enemy->drawX = core->drawX;
    enemy->drawY = core->drawY;
    enemy->drawScale = core->drawScale;
    
  // コアの削除
  } else {

    // 点滅の更新
    --enemy->params[ENEMY_PARAM_1];
    
    // 爆発の生成
    if (enemy->params[ENEMY_PARAM_1] != 0x00) {
      if ((enemy->params[ENEMY_PARAM_1] & 0x0f) == 0x00) {
        EnemyGenerateBomb(
          enemy->drawX + (SystemGetRandom() & 0x1f) - 0x10, 
          enemy->drawY + (SystemGetRandom() & 0x1f) - 0x10, 
          enemy->positionZ, 
          enemy->drawScale
        );
      }

    // エネミーの削除
    } else {
      enemy->type = ENEMY_TYPE_NULL;
    }
  }

  // エネミーの存在
  if (enemy->type != ENEMY_TYPE_NULL) {

    // 描画の設定
    GameEntryDraw(GAME_DRAW_ENEMY, enemy->index, enemy->positionZ);
  }
}

/*
 * スタンパロンを描画する
 */
static void EnemyDrawStamperonCore(struct Enemy *enemy)
{
}
static void EnemyDrawStamperonBody(struct Enemy *enemy)
{
  if ((enemy->params[ENEMY_PARAM_1] & 0x02) == 0x00) {
    int16_t pattern = 0x0180;
    int16_t x = (int16_t)enemy->drawX - 0x14 + GAME_VIEW_OX;
    int16_t y = (int16_t)enemy->drawY - 0x10 + GAME_VIEW_OY;
    for (int8_t i = 0x00; i < 0x04; i++) {
      AppClipPattern(pattern, pattern + 0x0040, x, y, 0x28, SYSTEM_VIDEO_FLIP_NORMAL);
      pattern += 0x0010;
      y += 0x08;
    }
  }
}

/*
 * 爆発を生成する
 */
static void EnemyGenerateBomb(struct Enemy *enemy)
{
  // エネミーの置き換え
  enemy->type = ENEMY_TYPE_BOMB;
  enemy->params[ENEMY_PARAM_0] = 0x20;
  enemy->state = ENEMY_STATE_NULL;
}
static void EnemyGenerateBomb(int8_t x, int8_t y, int8_t z, int8_t scale)
{
  // エネミーの生成
  struct Enemy *enemy = EnemyGetEmpty();
  if (enemy != NULL) {
    enemy->type = ENEMY_TYPE_BOMB;
    enemy->positionZ = z;
    enemy->drawX = x;
    enemy->drawY = y;
    enemy->drawScale = scale;
    enemy->params[ENEMY_PARAM_0] = 0x20;
    enemy->state = ENEMY_STATE_NULL;
  }
}

/*
 * 爆発を更新する
 */
static void EnemyUpdateBomb(struct Enemy *enemy)
{
  // パラメータの更新
  if (--enemy->params[ENEMY_PARAM_0] == 0x00) {
    enemy->type = ENEMY_TYPE_NULL;

  // 描画の設定
  } else {
    GameEntryDraw(GAME_DRAW_ENEMY, enemy->index, enemy->positionZ);
  }
}

/*
 * 爆発を描画する
 */
static void EnemyDrawBomb(struct Enemy *enemy)
{
  uint8_t *p = (uint8_t *)enemyBombPatterns + enemy->drawScale * 0x0b;
  int16_t x = (int16_t)enemy->drawX + GAME_VIEW_OX;
  int16_t y = (int16_t)enemy->drawY + GAME_VIEW_OY;
  uint8_t flip = (enemy->params[ENEMY_PARAM_0] & 0x04) != 0x00 ? SYSTEM_VIDEO_FLIP_NORMAL : SYSTEM_VIDEO_FLIP_REVERSE;
  int8_t count = pgm_read_byte(p + 0x00);
  ++p;
  while (count-- > 0x00) {
    AppClipPattern(
      pgm_read_byte(p + 0x00), 
      pgm_read_byte(p + 0x01), 
      x + (int8_t)pgm_read_byte(p + 0x02), 
      y + (int8_t)pgm_read_byte(p + 0x03), 
      pgm_read_byte(p + 0x04), 
      flip
    );
    p += 0x05;
  }
}
