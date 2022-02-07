// ================ ВЫВОД НА ДИСПЛЕЙ ================

void displayPrint() {

  switch (stateCode) {
    case 0: //код локальный //начальное значение (можно убрать!)
      display.showNumberDec(RPM); // выводим на дисплей значение оборотов
      break;
    case 10: //код STARTING
      display.showNumberDec(RPM); // выводим на дисплей значение оборотов
      break;
    case 20: //код ONLINE
      display.showNumberDec(RPM); // выводим на дисплей значение оборотов
      break;
    case 30: //код NO TACHO
      display.setSegments(SEG_taho);
      break;
    case 40: //код NO DIMM
      display.setSegments(SEG_rEG);
      break;
    case 50: //код NO RELAY
      display.setSegments(SEG_rELE);
      break;
    case 60: //код OVERHEAT
      display.setSegments(SEG_hot);
      break;
    case 70: //код OVERHEAT&TROUBLE
      display.setSegments(SEG_nhot);
      break;
    case 90: //код локальный // DONE
      display.setSegments(SEG_dOnE);
      break;
  }
}

//  if (errorCode >= 30) stateCode = errorCode; //
//  else stateCode = statusCode;

// ===============КОДЫ СТАТУСА===============
// statusCode = 10; // STARTING
// statusCode = 20; // ONLINE
// statusCode = 90; // DONE

// ===============КОДЫ ОШИБОК===============
// errorCode = 30; // NO TACHO
// errorCode = 40; // NO DIMM
// errorCode = 50; // NO RELAY
// errorCode = 60; // OVERHEAT
// errorCode = 70; // OVERHEAT&TROUBLE
