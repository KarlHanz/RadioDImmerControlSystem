#include <GyverTimers.h>  // библиотека таймера
#include <GyverDimmer.h>
#include <OneWire.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <TM1637Display.h>


Dimmer<dimPin> dim;   // указать пин диммера

RF24 radio(9, 10); // "создать" модуль на пинах 9 и 10 (pin3=CE->D9, pin4=CSN->D10)
OneWire ds(7); // "создать" модуль на пине 7
TM1637Display display(CLK, DIO); //инициализация дисплея

const uint64_t pipe = {0xF0F0F0F0E1LL};

uint32_t tmrStart,timeOut = millis(); // для таймеров


volatile int temp; //температура
volatile int tic;
uint8_t dimmer = 255; // диммирование прямое (0...255) // при старте на максимум (0 - это выкл, 255 - полная мощность)

uint8_t  dimmerInt; // димирование внутреннее при отсутствии радиосвязи
uint8_t  dimmerExt; // димирование внешнее с радиомодуля. начальное значение диммера для запуска без радиоканала на 100%
uint8_t  powerDev; // мощность потока для передачи на блок управления для отображения на дисплее

bool relay = 0; // начальное значение управление реле - реле ВЫКЛЮЧЕНО, контакты замкнуты, запуск напрямую = LOAD
bool tachoFail = 0; // ошибка - не подключен тахометр
bool dimmFail = 0; // ошибка - не корректно работает диммер
bool tempFail = 0; // ошибка - перегрев
bool relayFail = 0; // ошибка - реле не отключает (пригорели контакты) (либо замкнуло симистор)
bool checkErrStart = 1; //флаг проверки на ошибки
bool lastCheckErrStart = 1; //флаг проверки на ошибки, предыдущее состояние
bool radioCommState = 0; // текущее состояние канала связи
bool lastRadioCommState = 0; // предыдущее состояние канала связи

int brght; //яркость

uint8_t statusCode = 0;  // коды статуса и ошибок


byte n = 7;      // число магнитов
float rev_time;
volatile uint32_t lastflash, flash, lastshow;
uint16_t RPM_in = 0;
uint16_t RPM_prev = 0;
uint16_t RPM = 0;

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


int recieved_data[1]; // массив принятых данных (одно значение, dimmer 0...255)
int telemetry_data[5]; // массив передаваемых данных (пять значений)

// ===============МАССИВ ПЕРЕДАЧИ ТЕЛЕМЕТРИИ===============
//  telemetry_data[0] = RPM;      //передаём точные обороты (0...2700)
//  telemetry_data[1] = powerDev; //возвращаем на дисплей значение мощности (0...100)
//  telemetry_data[2] = relay;     //передаём состояние реле (0,1)
//  telemetry_data[3] = temp;     //передаём температуру (0...100)
//  telemetry_data[4] = statusCode;  //передаём цифровой код статуса



const uint8_t SEG_oooo[] = {
  SEG_C | SEG_D | SEG_E | SEG_G,                  // o
  SEG_C | SEG_D | SEG_E | SEG_G,                  // o
  SEG_C | SEG_D | SEG_E | SEG_G,                  // o
  SEG_C | SEG_D | SEG_E | SEG_G                   // o
};

const uint8_t SEG_taho[] = {
  SEG_D | SEG_E | SEG_F | SEG_G,                   // t
  SEG_A | SEG_B | SEG_C | SEG_E | SEG_F | SEG_G,   // A
  SEG_C | SEG_E | SEG_F | SEG_G,                   // h
  SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F     // O
};

const uint8_t SEG_rELE[] = {
  SEG_E | SEG_G,                                   // r
  SEG_A | SEG_D | SEG_E | SEG_F | SEG_G,           // E
  SEG_D | SEG_E | SEG_F,                           // L
  SEG_A | SEG_D | SEG_E | SEG_F | SEG_G            // E
};

const uint8_t SEG_rEG[] = {
  SEG_D,                                           // _
  SEG_E | SEG_G,                                   // r
  SEG_A | SEG_D | SEG_E | SEG_F | SEG_G,           // E
  SEG_A | SEG_C | SEG_D | SEG_E | SEG_F            // G
};

const uint8_t SEG_hot[] = {
  SEG_D,                                           // _
  SEG_C | SEG_E | SEG_F | SEG_G,                   // h
  SEG_C | SEG_D | SEG_E | SEG_G,                   // o
  SEG_D | SEG_E | SEG_F | SEG_G                    // t
};


const uint8_t SEG_nhot[] = {
  SEG_C | SEG_E | SEG_D,                           // n
  SEG_C | SEG_E | SEG_F | SEG_G,                   // h
  SEG_C | SEG_D | SEG_E | SEG_G,                   // o
  SEG_D | SEG_E | SEG_F | SEG_G                    // t
};

const uint8_t SEG_dOnE[] = {
  SEG_B | SEG_C | SEG_D | SEG_E | SEG_G,           // d
  SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F,   // O
  SEG_C | SEG_E | SEG_G,                           // n
  SEG_A | SEG_D | SEG_E | SEG_F | SEG_G            // E
};
