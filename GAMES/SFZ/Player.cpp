/*
 * Player.cpp : プレイヤ
 */

/*
 * 参照ファイル
 */
#include "System.h"
#include "App.h"
#include "Game.h"
#include "Player.h"
#include "Shot.h"


/*
 * 変数の定義
 */

// 状態
static struct Player player;

// パターン
static const uint8_t playerPatterns[]  = {
  0x02, 0x20, 0x40, 0xf4, 0xf8, 0x18, 0x30, 0x50, 0xf4, 0x00, 0x18, 
  0x02, 0x23, 0x43, 0xf4, 0xf8, 0x18, 0x33, 0x53, 0xf4, 0x00, 0x18, 
  0x01, 0x6f, 0x7f, 0xfc, 0xfc, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x02, 0x6b, 0x6d, 0xf8, 0xf8, 0x10, 0x7b, 0x7d, 0xf8, 0x00, 0x10, 
  0x02, 0x67, 0x69, 0xf8, 0xf8, 0x10, 0x77, 0x79, 0xf8, 0x00, 0x10, 
};


/*
 * 内部関数の宣言
 */
static void PlayerPlay(void);
static void PlayerDead(void);


/*
 * プレイヤを初期化する
 */
void PlayerInitialize(void)
{
  // 位置の設定
  player.positionX = 0x00;
  player.positionY = 0x00;

  // 体力の設定
  player.life = PLAYER_LIFE;

  // ダメージの設定
  player.damage = 0x00;

  // 発射の設定
  player.fire = PLAYER_FIRE;

  // 状態の設定
  player.state = PLAYER_STATE_PLAY;
}

/*
 * プレイヤを更新する
 */
void PlayerUpdate(void)
{
  // 状態別の処理
  switch (player.state & 0xf0) {

  // 何もしない
  case PLAYER_STATE_NULL:
    break;

  // 待機
  case PLAYER_STATE_STAY:
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
  // パターンの初期化
  uint8_t *p = NULL;
  
  // 状態別の処理
  switch (player.state & 0xf0) {

  // 何もしない
  case PLAYER_STATE_NULL:
    break;

  // 待機
  case PLAYER_STATE_STAY:
    break;

  // プレイ
  case PLAYER_STATE_PLAY:
    if ((player.damage & 0x02) == 0x00) {
      p = (uint8_t *)(playerPatterns + 0x0000 + ((player.animation >> 3) & 0x01) * 0x0b);
    }
    break;

  // 死亡
  case PLAYER_STATE_DEAD:
    p = (uint8_t *)(playerPatterns + 0x0016 + (player.animation >> 3) * 0x0b);
    break;

  // その他
  default:
    break;
  }

  // パターンの描画
  if (p != NULL) {
    int8_t count = pgm_read_byte(p + 0x00);
    int16_t x = player.positionX + GAME_VIEW_OX;
    int16_t y = player.positionY + GAME_VIEW_OY;
    ++p;
    while (count-- > 0x00) {
      AppClipPattern(
        pgm_read_byte(p + 0x00), 
        pgm_read_byte(p + 0x01), 
        x + (int8_t)pgm_read_byte(p + 0x02), 
        y + (int8_t)pgm_read_byte(p + 0x03), 
        pgm_read_byte(p + 0x04), 
        SYSTEM_VIDEO_FLIP_NORMAL
      );
      p += 0x05;
    }
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

  // ←→の移動
  if (SystemIsInputPush(LEFT_BUTTON)) {
    player.positionX -= PLAYER_SPEED;
    if (player.positionX < GAME_VIEW_LEFT + 0x08) {
      player.positionX = GAME_VIEW_LEFT + 0x08;
    }
  } else if (SystemIsInputPush(RIGHT_BUTTON)) {
    player.positionX += PLAYER_SPEED;
    if (player.positionX > GAME_VIEW_RIGHT - 0x08) {
      player.positionX = GAME_VIEW_RIGHT - 0x08;
    }
  }

  // ↑↓の移動
  if (SystemIsInputPush(UP_BUTTON)) {
    player.positionY -= PLAYER_SPEED;
    if (player.positionY < GAME_VIEW_TOP + 0x08) {
      player.positionY = GAME_VIEW_TOP + 0x08;
    }
  } else if (SystemIsInputPush(DOWN_BUTTON)) {
    player.positionY += PLAYER_SPEED;
    if (player.positionY > GAME_VIEW_BOTTOM - 0x08) {
      player.positionY = GAME_VIEW_BOTTOM - 0x08;
    }
  }

  // 発射
  if (SystemIsInputEdge(A_BUTTON)) {
    ShotEntry(player.positionX, player.positionY);
    player.fire = PLAYER_FIRE;
  } else if (SystemIsInputPush(A_BUTTON)) {
    if (player.fire > 0x00) {
      --player.fire;
    }
    if (player.fire == 0x00) {
      ShotEntry(player.positionX, player.positionY);
      player.fire = PLAYER_FIRE;
    }
  } else {
    player.fire = PLAYER_FIRE;
  }
  
  // ダメージの更新
  if (player.damage > 0x00) {
    --player.damage;
  }

  // アニメーションの更新
  ++player.animation;
}

/*
 * プレイヤーが死亡する
 */
static void PlayerDead(void)
{
  // 初期化
  if ((player.state & 0x0f) == 0x00) {

    // アニメーションの設定
    player.animation = 0x18;

    // 状態の更新
    ++player.state;
  }

  // アニメーションの更新
  if (--player.animation == 0x00) {
    player.state = PLAYER_STATE_NULL;
  }
}

/*
 * プレイヤの存在を取得する
 */
bool PlayerIsLive(void)
{
  return (player.state & 0xf0) == PLAYER_STATE_PLAY ? true : false;
}
bool PlayerIsDead(void)
{
  return (player.state & 0xf0) == PLAYER_STATE_NULL ? true : false;
}

/*
 * プレイヤの状態を判定する
 */
bool PlayerIsState(uint8_t state)
{
  return (player.state & 0xf0) == state;
}

/*
 * プレイヤの状態を設定する
 */
void PlayerSetState(uint8_t state)
{
  player.state = state;
}

/*
 * プレイヤの位置を取得する
 */
bool PlayerGetPosition(int8_t *x, int8_t *y, int8_t *z)
{
  bool result = false;
  if ((player.state & 0xf0) == PLAYER_STATE_PLAY) {
    *x = player.positionX;
    *y = player.positionY;
    *z = GAME_DISTANCE_NEAR;
    result = true;
  }
  return result;
}

/*
 * プレイヤの矩形を取得する
 */
bool PlayerGetRect(int8_t *rect, int8_t *z)
{
  bool result = false;
  if ((player.state & 0xf0) == PLAYER_STATE_PLAY && player.life > 0x00 && player.damage == 0x00) {
    rect[GAME_RECT_LEFT] = player.positionX + PLAYER_RECT_LEFT;
    rect[GAME_RECT_RIGHT] = player.positionX + PLAYER_RECT_RIGHT;
    rect[GAME_RECT_TOP] = player.positionY + PLAYER_RECT_TOP;
    rect[GAME_RECT_BOTTOM] = player.positionY + PLAYER_RECT_BOTTOM;
    *z = GAME_DISTANCE_NEAR;
    result = true;
  }
  return result;
}

/*
 * プレイヤの体力を取得する
 */
uint8_t PlayerGetLife(void)
{
  return player.life;
}

/*
 * プレイヤがダメージを受ける
 */
void PlayerDamage(void)
{
  if ((player.state & 0xf0) == PLAYER_STATE_PLAY) {
    if (player.life > 0x00 && player.damage == 0x00) {
      if (--player.life == 0x00) {
        player.state = PLAYER_STATE_DEAD;
      } else {
        player.damage = PLAYER_DAMAGE;
      }
    }
  }
}
