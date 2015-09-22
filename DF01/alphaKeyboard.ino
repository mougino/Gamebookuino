// PROGMEM const char keyboard[]="qwertyuiopasdfghjkl<zxcvbnm_\030"; // Qwerty keyboard
// PROGMEM const char keyboard[]="azertyuiopqsdfghjklmwxcvbn_\030<"; // Azerty keyboard

void printAlpha(boolean caps, byte i) {
  char c=(char)pgm_read_byte(&(keyboard[i])); // read char array in PROGMEM
  if(caps && c>='a' && c<='z') c=c&~(0x20); // if(caps) convert to upper case
  gb.display.print(c); gb.display.print(F(" "));
}

void alphaKeyboard(char* tgt, byte lmax) {
  byte x=0, y=0;
  boolean caps=false;
  while(1) {
    if (gb.update()) {
      // draw input bar
      gb.display.cursorY++; if(strlen(tgt)) gb.display.print(tgt);
      #ifdef FONT5X7_C
      gb.display.setFont(font5x7);
      #endif
      gb.display.cursorY--; if((gb.frameCount/4)%2) gb.display.print(F("_"));
      gb.display.drawFastHLine(0,gb.display.fontHeight+1,LCDWIDTH);
      // draw keyboard
      #ifdef FONT5X7_C
      gb.display.setFont(font3x5);
      #endif
      moveCursor(3, gb.display.fontHeight+5);
      for(byte i=0; i<10; i++) printAlpha(caps, i);
      moveCursor(3, CURRENT+gb.display.fontHeight+2);
      for(byte i=10; i<20; i++) printAlpha(caps, i);
      moveCursor(3, CURRENT+gb.display.fontHeight+2);
      for(byte i=20; i<29; i++) printAlpha(caps, i);
      gb.display.println(F("ok"));
      moveCursor(3, CURRENT+2);
      for(char c='0'; c<='9'; c++) { gb.display.print(c); gb.display.print(F(" ")); }
      // draw select box
      gb.display.drawRoundRect(2*x*gb.display.fontWidth+1, (y+1)*(gb.display.fontHeight+2)+(y?1:2), ((x==9 && y==2)?2:1)*gb.display.fontWidth+3, gb.display.fontHeight+3, 3);
      // draw legend
      moveCursor(LCDWIDTH-strlen(CANCEL)*gb.display.fontWidth-6, LCDHEIGHT-gb.display.fontHeight+1);
      gb.display.print(F(CANCEL));
      #ifdef FONT5X7_C
      gb.display.setFont(font5x7);
      gb.display.cursorY-=2;
      #endif
      gb.display.print(F("\026"));
      #ifdef FONT5X7_C
      gb.display.setFont(font3x5);
      #endif
      // handle user input
      if(gb.buttons.pressed(BTN_RIGHT)) x=(x+1)%10;
      if(gb.buttons.pressed(BTN_LEFT)) { x--; if(x==255) x=9; }
      if(gb.buttons.pressed(BTN_DOWN)) y=(y+1)%4;
      if(gb.buttons.pressed(BTN_UP)) { y--; if(y==255) y=3; }
      if(gb.buttons.pressed(BTN_B)) { tgt[0]=NULL; break; } // Cancelled by user
      if(gb.buttons.pressed(BTN_A)) {
        byte l=strlen(tgt);
        if(x==9 && y==2) { // Ok
          break;
        } else if(y==3) { // 0-9
          if(l<lmax) { tgt[l++]=(char)'0'+x; tgt[l]=NULL; }
        } else { // Other key
          char c=(char)pgm_read_byte(&(keyboard[10*y+x])); // read char array in PROGMEM
          if(c=='<') { // Del
            if(l) tgt[l-1]=NULL;
          } else if(c=='\030' || c=='\031') { // Shift
            caps=!caps;
          } else if(l<lmax) { // a-z, A-Z
            if(c=='_') c=' '; else if(caps) c=c&~(0x20); // convert space +if(caps) put in upper case
            tgt[l++]=(char)c; tgt[l]=NULL;
          } // if(l<lmax)
        } // Other key
      } // if(gb.buttons.pressed(BTN_A))
    } // if (gb.update())
  } // while(1)
} // alphaKeyboard()

