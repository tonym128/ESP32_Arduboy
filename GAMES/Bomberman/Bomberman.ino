/*
   作者:LHW-HWT
   发现BUG或者想完善游戏可以发送邮件到1281702594@qq.com
   本作品使用CC协议 可以用作教育用途 禁止 盈利性用途
  您可以自由地：
  共享 — 在任何媒介以任何形式复制、发行本作品
  演绎 — 修改、转换或以本作品为基础进行创作
  只要你遵守许可协议条款，许可人就无法收回你的这些权利。
  署名 — 您必须给出适当的署名，提供指向本许可协议的链接，同时标明是否（对原始作品）作了修改。您可以用任何合理的方式来署名，但是不得以任何方式暗示许可人为您或您的使用背书。
  非商业性使用 — 您不得将本作品用于商业目的。包括直接拿去参加比赛
  相同方式共享 — 如果您再混合、转换或者基于本作品进行创作，您必须基于与原先许可协议相同的许可协议 分发您贡献的作品。
  没有附加限制 — 您不得适用法律术语或者 技术措施 从而限制其他人做许可协议允许的事情。
*/

#define FRAMERATE 15

#include <Arduboy2.h>
Arduboy2 arduboy;
/*=========================================================
                         位图
  =========================================================*/
/*
   炸弹人
*/
const uint8_t Man_L_1[] PROGMEM = {0x7d, 0x2e, 0x7d, 0x3c, 0x56, 0x6a, 0x3c, 0x28};
const uint8_t Man_L_2[] PROGMEM = {0x7d, 0x2e, 0x7d, 0x3c, 0x56, 0x2a, 0x3c, 0x28};
const uint8_t Man_L_3[] PROGMEM = {0x7d, 0x2e, 0x7d, 0x3c, 0x54, 0x6a, 0x3c, 0x28};
const uint8_t Man_R_1[] PROGMEM = {0xbe, 0x74, 0xbe, 0x3c, 0x56, 0x6a, 0x3c, 0x14};
const uint8_t Man_R_2[] PROGMEM = {0xbe, 0x74, 0xbe, 0x3c, 0x56, 0x2a, 0x3c, 0x14};
const uint8_t Man_R_3[] PROGMEM = {0xbe, 0x74, 0xbe, 0x3c, 0x54, 0x6a, 0x3c, 0x14};
const uint8_t Man_U_1[] PROGMEM = {0x3c, 0x3c, 0x3c, 0x3c, 0x56, 0x6a, 0x3c, 0x28};
const uint8_t Man_U_2[] PROGMEM = {0x3c, 0x3c, 0x3c, 0x3c, 0x56, 0x2a, 0x3c, 0x28};
const uint8_t Man_U_3[] PROGMEM = {0x3c, 0x3c, 0x3c, 0x3c, 0x54, 0x6a, 0x3c, 0x28};
const uint8_t Man_D_1[] PROGMEM = {0x3c, 0x28, 0x3c, 0x3c, 0x56, 0x6a, 0x3c, 0x28};
const uint8_t Man_D_2[] PROGMEM = {0x3c, 0x28, 0x3c, 0x3c, 0x56, 0x2a, 0x3c, 0x28};
const uint8_t Man_D_3[] PROGMEM = {0x3c, 0x28, 0x3c, 0x3c, 0x54, 0x6a, 0x3c, 0x28};
const unsigned char *Man_table[] = { Man_U_1, Man_U_2, Man_U_3, Man_R_1, Man_R_2, Man_R_3, Man_D_1, Man_D_2, Man_D_3, Man_L_1, Man_L_2, Man_L_3};
/*
   怪物
*/
const uint8_t M_L[] PROGMEM = {0x3c, 0x42, 0xa9, 0xa9, 0x81, 0x81, 0x7e, 0x18};
const uint8_t M_R[] PROGMEM = {0x3c, 0x42, 0x95, 0x95, 0x81, 0x81, 0x7e, 0x18};
const uint8_t M_U[] PROGMEM = {0x3c, 0x42, 0x81, 0x81, 0x81, 0x81, 0x7e, 0x18};
const uint8_t M_D[] PROGMEM = {0x3c, 0x42, 0xa5, 0xa5, 0x81, 0x81, 0x7e, 0x18};
const unsigned char *M_table[] = {M_U, M_R, M_D, M_L};
/*
   方块以及物品
*/
const uint8_t WALL_1[] PROGMEM = {0xfd, 0xfd, 0xfd, 0xfd, 0xfd, 0xfd, 0x01, 0xff};
const uint8_t WALL_2[] PROGMEM = {0x80, 0xff, 0x30, 0x20, 0xff, 0x06, 0x04, 0xff};
const uint8_t TNT_1[] PROGMEM = {0x0c, 0x12, 0x78, 0xdc, 0xbc, 0xbc, 0xfc, 0x78};
const uint8_t TNT_2[] PROGMEM = {0x0d, 0x12, 0x79, 0xdc, 0xbc, 0xbc, 0xfc, 0x78};
const uint8_t DOOR[] PROGMEM = {0x6e, 0xef, 0xef, 0xab, 0xab, 0xef, 0xef, 0x6e};
const uint8_t LOVE[] PROGMEM = {0x00, 0x66, 0xff, 0xff, 0xff, 0x7e, 0x3c, 0x18};
const uint8_t START_TITLE[] PROGMEM = {0xff, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x00, 0x00, 0x00, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc1, 0x80, 0x00, 0x00, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc1, 0x80, 0x00, 0x00, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc1, 0x80, 0x00, 0x00, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc1, 0x80, 0x00, 0x00, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc1, 0x87, 0x8f, 0xfe, 0x3f, 0x0f, 0x8f, 0x3f, 0xf0, 0xf8, 0xfc, 0xc1, 0x87, 0x8f, 0xfe, 0x3f, 0x0f, 0x8f, 0x3f, 0xf0, 0xf8, 0xfc, 0xff, 0x1c, 0xce, 0x67, 0x33, 0x99, 0xce, 0x33, 0x19, 0x8c, 0xc6, 0xff, 0x1c, 0xce, 0x67, 0x33, 0x99, 0xce, 0x33, 0x19, 0x8c, 0xc6, 0xc1, 0x9c, 0xce, 0x67, 0x33, 0x9f, 0xcc, 0x33, 0x18, 0x7c, 0xc6, 0xc1, 0x9c, 0xce, 0x67, 0x33, 0x98, 0x0c, 0x33, 0x18, 0xcc, 0xc6, 0xc1, 0x9c, 0xce, 0x67, 0x33, 0x98, 0x0c, 0x33, 0x18, 0xcc, 0xc6, 0xc1, 0x9c, 0xce, 0x67, 0x33, 0x98, 0x0c, 0x33, 0x19, 0x8c, 0xc6, 0xc1, 0x9c, 0xce, 0x67, 0x33, 0x98, 0x0c, 0x33, 0x19, 0x8c, 0xc6, 0xc1, 0x9c, 0xce, 0x67, 0x33, 0x99, 0xcc, 0x33, 0x19, 0x8c, 0xc6, 0xc1, 0x9c, 0xce, 0x67, 0x33, 0x99, 0xcc, 0x33, 0x19, 0x8c, 0xc6, 0xff, 0x07, 0x8e, 0x67, 0x3f, 0x0f, 0x8c, 0x33, 0x18, 0xfc, 0xc6, 0xff, 0x07, 0x8e, 0x67, 0x3f, 0x0f, 0x8c, 0x33, 0x18, 0xfc, 0xc6, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x8a, 0x02, 0x8a, 0x02, 0x0a, 0x0a, 0x88, 0x02, 0x08, 0x88, 0x82, 0x55, 0x05, 0x04, 0x45, 0x15, 0x05, 0x04, 0x11, 0x10, 0x54, 0x44, 0x80, 0x88, 0x8a, 0x22, 0x22, 0x88, 0x88, 0x22, 0x08, 0x88, 0x82, 0x41, 0x14, 0x44, 0x45, 0x11, 0x11, 0x44, 0x11, 0x11, 0x04, 0x44, 0x80, 0x88, 0x88, 0x20, 0x20, 0x88, 0x08, 0x20, 0x08, 0x88, 0x80, 0x41, 0x14, 0x44, 0x45, 0x11, 0x10, 0x04, 0x11, 0x11, 0x04, 0x44, 0x80, 0x88, 0x8a, 0x22, 0x22, 0x88, 0x08, 0x22, 0x08, 0x88, 0x82, 0x41, 0x14, 0x44, 0x45, 0x11, 0x10, 0x04, 0x11, 0x10, 0x44, 0x44, 0x80, 0x88, 0x8a, 0x02, 0x02, 0x8a, 0x88, 0x02, 0x08, 0x08, 0x82, 0x55, 0x14, 0x44, 0x45, 0x11, 0x11, 0x44, 0x11, 0x11, 0x04, 0x44, 0xaa, 0x08, 0x8a, 0x22, 0x22, 0x88, 0x8a, 0x22, 0x08, 0x88, 0x82, 0x41, 0x05, 0x05, 0x54, 0x15, 0x05, 0x05, 0x15, 0x50, 0x50, 0x54, 0x80, 0x80, 0x88, 0xa8, 0x28, 0x08, 0x88, 0x28, 0xa0, 0xa8, 0xa8, 0x41, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x41, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x55, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xaa, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
const uint8_t TITLE_TNT[] PROGMEM = {0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x0a, 0x00, 0x00, 0x00, 0x05, 0x53, 0xf8, 0x00, 0x00, 0x2e, 0xaa, 0xa8, 0x00, 0x00, 0x55, 0x00, 0x38, 0x00, 0x00, 0x31, 0xa2, 0xa0, 0x00, 0x00, 0x40, 0x00, 0x20, 0x00, 0x01, 0xe0, 0xaa, 0xa0, 0x00, 0x01, 0x41, 0x00, 0x30, 0x00, 0x1b, 0xa3, 0xaa, 0xf8, 0x00, 0x15, 0x40, 0x13, 0x00, 0x00, 0x0f, 0xe0, 0x0a, 0x00, 0x00, 0x55, 0x50, 0x0a, 0x00, 0x01, 0xbb, 0xba, 0x04, 0x00, 0x05, 0x55, 0x55, 0x00, 0x00, 0x0f, 0xff, 0xff, 0x80, 0x00, 0x15, 0x55, 0x55, 0x40, 0x00, 0x1b, 0xbb, 0xbb, 0x80, 0x00, 0x15, 0x55, 0x55, 0x40, 0x00, 0x6f, 0xef, 0xef, 0xe0, 0x00, 0x55, 0x55, 0x55, 0x50, 0x00, 0x3b, 0xbb, 0xbb, 0xb0, 0x00, 0x55, 0x55, 0x55, 0x50, 0x00, 0xff, 0xff, 0xff, 0xf8, 0x00, 0x55, 0x55, 0x55, 0x50, 0x00, 0xbb, 0xbb, 0xbb, 0xb8, 0x00, 0x55, 0x55, 0x55, 0x50, 0x00, 0xef, 0xef, 0xef, 0xe8, 0x00, 0x55, 0x55, 0x55, 0x50, 0x00, 0x3b, 0xbb, 0xbb, 0xb0, 0x00, 0x55, 0x55, 0x55, 0x50, 0x00, 0x7f, 0xff, 0xff, 0xf0, 0x00, 0x15, 0x55, 0x55, 0x40, 0x00, 0x3b, 0xbb, 0xbb, 0x80, 0x00, 0x15, 0x55, 0x55, 0x40, 0x00, 0x0f, 0xef, 0xef, 0x80, 0x00, 0x05, 0x55, 0x55, 0x00, 0x00, 0x01, 0xbb, 0xb8, 0x00, 0x00, 0x00, 0x55, 0x50, 0x00, 0x00, 0x00, 0x1f, 0xc0, 0x00, 0x00};
const uint8_t LHW[] PROGMEM = {0x72, 0x25, 0x10, 0x94, 0x5e, 0x8a, 0x25, 0x16, 0x94, 0x48, 0xaa, 0x3d, 0x56, 0xf5, 0x48, 0x8a, 0x25, 0x50, 0x95, 0x48, 0x73, 0xa4, 0xa0, 0x92, 0x88};
const unsigned char *TNT_table[] = {TNT_1, TNT_2};
/*
   爆炸效果
*/
const uint8_t BOOM_1[] PROGMEM = {0x00, 0x10, 0x2c, 0x3a, 0x5c, 0x34, 0x08, 0x00};
const uint8_t BOOM_2[] PROGMEM = {0x00, 0x52, 0x2c, 0x22, 0x44, 0x34, 0x4a, 0x00};
const uint8_t BOOM_3[] PROGMEM = {0x50, 0x21, 0x0a, 0x21, 0x98, 0x40, 0x84, 0x0a};
const unsigned char *BOOM_table[] = {BOOM_1, BOOM_2, BOOM_3};

/*=========================================================
                         变量
  =========================================================*/
uint8_t LIFE; //玩家生命值
uint8_t LEVEL; //关卡 每一关会多一个怪物 直到10关通关
uint8_t KeyBack; //按键返回值
uint8_t MAP[31][15]; //临时存放地图
uint8_t monster[10][2]; //格式 怪物编号 坐标
uint8_t MLRUD[10]; //怪物方向 0为左 1为右 2为上 3为下 255为怪物已经死亡
uint8_t PX, PY, PP, PS; //玩家坐标 方向 动画帧
bool PMove = false;
bool MMMod; //怪物移动模式 false 为顺时针 true为逆时针
bool BMove; //判断是否有障碍物的全局返回值
signed char CSX, CSY; //坐标轴画布移位值 0-7
uint8_t SBDPL[] = {1, 2, 3}; //障碍物列表
uint8_t TntList[10][2];  //TNT坐标
uint8_t TNTN; //TNT个数
unsigned long TntTime[10]; //TNT爆炸时间计算
unsigned long MMTime; //怪物移动冷却
unsigned long PIT; //玩家无敌时间
uint8_t TNTS; //TNT动态效果
bool PWIN;
#define BOOMTime 3000 //TNT爆炸需要等待的时间
#define MMTimeOut 100 //怪物移动冷却时间
#define Invincible_Time 5000 //无敌时间
/*====================================================================
                             软重启函数
  ====================================================================*/
void(* resetFunc) (void) = 0; //制造重启命令
/*=========================================================
                     只循环一次
  =========================================================*/
void setup() {
  arduboy.boot();
  //Serial.begin(115200);
  randomSeed(analogRead(A0));
  MENU();
  LEVEL = 1;
  ShowLevel();
  BuildMap();
}
/*=========================================================
                     不停循环
  =========================================================*/
void loop() {
 static uint32_t millisprev;
  while (millis()-millisprev < 1000/FRAMERATE) delay(1);
  millisprev = millis();
  if (LIFE == 0) Fail();
  key();
  Draw();
  logic();
}
/*=========================================================
  按键扫描
  =========================================================*/
void key() {
  /*
      0  1  2  3  4  5
      ↑ ↓← →  A  B
  */
  KeyBack = 255;
  if (arduboy.pressed(UP_BUTTON)) KeyBack = 0;
  if (arduboy.pressed(DOWN_BUTTON)) KeyBack = 1;
  if (arduboy.pressed(LEFT_BUTTON)) KeyBack = 2;
  if (arduboy.pressed(RIGHT_BUTTON)) KeyBack = 3;
  if (arduboy.pressed(A_BUTTON)) KeyBack = 4;
  if (arduboy.pressed(B_BUTTON)) KeyBack = 5;
}
/*=========================================================
                     地图生成器
  =========================================================*/
void BuildMap() {
  /*
     0  空气
     1  不可摧毁墙
     2  普通墙
     3  TNT
     4  爆炸3
     5  爆炸2
     6  爆炸1
  */
  //生成墙和怪物
  LIFE = 3; //开局3条命
  uint8_t MN = 0; //重置怪物计数器
  PP = 2; //设置玩家方向头朝下
  for (uint8_t n = 0; n < 10; n++) {
    TntTime[n] = 0;  //防止TNT在下一关爆炸
    MLRUD[n] = 255;
  }
  for (uint8_t y = 0; y < 15; y++) {
    for (uint8_t x = 0; x < 31; x++) {
      if (y == 0 || y == 14) {
        MAP[x][y] = 1;
      } else if (x == 0 || x == 30) {
        MAP[x][y] = 1;
      } else {
        if (x % 2 == 0 && y % 2 == 0) {
          MAP[x][y] = 1;
        } else if (random(0, 4) == 0) {
          MAP[x][y] = 2;
        } else if (random(0, 28) == 0) {
          //生成怪物
          if (MN < LEVEL) {
            monster[MN][0] = x;
            monster[MN][1] = y;
            MLRUD[MN] = 2; //设置怪物方向为头朝下
            MN++;
          }
        } else MAP[x][y] = 0;
      }
    }
  }
  //设置玩家出生点
  for (uint8_t py = 0; py < 3; py++) {
    for (uint8_t px = 0; px < 3; px++) {
      //清空出生点附近的普通墙和怪物，确保出生点安全
      MAP[15 - 1 + px][7 - 1 + py] = 0;
    }
  }
  PX = 15;
  PY = 7;
  PIT = millis();
}
/*====================================================================
                             绘图
  ====================================================================*/
void Draw() {
  //arduboy.clear(); //清空显存
  DrawMap(); //渲染地图
  DrawEntity(); //渲染实体
  arduboy.fillRect(0, 56, 128, 8, 0);
  for (uint8_t ni = 0; ni < LIFE; ni++) {
    arduboy.drawSlowXYBitmap(ni * 9, 56, LOVE, 8, 8, 1); //生命条
  }
  arduboy.display(); //发送画面到屏幕
}
/*====================================================================
                             渲染地图
  ====================================================================*/
void DrawMap() {
  arduboy.fillRect(0, 0, 128, 64, 1);
  for (signed char y = PY - 4; y < PY + 5; y++) {
    for (signed char x = PX - 8; x < PX + 10; x++) {
      if (x >= 0 && y >= 0 && x <= 30 && y <= 14) {
        switch (MAP[x][y]) {
          case 1:
            arduboy.drawSlowXYBitmap(x * 8 - (PX - 15) * 8 - 64 + CSX, y * 8 - (PY - 7) * 8 - 32 + CSY, WALL_1, 8, 8, 0);
            break;
          case 2:
            arduboy.drawSlowXYBitmap(x * 8 - (PX - 15) * 8 - 64 + CSX, y * 8 - (PY - 7) * 8 - 32 + CSY, WALL_2, 8, 8, 0);
            break;
          case 3:
            arduboy.drawSlowXYBitmap(x * 8 - (PX - 15) * 8 - 64 + CSX, y * 8 - (PY - 7) * 8 - 32 + CSY, TNT_table[TNTS], 8, 8, 0);
            break;
          case 4:
            arduboy.drawSlowXYBitmap(x * 8 - (PX - 15) * 8 - 64 + CSX, y * 8 - (PY - 7) * 8 - 32 + CSY, BOOM_1, 8, 8, 0);
            break;
          case 5:
            arduboy.drawSlowXYBitmap(x * 8 - (PX - 15) * 8 - 64 + CSX, y * 8 - (PY - 7) * 8 - 32 + CSY, BOOM_2, 8, 8, 0);
            break;
          case 6:
            arduboy.drawSlowXYBitmap(x * 8 - (PX - 15) * 8 - 64 + CSX, y * 8 - (PY - 7) * 8 - 32 + CSY, BOOM_3, 8, 8, 0);
            break;
        }
      }
    }
  }
  TNTS++;
  if (TNTS >= 2) TNTS = 0;
}
/*====================================================================
                             渲染实体
  ====================================================================*/
void DrawEntity() {
  if (LIFE > 0) {
    //渲染怪物
    for (uint8_t n = 0; n < 10; n++) {
      if (MLRUD[n] != 255) arduboy.drawSlowXYBitmap(monster[n][0] * 8 - (PX - 15) * 8 - 64 + CSX, monster[n][1] * 8  - (PY - 7) * 8 - 32 + CSY, M_table[uint8_t(MLRUD[n])], 8, 8, 0);
    }
    //渲染玩家
    if (millis() >= PIT + Invincible_Time) {
      arduboy.drawSlowXYBitmap(56, 24, Man_table[PP * 3 + PS] , 8, 8, 0); //玩家图像为方向*3+动画帧
    } else if (PS == 0) arduboy.drawSlowXYBitmap(56, 24, Man_table[PP * 3 + PS] , 8, 8, 0); //无敌模式的时候闪烁效果
    if (PMove == true || millis() < PIT + Invincible_Time) { //只有在玩家移动的时候或者无敌模式 才会有移动动画
      PS++;
      if (PS > 2) PS = 0;
    } else PS = 0;
  }
}
/*====================================================================
                              障碍物判断
  ====================================================================*/
void SBDP(uint8_t SBP, uint8_t sx, uint8_t sy) {
  BMove = true;
  signed char SX, SY;
  switch (SBP) {
    case 3:
      SX = -1;
      SY = 0;
      break;
    case 1:
      SX = +1;
      SY = 0;
      break;
    case 0:
      SX = 0;
      SY = -1;
      break;
    case 2:
      SX = 0;
      SY = +1;
      break;
  }
  uint8_t length = sizeof(SBDPL) / sizeof(SBDPL[0]);
  for (uint8_t i = 0; i < length; i++) {
    if (MAP[sx + SX][sy + SY] == SBDPL[i]) BMove = false;

  }
}
/*=========================================================
  逻辑
  =========================================================*/
void logic() {
  /*
     怪物AI
  */
  if (LIFE > 0) {
    if (millis() >= MMTime + MMTimeOut) {

      MMTime = millis();
      for (uint8_t n = 0; n < 10; n++) {
        if (MLRUD[n] != 255) {

          SBDP(MLRUD[n], monster[n][0], monster[n][1]); //通过怪物方向 坐标调用障碍物判断函数
          if (BMove == true) {
            //移动合法
            switch (MLRUD[n]) {
              case 0:
                monster[n][1]--;
                break;
              case 1:
                monster[n][0]++;
                break;
              case 2:
                monster[n][1]++;
                break;
              case 3:
                monster[n][0]--;
                break;
            }
          } else MLRUD[n] = random(0, 4);
        }
      }
    }
  }
  /*
     判断是否通关
  */
  PWIN = true;
  for (uint8_t n = 0; n < 10; n++) {
    if (MLRUD[n] != 255) PWIN = false;
  }
  if (PWIN == true) WIN();
  /*
     遍历地图 刷新数据
  */
  for (uint8_t y = 0; y < 15; y++) {
    for (uint8_t x = 0; x < 31; x++) {
      if (MAP[x][y] == 4) MAP[x][y] = 5; else if (MAP[x][y] == 5) MAP[x][y] = 6; else if (MAP[x][y] == 6) MAP[x][y] = 0; //让爆炸切换下一帧
    }
  }
  if (LIFE > 0) {
    for (uint8_t i = 0; i < 10; i++) {
      if (MAP[monster[i][0]][monster[i][1]] >= 4 && MLRUD[i] != 255) { //怪物脚下是爆炸并且怪物不能为死的状态
        //怪物脚下是爆炸
        MLRUD[i] = 255; //设置怪物已经死亡
      }
      if (millis() >= PIT + Invincible_Time) { //玩家不在无敌状态
        if (PX == monster[i][0] && PY == monster[i][1] && MLRUD[i] != 255 || MAP[PX][PY] >= 4) { //怪物伤害 或者 TNT伤害
          LIFE--;
          PIT = millis();
        }
      }
    }
    /*
      控制移动 以及移动相关动画
    */
    switch (KeyBack) {
      case 0:
        PP = 0;
        SBDP(PP, PX, PY);
        if (PY > 1 && BMove == true) {
          PMove = true;
          for (CSY = 1; CSY <= 7; CSY += 3) Draw();
          PY--;
        }
        break;
      case 1:
        PP = 2;
        SBDP(PP, PX, PY);
        if (PY < 13 && BMove == true) {
          PMove = true;
          for (CSY = -1; CSY >= -7; CSY -= 3) Draw();
          PY++;
        }
        break;
      case 2:
        PP = 3;
        SBDP(PP, PX, PY);
        if (PX > 1 && BMove == true) {
          PMove = true;
          for (CSX = 1; CSX <= 7; CSX += 3) Draw();
          PX--;
        }
        break;
      case 3:
        PP = 1;
        SBDP(PP, PX, PY);
        if (PX < 29 && BMove == true) {
          PMove = true;
          for (CSX = -1; CSX >= -7; CSX -= 3) Draw();
          PX++;
        }
        break;
      case 4:
        //放TNT
        if (TNTN < 10 && MAP[PX][PY] != 3) {
          //注意0为没有TNT 范围1-10
          TNTN++;
          TntList[TNTN - 1][0] = PX;
          TntList[TNTN - 1][1] = PY;
          MAP[PX][PY] = 3;
          TntTime[TNTN - 1] = millis();
        }
        break;
    }
    if (PMove == true) {
      CSX = 0;
      CSY = 0;
      PMove = false;
    }
    /*
       计算TNT爆炸
    */
    if (TNTN != 0) {
      //存在炸弹
      if (millis() >= TntTime[0] + BOOMTime) {  //检查最近一个TNT是否到起爆时间
        MAP[TntList[0][0]][TntList[0][1]] = 4; //引爆
        //摧毁附近的非坚固实体或者方块
        for (uint8_t BOOMx = 0; BOOMx < 3; BOOMx++) {
          if (MAP[TntList[0][0] - 1 + BOOMx][TntList[0][1]] != 1 && MAP[TntList[0][0] - 1 + BOOMx][TntList[0][1]] != 3) {
            MAP[TntList[0][0] - 1 + BOOMx][TntList[0][1]] = 4;
          }
        }
        for (uint8_t BOOMy = 0; BOOMy < 3; BOOMy++) {
          if (MAP[TntList[0][0]][TntList[0][1] - 1 + BOOMy] != 1 && MAP[TntList[0][0] - 1 + BOOMy][TntList[0][1]] != 3) {
            MAP[TntList[0][0]][TntList[0][1] - 1 + BOOMy] = 4;
          }
        }
        //让TNT列表向前移位
        TNTN--; //减少一枚TNT
        for (uint8_t TNTi = 0; TNTi < TNTN; TNTi++) {
          TntList[TNTi][0] = TntList[TNTi + 1][0];
          TntList[TNTi][1] = TntList[TNTi + 1][1];
          TntTime[TNTi] = TntTime[TNTi + 1];
        }

      }
    }
  }
}
/*=========================================================
                     通关
  =========================================================*/
void WIN() {
  if (LEVEL == 10) {
    arduboy.clear();
    arduboy.setCursor(16, 0);
    arduboy.println(F("CONGRATULATIONS"));
    arduboy.println(F(" BOMBER MAN BECOMES"));
    arduboy.println(F("       RUNNER"));
    arduboy.println(F("SEE YOU AGAIN IN LODE"));
    arduboy.println(F("       RUNNER"));
    arduboy.drawSlowXYBitmap(56, 48, Man_table[3] , 8, 8, 1);
    arduboy.fillRect(0, 56, 128, 8, 1);
    for (uint8_t x = 0; x < 128; x += 8) {
      arduboy.drawSlowXYBitmap(x, 56, WALL_2, 8, 8, 0);
    }
    arduboy.display();
    while (1) {}
  } else {
    LEVEL++;   //关卡+1
    ShowLevel(); //显示第几关
    BuildMap();  //构建地图
  }
}
/*=========================================================
                     显示关卡
  =========================================================*/
void ShowLevel() {
  arduboy.clear();
  arduboy.setCursor(52, 16);
  arduboy.println(F("LEVEL"));
  arduboy.setCursor(64, 32);
  arduboy.println(LEVEL);
  arduboy.display();
  delay(1000);
}
/*=========================================================
                     玩家死亡
  =========================================================*/
void Fail() {
  for (uint8_t y = 0; y < 15; y++) {
    for (uint8_t x = 0; x < 31; x++) {
      MAP[x][y] = 4;
    }
  }
  while (MAP[0][0] >= 3) {
    Draw();
    logic();
    delay(500);
  }
  Draw();
  arduboy.drawSlowXYBitmap(56, 24, Man_table[3] , 8, 8, 0);
  arduboy.display();
  delay(5000);
  resetFunc(); //重启游戏
}
/*=========================================================
                     主菜单
  =========================================================*/
void MENU() {
  bool POA = false;
  while (POA == true || KeyBack != 4) {
    delay(0);
    key();
    switch (KeyBack) {
      case 0:
        POA = false;
        break;
      case 1:
        POA = true;
        break;
      case 4:
        if (POA == true) {
          KeyBack = 255;
          arduboy.clear();
          arduboy.setCursor(0, 0);
          arduboy.println(F(" >About"));
          arduboy.println(F(""));
          arduboy.println(F("LHW programming"));
          arduboy.println(F("LHW Art"));
          arduboy.println(F("E-mail"));
          arduboy.println(F("1281702594@qq.com"));
          arduboy.println(F(""));
          arduboy.println(F("Any key back..."));
          arduboy.display();
          delay(200);
          while (KeyBack == 255) {key();delay(0);}
          delay(200);
        }
        break;
    }
    arduboy.clear();
    arduboy.drawSlowXYBitmap(39, 1, START_TITLE , 87, 39, 1);  //大标题
    arduboy.drawSlowXYBitmap(0, 23, TITLE_TNT , 37, 41, 1);    //TNT图标
    arduboy.drawSlowXYBitmap(65, 58, LHW , 39, 5, 1);          //作者信息
    arduboy.setCursor(70, 39);
    arduboy.println(F("PLAY"));
    arduboy.setCursor(70, 47);
    arduboy.println(F("ABOUT"));
    if (POA == false) arduboy.setCursor(62, 39); else arduboy.setCursor(62, 47);
    arduboy.println(F("*"));
    arduboy.display();
  }
}
