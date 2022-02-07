
// РЕГУЛЯТОР НА НОВОЙ БИБЛИОТЕКЕ GYVER РАБОТАЕТ! 

/*  https://arduinomaster.ru/datchiki-arduino/arduino-ds18b20/
    https://lesson.iarduino.ru/page/termometr-na-baze-ds18b20-i-oled-displeya-128x64/
    http://arduino.ru/forum/programmirovanie/zamiranie-vsego-koda-iz-za-ds18b20
    http://arduino.ru/forum/programmirovanie/pochistil-sketch-primera-raboty-s-ds18b20-iz-bibly-oneware?page=2
    http://78.46.72.113/forum/programmirovanie/pochistil-sketch-primera-raboty-s-ds18b20-iz-bibly-oneware?page=3
    http://arduino.ru/forum/programmirovanie/pomogite-s-obratnoi-svyazyu-nrf24l01
   http://arduino.ru/forum/programmirovanie/peredacha-trekh-peremennykh-po-nrf24l01
   https://wiki.iarduino.ru/page/NRF24L01-trema/
   http://arduino.ru/forum/programmirovanie/pomogite-s-obratnoi-svyazyu-nrf24l01
  фотодатчик: свет = 0, темнота = 1.
    ВНИМАНИЕ!!! enableAckPayload НЕ РАБОТАЕТ НА СКОРОСТИ 250 kbps!
*/
// ? Взаимосвязь между диммированием dimmer, оборотами RPM, мощностью потока powerDev


#define dimPin  A0 // выход на диммер
#define zeroPin  2 // детектор нуля
#define K1 6 // выход на реле
#define Brg A6 // вход с фотодатчика
#define DIO A4 //дисплей TM1637
#define CLK A5 //дисплей TM1637
