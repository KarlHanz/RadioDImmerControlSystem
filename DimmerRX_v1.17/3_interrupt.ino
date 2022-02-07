//============== ОБРАБОТЧИК ПРЕРЫВАНИЯ WDT для термометра ==============

ISR (WDT_vect) { //вектор прерывания WD
  byte data[2];

  static boolean n = 0; // флаг работы: запрос температуры или её чтение
  n = !n;
  if (n) {
    ds.reset();  // сброс шины
    ds.write(0xCC); // выбор адреса
    ds.write(0x44); // начать преобразование (без паразитного питания)
  }
  else   {
    ds.reset();
    ds.write(0xCC);
    ds.write(0xBE);
    data[0] = ds.read();
    data[1] = ds.read();
    // Формируем значение
    temp = (data[1] << 8) + data[0];
    temp = temp >> 4;
  }
}


// ================ ОБРАБОТЧИК ПРЕРЫВАНИЯ sens для ТАХОМЕТРА ================

void sens() {
  flash = micros() - lastflash;   // вычислить время между двумя оборотами
  lastflash = micros();           // запомнить время последнего оборота
}


//============== ОБРАБОТЧИК ПРЕРЫВАНИЯ для диммера ==============

void isr() {
  // вызывать в прерывании детектора нуля
  if (dim.tickZero()) Timer2.setPeriod(dim.getPeriod());   // если tickZero() - true - нужно перезапустить таймер с периодом getPeriod()
  else Timer2.restart();   // иначе перезапустить со старым

}

// прерывание таймера
ISR(TIMER2_A) {
  dim.tickTimer();    // вызвать tickTimer()
  Timer2.stop();      // останавливаем таймер
}



void radioSetup() {         // настройка радио

// ================ ИНИЦИАЛИЗАЦИЯ РАДИО ================
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
