// Graphical pattern functions for glasses

// Global variables
boolean effectInit = false;       // flag for whether an effect has been run previously
unsigned int effectDelay = 0;

// Draw a sine wave on the bit array
// Speeds up and slows down in a cycle
void sines() {

  if (!effectInit) {
    switchDrawType(0, 0);
    effectInit = true;
    effectDelay = 5;
  }

  static float incrval = 0;
  static float incr = 0.3;
  static byte incdir = 1;

  for (int i = 0; i < 24; i++) {
    GlassesBits[i][0] = 3 << (int)(sin(i / 2.0 + incrval) * 3.5 + 3.5);
  }
  writeBitFrame(0, 0);

  incrval += incr;
  if (incdir == 1 ) {
    incr += 0.001;
  }
  else {
    incr -= 0.001;
  }

  if (incr > 0.5) incdir = 0;
  if (incr < 0.1) incdir = 1;

}


// Draw a circular sine plasma

void Plasma() {

  if (!effectInit) {
    switchDrawType(0, 1);
    effectInit = true;
    effectDelay = 1;
  }
  static int plasoffset = 0;
  static float offset  = 0;
  static float plasIncr = -1;
  static float plasVector = 0;

  for (int x = 0; x < 24; x++) {
    for (int y = 0; y < 8; y++) {
      byte brightness = qsine(sqrt((x - 11.5) * (x - 11.5) + (y - 3.5) * (y - 3.5)) * 60 + plasoffset);

      GlassesPWM[x][y] = pgm_read_byte(&cie[brightness]);
    }
  }

  writePWMFrame(0);
  plasoffset += 15;
  if (plasoffset > 359) plasoffset -= 359;

}

#define fadeIncrement 0.9
void fadeAllPWM() {
  for (int x = 0; x < 24; x++) {
    for (int y = 0; y < 8; y++) {
      GlassesPWM[x][y] *= fadeIncrement;
    }
  }
}


// Message scrolling functions
// Initialize / load message string
byte currentCharColumn = 0;
int currentMessageChar = 0;
void initMessage(byte message) {
  currentCharColumn = 0;
  currentMessageChar = 0;
  selectFlashString(message);
  loadCharBuffer(loadStringChar(message, currentMessageChar));
}

// Draw message scrolling across the two arrays
// SCROLL1X is normal scrolling
// SCROLL2X is page-flipping scroll that simulates double horizontal resolution using persistence of vision
#define SCROLL1X 0
#define SCROLL2X 1
void scrollMessage(byte message, byte mode) {

  if (!effectInit) {
    switchDrawType(0, 0);
    initMessage(message);
    effectInit = true;
    if (mode != SCROLL2X) {
      effectDelay = 30;
    } else {
      effectDelay = 10;
    }
  }

  if ((currentCharColumn % 2 == 0) || mode != SCROLL2X) {
    scrollBits(1, 0);
  } else {
    scrollBits(1, 1);
  }

  if ((currentCharColumn % 2 == 1) || mode != SCROLL2X) {
    GlassesBits[23][0] = charBuffer[currentCharColumn];
    writeBitFrame(0, 0);
  } else {
    GlassesBits[23][1] = charBuffer[currentCharColumn];
    writeBitFrame(0, 1);
  }

  currentCharColumn++;
  if (currentCharColumn > 7) {
    currentCharColumn = 0;
    currentMessageChar++;
    char nextChar = loadStringChar(message, currentMessageChar);
    if (nextChar == 0) {
      currentMessageChar = 0;
      nextChar = loadStringChar(message, currentMessageChar);
    }
    loadCharBuffer(nextChar);
  }

}

void sideRain(byte dir) {

  if (!effectInit) {
    switchDrawType(0, 0);
    effectInit = true;
    effectDelay = 20;
  }

  byte tempRain = 0;

  tempRain = (1 << random(0, 8)) | (1 << random(0, 8));

  if (dir == 0) {
    scrollBits(0, 0);
    GlassesBits[0][0] = tempRain;
  } else {
    scrollBits(1, 0);
    GlassesBits[23][0] = tempRain;
  }

  writeBitFrame(0, 0);

}

void rain() {

  if (!effectInit) {
    switchDrawType(0, 0);
    effectInit = true;
    effectDelay = 20;
  }

  for (int i = 0; i < 24; i++) {
    GlassesBits[i][0] <<= 1;
  }

  GlassesBits[random(0, 24)][0] |= 1;
  GlassesBits[random(0, 24)][0] |= 1;

  writeBitFrame(0, 0);

}

typedef struct Stars {
  float xIncr;
  float yIncr;
  float xPos;
  float yPos;
};

Stars stars[10];

void starField() {

  if (!effectInit) {
    switchDrawType(0, 1);
    effectInit = true;
    effectDelay = 1;
  }


  fadeAllPWM();
  for (int i = 0; i < 10; i++) {
    if (abs(stars[i].xIncr) < 0.02 || abs(stars[i].yIncr) < 0.02) {
      stars[i].xPos = 11.5;
      stars[i].yPos = 3.5;
      stars[i].xIncr = random(0, 200) / 100.0 - 1.0;
      stars[i].yIncr = random(0, 200) / 200.0 - 0.5;
    }

    stars[i].xPos += stars[i].xIncr;
    stars[i].yPos += stars[i].yIncr;

    int xPos = (int)stars[i].xPos;
    int yPos = (int)stars[i].yPos;
    if (xPos < 0 || xPos > 23 || yPos < 0 || yPos > 7) {
      stars[i].xIncr = 0;
      stars[i].yIncr = 0;
    } else {
      GlassesPWM[xPos][yPos] = 255;
    }
  }
  writePWMFrame(0);

}



void fullOn() {

  if (!effectInit) {
    switchDrawType(0, 1);
    effectInit = true;
    effectDelay = 50;
  }

  static byte blinkAction = 0;

  if (blinkAction == 0) {
    blinkAction = 255;
  } else {
    blinkAction = 0;
  }

  for (int x = 0; x < 24; x++) {
    for (int y = 0; y < 8; y++) {
      GlassesPWM[x][y] = blinkAction;
    }
  }

  writePWMFrame(0);

}


void slantBars() {

  if (!effectInit) {
    switchDrawType(0, 1);
    effectInit = true;
    effectDelay = 8;
  }

  static int slantPos = 23;

  for (int x = 0; x < 24; x++) {
    for (int y = 0; y < 8; y++) {
      GlassesPWM[x][y] = pgm_read_byte(&cie[(((x + y + (int)slantPos) % 8) * 32)]);
    }
  }

  slantPos--;
  if (slantPos < 0) slantPos = 23;

  writePWMFrame(0);

}

void sparkles() {

  if (!effectInit) {
    switchDrawType(0, 1);
    effectInit = true;
    effectDelay = 1;
  }

  for (int i = 0; i < 5; i++) GlassesPWM[random(0, 24)][random(0, 8)] = 255;
  writePWMFrame(0);
}

void rider() {

  if (!effectInit) {
    switchDrawType(0, 1);
    effectInit = true;
    effectDelay = 30;
  }

  static int riderPos = 0;
  static int riderDir = 1;


  for (int x = riderPos * 3; x < (riderPos * 3 + 3); x++) {
    if (x >= 0 && x < 24) {
      for (int y = 0; y < 8; y++) {
        GlassesPWM[x][y] = 255;
      }
    }
  }

  riderPos += riderDir;
  if (riderPos > 10 || riderPos < -3 ) riderDir *= -1;

  writePWMFrame(0);

}


// Simply grab a character from the font and put it in the 8x8 section of both sides of the glasses
void displayChar(int character) {

  if (!effectInit) {
    switchDrawType(0, 0);
    effectInit = true;
    effectDelay = 5;
  }

  loadCharBuffer(character);

  for (int i = 0; i < 8; i++) {
    GlassesBits[i + 1][0] = charBuffer[i];
    GlassesBits[i + 15][0] = charBuffer[i];
  }

  writeBitFrame(0, 0);

}




void fire() {

  if (!effectInit) {
    switchDrawType(0, 1);
    effectInit = true;
    effectDelay = 2;
  }

  static byte lineBuffer[24] = {0};

  byte x;
  for (x = 0; x < 24; x++) {
    GlassesPWM[x][8] = (random(0, 4) == 1) * 255;
  }


  for (int y = 1; y < 9 ; y++) {
    for (x = 0; x < 24; x++) lineBuffer[x] = GlassesPWM[x][y];
    for (x = 0; x < 24; x++) {
      int tempBright = (lineBuffer[(x - 1) % 24] + lineBuffer[(x + 1) % 24] + lineBuffer[x] + GlassesPWM[x][(y + 1) % 9]) / 3.7 - 10;
      if (tempBright < 0) tempBright = 0;
      if (tempBright > 255) tempBright = 255;
      GlassesPWM[x][y - 1] = tempBright;
    }
  }

  writePWMFrame(0);

}

// Awww!
void beatingHearts() {

  if (!effectInit) {
    switchDrawType(0, 0);
    effectInit = true;
    effectDelay = 60;
  }

  static byte currentHeartFrame = 0;


  if (currentHeartFrame < 3) {
    loadGraphicsFrame(currentHeartFrame);
  } else {
    loadGraphicsFrame(5 - currentHeartFrame);
  }

  currentHeartFrame++;
  if (currentHeartFrame > 5) currentHeartFrame = 0;

  writeBitFrame(0, 0);

}

byte eqLevels[12] = {0};
int eqDecay = 0;
int eqRandomizerDelay = 0;
int eqRandomizerCap = 0;
#define eqDecaySpeed 40
#define eqMinInterval 50
#define eqMaxInterval 200

void fakeEQ() {

  // start new pattern
  if (!effectInit) {
    switchDrawType(0, 0);
    effectInit = true;
    eqRandomizerCap = random(0, eqMaxInterval - eqMinInterval) + eqMinInterval;
    effectDelay = 1;
  }

  // decay the eq array at a set interval
  eqDecay++;
  if (eqDecay > eqDecaySpeed) {
    eqDecay = 0;
    for (byte i = 0; i < 12; i++) {
      if (eqLevels[i] > 0) eqLevels[i]--;
    }
  }

  // splash random bars at a semi-random interval
  eqRandomizerDelay++;
  if (eqRandomizerDelay >= eqRandomizerCap) {
    eqRandomizerDelay = 0;
    eqRandomizerCap = random(0, eqMaxInterval - eqMinInterval) + eqMinInterval;
    for (byte i = 0; i < 12; i++) {
      byte eqNewLevel = random(0, 9);
      if (eqLevels[i] < eqNewLevel) eqLevels[i] = eqNewLevel;
    }
  }

  // render the bars if something visible has happened
  if (eqDecay == 0 || eqRandomizerDelay == 0) {
    for (byte i = 0; i < 12; i++) {
      GlassesBits[i * 2][0] = 0xFF << (8 - eqLevels[i]);
      GlassesBits[i * 2 + 1][0] = 0xFF << (8 - eqLevels[i]);
    }
    writeBitFrame(0, 0);
  }
}

void scrollingHearts() {
  if (!effectInit) {
    switchDrawType(0, 0);
    effectInit = true;
    effectDelay = 25;
  }

  static float heartPos1;
  static float heartPos2;
  static int heartAngle;

  loadGraphicsFrame(2);
  for (byte x = 0; x < 12; x++) {
    if (heartPos1 < 8) GlassesBits[x][0] = GlassesBits[x][0] << (7-(byte)heartPos1);
    if (heartPos1 > 7) GlassesBits[x][0] = GlassesBits[x][0] >> ((byte)heartPos1-7);
  }

  for (byte x = 12; x < 24; x++) {
    if (heartPos2 < 8) GlassesBits[x][0] = GlassesBits[x][0] << (7-(byte)heartPos2);
    if (heartPos2 > 7) GlassesBits[x][0] = GlassesBits[x][0] >> ((byte)heartPos2-7);
  }

  heartPos1 += 0.6 + qsine(heartAngle*1.4)/255.0;
  if (heartPos1 > 15) heartPos1 = 0;

  heartPos2 += 0.5 + qsine(heartAngle*1.7)/255.0;
  if (heartPos2 > 15) heartPos2 = 0;

  heartAngle += 1;
  if (heartAngle > 359) heartAngle = 0;

  writeBitFrame(0, 0);
}

void messageOne() {
  scrollMessage(0, SCROLL1X); 
}

void messageTwo() {
  scrollMessage(1, SCROLL1X);
}

void messageThree() {
  scrollMessage(2, SCROLL1X);
}

