
int char2int(char* str) { // return the numerical value of a string
  int rslt=0;
  for(byte i=0; i<strlen(str); i++) {
    if(str[i]<'0' || str[i]>'9') return (i?rslt:-1); // Not a Number: return -1 if in first place, or conversion so far if > @[0]
    rslt = 10*rslt + str[i]-'0';
  }
  return rslt;
}

void int2char(int nb, char* str) { // convert an integer to a string
  String(nb).toCharArray(str, String(nb).length()+1);
}

int numKeyboard(int init, byte digits) {
  char nb[6]="";
  byte x=0, y=0;
  if(init!=-1) int2char(init, nb);
  if(digits>5) digits=5; // an int is 32767 max = 5 digits
  while(1) {
    if (gb.update()) {
      #ifdef FONT5X7_C
      gb.display.setFont(font5x7);
      #endif
      // draw input bar
      if(strlen(nb)) gb.display.print(nb);
      if((gb.frameCount/4)%2) gb.display.print(F("_"));
      gb.display.drawFastHLine(0,gb.display.fontHeight+1,LCDWIDTH);
      // draw numbers
      moveCursor(2*gb.display.fontWidth, gb.display.fontHeight+4);
      gb.display.println(F("1  2  3  <"));
      moveCursor(2*gb.display.fontWidth, CURRENT+2);
      gb.display.println(F("4  5  6  0"));
      moveCursor(2*gb.display.fontWidth, CURRENT+2);
      gb.display.println(F("7  8  9  ok"));
      // draw select box
      gb.display.drawRoundRect((2+3*x)*gb.display.fontWidth-3, (y+1)*(gb.display.fontHeight+2), ((x==3 && y==2)?2.5:1.5)*gb.display.fontWidth+2, gb.display.fontHeight+3, 3);
      // draw legend
      gb.display.cursorY=LCDHEIGHT-gb.display.fontHeight+1;
      gb.display.print(F("\026"CANCEL));
      #ifdef FONT5X7_C
      gb.display.setFont(font3x5);
      #endif
      // handle user input
      if(gb.buttons.pressed(BTN_RIGHT)) x=(x+1)%4;
      if(gb.buttons.pressed(BTN_LEFT)) { x--; if(x==255) x=3; }
      if(gb.buttons.pressed(BTN_DOWN)) y=(y+1)%3;
      if(gb.buttons.pressed(BTN_UP)) { y--; if(y==255) y=2; }
      if(gb.buttons.pressed(BTN_A)) {
        if(x==3 && y==2) { // 'ok' button
          return char2int(nb);
        } else if(x==3 && y==0) { // '<' (del) button
          nb[strlen(nb)-1]=NULL;
        } else if(strlen(nb)<digits) { // number
          byte n; if(x==3) n='0'; else n='1'+3*y+x;
          byte l=strlen(nb)-1;
          if(!l && nb[l]=='0') nb[l]=(char)n; else nb[++l]=(char)n; // handle special case to rewrite on top of "0"
          nb[++l]=NULL;
        }
      }
      if (gb.buttons.pressed(BTN_B))
        return -1; // Cancelled by user
    } // if (gb.update())
  } // while(1)
} // numKeyboard()

