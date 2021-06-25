void gameLogicLoop(void *)
{
  for (;;) {
    inogameloop(); 
    ArduinoOTA.handle();
  }
}

void setup() {
  inogamesetup();
  xTaskCreatePinnedToCore(gameLogicLoop, "g", 4096, nullptr, 0, nullptr, 0);
}

void loop() {
	delay(100);
}
