void brightLvl() {
  if (brght >= 1000 )  {
    display.setBrightness(0x0a); //яркость пониженная когда темно
  }
  if (brght < 1000 )  {
    display.setBrightness(0x0f);
  } ////яркость повышенная когда светло  }
}
