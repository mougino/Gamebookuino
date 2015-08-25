
// The following 3 functions use strncat instead of strcat to avoir buffer overflows
void catStr(const __FlashStringHelper* nstr) { strncat_P(txt, (const prog_char*)nstr, TXTSIZE-1-strlen(txt)); } // concatenate a PROGMEM string to txt[]
void catStr(char* nstr) { strncat(txt, nstr, strlen(nstr)); } // concatenate an SRAM string to txt[]
void catInt(int ni) { char ns[4]; int2char(ni, ns); strncat(txt, ns, TXTSIZE-1-strlen(txt)); } // concatenate an int to txt[]

void showAdventureSheet() {
  strcpy(txt, ""); // reset screen text buffer

  if(crline==0) { // [ADVENTURE SHEET]
    catStr(PGMT(DF_adv)); catStr("\n"); }
  if(crline<=1) { // Skill
    catStr(">\001"); catStr(PGMT(DF_ski)); catInt(skiC); catStr("/"); catInt(skiT); catStr("\n"); }
  if(crline<=2) { // Stamina
    catStr(">\002"); catStr(PGMT(DF_sta)); catInt(staC); catStr("/"); catInt(staT); catStr("\n"); }
  if(crline<=3) { // Luck
    catStr(">\003"); catStr(PGMT(DF_luc)); catInt(luC); catStr("/"); catInt(luT); catStr("\n"); }
  if(crline<=4) { // Gold
    catStr(">\004"); catStr(PGMT(DF_gold1)); catInt(gold); catStr(PGMT(DF_gold2)); catStr("\n"); }
  if(crline<=5) { // Provisions
    catStr(">\005"); catStr(PGMT(DF_prov)); catInt(prov); catStr("\n"); }
  if(crline<=6) { // Potion
    if(potionType==255) {
      catStr(">\006"); catStr(PGMT(DF_pot255)); catStr(PGMT(DF_potion)); catStr("\n"); } // choose potion
    else {
      catStr(">\006"); catStr(PGMT(DF_potion)); // potion..
      if(potionType==0) catStr(PGMT(DF_pot0)); // ..of skill
      else if(potionType==1) catStr(PGMT(DF_pot1)); // ..of stamina
      else if(potionType==2) catStr(PGMT(DF_pot2)); // ..of luck
      if(potion==2) catStr("\011"); // full (2 measures left)
      else if(potion==1) catStr("\010"); // half-empty (1 measure left)
      else catStr("\007"); // empty (no measure left)
      catStr("\n");
    }
  }
  if(crline<=7) { // [ACTIONS]
    catStr(PGMT(DF_axn)); catStr("\n"); }
  if(crline<=8) { // save game
    catStr(">\011"); catStr(PGMT(DF_save)); catStr("\n"); }
  if(crline<=9) { // roll 1 die
    catStr(">\012"); catStr(PGMT(DF_roll1)); catStr("\n"); }
  if(crline<=10) { // roll both dice
    catStr(">\013"); catStr(PGMT(DF_roll2)); catStr("\n"); }
  if(crline<=11) { // goto paragraph
    catStr(">\014"); catStr(PGMT(DF_goto));
    #ifdef debug
    catInt(par);
    #endif
    catStr("\n");
  }
  if(crline<=12) { // [EQUIPMENT]
    catStr(PGMT(DF_eqp)); catStr("\n"); }
  for(byte i=0; i<nbOfEquipment(); i++) { // items
    if(crline<=13+i) { catStr(eqp+itemOffset(i)); break; }
  }
  catStr("\n>\015"); catStr(PGMT(DF_add));
  txt[TXTSIZE-1]=NULL; // final '\0' for protection
} // showAdventureSheet()

byte nbOfEquipment() {
  byte n=0;
  for(byte i=0; i<EQPSIZE-1; i++) {
    if(eqp[i]==NULL) return ++n;
    if(eqp[i]=='\n') n++;
  }
}

byte itemOffset(byte nItm) {
  if(nItm==1) return 0;
  byte nb,offset;
  for(offset=0; offset<EQPSIZE-1; offset++) {
    if(eqp[offset]==NULL || eqp[offset]=='\n') { nb++; if(nb==nItm) return ++offset; }
  }
}

void showAbout() {
  gb.battery.show = true;
  while(1) {
    if (gb.update()) {
      // display title
      gb.display.println(PGMT(DF_about));
      gb.display.drawFastHLine(0,gb.display.cursorY,LCDWIDTH);
      moveCursor(CURRENT, CURRENT+2); // gb.display.cursorY+=2;
      // display lines of information
      #ifdef FRENCH
      gb.display.println(F("Moteur Gamebookuino"));
      gb.display.print(F(" "));
      gb.display.println(F(ENGINE_VER));
      gb.display.println(F("Logiciel PC crea LDV"));
      gb.display.print(F(" "));
      gb.display.println(F(PCSOFT_VER));
      gb.display.println(F("web: mougino.free.fr"));
      #else
      gb.display.println(F("Gamebookuino engine"));
      gb.display.print(F(" "));
      gb.display.println(F(ENGINE_VER));
      gb.display.println(F("PC software LDV maker"));
      gb.display.print(F(" "));
      gb.display.println(F(PCSOFT_VER));
      gb.display.println(F("web: mougino.free.fr"));
      #endif
      #ifdef FONT5X7_C
      gb.display.setFont(font5x7);
      #endif
      moveCursor(LCDWIDTH-3*gb.display.fontWidth, LCDHEIGHT-gb.display.fontHeight);
      gb.display.println(F("\025ok"));
      #ifdef FONT5X7_C
      gb.display.setFont(font3x5);
      #endif
      // handle user input
      if (gb.buttons.pressed(BTN_A) || gb.buttons.pressed(BTN_B) || gb.buttons.pressed(BTN_C)) break;
    } // if (gb.update())
  } // while(1)
  gb.battery.show = false;
} // showAbout()

void printCmprSgn(byte val1, byte val2) { // print the comparison sign result between 2 values
  if(val1==val2) gb.display.print(F("="));
  else if(val1<val2) gb.display.print(F("<"));
  else { gb.display.cursorX++; gb.display.print(F(">")); }
}

boolean showDice(byte mode) { // in TEST_YOUR_LUCK mode: return true if lucky, false if unlucky
  gb.battery.show = true;
  byte die1=0, die2=random(1,7);
  if(mode!=ROLL_ONE_DIE) die1=random(1,7);
  while(1) {
    if (gb.update()) {
        // display menu title
      if(mode==TEST_YOUR_LUCK) 
        gb.display.println(PGMT(DF_tyl));
      else if(mode==ROLL_ONE_DIE)
        gb.display.println(PGMT(DF_roll1));
      else if(mode==ROLL_BOTH_DICE)
        gb.display.println(PGMT(DF_roll2));
      gb.display.drawFastHLine(0,gb.display.cursorY,LCDWIDTH);
      // display dice +result
      if(die1) gb.display.drawBitmap(25, 12, yd[die1-1]);
      gb.display.drawBitmap(36, 12, yd[die2-1]);
      moveCursor(11.5*gb.display.fontWidth, 13);
      printCmprSgn(1,1); // print '='
      gb.display.println(die1+die2);
      if(mode==TEST_YOUR_LUCK) { // display current luck + result
        #ifdef FONT5X7_C
        gb.display.setFont(font5x7);
        moveCursor(9, CURRENT+4);
        #else
        moveCursor(24, CURRENT+6);
        #endif
        printCmprSgn(die1+die2, luC); // print comparison sign between dice and current luck
        gb.display.print(PGMT(DF_luc));
        gb.display.println(luC);
        gb.display.cursorY+=4;
        #ifdef FONT5X7_C
        gb.display.setFont(font3x5);
        #endif
        gb.display.fontSize=2;
        if(die1+die2<=luC) gb.display.print(PGMT(DF_lucky)); else gb.display.print(PGMT(DF_unlucky));
        gb.display.fontSize=1;
      }
      // handle user input
      if (gb.buttons.pressed(BTN_A) || gb.buttons.pressed(BTN_B) || gb.buttons.pressed(BTN_C)) {
        if(mode==TEST_YOUR_LUCK && luC>0) luC--; // test your luck consumes 1 point of luck
        gb.battery.show = false;
        return !(mode==TEST_YOUR_LUCK && (die1+die2>luC));
      }
    } // if (gb.update())
  } // while(1)
} // showDice()

void reduceStat(byte &stat) {
  stat-=2; if(stat>99) stat=0; // stat is a byte, if going negative it bitshifts back to 255 or under
}

void printFString(boolean right, const char* str) {
  if(right) gb.display.cursorX+=LCDWIDTH/2+2;
  gb.display.print(str);
}

void printFStamina(boolean right, byte stat, boolean condition) {
  if(!condition) return;
  if(right) gb.display.cursorX+=LCDWIDTH/2+2;
  if(stat) {
    gb.display.print(PGMT(DF_fsta));
    gb.display.print(stat);
  } else gb.display.drawBitmap(gb.display.cursorX+16, 16, skull); // if no more stamina, show skull during ending animation (2s)
}

void showFight() {
  gb.battery.show = true;
  byte eos,eski,esta,endanim=0;
  byte di1,di2,yas,en1,en2,eas;
  boolean before=true,flee=false,tyl=false;
  for(eos=pcaret+1; ; eos++) { if(txt[eos]=='\n' || txt[eos]==NULL) break; } // eos = end of FIGHT string "Enemy_Name snn/Snn"
  eski=char2int(txt+eos-6); esta=char2int(txt+eos-2);
  while(1) {
    if (gb.update()) {

      if((!esta || !staC) && !endanim) { before=true; endanim=21; } // ending animation (2s)
      if(endanim) { endanim--; if(endanim==1) break; }

      // display menu title
      gb.display.println(PGMT(DF_fight));
      gb.display.drawFastHLine(0,6,LCDWIDTH);
      gb.display.cursorY+=2;
      // display names
      gb.display.print(PGMT(DF_you));
      gb.display.cursorX=0;
      txt[eos-8]=(char)NULL; // trick to display enemy name: change FIGHT string to "Enemy_Name\0snn/Snn"
      printFString(RIGHT, txt+pcaret+1);
      txt[eos-8]=(char)' '; // revert FIGHT string to "Enemy_Name snn/Snn"
      // display your stamina, except 1 frame /2 after a round where you've been wounded or when escaping
      moveCursor(8, NEXTLINE);
      printFStamina(LEFT, staC, !(!before && (yas<eas) && (gb.frameCount/4)%2 || (!before && flee && (gb.frameCount/4)%2)));
      // display enemy's stamina, except 1 frame /2 after a round where enemy has been wounded
      moveCursor(8, CURRENT);
      printFStamina(RIGHT, esta, (!before & flee) || !(!before && (yas>eas) && (gb.frameCount/4)%2));
      moveCursor(CURRENT, NEXTLINE);
      // display dice +attack strength
      if(!before && !flee) {
        gb.display.drawBitmap(0, gb.display.cursorY, yd[di1-1]); // your dice
        gb.display.drawBitmap(8, gb.display.cursorY++, yd[di2-1]);
        gb.display.cursorX=16;
        gb.display.print(F("+"));
        gb.display.print(skiC);
        gb.display.print(PGMT(DF_fski));
        gb.display.drawBitmap(LCDWIDTH/2+2, --gb.display.cursorY, ed[en1-1]); // enemy's dice
        gb.display.drawBitmap(LCDWIDTH/2+10, gb.display.cursorY++, ed[en2-1]);
        gb.display.cursorX=LCDWIDTH/2+18;
        gb.display.print(F("+"));
        gb.display.print(eski);
        gb.display.println(PGMT(DF_fski));
        // total (attack strengths)
        #ifdef FONT5X7_C
        gb.display.setFont(font5x7);
        #else
        gb.display.cursorY+=2;
        #endif
        gb.display.cursorX=10;
        printCmprSgn(1,1); // print '='
        gb.display.print(yas);
        gb.display.cursorX=LCDWIDTH/2+12;
        printCmprSgn(1,1); // print '='
        gb.display.print(eas);
      }
      gb.display.drawFastVLine(LCDWIDTH/2,7,29);
      // display result of current round
      if(!before && !flee) {
        #ifdef FONT5X7_C
        gb.display.setFont(font3x5);
        #else
        gb.display.cursorY-=2;
        #endif
        moveCursor(LCDWIDTH/2-2, CURRENT+1);
        printCmprSgn(yas, eas); // print comparison sign between your attack strength and the enemy's
        gb.display.drawRoundRect(LCDWIDTH/2-4, gb.display.cursorY-1, gb.display.fontWidth+4, gb.display.fontHeight+2, 3);
      }
      gb.display.drawFastHLine(0,36,LCDWIDTH);
      // if no ending animation
      if(!endanim) {
        // display user options
        moveCursor(0, 37);
        if(before) { // before an attack
          printCaret(!flee); gb.display.println(PGMT(DF_fight));
          printCaret(flee); gb.display.print(PGMT(DF_flee));
        } else { // after an attack
          printCaret(!tyl); gb.display.println(F("Ok"));
          if(flee || (yas!=eas)) { printCaret(tyl); gb.display.print(PGMT(DF_tyl)); }
        }
        // handle user input
        if(before) {
          if (gb.buttons.pressed(BTN_DOWN) || gb.buttons.pressed(BTN_UP)) flee=!flee;
          if (gb.buttons.pressed(BTN_A)) {
            if(flee)
              reduceStat(staC);
            else {
              en1=random(1,7); en2=random(1,7); // roll enemy's dice
              di1=random(1,7); di2=random(1,7); // roll your dice
              yas=di1+di2+skiC; // your attack strength
              eas=en1+en2+eski; // enemy's attack strength
              if(yas<eas) reduceStat(staC); // you were hit
              else if(yas>eas) reduceStat(esta); // you hit the enemy
            }
            before=false; tyl=false; // display ok/test-your-luck
          } // BTN_A
        } else { // if(!before)
          if (gb.buttons.pressed(BTN_DOWN) || gb.buttons.pressed(BTN_UP)) { if(flee || (yas!=eas)) tyl=!tyl; }
          if (gb.buttons.pressed(BTN_A)) {
            if(tyl) {
              if(showDice(TEST_YOUR_LUCK)) { // lucky
                if(flee || (yas<eas)) staC++; // the wound you received is minimized
                else reduceStat(esta); // you inflicted a more severe wound to the enemy
              } else { // unlucky
                if(flee || (yas<eas)) reduceStat(staC); // you have received a more serious blow
                else esta++; // you inflicted a minor hit to the enemy
              }
            }
            if(flee) break; // leave fight after escape result
            before=true; flee=false; // display fight options
          } // BTN_A
        } // if(!before)
      } // if(!endanim)
    } // if (gb.update())
  } // while(1)
  gb.battery.show = false;
} // showFight()

