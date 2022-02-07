#include <Nextion.h>
#include "nRF24L01.h"
#include "RF24.h"
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <SoftwareSerial.h>
#include <DFRobotDFPlayerMini.h>
#include <SPI.h>
#include "GyverTimer.h"   // библиотека таймера
#include <GyverButton.h>


GButton butt1(BTN1); //BTN1 2 // кнопка PROGRAM MODE
GButton butt2(BTN2); //BTN2  3 // кнопка CONFIRM

SoftwareSerial softSerial(RX, TX);
DFRobotDFPlayerMini myDFPlayer;


RF24 radio(7, 8); // "создать" модуль на пинах 7 и 8 Для Уно (pin3=CE->D7, pin4=CSN->D8)

DHT dht(DHTPIN, DHTTYPE);

const uint64_t pipe = {0xF0F0F0F0E1LL};

int transmit_data[1];   // массив пересылаемых данных (одно значение, dimmer 0...255)
int telemetry_data[5]; // массив принятых данных (пять значений)

uint32_t currentMillis = 0;
uint32_t previousMillis = 0;        // прошлое значение времени милисекунд
uint32_t tmrColor,tmrMenu, tmrDim = 0;


// ===============ДИСПЛЕЙ===============
NexRtc  rtc;
NexGpio gpio;

NexProgressBar pwrDev  = NexProgressBar(1, 2, "pwrDev"); // Progress bar
NexPicture tach = NexPicture(1, 7, "tach"); // Сектор (картринка)
NexPicture relay = NexPicture(1, 6, "relay"); // Реле статус (картинка)
NexNumber tmp = NexNumber(1, 8, "tmp"); // Температура (число)
NexNumber temp = NexNumber(1, 5, "temp"); // Температура (число)
NexNumber hmd = NexNumber(1, 4, "hmd"); // Влажность (число)
NexText sts = NexText(1, 3, "sts"); // Статус (текст)
NexText mode = NexText(1, 3, "mode"); // Режим (текст)


// ===============МЕНЮ===============
int modes = 0; // программа: 0,1,2,3,4,5,6
bool progStart = 0; //флаг на запуск программы
bool progEnd = 0; //флаг окончания программы



// ===============RTC===============
uint32_t year, mon, day, hour, min, sec, week = 0;
uint32_t time[] = {0}; // массив RTC
uint32_t timeHrs = 0; //значение "час" здесь
uint32_t timeMins = 0; //значение "минуты" здесь
uint32_t timeSec = 0; //значение "секунды" здесь
uint32_t timeCountS = 0; //для отчёта времени
uint32_t timeCountM = 0; //для отчёта времени
uint32_t timeCountH = 0; //для отчёта времени


// ===============МАССИВ ПРИЕМА ТЕЛЕМЕТРИИ===============
uint32_t powerDev; //мощность потока на дисплей, значение мощности (0...100)
uint32_t RPM;      //принимаем точные обороты (0...2700)
uint32_t tempExt;     // принимаем внешнюю температуру (0...100)
bool statusRelay;     // принимаем состояние реле (0,1)
uint32_t statusCode;  //принимаем цифровой код статуса (10,20,90)
uint32_t tacho; //обороты для отображения на дисплее //значение 0...113(картинок)
int dimmer = 255; //передаваемое на исполнительный блок начальное значение диммера (0...255) 

uint32_t bright = 100; // величина яркости на дисплей (0...100)
uint32_t hmdInt, tempInt = 0; // датчик DHT11 для отправки на дисплей

// ===============КОДЫ СТАТУСА===============
// statusCode = 0; // NULL
// statusCode = 10; // STARTING
// statusCode = 20; // ONLINE
// statusCode = 90; // DONE
// statusCode = 110; // NO TACHO
// statusCode = 120; // NO DIMM
// statusCode = 130; // NO RELAY
// statusCode = 140; // OVERHEAT
// statusCode = 150; // OVERHEAT&TROUBLE

bool ledState = 0;
bool lastlightStatus, lightStatus = 0; //датчик света

bool radioCommState; // текущее состояние канала связи

bool timerFlag = 0;
uint32_t timeOut = 0; // для таймера
bool flagStart = 1; // для запуска мотора на высокие обороты

GTimer myTimer0(MS); // таймер на яркость дисплея после откл света
GTimer myTimer1(MS); // таймер на выполнение программы
GTimer myTimer2(MS); // таймер для программ
GTimer myTimer3(MS); // таймер для программ
GTimer myTimer4(MS); // таймер для выключения диммера
