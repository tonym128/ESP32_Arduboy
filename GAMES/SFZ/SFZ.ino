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
void setup()
{
  // システムの初期化
  SystemInitialize(60);

  // アプリケーションの更新
  AppInitialize();
}

/*
 * プログラムのメインルーチン
 */
void loop()
{
  // システムの更新
  if (SystemUpdate()) {

    // アプリケーションの更新
    AppUpdate();
  }
}

