

//====ОБРАБОТЧИК ДАТЧИКА ОСВЕЩЕННОСТИ=====

/*
void lightSensor() {

  if (lightStatus != lastlightStatus) { //при изменении статуса света запуск таймера
    if (lightStatus == LOW)   { //если свет был ВКЛ
      Serial.print("dim="); // установка яркости дисплея 100% командой dim
      Serial.print(100);
      Serial.write(0xff);
      Serial.write(0xff);
      Serial.write(0xff);
    }
    if (lightStatus == HIGH) { //если свет отключен
    myTimer0.setTimeout(30000); //установка таймера на 30 сек
      Serial.print("dim="); // установка яркости дисплея 50% командой dim
      Serial.print(50);
      Serial.write(0xff);
      Serial.write(0xff);
      Serial.write(0xff);
    }
  }
  if (myTimer0.isReady()) {  //если прошло более 30 сек
    Serial.print("dim="); // установка яркости дисплея 0% командой dim
    Serial.print(0);
    Serial.write(0xff);
    Serial.write(0xff);
    Serial.write(0xff);
  }

}
*/
//====ОПРОС КНОПОК БЫСТРОГО ВЫБОРА ПРОГРАММ=====
void isr() {
  butt1.tick();  // опрашиваем в прерывании, чтобы поймать нажатие в любом случае
  butt2.tick();
}


//====ЗАПРОС ВРЕМЕНИ С ДИСПЛЕЯ=====
void rtcHandler() {

  rtc.read_rtc_time(time, 7); //получаем массив RTC

  year = time[0];
  mon = time[1];
  day = time[2];
  hour = time[3];
  min = time[4];
  sec = time[5];
  week = time[6];

  timeHrs = hour;
  timeMins = min;
  timeSec = sec;

}

//====ИНИЦИАЛИЗАЦИЯ РАДИОМОДУЛЯ=====
void radioSetup() {         // настройка радио
  radio.begin(); //активировать модуль
  radio.setChannel(0x60);  //выбираем канал (в котором нет шумов!)
  radio.setAutoAck(1);         //режим подтверждения приёма, 1 вкл 0 выкл
  radio.setRetries(15, 15);    //(время между попыткой достучаться, число попыток)
  radio.enableAckPayload();    //разрешить отсылку данных в ответ на входящий сигнал
  radio.setPayloadSize(32);     //размер пакета, в байтах
  radio.openReadingPipe(1, pipe);     //хотим слушать трубу 0
  radio.openWritingPipe(pipe);   //мы - труба 0, открываем канал для передачи данных
  radio.setPALevel (RF24_PA_MAX); //уровень мощности передатчика. На выбор RF24_PA_MIN, RF24_PA_LOW, RF24_PA_HIGH, RF24_PA_MAX
  radio.setDataRate (RF24_1MBPS); //скорость обмена. На выбор RF24_2MBPS, RF24_1MBPS, RF24_250KBPS
  //должна быть одинакова на приёмнике и передатчике!
  //при самой низкой скорости имеем самую высокую чувствительность и дальность!!

  radio.powerUp(); //начать работу
  radio.stopListening();  //не слушаем радиоэфир, мы передатчик
}

//=========ВЫВОД ОБОРОТОВ========
void tachoRPM() {
  tacho  = map (RPM, 0, 2700, 0, 113);
  tacho = constrain(tacho, 0, 113);
}

//========светодиод состояния STATUS========
void statusLED() {
  if (radioCommState == 0) { //если нет связи с исполнительным блоком
    ledState = 0; //погасить светодиод
    statusCode = 80; //корректно ли?
  }

  if (radioCommState == 1 && errorCode < 30) { //если есть связь с приёмным блоком
    ledState = 1; //включить светодиод
  }

  if (radioCommState == 1 && errorCode >= 30) { //мигание светодиодом (наличие ошибок)
    currentMillis = millis();
    if (currentMillis - previousMillis >= 300) {
      previousMillis = currentMillis;
      if (ledState == 0) {
        ledState = 1;
      } else {
        ledState = 0;
      }
    }
  }

//  digitalWrite(ledStatus, ledState); // вывод на светодиод состояния STATUS
}
