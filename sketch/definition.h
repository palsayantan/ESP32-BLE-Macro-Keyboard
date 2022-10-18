
// 5x2 key martix 
//IP
#define R1 17
#define R2 16
//OP
#define C1 14
#define C2 27
#define C3 26
#define C4 25
#define C5 33

// rotary encoder
#define CLK     18
#define DT      19
#define SW      5

// neopixel backlight
#define NUM_LEDS 10 
#define DATA_PIN 32

// battery voltage
#define batPin 35

// status indicator LED [connected/disconnected]
#define statusLed 2

float vbat = 3.7;
byte count = 0;
uint8_t hue = 0;
int brightness = 240;
int fadeAmount = 1;
unsigned long timer,timer2;
