void printDisplay() {


  //=========ВЫВОД ЗНАЧЕНИЙ МАССИВА НА ДИСПЛЕЙ======
  tach.setPic(tacho);
  pwrDev.setValue(powerDev);
  temp.setValue(tempExt);
  tmp.setValue(tempInt);
  hmd.setValue(hmdInt);

  if (radioCommState == 0) { //если нет связи
    relay.setPic(117); //BLANK
  }
  if (statusRelay == 0 && radioCommState == 1) { //если реле выключено и есть связь
    relay.setPic(118); //DIMM
  }
  if (statusRelay == 1 && radioCommState == 1) { //если реле включено и есть связь
    relay.setPic(119); //LOAD
  }

  //========ВЫВОД СТАТУСА и ОШИБОК========

  switch (statusCode) {

    case 0: //код локальный NULL
      sts.setText("OFFLINE");
      break;

    case 10: //код принят от исполнительного блока
      sts.setText("STARTING");
      break;

    case 20: //код принят от исполнительного блока
      sts.setText("ONLINE");
      break;

    case 90:  //код локальный
      sts.setText("DONE!");
      break;

    case 110: //код принят от исполнительного блока
      sts.setText("NO TACHO");
      break;

    case 120: //код принят от исполнительного блока
      sts.setText("NO DIMM");
      break;

    case 130: //код принят от исполнительного блока
      sts.setText("NO RELAY");
      break;

    case 140: //код принят от исполнительного блока
      sts.setText("OVERHEAT");
      break;

    case 150: //код принят от исполнительного блока
      sts.setText("HEAT&FAIL");
      break;
  }

}
