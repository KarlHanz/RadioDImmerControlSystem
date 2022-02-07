void setup() {

  pinMode (K1, OUTPUT); // режим пина как выход
  pinMode (Brg, INPUT); // режим пина яркости дисплея как вход
  pinMode(dimPin, OUTPUT);
  pinMode(zeroPin, INPUT);

  WDTCSR = (1 << WDCE) | (1 << WDE); //установить биты WDCE WDE (что б разрешить запись в другие биты
  WDTCSR = (1 << WDIE) | (1 << WDP3); // разрешение прерывания + выдержка 4 секунды

    
  attachInterrupt(0, isr, RISING);  // завести прерывание на детектор нуля //  D2 == 0
  attachInterrupt(1, sens, RISING);    // подключить прерывание на 3 пин при повышении сигнала);

  Timer2.enableISR();   // разрешаем прерывания по таймеру
  
  display.clear();
  display.setBrightness(0x0f); //яркость дисплея
  
  radioSetup();
  display.setSegments(SEG_oooo);
  delay(1000);

}
