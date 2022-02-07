void setup() {

 // radioSetup();

  pinMode (K1, OUTPUT); // режим пина как выход
  pinMode (Brg, INPUT); // режим пина яркости дисплея как вход
  pinMode(dimPin, OUTPUT);
  pinMode(zeroPin, INPUT);

  WDTCSR = (1 << WDCE) | (1 << WDE); //установить биты WDCE WDE (что б разрешить запись в другие биты
  WDTCSR = (1 << WDIE) | (1 << WDP3); // разрешение прерывания + выдержка 4 секунды


  attachInterrupt(0, isr, RISING);  // завести прерывание на детектор нуля //  D2 == 0
  attachInterrupt(1, sens, RISING);    // подключить прерывание на 3 пин при повышении сигнала);

  Timer2.enableISR();   // разрешаем прерывания по таймеру

  display.clear(); //сброс дисплея
  display.setBrightness(0x0f); //яркость дисплея
  display.setSegments(SEG_oooo);


  radio.begin(); //активировать модуль
  radio.setChannel(0x60);  //выбираем канал (в котором нет шумов!)
  radio.setAutoAck(1);         //режим подтверждения приёма, 1 вкл 0 выкл
  radio.setRetries(15, 15);    //(время между попыткой достучаться, число попыток)
  radio.enableAckPayload();    //разрешить отсылку данных в ответ на входящий сигнал
  radio.setPayloadSize(32);     //размер пакета, в байтах
  radio.openReadingPipe(1, pipe);     //хотим слушать трубу 0
  radio.openWritingPipe(pipe); //Открывем трубу для отправки
  radio.setPALevel (RF24_PA_MAX); //уровень мощности передатчика. На выбор RF24_PA_MIN, RF24_PA_LOW, RF24_PA_HIGH, RF24_PA_MAX
  radio.setDataRate (RF24_1MBPS); //скорость обмена. На выбор RF24_2MBPS, RF24_1MBPS, RF24_250KBPS
  radio.powerUp(); //начать работу
  radio.startListening();  //начинаем слушать эфир, мы приёмный модуль
  radio.writeAckPayload(1, &telemetry_data, sizeof(telemetry_data)); //отправка ответного пакета телеметрии

}
