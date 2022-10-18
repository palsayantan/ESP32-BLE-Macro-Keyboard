
const byte ROWS = 2;
const byte COLS = 5;

char keys[ROWS][COLS] = {
  { 0 , 1 , 2 , 3 , 4 },
  { 5 , 6 , 7 , 8 , 9 }
};

byte rowPins[ROWS] = {R1, R2};
byte colPins[COLS] = {C1, C2, C3, C4, C5};

const byte numLayout = 3;
int customKey[numLayout][ROWS*COLS] = {
  //layout 1
  {'a','b','c','d','e',
   'f','g','h','i','j'},
  //layout 2
  {'1','2','3','4','5',
   '6','7','8','9','0'},
  //layout 3
  {'S','u','b','s','c',
   '!','e','b','i','r'},
};

enum Menu {
  layer,
  light,
  settings
};

enum Layout {
  layout1,
  layout2,
  layout3
};

enum Effect {
  effect1,
  effect2,
  effect3
};

enum Settings {
  color,
  opacity,
  speed
};