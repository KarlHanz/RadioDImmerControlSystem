void setup() {

  pinMode(lightSens, INPUT); // режим пина как вход
  pinMode (ledStatus, OUTPUT); // режим пина как выход
  pinMode(Disp, OUTPUT); // режим пина как выход
  digitalWrite(Disp, HIGH); // ВКЛ дисплей при загрузке;

  attachInterrupt(0, isr, CHANGE);
  attachInterrupt(1, isr, CHANGE);

  nexInit();

  // Change Nextion BAUD rate
  nexSerial.print("baud=");
  nexSerial.print(115200);
  nexSerial.write(0xff);
  nexSerial.write(0xff);
  nexSerial.write(0xff);
  delay(10);
  nexSerial.flush();
  nexSerial.begin(115200);
  nexSerial.print("");
  nexSerial.write(0xFF);
  nexSerial.write(0xFF);
  nexSerial.write(0xFF);
  nexSerial.print("");
  nexSerial.write(0xFF);
  nexSerial.write(0xFF);
  nexSerial.write(0xFF);
  nexSerial.print("");
  nexSerial.write(0xFF);
  nexSerial.write(0xFF);
  nexSerial.write(0xFF);
  delay(10);
  nexSerial.flush();


  radioSetup();


  softSerial.begin(9600);
  myDFPlayer.begin(softSerial);
  dht.begin();

  myDFPlayer.volume(20);  //Set volume value. From 0 to 30
  //  myDFPlayer.play(1);  //Play the first mp3



  //===========настройка кнопок на дисплее============

  //gpio.pin_mode(5,1,11);
  //gpio.pin_mode(2,1,12);

  //===========настройка кнопок============
  butt1.setTimeout(1000);        // настройка таймаута на удержание (по умолчанию 500 мс)
  butt2.setTimeout(1000);        // настройка таймаута на удержание (по умолчанию 500 мс)

}
