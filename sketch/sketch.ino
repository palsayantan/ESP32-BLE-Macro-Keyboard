#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <BleKeyboard.h>
#include <FastLED.h>

#include "definition.h"
#include "layout.h"

BleKeyboard bleKeyboard("BLEkeyboard", "Manufacturer", 100);
Adafruit_SSD1306 display(128, 32, &Wire, -1);
CRGB leds[NUM_LEDS];
Menu menu;
Layout layout;
Effect effect;
Settings param;

void setup() {
  Serial.begin(115200);

  FastLED.addLeds<WS2812, DATA_PIN, RGB>(leds, NUM_LEDS);
  FastLED.setBrightness(brightness);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ;
  }
  display.clearDisplay();
  display.display();
  display.setTextColor(WHITE, BLACK);
  display.cp437(true);

  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(35, 1);
  display.print("ESP32 BLE");
  display.setTextSize(2);
  display.setCursor(0, 12);
  display.print(" Keyboard ");
  display.display();

  pinMode(batPin, INPUT);
  pinMode(statusLed, OUTPUT);
  pinMode(CLK, INPUT_PULLUP);
  pinMode(DT, INPUT_PULLUP);
  pinMode(SW, INPUT_PULLUP);

  for (int i = 0; i < ROWS; i++) {
    pinMode(rowPins[i], INPUT_PULLDOWN);
  }
  for (int i = 0; i < COLS; i++) {
    pinMode(colPins[i], OUTPUT);
    digitalWrite(colPins[i], LOW);
  }
  bleKeyboard.begin();
}

void loop() {

  if (millis() - timer2 > 1000) {
    timer2 = millis();
    int adc = analogRead(batPin);
    vbat = adc * (3.3 / 4095.0) * 2;
    int percentage = map(vbat * 100, 290, 500, 0, 100);
    //bleKeyboard.setBatteryLevel(percentage);

    display.setTextSize(1);
    display.setCursor(110, 1);
    display.print(percentage);
    display.print("%");
    display.display();
  }

  if (digitalRead(SW) == LOW) {
    delay(250);
    display.clearDisplay();
    display.setTextSize(1);
    display.setCursor(40, 2);
    switch (menu) {
      case layer:
        menu = light;
        display.print("Effects");
        break;
      case light:
        menu = settings;
        display.print("Settings");
        break;
      case settings:
        menu = layer;
        display.print("Layout");
        break;
    }
    display.display();
  }

  bool clkValue = digitalRead(CLK);
  delay(10);
  if (clkValue == LOW) {
    int dtValue = digitalRead(DT);
    if (clkValue == LOW && dtValue == HIGH) {
      count += 1;
    }
    if (clkValue == LOW && dtValue == LOW) {
      count -= 1;
    }

    if (count < 0) count = 0;
    Serial.println(count);

    display.setTextSize(2);
    display.setCursor(0, 12);
    switch (menu) {
      case layer:
        switch (layout) {
          case layout1:
            layout = layout2;
            display.print("  Layout2  ");
            break;
          case layout2:
            layout = layout3;
            display.print("  Layout3  ");
            break;
          case layout3:
            layout = layout1;
            display.print("  Layout1  ");
            break;
          default:
            layout = layout1;
            display.print("  Layout1  ");
            break;
        }
        break;
      case light:
        switch (effect) {
          case effect1:
            effect = effect2;
            display.print(" Breathing ");
            break;
          case effect2:
            effect = effect3;
            display.print("  Fadeing  ");
            break;
          case effect3:
            effect = effect1;
            display.print("  Rainbow  ");
            break;
          default:
            effect = effect1;
            display.print("  Rainbow  ");
            break;
        }
        break;
      case settings:
        switch (param) {
          case color:
            param = opacity;
            display.print(" Brightness ");
            break;
          case opacity:
            param = speed;
            display.print("   Speed   ");
            break;
          case speed:
            param = color;
            display.print("   Color   ");
            break;
        }
        break;
    }
    display.display();
    delay(100);
  }

  if (millis() - timer > 10) {
    timer = millis();
    switch (effect) {
      // rainbow
      case effect1:
        fill_rainbow(leds, NUM_LEDS, hue, 30);
        FastLED.show();
        hue++;
        break;
      // fading
      case effect2:
        fill_solid(leds, NUM_LEDS, CHSV(hue, 255, brightness));
        FastLED.show();
        brightness = brightness + fadeAmount;
        if (brightness <= 5 || brightness >= 250) fadeAmount = -fadeAmount;
        break;
      // breathing
      case effect3:
        fill_solid(leds, NUM_LEDS, CHSV(hue, 255, 192));
        FastLED.show();
        hue++;
        break;
    }

    int key = getKey();
    if (key != -1) {
      int keyMap = customKey[layout][key];
      Serial.println((char)keyMap);

      if (bleKeyboard.isConnected()) {
        digitalWrite(statusLed, HIGH);
        bleKeyboard.write((char)keyMap);
        delay(100);
      } else {
        digitalWrite(statusLed, LOW);
      }
    }
  }
}

int getKey() {
  int k;
  int r, c;
  for (c = 0; c < COLS; c++) {
    digitalWrite(colPins[c], HIGH);
    delay(1);
    for (r = 0; r < ROWS; r++) {
      if (digitalRead(rowPins[r]) == HIGH) {
        digitalWrite(colPins[c], LOW);
        k = keys[r][c];
        return k;
      }
    }
    digitalWrite(colPins[c], LOW);
  }
  return -1;
}