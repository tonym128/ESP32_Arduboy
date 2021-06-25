/*
 * Player.cpp : プレイヤ
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
#include "Condition.h"
#include "Item.h"


/*
 * 変数の定義
 */

// プレイヤ
static struct Player player;

// コンディション
static const uint16_t playerConditionFrames[]  = {
  0x0000, 
  0x0400, 
  0x0200, 
  0x0400, 
  0x0200, 
  0x0001, 
};


/*
 * 内部関数の宣言
 */
static void PlayerStay(void);
static void PlayerPlay(void);
static void PlayerDead(void);
static int8_t PlayerGetSpeed(void);
static int8_t PlayerGetHit(void);
static void PlayerClearCondition(void);
static void PlayerUpdateCondition(void);
static void PlayerClearControl(void);


/*
 * プレイヤを初期化する
 */
void PlayerInitialize(void)
{
  // 状態の設定
  player.state = PLAYER_STATE_PLAY;
  
  // ルームと位置の設定
  DungeonGetStartRoom(&player.room, &player.position.x, &player.position.y);

  // 向きの設定
  player.direction = PLAYER_DIRECTION_DOWN;

  // 体力の設定
  player.life = PLAYER_LIFE;

  // 速度の設定
  player.speed = 0;

  // ヒットの設定
  player.hit = 0;
  
  // ダメージの設定
  player.damage = 0;

  // コンディションの設定
  PlayerClearCondition();

  // アイテムの設定
  for (int i = 0; i < ITEM_SIZE; i++) {
    player.items[i] = 0;
  }
  player.items[ITEM_SWORD] = 1;
  player.items[ITEM_SHIELD] = 1;

  // 操作の設定
  PlayerClearControl();

  // アニメーションの設定
  player.animation = 0;

  // マスクの設定
  player.mask = 0xff;
}

/*
 * プレイヤを更新する
 */
void PlayerUpdate(void)
{
  // 状態別の処理
  switch (player.state & 0xf0) {

  // 待機
  case PLAYER_STATE_STAY:
    PlayerStay();
    break;
    
  // プレイ
  case PLAYER_STATE_PLAY:
    PlayerPlay();
    break;

  // 死亡
  case PLAYER_STATE_DEAD:
    PlayerDead();
    break;

  // その他
  default:
    break;
  }
}

/*
 * プレイヤを描画する
 */
void PlayerRender(void)
{
  // プレイヤの存在
  if (player.state != PLAYER_STATE_NULL) {
    
    // ダメージの描画
    if (player.damage > 0) {
      AppReversePattern(
        0x30 + player.direction * 2 + ((player.animation >> 3) & 0x01), 
        player.position.x + PLAYER_RECT_X, 
        player.position.y + PLAYER_RECT_Y, 
        0x08, 
        SYSTEM_VIDEO_FLIP_NORMAL
      );
  
    // 通常の描画
    } else {
      AppMaskPattern(
        0x30 + player.direction * 2 + ((player.animation >> 3) & 0x01), 
        player.mask, 
        player.position.x + PLAYER_RECT_X,  
        player.position.y + PLAYER_RECT_Y, 
        0x08, 
        SYSTEM_VIDEO_FLIP_NORMAL
      );
    }
  }
}

/*
 * プレイヤを待機する
 */
static void PlayerStay(void)
{
  // 初期化
  if ((player.state & 0x0f) == 0x00) {

    // ダメージのクリア
    player.damage = 0;

    // 状態の更新
    ++player.state;
  }
}

/*
 * プレイヤをプレイする
 */
static void PlayerPlay(void)
{
  // 初期化
  if ((player.state & 0x0f) == 0x00) {

    // 状態の更新
    ++player.state;
  }

  // 移動
  if (!PlayerIsItem(ITEM_EXIT)) {
    
    // 矩形の取得
    int8_t left = player.position.x + PLAYER_RECT_LEFT;
    int8_t top = player.position.y + PLAYER_RECT_TOP;
    int8_t right = player.position.x + PLAYER_RECT_RIGHT;
    int8_t bottom = player.position.y + PLAYER_RECT_BOTTOM;

    // 移動量の取得
    int8_t movex = 0;
    int8_t movey = 0;

    // 速度の取得
    int8_t speed = PlayerGetSpeed();

    // コリジョンの取得
    uint8_t collision = 0x00;

    // ↑
    if (SystemIsInputPush(player.controls[PLAYER_CONTROL_UP])) {
      if (player.direction != PLAYER_DIRECTION_UP) {
        player.direction = PLAYER_DIRECTION_UP;
        player.speed = speed;
      }
      if (++player.speed >= speed) {
        collision |= DungeonIsCollision(player.room, left, top - 1) ? 0x01 : 0x00;
        collision |= DungeonIsCollision(player.room, right, top - 1) ? 0x02 : 0x00;
        if (collision == 0x00) {
          movey = -1;
        } else if (collision == 0x01) {
          movex = 1;
        } else if (collision == 0x02) {
          movex = -1;
        }
      }
      ++player.animation;
  
    // ↓
    } else if (SystemIsInputPush(player.controls[PLAYER_CONTROL_DOWN])) {
      if (player.direction != PLAYER_DIRECTION_DOWN) {
        player.direction = PLAYER_DIRECTION_DOWN;
        player.speed = speed;
      }
      if (++player.speed >= speed) {
        collision |= DungeonIsCollision(player.room, left, bottom + 1) ? 0x01 : 0x00;
        collision |= DungeonIsCollision(player.room, right, bottom + 1) ? 0x02 : 0x00;
        if (collision == 0x00) {
          movey = 1;
        } else if (collision == 0x01) {
          movex = 1;
        } else if (collision == 0x02) {
          movex = -1;
        }
      }
      ++player.animation;
  
    // ←
    } else if (SystemIsInputPush(player.controls[PLAYER_CONTROL_LEFT])) {
      if (player.direction != PLAYER_DIRECTION_LEFT) {
        player.direction = PLAYER_DIRECTION_LEFT;
        player.speed = speed;
      }
      if (++player.speed >= speed) {
        collision |= DungeonIsCollision(player.room, left - 1, top) ? 0x01 : 0x00;
        collision |= DungeonIsCollision(player.room, left - 1, bottom) ? 0x02 : 0x00;
        if (collision == 0x00) {
          movex = -1;
        } else if (collision == 0x01) {
          movey = 1;
        } else if (collision == 0x02) {
          movey = -1;
        }
      }
      ++player.animation;
  
    // →
    } else if (SystemIsInputPush(player.controls[PLAYER_CONTROL_RIGHT])) {
      if (player.direction != PLAYER_DIRECTION_RIGHT) {
        player.direction = PLAYER_DIRECTION_RIGHT;
        player.speed = speed;
      }
      if (++player.speed >= speed) {
        collision |= DungeonIsCollision(player.room, right + 1, top) ? 0x01 : 0x00;
        collision |= DungeonIsCollision(player.room, right + 1, bottom) ? 0x02 : 0x00;
        if (collision == 0x00) {
          movex = 1;
        } else if (collision == 0x01) {
          movey = 1;
        } else if (collision == 0x02) {
          movey = -1;
        }
      }
      ++player.animation;
    }

    // 移動と攻撃
    if (movex != 0 || movey != 0) {
      int8_t attack = 0;
      if (collision == 0x00) {
        if (++player.hit >= PlayerGetHit()) {
          attack = PlayerGetAttack();
          player.hit = 0;
        }
      }
      if (!EnemyIsHit(left + movex, top + movey, right + movex, bottom + movey, attack)) {
        player.position.x += movex;
        player.position.y += movey;
        {
          int8_t item = DungeonPickUpItem(player.room, player.position.x, player.position.y);
          if (item != ITEM_NULL) {
            if (item == ITEM_POTION) {
              PlayerHeal();
            } else {
              ++player.items[item];
            }
          }
        }
      }
      player.speed = 0;
    }
  }

  // デバッグ
#if SYSTEM_DEBUG
  /* {
    if (SystemIsInputEdge(A_BUTTON)) {
      player.items[ITEM_SWORD] = 5;
      player.items[ITEM_SHIELD] = 4;
      player.items[ITEM_BOOTS] = 0;
      player.items[ITEM_RING] = 0;
      player.items[ITEM_HAMMER] = 1;
      player.items[ITEM_CANDLE] = 1;
      player.items[ITEM_MIRROR] = 0;
      player.items[ITEM_KEY] = 0;
      player.items[ITEM_CRYSTAL] = 0;
    }
    if (SystemIsInputEdge(B_BUTTON)) {
      ;
    }
  } */
#endif

  // ダメージの更新
  if (player.damage > 0) {
    --player.damage;
  }

  // コンディションの更新
  PlayerUpdateCondition();
}

/*
 * プレイヤが死亡する
 */
static void PlayerDead(void)
{
  // 初期化
  if ((player.state & 0x0f) == 0x00) {

    // アニメーションの設定
    player.animation = PLAYER_DEAD;

    // 状態の更新
    ++player.state;
  }

  // ダメージの更新
  if (player.damage > 0) {
    --player.damage;

  // アニメーションの更新
  } else if (++player.animation >= PLAYER_DEAD) {

    // マスクの更新
    player.mask = (player.mask >> 1) & 0x77;

    // 死亡の継続
    if (player.mask > 0x00) {
      
      // アニメーションの設定
      player.animation = 0;
      
    // 死亡の完了
    } else {

      // 状態の設定
      player.state = PLAYER_STATE_NULL;
    }
  }
}

/*
 * プレイヤとのヒットを判定する
 */
bool PlayerIsHit(int8_t left, int8_t top, int8_t right, int8_t bottom, int8_t attack, int8_t condition)
{
  bool result = false;
  if (PlayerIsLive()) {
    if (
      left <= player.position.x + PLAYER_RECT_RIGHT && 
      top <= player.position.y + PLAYER_RECT_BOTTOM && 
      right >= player.position.x + PLAYER_RECT_LEFT && 
      bottom >= player.position.y + PLAYER_RECT_TOP
    ) {
      if (attack > 0) {
        attack = attack - PlayerGetDefense();
        if (attack < 1) {
          attack = 1;
        }
        if (player.life > attack) {
          player.life -= attack;
          uint8_t rate = SystemGetRandom();
          if (player.conditions[condition] == 0) {
            if (rate < CONDITION_RATE) {
              PlayerSetCondition(condition);
            }
          } else if (condition == CONDITION_SLEEP) {
            if (rate < CONDITION_SLEEP_AWAKE_RATE) {
              player.conditions[CONDITION_SLEEP] = 1;
            }
          }
        } else {
          player.life = 0;
          player.state = PLAYER_STATE_DEAD;
        }
        player.damage = PLAYER_DAMAGE;
      }
      result = true;
    }
  }
  return result;
}

/*
 * プレイヤがルームを移動する
 */
bool PlayerExitRoom(void)
{
  return DungeonExitRoom(&player.room, &player.position.x, &player.position.y);
}

/*
 * プレイヤのいるルームを取得する
 */
int8_t PlayerGetRoom(void)
{
  return player.room;
}

/*
 * プレイヤのいる位置を取得する
 */
void PlayerGetPosition(int8_t *x, int8_t *y)
{
  *x = player.position.x;
  *y = player.position.y;
}

/*
 * プレイヤの速度を取得する
 */
static int8_t PlayerGetSpeed(void)
{
  return PlayerIsCondition(CONDITION_SLOW) ? PLAYER_SPEED_SLOW : (PlayerIsItem(ITEM_BOOTS) ? PLAYER_SPEED_FAST : PLAYER_SPEED_NORMAL);
}

/*
 * プレイヤの攻撃速度を取得する
 */
static int8_t PlayerGetHit(void)
{
  return PlayerIsItem(ITEM_RING) ? PLAYER_HIT_FAST : PLAYER_HIT_NORMAL;
}

/*
 * プレイヤが存在するかどうかを判定する
 */
bool PlayerIsLive(void)
{
  return player.state != PLAYER_STATE_NULL && player.life > 0 ? true : false;
}
bool PlayerIsDead(void)
{
  return player.state == PLAYER_STATE_NULL && player.life <= 0 ? true : false;
}

/*
 * プレイヤを回復する
 */
void PlayerHeal(void)
{
  player.life = PLAYER_LIFE;
  PlayerClearCondition();
}

void PlayerHeal(int8_t life)
{
  player.life += life;
  if (player.life > PLAYER_LIFE) {
    player.life = PLAYER_LIFE;
  }
  PlayerClearCondition();
}

/*
 * プレイヤの体力を取得する
 */
int8_t PlayerGetLife(void)
{
  return player.life;
}

/*
 * プレイヤの攻撃力を取得する
 */
int8_t PlayerGetAttack(void)
{
  return player.items[ITEM_SWORD];
}

/*
 * プレイヤの防御力を取得する
 */
int8_t PlayerGetDefense(void)
{
  return player.items[ITEM_SHIELD];
}

/*
 * プレイヤのコンディションをクリアする
 */
static void PlayerClearCondition(void)
{
  for (int8_t i = 0; i < CONDITION_SIZE; i++) {
    player.conditions[i] = 0;
  }
  PlayerClearControl();
}

/*
 * プレイヤのコンディションを確認する
 */
bool PlayerIsCondition(int8_t condition)
{
  return PlayerIsLive() && player.conditions[condition] > 0 ? true : false;
}

/*
 * プレイヤのコンディションを設定する
 */
void PlayerSetCondition(int8_t condition)
{
  if (PlayerIsLive()) {
    if (player.conditions[condition] == 0) {
      if (condition == CONDITION_SLEEP) {
        for (int8_t i = 0; i < PLAYER_CONTROL_SIZE; i++) {
          player.controls[i] = 0;
        }
        player.conditions[CONDITION_CONFUSE] = 0;
      } else if (condition == CONDITION_CONFUSE) {
        for (int8_t i = 0; i < PLAYER_CONTROL_SIZE; i++) {
          int8_t j = (SystemGetRandom() >> 1) & (PLAYER_CONTROL_SIZE - 1);
          int8_t t = player.controls[i];
          player.controls[i] = player.controls[j];
          player.controls[j] = t;
        }
        player.conditions[CONDITION_SLEEP] = 0;
      }
      player.conditions[condition] = pgm_read_word(&playerConditionFrames[condition]);
    }
  }
}

/*
 * コンディションを更新する
 */
static void PlayerUpdateCondition(void)
{
  if (player.conditions[CONDITION_SLOW] > 0) {
    --player.conditions[CONDITION_SLOW];
  }
  if (player.conditions[CONDITION_SLEEP] > 0) {
    if (--player.conditions[CONDITION_SLEEP] == 0) {
      PlayerClearControl();
    }
  }
  if (player.conditions[CONDITION_BLIND] > 0) {
    --player.conditions[CONDITION_BLIND];
  }
  if (player.conditions[CONDITION_CONFUSE] > 0) {
    if (--player.conditions[CONDITION_CONFUSE] == 0) {
      PlayerClearControl();
    }
  }
  if (player.conditions[CONDITION_POISON] > 0) {
    if (--player.conditions[CONDITION_POISON] == 0) {
      if (player.life > 1) {
        --player.life;
        player.conditions[CONDITION_POISON] = CONDITION_POISON_INTERVAL;
      }
    }
  }
}

/*
 * アイテムを所持しているかどうかを判定する
 */
bool PlayerIsItem(int8_t item)
{
  return player.items[item] > 0 ? true : false;
}

/*
 * プレイヤの操作をクリアする
 */
static void PlayerClearControl(void)
{
  player.controls[PLAYER_CONTROL_UP] = UP_BUTTON;
  player.controls[PLAYER_CONTROL_DOWN] = DOWN_BUTTON;
  player.controls[PLAYER_CONTROL_LEFT] = LEFT_BUTTON;
  player.controls[PLAYER_CONTROL_RIGHT] = RIGHT_BUTTON;
}


