#include<M5Stack.h>
#include <Wire.h>
#include <CCS811.h>

CCS811 sensor;
uint8_t brightness = 60;
uint8_t rotation = 1;

void setup(void)
{
  M5.begin();
  M5.Power.begin();
  initLcd();
  // sensor
  while (sensor.begin() != 0) {
    delay(1000);
  }
  sensor.setMeasCycle(sensor.eCycle_250ms);
  resetLcd();
}

void loop() {
  if (sensor.checkDataReady() == true) {
    checkButtons();
    updateLcd(sensor.getCO2PPM());
  }
  sensor.writeBaseLine(0x847B);
  delay(500);
}

void initLcd() {
  M5.Lcd.setTextSize(3);
  M5.Lcd.print("Initializing...");
}

void resetLcd() {
  M5.Lcd.clear();
  M5.Lcd.setBrightness(brightness);
  M5.Lcd.setRotation(rotation);
  // title
  M5.Lcd.setTextSize(3);
  M5.Lcd.setCursor(0, 0);
  M5.Lcd.print("CO2 PPM");
}

void updateLcd(uint16_t co2) {
  uint16_t color, posx;
  // text position
  if (co2 < 1000) {
    posx = 60;
  } else if (co2 < 10000) {
    posx = 40;
  } else {
    posx = 20;
  }
  // backgroud color
  if (co2 <= 1000) {
    color = TFT_GREEN;
  } else if (co2 <= 1500) {
    color = TFT_BLUE;
  } else if (co2 <= 2000) {
    color = TFT_YELLOW;
  } else {
    color = TFT_RED;
  }
  // text
  M5.Lcd.setTextSize(7);
  M5.Lcd.setCursor(posx, 95);
  M5.Lcd.printf(" %d ", co2);
  // backgroud
  M5.Lcd.fillRect(0, 30, 320, 50, color);
  M5.Lcd.fillRect(0, 160, 320, 50, color);
  M5.Lcd.fillRect(0, 31, 10, 130, color);
  M5.Lcd.fillRect(310, 31, 10, 130, color);
  // battery
  M5.Lcd.setTextSize(2);
  M5.Lcd.setCursor(224, 220);
  M5.Lcd.printf("BATT %3d", M5.Power.getBatteryLevel());
}

void checkButtons() {
  // rotation
  if (M5.BtnB.wasPressed()) {
    if (rotation == 1) {
      rotation = 3;
    } else {
      rotation = 1;
    }
    resetLcd();
  }
  // brightness
  if (M5.BtnA.wasPressed() || M5.BtnC.wasPressed()) {
    brightness += 50;
    if (brightness > 110) {
        brightness = 10;
    }
    resetLcd();
  }
  M5.update();
}
