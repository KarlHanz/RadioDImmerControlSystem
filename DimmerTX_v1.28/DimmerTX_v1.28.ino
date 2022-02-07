
/*
// https://forum.cxem.net/index.php?/topic/151543-обзор-touch-дисплея-nextion-часть-1-2/
// http://qaru.site/questions/14059099/data-sending-with-raspberry-pi-where-nextion-24-display
// https://nextion.tech/editor_guide/
// http://arduino.ru/forum/apparatnye-voprosy/nextion-displei?page=4
// https://github.com/itead/ITEADLIB_Arduino_Nextion/blob/master/examples/CompRtc/CompRtc.ino
// https://itead.cc/nextion/nextion-tutorial-based-on-nextion-arduino-library/
// https://nextion.tech/2021/01/11/the-sunday-blog-the-enhanced-k-hmis-part1-rtc-and-gpio/
// https://habr.com/ru/company/flprog/blog/400463/
// https://www.drive2.ru/l/559405129007104312/
// http://arduino.ru/forum/programmirovanie/peredacha-dannykh-na-nextion
https://habr.com/ru/company/flprog/blog/400463/ про виртуaльные кнопки. ИЗУЧИТЬ СТАТЬЮ!!!

https://nextion.tech/2021/01/11/the-sunday-blog-the-enhanced-k-hmis-part1-rtc-and-gpio/ - подключение кнопок и настройка RTC
https://nextion.tech/2020/08/24/the-sunday-blog-understanding-and-customizing-hmi-components-part-2-a-project-without-visible-components/
http://arduino.ru/forum/programmirovanie/nextion-arduino-ne-chitaet-komandy-s-displeya
http://78.46.72.113/forum/apparatnye-voprosy/nextion-displei?page=5 отправка/прием без библиотеки
https://forum.arduino.cc/t/how-to-send-a-rtc-updating-value-from-my-nextion-to-an-arduino-without-lib/695983  !!!отправка данных на ардуино
https://quabr.com/51121356/nextion-not-sending-data-to-arduino отправка через подпрограмму
http://forum.amperka.ru/threads/%D0%94%D0%B8%D1%81%D0%BF%D0%BB%D0%B5%D0%B9-nextion-%D0%B0%D0%B7%D1%8B-arduino-esp8266.9204/ команда prints
https://mschoeffler.com/2018/01/05/arduino-tutorial-how-to-use-the-rdm630-rdm6300-rfid-reader/ HEX to DEC
https://github.com/benrugg/Arduino-Hex-Decimal-Conversion/blob/master/hex_dec.ino
http://arduino.ru/forum/apparatnye-voprosy/nextion-displei?page=4 - парсинг
https://github.com/Seithan/EasyNextionLibrary - ограниченная библиотека для nextion
https://seithan.com/Easy-Nextion-Library/Use-Nextion-General-View/ - описание дисплея
https://www.youtube.com/watch?v=Fr34EQnlJwY - код для запроса данных с дисплея (для часов)
https://nextion.tech/2021/01/11/the-sunday-blog-the-enhanced-k-hmis-part1-rtc-and-gpio/ - rtc
https://forum.homedistiller.ru/index.php?topic=288890.0 - упоминание про настройку bauds в самом дисплее
https://forum.arduino.cc/t/nextion-v0-9-0-library-with-working-gui-events/596412 - правильные файлы в библиотеку!
https://githubmemory.com/repo/jyberg/Enhanced-Nextion-Library/issues/12
https://github.com/johnseghersmsft/ITEADLIB_Arduino_Nextion
https://github.com/jyberg/Enhanced-Nextion-Library/issues/12 - баг в библиотеке rtc
https://github.com/jyberg/Enhanced-Nextion-Library - улучшенная библиотека





*/

#define BTN1 2 // кнопка PROGRAM MODE 
#define BTN2 3 // кнопка CONFIRM
#define DHTPIN 4 // датчик влажности
#define DHTTYPE DHT11 //тип датчика влажности
#define lightSens A0 // фотодатчик
#define ledStatus 9 // индикатор состояния (горит постоянно, когда ONLINE, мигает, когда ошибки)
#define Disp A5 // питание на дисплей

#define RX 6 // никуда не подключается
#define TX 5 // подключается ко входу RX звукового модуля
