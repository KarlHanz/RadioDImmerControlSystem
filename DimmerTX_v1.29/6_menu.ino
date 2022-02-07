void menuHandler() {

  if (butt1.isClick()) { // кнопка PROGRAM MODE (по кольцу)
    modes++;
    myDFPlayer.play(1);  // звук нажатия кнопки 1
    if (modes > 6) modes = 0;
  }

  if (gpio.digital_read(5) == 0)
  {
    modes--;
    if (modes < 0) modes = 0;
  }

  if (gpio.digital_read(2) == 0)
  {
    modes++;
    if (modes > 6) modes = 6;
  }


  // Управление кнопками оборотами работает независимо от программ
  if (gpio.digital_read(3) == 0)
  {
    dimmer = dimmer - 10;
    if (dimmer <= 0) dimmer = 0;
  }

  if (gpio.digital_read(4) == 0)
  {
    dimmer = dimmer + 10;
    if (dimmer >= 250) dimmer = 250;
  }


  //========ПРОГРАММЫ РАБОТЫ ДЕВАЙСА ========
  switch (modes) {   // AUTO -> DRY -> TIMER -> NIGHT -> ON -> OFF

    //     if (radio_yes == 0) { // если нет связи - ничего не делать

    /*
      ============  mode 0 - AUTO  ===========
      Режим по свету:

      //  2700 оборотов при включении света, через 15 мин - 2200, через 15 мин - 1200, при выключении света те же обороты ещё 15 мин и выкл.
      При наличии влажности >30% = 2200. >70% = 2700
      По окончании программы обороты = 0, высветить done!
    */
    case 0: // режим 0 - AUTO = SILENT+DRY  //РАБОТАЕТ, но коллизии не исследованы //при старте девайса как запустится программа?
      mode.setText("AUTO");
      if (gpio.digital_read(1) == 0) { //если нажата кнопка ENTER, начинаем выполнение программы
        mode.Set_font_color_pco(2047); //цвет текста бирюзовый
        myTimer1.setTimeout(300);
      }
      if (butt2.isClick()) {
        mode.Set_font_color_pco(2047); //цвет текста бирюзовый
        myDFPlayer.play(2);  // звук нажатия кнопки 2
        myTimer1.setTimeout(300);
      }
      if (myTimer1.isReady()) {
        mode.Set_font_color_pco(65504); //цвет текста обратно зелёный
        myTimer2.setTimeout(10000); //установка таймера на 10 сек (10-15 мин желательно) //выставить его в setup
        dimmer = 255; //запуск оборотов на полную при выборе этой программы кнопкой
      }

      if (myTimer2.isReady()) dimmer = 170; //запуск оборотов на средний режим. Таймер срабатывает однократно! РАБОТАЕТ!

      //Всегда отслеживаем влажность, без света тоже
      if ((hmdInt > 70) && (hmdInt <= 100)) dimmer = 250; //запуск оборотов на макс при наличии влажности
      if ((hmdInt > 40) && (hmdInt <= 70)) dimmer = 190; //запуск оборотов на средний режим // запустится ли с ноля оборотов?
      if ((hmdInt >= 0) && (hmdInt <= 40)) { //при малой влажности работа в зависимости от света

        if (lightStatus != lastlightStatus) { //при изменении статуса света перезапуск таймеров
          if (lightStatus == HIGH) { //если свет был ВЫКЛ
            dimmer = 110; //для отслеживания команды
            myTimer4.setTimeout(10000); //
          }
          if (lightStatus == LOW)   { //если свет ВКЛ, перезапуск программы
            myTimer1.setTimeout(300);  //перезапуск таймера
            myTimer4.stop(); //для устранения коллизии, если светом моргнуть
          }
          lastlightStatus = lightStatus;
        }
        if (myTimer4.isReady()) { //таймер на выключение. но если будет влажность, после его срабатывания отслеживание влажности продолжится
          dimmer = 0; //выключить
          //       statusCode = 90; //высветить dOnE на внутреннем дисплее
        }
      }
      break;

    /*
      ============  mode 2 - DRY  ===========
      Максимальные обороты на 30 мин, далее снижение оборотов до 2200 на 15 мин, далее снижение оборотов до 1200 на 15 мин
      Не зависит от света
      По окончании программы обороты = 0, высветить done!
    */
    case 1: // режим 1 - программа сухости РАБОТАЕТ!!! Выставить таймеры и мощность! При включении света перезапуск программы
      mode.setText("DRY");
      if (gpio.digital_read(1) == 0) { //если нажата кнопка ENTER, начинаем выполнение программы
        mode.Set_font_color_pco(2047); //цвет текста бирюзовый
        myTimer1.setTimeout(300);
      }
      if (butt2.isClick()) {
        mode.Set_font_color_pco(2047); //цвет текста бирюзовый
        myDFPlayer.play(2);  // звук нажатия кнопки 2
        myTimer1.setTimeout(300);
      }
      if (myTimer1.isReady()) { //начинаем выполнение программы по истечении таймера
        mode.Set_font_color_pco(65504); //цвет текста обратно зелёный
        myTimer2.setTimeout(10000); //таймер на 30 мин
        dimmer = 255; // максимальные обороты на 30 мин
      }
      if (myTimer2.isReady()) {
        dimmer = 150; // после срабатывания таймера обороты 2200
        myTimer3.setTimeout(10000);  // после срабатывания таймера запуск другого таймера на 10 мин
      }
      if (myTimer3.isReady()) {
        dimmer = 100; // после срабатывания таймера обороты 2200
        myTimer4.setTimeout(10000);  // после срабатывания таймера запуск другого таймера на 10 мин
      }
      if (myTimer4.isReady()) {
        dimmer = 0; //выключаем обороты после 30 мин работы
        progEnd = 1;
        //       statusCode = 90; //высветить dOnE на внутреннем дисплее
      }

      if (lightStatus != lastlightStatus) { //при изменении статуса света перезапуск таймеров
        if (lightStatus == LOW && progEnd == 1)   { //если свет ВКЛ и программа закончилась
          myTimer1.setTimeout(300);  //перезапуск таймера1 - запуск программы 0 заново
          progEnd == 0; //обнуляю флаг
        }
        lastlightStatus = lightStatus;
      }
      break;

    /*
      ============  mode 3 - SILENT  ===========
      Тихий режим. Обычно выключено, отслеживание влажности: включается и выключается без отслеживания света
      Не зависит от света
      По окончании программы обороты = 0, высветить done!
    */
    case 2: // режим 2 - программа SILENT! РАБОТАЕТ! //Не требует подтверждения. Для подтверждения добавить bool startEn
      mode.setText("SILENT");
      if (gpio.digital_read(1) == 0) { //если нажата кнопка ENTER, начинаем выполнение программы
        mode.Set_font_color_pco(2047); //цвет текста бирюзовый
        myTimer1.setTimeout(300);
      }
      if (butt2.isClick()) {
        mode.Set_font_color_pco(2047); //цвет текста бирюзовый
        myDFPlayer.play(2);  // звук нажатия кнопки 2
        myTimer1.setTimeout(300);
      }
      if (myTimer1.isReady()) { //таймер срабатывает единократно!!!!! и код выполняется ОДИН раз
        mode.Set_font_color_pco(65504); //цвет текста обратно зелёный
      }
      if ((hmdInt > 70) && (hmdInt <= 100)) dimmer = 250; //запуск оборотов на макс
      if ((hmdInt > 40) && (hmdInt <= 70)) dimmer = 170; //запуск оборотов на средний режим // запустится ли с ноля оборотов?
      if ((hmdInt >= 0) && (hmdInt <= 40)) dimmer = 0; //выключить //

      break;


    /*
      ============  mode 3 - TIMER  ===========
      С 10 до 22 включение каждый час на 15 мин на 2700, с 22 до 10 включение каждый час на 30 мин на 1200, но 30 сек N=2000
      ? проверка соответствия скорости оборотов!
    */

    case 3: // режим 3 - программа низких оборотов (на ночь) РАБОТАЕТ! Не требует подтверждения. Выставить таймеры и мощность!
      mode.setText("TIMER");
      if (gpio.digital_read(1) == 0) { //если нажата кнопка ENTER, начинаем выполнение программы
        mode.Set_font_color_pco(2047); //цвет текста бирюзовый
        myTimer1.setTimeout(300);
      }
      if (butt2.isClick()) {
        mode.Set_font_color_pco(2047); //цвет текста бирюзовый
        myDFPlayer.play(2);  // звук нажатия кнопки 2
        myTimer1.setTimeout(300);
      }
      if (myTimer1.isReady()) {
        mode.Set_font_color_pco(65504); //цвет текста обратно зелёный
        flagStart = 1;
      }

      if (timeHrs >= 10 && timeHrs <= 22 && timeMins >= 0 && timeMins <= 10) {  // с 10 до 22 включение каждый час на 10 мин на 2700
        dimmer = 255; //  С 10 до 22 включение каждый час на 10 мин на 2700
      }
      if (timeHrs >= 10 && timeHrs <= 22 && timeMins > 10 && timeMins <= 59) {
        dimmer = 0; // отключить
      }

      if (timeHrs < 22 && timeHrs > 10 && timeMins >= 0 && timeMins <= 10) {  // с 22 до 10 включение каждый час на 10 мин на 1200
        if (flagStart == 1) {
          if (RPM <= 2600)  dimmer = 255;
          else {
            flagStart = 0;
            dimmer = 100; //уточнить обороты
          }
        }
      }
      if (timeHrs < 20 && timeHrs > 10 && timeMins > 10 && timeMins <= 59) {
        dimmer = 0; // отключить
        flagStart = 1;
      }
      break;


    /*
      ============  mode 4 - NIGHT  ===========
      Тихий режим, 1200 оборотов на ночь (например, до 6ч утра)
      При включении света обороты = 2400, при выключении опять 1200.
      По окончании программы обороты = 0, высветить done!
    */
    case 4: // режим 4 - программа NIGHT РАБОТАЕТ! //
      mode.setText("NIGHT");
      if (gpio.digital_read(1) == 0) { //4если нажата кнопка ENTER, начинаем выполнение программы
        mode.Set_font_color_pco(2047); //цвет текста бирюзовый
        myTimer1.setTimeout(300);
      }
      if (butt2.isClick()) {
        mode.Set_font_color_pco(2047); //цвет текста бирюзовый
        myDFPlayer.play(2);  // звук нажатия кнопки 2
        myTimer1.setTimeout(300);
      }

      if (myTimer1.isReady()) {
        mode.Set_font_color_pco(65504); //цвет текста обратно зелёный
        dimmer = 190; //выкл обороты //как днём!
      }
      if (timeHrs >= 17 && timeHrs <= 18) { //с нуля до шести утра (5:59) следим за светом
        if (lightStatus != lastlightStatus) { //при изменении статуса света запуск таймера
          if (lightStatus == HIGH) { //если свет был ВЫКЛ, РАБОТАЕТ!
            dimmer = 90; //запуск оборотов на малый режим
          }
          if (lightStatus == LOW)   { //если свет ВКЛ, РАБОТАЕТ!
            dimmer = 190; //запуск оборотов на средний режим
          }
          lastlightStatus = lightStatus;
        }
      }
      if (timeHrs == 6 && timeMins == 0) { //в 6:00 утра сменить режим на №0
        dimmer = 0; //выкл обороты
        modes = 0; //в 6:00 утра сменить режим на №0
      }


      break;

    /*
      ============  mode 5 - ON  ===========
      Зависит от света. При вкл света - ON, при выключении света - OFF через час)
    */
    case 5: // режим 5 - ВКЛ обороты //РАБОТАЕТ!!!
      mode.setText("ON");
      if (gpio.digital_read(1) == 0) { //если нажата кнопка ENTER, начинаем выполнение программы
        mode.Set_font_color_pco(2047); //цвет текста бирюзовый
        myTimer1.setTimeout(300);
      }
      if (butt2.isClick()) {
        mode.Set_font_color_pco(2047); //цвет текста бирюзовый
        myDFPlayer.play(2);  // звук нажатия кнопки 2
        myTimer1.setTimeout(300);
      }
      if (myTimer1.isReady()) {
        mode.Set_font_color_pco(65504); //цвет текста обратно зелёный
        dimmer = 255;
      }

      if (lightStatus != lastlightStatus) { //при изменении статуса света запуск таймера
        if (lightStatus == LOW)   { //если свет был ВКЛ
          dimmer = 255; //запуск оборотов на полную
        }
        if (lightStatus == HIGH) { //если свет был ВЫКЛ
          myTimer4.setTimeout(10000); //таймер на 60 мин
          dimmer = 170; //запуск оборотов на макс
        }
        lastlightStatus = lightStatus;
      }
      if (myTimer4.isReady()) dimmer = 0;
      break;


    /*
      ============  mode 6 - OFF  ===========
      Обороты = 0
      (при выключении света программа меняется на авто)
    */
    case 6: // режим 6 - ВЫКЛ обороты //РАБОТАЕТ!!!
      mode.setText("OFF");
      if (gpio.digital_read(1) == 0) { //6если нажата кнопка ENTER, начинаем выполнение программы
        mode.Set_font_color_pco(2047); //цвет текста бирюзовый
        myTimer1.setTimeout(300); //таймер запуска программы
      }
      if (butt2.isClick()) {
        mode.Set_font_color_pco(2047); //цвет текста бирюзовый
        myDFPlayer.play(2);  // звук нажатия кнопки 2
        myTimer1.setTimeout(300);
      }
      if (myTimer1.isReady()) { //отработка программы
        mode.Set_font_color_pco(65504); //цвет текста обратно зелёный
        dimmer = 0;
      }
      if (lightStatus != lastlightStatus) { //при изменении статуса света смена программы
        if (lightStatus == HIGH)  modes = 0;  //если свет был ВЫКЛ переключение на программу №0 без её запуска
        lastlightStatus = lightStatus;
      }
      break;
  }

}


/*
  myDFPlayer.play(1);  // звук запуска
  myDFPlayer.play(2);  // звук нажатия кнопки 1
  myDFPlayer.play(3);  // звук нажатия кнопки 2
  myDFPlayer.play(4);  // звук разблокировки unlock
  myDFPlayer.play(5);  // звук блокировки lock
  myDFPlayer.play(6);  // звук запуска выбранной программы
  myDFPlayer.play(7);  // звук завершения программы (обороты =0) dOnE
  myDFPlayer.play(8);  // звук наличия ошибки "звонок"
  myDFPlayer.play(9);  // звук резерв
  myDFPlayer.play(10);  // звук резерв
*/
