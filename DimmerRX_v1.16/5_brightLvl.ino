void brightLvl() {
  if (brght >= 800)  display.setBrightness(0x0a); //яркость пониженная когда темно
  if (brght < 800)  display.setBrightness(0x0f);  //яркость повышенная когда светло  }
}
