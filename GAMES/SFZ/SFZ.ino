/* 
 * *.ino : メインプログラム 
 */

/* 
 * 参照ファイル
 */
#include "System.h"
#include "App.h"


/* 
 * プログラムを初期化する
 */
void inogamesetup()
{
  // システムの初期化
  SystemInitialize(60);

  // アプリケーションの更新
  AppInitialize();
}

/*
 * プログラムのメインルーチン
 */
void inogameloop()
{
  // システムの更新
  if (SystemUpdate()) {

    // アプリケーションの更新
    AppUpdate();
  }
}

void gameLogicLoop(void *)
{
  for (;;) {
    inogameloop(); 
    // ArduinoOTA.handle();
  }
}

void setup() {
  inogamesetup();
  xTaskCreatePinnedToCore(gameLogicLoop, "g", 4096, nullptr, 0, nullptr, 0);
}

void loop() {
	delay(60000);
}
