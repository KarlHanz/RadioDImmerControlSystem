//====ОСНОВНАЯ ПРОГРАММА=====
void loop() {


  // lightSensor();
  printDisplay();
  tachoRPM();
  menuHandler();
  rtcHandler();
  statusLED();



  hmdInt = dht.readHumidity(); //показания влажности
  tempInt = dht.readTemperature(); //Измеряем температуру

  butt1.tick();  // опрашиваем в скетче, иначе не будут работать проверки по времени!
  butt2.tick();



  //=======РАДИО ПРИЁМ/ПЕРЕДАЧА======

  transmit_data[0] = dimmer; // значение передаваемого 0-го элемента массива = значение диммирования
  radio.write(&transmit_data, sizeof(transmit_data)); // отправить по радио

  if (radio.available() ) {
    radio.read(&telemetry_data, sizeof(telemetry_data));    // читаем входящий сигнал
    radioCommState = 1;                    //флаг наличия радиосвязи с регулятором
    RPM = telemetry_data[0];    //принимаем обороты (0...2700)
    powerDev = telemetry_data[1];       //принимаем значение мощности (0...100)
    statusRelay = telemetry_data[2];         //принимаем состояние реле (0,1)
    tempExt = telemetry_data[3];        //принимаем температуру (0...100)
    statusCode = telemetry_data[4];    //принимаем цифровой код статуса (10,20,90) //90 не примем!
  }
  else {
    radioCommState = 0;                    //если радиосвязи нет, все элементы массива обнуляем
    RPM = 0;
    powerDev = 0;
    statusRelay = 0;
    tempExt = 0;
    statusCode = 0;
  }


  lightStatus = digitalRead (lightSens); //датчик света. 0 если светло, 1 если темно
  digitalWrite(ledStatus, ledState); // вывод на светодиод состояния STATUS

}
