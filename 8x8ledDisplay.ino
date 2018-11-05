uint8_t rowPins[8] = {10, 7, 12, 6, 2, 13, A1,  5};
uint8_t colPins[8] = {A3, A2, 3, 11, 4, 9, 8, A0};


//https://rodrigosc.github.io/ArduinoLedMatrix/char_builder/builder.html
byte alphabets[][8] = {
  {0x0, 0x0, 0x6, 0x9, 0x9, 0xf, 0x9, 0x9},             //a
  {0x0, 0x0, 0xf, 0x5, 0x5, 0x6, 0x5, 0xf},             //b
  {0x0, 0x0, 0x7, 0x8, 0x8, 0x8, 0x8, 0x7},             //c
  {0x0, 0x0, 0x1e, 0x9, 0x9, 0x9, 0x9, 0x1e},           //d
  {0x0, 0x0, 0xf, 0x8, 0x8, 0xe, 0x8, 0xf},             //e
  {0x0, 0x0, 0xf, 0x8, 0x8, 0xe, 0x8, 0x8},             //f
  {0x0, 0x0, 0xf, 0x10, 0x10, 0x17, 0x15, 0x1d},        //g
  {0x0, 0x0, 0x9, 0x9, 0x9, 0xf, 0x9, 0x9},             //h
  {0x0, 0x0, 0x7, 0x2, 0x2, 0x2, 0x2, 0x7},             //i
  {0x0, 0x0, 0xf, 0x2, 0x2, 0x2, 0xa, 0x6},             //j
  {0x0, 0x0, 0x9, 0xa, 0xc, 0xc, 0xa, 0x9},             //k
  {0x0, 0x0, 0x8, 0x8, 0x8, 0x8, 0x8, 0xf},             //l
  {0x0, 0x0, 0x11, 0x1b, 0x15, 0x11, 0x11, 0x11},       //m
  {0x0, 0x0, 0x11, 0x19, 0x15, 0x15, 0x13, 0x11},       //n
  {0x0, 0x0, 0xe, 0x11, 0x11, 0x11, 0x11, 0xe},         //o
  {0x0, 0x0, 0x1e, 0x11, 0x11, 0x1e, 0x10, 0x10},       //p
  {0x0, 0x0, 0xe, 0x11, 0x11, 0x13, 0x15, 0xe},         //q
  {0x0, 0x0, 0x1e, 0x11, 0x13, 0x1c, 0x12, 0x11},       //r
  {0x0, 0x0, 0x1f, 0x10, 0x10, 0x1f, 0x1, 0x1f},        //s
  {0x0, 0x0, 0x1f, 0x4, 0x4, 0x4, 0x4, 0x4},            //t
  {0x0, 0x0, 0x9, 0x9, 0x9, 0x9, 0x9, 0xf},             //u
  {0x0, 0x0, 0x11, 0x11, 0x11, 0x11, 0xa, 0x4},         //v
  {0x0, 0x0, 0x11, 0x11, 0x11, 0x15, 0x1b, 0x11},       //w
  {0x0, 0x0, 0x11, 0xa, 0x4, 0x4, 0xa, 0x11},           //x
  {0x0, 0x0, 0x11, 0xa, 0x4, 0x4, 0x4, 0x4},            //y
  {0x0, 0x0, 0x1f, 0x1, 0x2, 0x4, 0x8, 0x1f},           //z
  {0x81, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x81}      // space
};

double speed;

void setup() {
  speed = 1.5;                      //lower is higher scrolling/strobbing/blinking rate
  initialize();
}


void loop() {
  
  blink("hello");
  delay(1000);
  typewriter("Namaste");
  delay(1000);
  scroll("hola");
  delay(10000);
}

void scroll(char* text) {
  int len = length(text);

  for (int index = 0; index < len; index++) {
    byte* screen = screenShot(findAlphabet(text[index]));
    for (int frame = 0, shiftFrameLimit = speed * 8; frame < speed * 60; frame++) {
      if (frame % shiftFrameLimit == 0)
        shiftFrameBy1(screen, 'l');

      multiplex(screen);
    }
  }
}

void typewriter(char* text) {
  int len = length(text);

  for (int index = 0; index < len; index++) {
    byte* screen = screenShot(findAlphabet(text[index]));

    char direction = 'l';
    for (int frame = 0, shiftFrameLimit = speed * 10; frame < speed * 60; frame++) {
      if (frame > speed * 40) {
        direction = 'r';
        shiftFrameLimit = speed+1;
      }

      if (frame % shiftFrameLimit == 0)
        shiftFrameBy1(screen, direction);

      multiplex(screen);
    }
    free(screen);
  }
}

void blink(char* text) {
  int len = length(text);

  for (int index = 0; index < len; index++) {
    for (int frame = 0; frame < speed * 60; frame++) {
      multiplex(findAlphabet(text[index]));
    }
    delay(speed * 100);
  }
}

// This method prints the grid into the LED Matrix
void multiplex(byte* grid) {
  for (int row = 0; row < 8; row++) {
    for (int col = 0; col < 8; col++)
      digitalWrite(colPins[col], HIGH);

    digitalWrite(rowPins[row], HIGH);

    for (int col = 0; col < 8; col++)
      digitalWrite(colPins[7-col], grid[row] & 1 << col ? LOW : HIGH);

    delay(1);

    digitalWrite(rowPins[row], LOW);
  }
}

// Just initializing the pins as output
void initialize() {
  for(int i=0; i<8; i++) {
      pinMode(colPins[i],OUTPUT);
      pinMode(rowPins[i],OUTPUT);
    }
}

int length(char* text) {
  int len = 0;
  while(text[len] != '\0')
    len++;
  return len;
}

byte* screenShot(byte* original) {
  int len = 8;
  byte* copy = (byte*) malloc(len);

  for (int i = 0; i < len; i++) 
    copy[i] = original[i];

  return copy;
}

void shiftFrameBy1(byte* screen, char direction) {
  for (int row = 0; row < 8; row++) { 
    if (direction == 'l')
      screen[row] <<= 1;
    else
      screen[row] >>= 1;
  }
}

byte* findAlphabet(char character) {
  if (character == ' ')
    return alphabets[26];
  else if (isUpper(character))
    return alphabets[character - 65];
  else
    return alphabets[character - 97];
}

bool isUpper(char character) {
  if (character >= 65 && character <= 90)
    return true;
  return false;
}
