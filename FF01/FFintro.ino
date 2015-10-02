
PROGMEM const unsigned char gamebuinoLogo[] =
{
  84,10,
  B00000011, B11100001, B10000001, B10000110, B01111111, B00111110, B00011000, B01101101, B10000011, B00001111, B00001111, 
  B00001110, B00000001, B10000011, B10000110, B01100000, B00110011, B00011000, B01101101, B11000011, B00011001, B10001111, 
  B00011000, B00000011, B11000011, B10001110, B01100000, B00110011, B00011000, B01101101, B11100011, B00110000, B11001111, 
  B00011000, B00000011, B11000011, B10011110, B01100000, B00110110, B00110000, B11001101, B11100011, B01100000, B11001111, 
  B00110000, B00000110, B11000111, B10011110, B01111110, B00111110, B00110000, B11001101, B10110011, B01100000, B11001111, 
  B00110000, B00001100, B11000110, B11110110, B01100000, B00110011, B00110000, B11011001, B10110110, B01100000, B11001111, 
  B00110011, B11001111, B11001100, B11110110, B01100000, B01100001, B10110000, B11011011, B00011110, B01100000, B11001111, 
  B00110000, B11011000, B01101100, B11100110, B11000000, B01100001, B10110000, B11011011, B00011110, B01100001, B10001111, 
  B00011001, B10011000, B01101100, B11000110, B11000000, B01100011, B10110001, B10011011, B00001110, B00110011, B00001111, 
  B00001111, B10110000, B01111000, B11000110, B11111111, B01111110, B00011111, B00011011, B00000110, B00011110, B00001111, 
};

void myTitleScreen(const __FlashStringHelper* title1, const __FlashStringHelper* title2, const uint8_t* logo) {
  if(gb.startMenuTimer){
    gb.display.fontSize=1;
    gb.display.textWrap=false;
    gb.display.persistence=false;
    gb.battery.show=false;
    gb.display.setColor(BLACK);
    int frameTot=25;
    int t2scroll;
    while(1){
      if(gb.update()) {

        // draw graphics
        gb.display.drawBitmap(0, 0, gamebuinoLogo);
        gb.display.drawBitmap(0, 12+gb.display.fontHeight, logo);
        moveCursor(0, 12);

        if(!ldverr) { // file .LDV was found on sdcard
          // Print titles
          if(gb.frameCount%frameTot<20) { // first 20 frames (2s) : title1
            gb.display.print(title1);
          } else if(gb.frameCount%frameTot<25) { // next 5 frames (0.5s) : left part of title2
            t2scroll=gb.display.fontWidth*gb.display.print(title2)-LCDWIDTH;
            frameTot=20+5+t2scroll+5+t2scroll+5;
          } else if(gb.frameCount%frameTot-25<t2scroll) { // right-scroll title2
              gb.display.cursorX-=gb.frameCount%frameTot-25;
            gb.display.print(title2);
          } else if(gb.frameCount%frameTot-25-t2scroll<5) { // 5 frames (0.5s) : right part of title2
              gb.display.cursorX-=t2scroll;
            gb.display.print(title2);
          } else if(gb.frameCount%frameTot-25-t2scroll-5<t2scroll) { // left-scroll title2
              gb.display.cursorX-=t2scroll-(gb.frameCount%frameTot-25-t2scroll-5);
            gb.display.print(title2);
          } else { // last 5 frames (0.5s) : left part of title2
            gb.display.print(title2);
          }
          // display A button
          moveCursor(LCDWIDTH-gb.display.fontWidth*3-1, LCDHEIGHT-gb.display.fontHeight*3-3);
          if((gb.frameCount/16)%2) gb.display.println(F("\25 \20")); else gb.display.println(F("\25\20"));
          // display B button
          moveCursor(LCDWIDTH-gb.display.fontWidth*3-1, CURRENT+1);
          gb.display.print(F("\26"));
          gb.display.cursorX+=3; gb.display.println(F("i"));
        }
        else { // if file .LDV was NOT found on sdcard
          gb.display.fillRect(0,12,LCDWIDTH,28);
          gb.display.setColor(WHITE);
          gb.display.fillRect(2,14,LCDWIDTH-4,24);
          gb.display.setColor(BLACK);
          moveCursor(5, 18);
          gb.display.println((ldverr==FR_NO_FILE?F("Error: file missing"):(ldverr==FR_DISK_ERR?F("Error: sdcard ko"):F("Error:"))));
          moveCursor(5, CURRENT+4);
          gb.display.print((ldverr==FR_NO_FILE?F("/sdcard/" LDV):(ldverr==FR_DISK_ERR?F("or incorrect FAT"):F("sdcard not mounted"))));
          gb.display.cursorY=LCDHEIGHT-gb.display.fontHeight-2;
        }

        // display C button
        moveCursor(LCDWIDTH-gb.display.fontWidth*3-1, CURRENT+1);
        gb.display.print(F("\27"));
        gb.display.cursorX++; gb.display.println(F("SD"));
        
        // leave the menu when A is pressed
        if(!ldverr && (gb.buttons.pressed(BTN_A) || (gb.frameCount>=gb.startMenuTimer && gb.startMenuTimer!=255))) {
          gb.startMenuTimer=255; //don't automatically skip the title screen next time it's displayed
          inAdvSheet=false;
          pcaret=255;
          readBook(); // read where we were last time
          break;
        }

        // display info when B is pressed
        if(!ldverr && gb.buttons.pressed(BTN_B))
          showAbout();

        // flash the loader when C is pressed
        if(gb.buttons.pressed(BTN_C))
          gb.changeGame();

      } // if(gb.update())
    } // while(1)
  } // if(gb.startMenuTimer)
} // myTitleScreen()

