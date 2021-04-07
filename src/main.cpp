#include <Arduino.h>
#include "lcd595.h"

LCD display2(10, 8, 9);
LCD display1(4, 2, 3);

void setup() {
  display1.init(16, 2);
  display2.init(16, 2);
  display1.print("  OLA DISPLAY1 ");
  display2.print("  OLA DISPLAY2");
}
void loop() {
  // put your main code here, to run repeatedly:
}