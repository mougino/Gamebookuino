/*
 * (C) Copyright 2014 Aurélien Rodot. All rights reserved.
 *
 * This file is part of the Gamebuino Library (http://gamebuino.com)
 *
 * The Gamebuino Library is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>
 */

#include "Gamebuino.h"
void Gamebuino::begin() {
	timePerFrame = 50;
	//nextFrameMillis = 0;
	//frameCount = 0;
	frameEndMicros = 1;
	startMenuTimer = 255;
	//read default settings from flash memory (set using settings.hex)
	readSettings();
	//init everything
	backlight.begin();
	backlight.set(BACKLIGHT_MAX);
	buttons.begin();
	buttons.update();
	battery.begin();
	display.begin(SCR_CLK, SCR_DIN, SCR_DC, SCR_CS, SCR_RST);
}

boolean Gamebuino::update() {
	if (((nextFrameMillis - millis()) > timePerFrame) && frameEndMicros) { //if time to render a new frame is reached and the frame end has ran once
		nextFrameMillis = millis() + timePerFrame;
		frameCount++;

		frameEndMicros = 0;
		frameStartMicros = micros();

		backlight.update();
		buttons.update();
		battery.update();

		return true;

	} else {
		if (!frameEndMicros) { //runs once at the end of the frame
			updatePopup();
			displayBattery();
			
			display.update(); //send the buffer to the screen
			if(!display.persistence)
			display.clear(); //clear the buffer

			frameEndMicros = micros(); //measure the frame's end time
			frameDurationMicros = frameEndMicros - frameStartMicros;

			//            display.setTextColor(BLACK);
			//            display.setCursor(0, 40);
			//            display.print(frameDurationMicros / timePerFrame);
			//            display.print(" ");
			//            display.print(2048 - freeRam());

			//            display.setCursor(0, 32);
			//            display.print("CPU:");
			//            display.print(frameDurationMicros / timePerFrame);
			//            display.println("/1000");
			//            display.print("RAM:");
			//            display.print(2048 - freeRam());
			//            display.println("/2048");
		}
		return false;
	}
}

void Gamebuino::setFrameRate(uint8_t fps) {
	timePerFrame = 1000 / fps;
}

void Gamebuino::pickRandomSeed(){
	randomSeed(~battery.voltage * ~micros() * ~micros() + backlight.ambientLight + micros());
}

uint8_t Gamebuino::getCpuLoad(){
	return(frameDurationMicros/(10*timePerFrame));
}

uint16_t Gamebuino::getFreeRam() {
	//from http://www.controllerprojects.com/2011/05/23/determining-sram-usage-on-arduino/
	extern int __heap_start, *__brkval;
	int v;
	return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
}

int8_t Gamebuino::menu(const char* const* items, uint8_t length) {
#if (ENABLE_GUI > 0)
	display.persistence = false;
	int8_t activeItem = 0;
	int8_t currentY = LCDHEIGHT;
	int8_t targetY = 0;
	boolean exit = false;
	int8_t answer = -1;
	while (1) {
		if (update()) {
			if (buttons.pressed(BTN_A) || buttons.pressed(BTN_B) || buttons.pressed(BTN_C)) {
				exit = true; //time to exit menu !
				targetY = - display.fontHeight * length - 2; //send the menu out of the screen
				if (buttons.pressed(BTN_A)) answer = activeItem;
      }
			if (exit == false) {
				if (buttons.repeat(BTN_DOWN,4)) activeItem++;
				if (buttons.repeat(BTN_UP,4)) activeItem--;
				//don't go out of the menu
				if (activeItem == length) activeItem = 0;
				if (activeItem < 0) activeItem = length - 1;

				targetY = -display.fontHeight * activeItem + (display.fontHeight+4); //center the menu on the active item
			} else { //exit :
				if ((currentY - targetY) <= 1)
				return (answer);
			}
			//draw a fancy menu
			currentY = (currentY + targetY) / 2;
			display.cursorX = 0;
			display.cursorY = currentY;
			display.fontSize = 1;
			display.textWrap = false;
			for (byte i = 0; i < length; i++) {
				if (i == activeItem){
					display.cursorX = 3;
					display.cursorY = currentY + display.fontHeight * activeItem;
				}
				display.println((const __FlashStringHelper*)pgm_read_word(items+i));
			}

			//display.fillRect(0, currentY + 3 + 8 * activeItem, 2, 2, BLACK);
			display.setColor(WHITE);
			display.drawFastHLine(0, currentY + display.fontHeight * activeItem - 1, LCDWIDTH);
			display.setColor(BLACK);
			display.drawRoundRect(0, currentY + display.fontHeight * activeItem - 2, LCDWIDTH, (display.fontHeight+3), 3);
		}
	}
#else
	return 0;
#endif
}

void Gamebuino::popup(const __FlashStringHelper* text, uint8_t duration){
#if (ENABLE_GUI > 0)
	popupText = text;
	popupTimeLeft = duration+12;
#endif
}

void Gamebuino::popup(const char* text, uint8_t duration){
#if (ENABLE_GUI > 0)
	popupText = (reinterpret_cast <const __FlashStringHelper*> (text));
	popupTimeLeft = duration+12;
#endif
}

void Gamebuino::updatePopup(){
#if (ENABLE_GUI > 0)
	if (popupTimeLeft){
		uint8_t yOffset = 0;
		if(popupTimeLeft<12){
			yOffset = 12-popupTimeLeft;
		}
		display.fontSize = 1;
		display.setColor(WHITE);
		display.fillRoundRect(0,LCDHEIGHT-display.fontHeight+yOffset-3,84,display.fontHeight+3,3);
		display.setColor(BLACK);
		display.drawRoundRect(0,LCDHEIGHT-display.fontHeight+yOffset-3,84,display.fontHeight+3,3);
		display.cursorX = 4;
		display.cursorY = LCDHEIGHT-display.fontHeight+yOffset-1;
		display.print(popupText);
		popupTimeLeft--;
	}
#endif
}

void Gamebuino::displayBattery(){
#if (ENABLE_BATTERY > 0)
	display.setColor(BLACK, WHITE);
	display.cursorX = LCDWIDTH-display.fontWidth+1;
	display.cursorY = 0;
	switch(battery.level){
	case 0://battery critic, power down
		backlight.set(0);
		display.clear();
		display.fontSize = 1;
		display.print(F("LOW BATTERY\n"));
		display.print(battery.voltage);
		display.print(F("mV\n\nPLEASE\nTURN OFF"));
		display.update();
		set_sleep_mode(SLEEP_MODE_PWR_DOWN);
		sleep_enable();
		sleep_mode();
		sleep_disable();
		break;
	case 1: //empty battery
		if((frameCount % 16) < 8) display.print('\7'); //blinking battery
		else display.print('x');
		break;
	case 2://low battery
	case 3://full battery
	case 4://full battery
		if(battery.show){
			display.print(char(5+battery.level));
		}
		break;
	default:
		if(battery.show){
			display.print('/');
		}
		break;
	}
#endif
}

void Gamebuino::changeGame(){
	display.clear();
	display.print(F("\35 Flashing\n  loader...\n\nDON'T TURN OFF!"));
	display.update();
	//SPSR &= ~(1 << SPI2X); //clear SPI speed x2 for compatibility issues
	SPI.setClockDivider(SPI_CLOCK_DIV128);
	load_game("LOADER");
	display.persistence = false;
	display.println(F("\nNo SD card or\nno LOADER.HEX\n\n\25:Exit"));
	display.update();
	while(1){
		buttons.update();
		if(buttons.pressed(BTN_A))
		break;
		delay(50);
	}
}

boolean Gamebuino::settingsAvailable(){
	if(pgm_read_word(SETTINGS_PAGE) == SETTINGS_TOKEN)
	return true;
	else
	return false;
}

void Gamebuino::readSettings(){
	if(settingsAvailable()){
		display.contrast = pgm_read_byte(SETTINGS_PAGE+OFFSET_CONTRAST);
		backlight.backlightMin = pgm_read_byte(SETTINGS_PAGE+OFFSET_BACKLIGHT_MIN);
		backlight.backlightMax = pgm_read_byte(SETTINGS_PAGE+OFFSET_BACKLIGHT_MAX);
		backlight.ambientLightMin = pgm_read_word(SETTINGS_PAGE+OFFSET_LIGHT_MIN);
		backlight.ambientLightMax = pgm_read_word(SETTINGS_PAGE+OFFSET_LIGHT_MAX);
		
		startMenuTimer = pgm_read_byte(SETTINGS_PAGE+OFFSET_START_MENU_TIMER);
		
		battery.thresolds[0] = pgm_read_word(SETTINGS_PAGE+OFFSET_BATTERY_CRITIC);
		battery.thresolds[1] = pgm_read_word(SETTINGS_PAGE+OFFSET_BATTERY_LOW);
		battery.thresolds[2] = pgm_read_word(SETTINGS_PAGE+OFFSET_BATTERY_MED);
		battery.thresolds[3] = pgm_read_word(SETTINGS_PAGE+OFFSET_BATTERY_FULL);
	}
	else{
		display.contrast = SCR_CONTRAST;
		backlight.backlightMin = BACKLIGHT_MIN;
		backlight.backlightMax = BACKLIGHT_MAX;
		backlight.ambientLightMin = AMBIENTLIGHT_MIN;
		backlight.ambientLightMax = AMBIENTLIGHT_MAX;
		
		startMenuTimer = START_MENU_TIMER;
		
		battery.thresolds[0] = BAT_LVL_CRITIC;
		battery.thresolds[1] = BAT_LVL_LOW;
		battery.thresolds[2] = BAT_LVL_MED;
		battery.thresolds[3] = BAT_LVL_FULL;
	}
}

void Gamebuino::getDefaultName(char* string){
	if(settingsAvailable()){	
		for(byte i=0; i<USERNAME_LENGTH; i++){
			string[i] = (char)pgm_read_byte(SETTINGS_PAGE+OFFSET_USERNAME+i);
		}
	}
}

boolean Gamebuino::collidePointRect(int16_t x1, int16_t y1 ,int16_t x2 ,int16_t y2, int16_t w, int16_t h){
	if((x1>=x2)&&(x1<x2+w))
	if((y1>=y2)&&(y1<y2+h))
	return true;
	return false;
}

boolean Gamebuino::collideRectRect(int16_t x1, int16_t y1, int16_t w1, int16_t h1 ,int16_t x2 ,int16_t y2, int16_t w2, int16_t h2){
  return !( x2     >=  x1+w1  || 
            x2+w2  <=  x1     || 
            y2     >=  y1+h1  ||
            y2+h2  <=  y1     );
}

boolean Gamebuino::collideBitmapBitmap(int16_t x1, int16_t y1, const uint8_t* b1, int16_t x2, int16_t y2, const uint8_t* b2){
  int16_t w1 = pgm_read_byte(b1);
  int16_t h1 = pgm_read_byte(b1 + 1);
  int16_t w2 = pgm_read_byte(b2);
  int16_t h2 = pgm_read_byte(b2 + 1);

  if(collideRectRect(x1, y1, w1, h1, x2, y2, w2, h2) == false){
    return false;
  }
  
  int16_t xmin = (x1>=x2)? 0 : x2-x1;
  int16_t ymin = (y1>=y2)? 0 : y2-y1;
  int16_t xmax = (x1+w1>=x2+w2)? x2+w2-x1 : w1;
  int16_t ymax = (y1+h1>=y2+h2)? y2+h2-y1 : h1;
  for(uint8_t y = ymin; y < ymax; y++){
    for(uint8_t x = xmin; x < xmax; x++){
      if(display.getBitmapPixel(b1, x, y) && display.getBitmapPixel(b2, x1+x-x2, y1+y-y2)){
        return true;
      }
    }
  }
  return false;
}
