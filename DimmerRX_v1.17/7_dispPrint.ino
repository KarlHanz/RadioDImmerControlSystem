// ================ ВЫВОД НА ДИСПЛЕЙ ================

void displayPrint() {

  if (RPM > 100 && statusCode < 90) {
    display.showNumberDec(RPM); // выводим на дисплей значение оборотов
  }
  if (statusCode == 90) display.setSegments(SEG_dOnE);
  if (statusCode == 110) display.setSegments(SEG_taho);
  if (statusCode == 120) display.setSegments(SEG_rEG);
  if (statusCode == 130) display.setSegments(SEG_rELE);
  if (statusCode == 140) display.setSegments(SEG_hot);
  if (statusCode == 150) display.setSegments(SEG_nhot);
}
