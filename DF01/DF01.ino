// Nota: this app needs a modified version of the Gamebuino library
// with sound and keyboard stripped off, and enhanced gb.popup()
// If it was not provided to you when you downloaded this file,
// you can get it at https://github.com/mougino/Gamebookuino

#define ENGINE_VER "v0.3 (01-OCT-2015)"
#define PCSOFT_VER "v0.3 (01-OCT-2015)"

#include <SPI.h>
#include <Gamebuino.h>
#include <petit_fatfs.h>
#include <EEPROM.h>

//#define debug // Comment for live engine. Uncomment for demo engine
//#define FONT5X7_C // Comment if you want only Font3x5, numKeyboard() won't be as pretty but it'll save memory

//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// THIS IS THE PART TO CHANGE WHEN YOU BUILD THE APP WITH A DIFFERENT GAMEBOOK:
#define NPAR 401 // Number of paragraphs in this Gamebook
#define LDV "DF01.LDV"
#define CANCEL "Annuler"
PROGMEM const char DF_title1[]="DEFIS FANTASTIQUES 01";
PROGMEM const char DF_title2[]="Le Sorcier de la montagne de Feu";
PROGMEM const char DF_eqp_ini[]="^epee\n^armure de cuir\n^sac a dos\n^lanterne";
PROGMEM const char keyboard[]="azertyuiopqsdfghjklmwxcvbn_\030<"; // Used by alphaKeyboard.ino
// Adventure sheet constants
PROGMEM const char DF_adv[]="[FEUILLE D'AVENTURE]";
PROGMEM const char DF_ski[]="Habilete:";
PROGMEM const char DF_sta[]="Endurance:";
PROGMEM const char DF_luc[]="Chance:";
PROGMEM const char DF_gold1[]="Or:";
PROGMEM const char DF_gold2[]=" po";
PROGMEM const char DF_prov[]="Provisions:";
PROGMEM const char DF_meal[]="Endurance +4";
PROGMEM const char DF_pot255[]="Choisir ";
PROGMEM const char DF_potion[]="Potion ";
PROGMEM const char DF_pot0[]="d'Adresse";
PROGMEM const char DF_pot1[]="de Vigueur";
PROGMEM const char DF_pot2[]="Bon.Fortune";
PROGMEM const char DF_po_ski[]="Habilete niveau ini!";
PROGMEM const char DF_po_sta[]="Endurance niv. ini!";
PROGMEM const char DF_po_luc[]="Chance niveau ini+1";
PROGMEM const char DF_axn[]="[ACTIONS]";
PROGMEM const char DF_save[]="Sauver la partie";
PROGMEM const char DF_savok[]="Sauvegarde effectuee";
PROGMEM const char DF_back[]="Dernier paragraphe";
PROGMEM const char DF_roll1[]="Lancer un de";
PROGMEM const char DF_roll2[]="Lancer deux des";
PROGMEM const char DF_goto[]="Aller au...";
PROGMEM const char DF_about[]="A propos";
PROGMEM const char DF_eqp[]="[EQUIPEMENT]";
PROGMEM const char DF_add[]="Ajouter equipement";
// Game constants
PROGMEM const char DF_tyl[]="Tentez votre Chance";
PROGMEM const char DF_lucky[]="CHANCEUX";
PROGMEM const char DF_unlucky[]="MALCHANCEUX";
PROGMEM const char DF_fight[]="Combattez!";
PROGMEM const char DF_flee[]="Fuir le combat";
PROGMEM const char DF_you[]="VOUS";
PROGMEM const char DF_fski[]="HAB";
PROGMEM const char DF_fsta[]="END:";
PROGMEM const char DF_dead[]="Vous etes mort...\nPressez \025 pour\nretenter l'aventure";
// Menus
PROGMEM const char DF_m1s1[]="Modif. stat courante";
PROGMEM const char DF_m1s2[]="Modif. total initial";
PROGMEM const char DF_m2s1[]="Boire une mesure";
PROGMEM const char DF_m3s1[]="Prendre un repas";
PROGMEM const char DF_m3s2[]="Augmenter provisions";
PROGMEM const char DF_m4s1[]="Supprimer equipement";
PROGMEM const char DF_m5s1[]="Potion d'Adresse";
PROGMEM const char DF_m5s2[]="Potion de Vigueur";
PROGMEM const char DF_m5s3[]="P. de Bonne Fortune";
// About
PROGMEM const char DF_engine[]="Moteur Gamebookuino";
PROGMEM const char DF_pcsoft[]="Logiciel PC crea LDV";
#ifdef debug // declare dummy gamebook
PROGMEM const char dummy[]=
"Avant de vous lancer\ndans cette aventure,\nil vous faut d'abord\ndeterminer vos\npropres forces et\nfaiblesses. Vous\navez en votre\npossession une epee\net un bouclier,\nainsi qu'un sac a\ndos contenant des\nprovisions\n(nourriture et\nboissons) pour le\nvoyage. Afin de vous\npreparer a votre\nquete, vous vous\netes entraine au\nmaniement de l'epee\net vous vous etes\nexerce avec\nacharnement a\naccroitre votre\nendurance.\nPressez \026 pour\nouvrir la feuille\nd'aventure.\nTest paragraphe:\nallez au >2 ou au\n>123.\n<Tentez votre Chance!\nCombattez:\n^Ennemi numero 1 H06/E05";
#endif
// END OF THE PART TO CHANGE WHEN YOU BUILD THE APP WITH A DIFFERENT GAMEBOOK
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

Gamebuino gb;

#define ITMSIZE 17  // item of equipment: 17 characters max including starting '^' and final '\n' or '\0'
#define EQPSIZE 255 // equipment: 15 items of 17 chr (more if items < 17 characters)
#define NCHRX 21    // number of characters per line with font3x5
#define NCHRY 8     // number of lines per screen with font3x5
#define TXTSIZE 180 // text on screen: max 21x8 characters (with font3x5) + final '\0' + a few characters on next line for active links

#define EEPROM_IDENTIFIER 0xFFDF // FF = Fighting Fantasy, DF = Defis Fantastiques
#define PGMT(pgm_ptr) (reinterpret_cast <const __FlashStringHelper*> (pgm_ptr))
#define IS_SPECIAL_CHARACTER(T) (T=='>' || T=='<' || T=='^')
#define TEST_YOUR_LUCK 0
#define ROLL_ONE_DIE 1
#define ROLL_BOTH_DICE 2
#define LEFT false
#define RIGHT true
#define CURRENT 100
#define PREVLINE 150
#define NEXTLINE 200

PROGMEM const char DF_m0s0[] = CANCEL;
PROGMEM const char* DF_menu1[3] = {DF_m1s1, DF_m1s2, DF_m0s0}; // Edit stat menu
PROGMEM const char* DF_menu2[2] = {DF_m2s1, DF_m0s0}; // Drink potion menu
PROGMEM const char* DF_menu3[3] = {DF_m3s1, DF_m3s2, DF_m0s0}; // Provision menu
PROGMEM const char* DF_menu4[2] = {DF_m4s1, DF_m0s0}; // Delete equipment menu
PROGMEM const char* DF_menu5[4] = {DF_m5s1, DF_m5s2, DF_m5s3, DF_m0s0}; // Choose potion menu

PROGMEM const unsigned char DF_logo[] = 
{
  64,30,
  0x00,0x00,0x00,0x00,0x00,0x00,0x3C,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x6A,0x00,
  0x00,0x00,0x07,0xFE,0x00,0x00,0x39,0x00,
  0x00,0x00,0x10,0x40,0xF8,0x01,0xFA,0x00,
  0x00,0x00,0xB0,0xF9,0x8E,0x01,0xF8,0x00,
  0x00,0x0D,0x8F,0xFF,0x63,0xC7,0xF0,0x00,
  0x00,0x2B,0xFF,0xFF,0xFB,0x1F,0x00,0x00,
  0x00,0xC1,0xEF,0x7F,0x13,0xFC,0xE0,0x00,
  0x01,0x83,0xFD,0x3F,0x7B,0xFB,0xF0,0x00,
  0x03,0xEF,0xB8,0x91,0xF3,0xDF,0xE6,0x00,
  0x09,0xDF,0xFE,0xC6,0x14,0x6F,0xE1,0x80,
  0x10,0x3F,0xFF,0xCB,0x1F,0xF1,0xF3,0xA0,
  0x20,0x7F,0xFF,0xCE,0x01,0xFA,0xFB,0x30,
  0x58,0xFF,0xFF,0x44,0x47,0x41,0xBE,0x08,
  0xF8,0xFF,0xFC,0x50,0x1F,0xCF,0xBF,0x84,
  0xC0,0x07,0xEB,0x04,0xFF,0xF9,0x97,0xDC,
  0xC0,0x19,0xFA,0x81,0xFF,0xFE,0x2B,0xBA,
  0xFC,0x1A,0x0C,0x07,0xFF,0xFF,0x17,0xE0,
  0x1D,0xC0,0x38,0x5F,0xFF,0xFF,0xF7,0xF1,
  0x07,0xB0,0x30,0x3F,0xFF,0xFF,0xF7,0xF5,
  0x00,0x3F,0xC0,0xF8,0x1F,0xFF,0xFE,0x07,
  0x00,0x01,0x03,0xFC,0x9C,0x00,0x07,0x01,
  0x00,0x04,0x0F,0xE0,0x18,0x06,0x07,0x0E,
  0x00,0x00,0x17,0xF8,0x00,0x87,0x02,0x1E,
  0x00,0x20,0x40,0x1F,0xFC,0x00,0x0F,0xF0,
  0x00,0x83,0x00,0x00,0x3F,0xFF,0xFC,0x00,
  0x01,0x04,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x08,0x00,0x00,0x00,0x00,0x00,0x00,
  0x05,0x60,0x00,0x00,0x00,0x00,0x00,0x00,
  0x0B,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
};

// your dice (white)
PROGMEM const unsigned char d1[] = { 7,7,0xFE,0x82,0x82,0x92,0x82,0x82,0xFE };
PROGMEM const unsigned char d2[] = { 7,7,0xFE,0x82,0xA2,0x82,0x8A,0x82,0xFE };
PROGMEM const unsigned char d3[] = { 7,7,0xFE,0x82,0x86,0x92,0xC2,0x82,0xFE };
PROGMEM const unsigned char d4[] = { 7,7,0xFE,0x82,0xAA,0x82,0xAA,0x82,0xFE };
PROGMEM const unsigned char d5[] = { 7,7,0xFE,0xAA,0x82,0x92,0x82,0xAA,0xFE };
PROGMEM const unsigned char d6[] = { 7,7,0xFE,0xAA,0x82,0xAA,0x82,0xAA,0xFE };
const unsigned char* yd[] = {d1, d2, d3, d4, d5, d6};
// enemy's dice (black)
PROGMEM const unsigned char e1[] = { 7,7,0xFE,0xFE,0xFE,0xEE,0xFE,0xFE,0xFE };
PROGMEM const unsigned char e2[] = { 7,7,0xFE,0xBE,0xFE,0xFE,0xFE,0xFA,0xFE };
PROGMEM const unsigned char e3[] = { 7,7,0xFE,0xFA,0xFE,0xEE,0xFE,0xBE,0xFE };
PROGMEM const unsigned char e4[] = { 7,7,0xFE,0xBA,0xFE,0xFE,0xFE,0xBA,0xFE };
PROGMEM const unsigned char e5[] = { 7,7,0xFE,0xBA,0xFE,0xEE,0xFE,0xBA,0xFE };
PROGMEM const unsigned char e6[] = { 7,7,0xFE,0xBA,0xFE,0xBA,0xFE,0xBA,0xFE };
const unsigned char* ed[] = {e1, e2, e3, e4, e5, e6};

PROGMEM const unsigned char skull[] = { 9,9,0x7F,0x00,0xFF,0x80,0xDD,0x80,0x88,0x80,0xD8,0x80,0xFD,0x80,0xF7,0x80,0x7F,0x00,0x55,0x00 }; 

#ifdef FONT5X7_C
extern const byte font3x5[]; // a small but efficient font (default)
extern const byte font5x7[]; // a large, comfy font
#endif

// stats: skill, stamina, luck (current/total) ; paragraph & other variables
byte skiC, skiT, staC, staT, luC, luT, potion, potionType, gold, prov; // byte=0 to 255
byte nxline, nxpage, pcaret, oldpcaret;
int ldverr; // int=-32768 to 32767
word par, lastp, poffset, oldpoffset, crline, oldcrline; // word=0 to 65535
boolean bmpInPar,inAdvSheet=false; // boolean=0 or 1
char eqp[EQPSIZE]; // Offers EQPSIZE-1 usable characters, from eqp[0] to eqp[EQPSIZE-2] + final '\0' at eqp[EQPSIZE-1]
char txt[TXTSIZE]; // Offers TXTSIZE-1 usable characters, from txt[0] to txt[TXTSIZE-2] + final '\0' at txt[TXTSIZE-1]

// Following two functions are needed by petit_fatfs
byte rx() { SPDR=0xFF; loop_until_bit_is_set(SPSR, SPIF); return SPDR; }
void tx(byte d) { SPDR=d; loop_until_bit_is_set(SPSR, SPIF); }

// Prototypes
void myTitleScreen(const __FlashStringHelper* title1, const __FlashStringHelper* title2, const uint8_t* logo);
void catStr(const __FlashStringHelper* nstr);
void catStr(char* nstr);
void catInt(int ni);
void showAdventureSheet();
byte nbOfEquipment();
byte itemOffset(byte nItm);
void showAbout();
boolean showDice(byte mode);
void reduceStat(byte &stat);
void printFString(boolean right, const char* str);
void printFStamina(boolean right, byte stat, boolean condition);
void showFight();
int char2int(char* str);
void int2char(int nb, char* str);
int numKeyboard(int init, byte digits);
void printAlpha(boolean caps, byte i);
void alphaKeyboard(char* tgt, byte lmax);
void changeStat(byte &stat);
long Toc(int idx);
void backToBook();
void readBook();
void seekBook(int tgtline);
void printBook();
void printCaret(boolean condition);
byte prevCaretOnScreen();
byte nextCaretOnScreen();

void moveCursor(byte x, byte y) {
  gb.display.cursorX+=
    (x>=CURRENT-NCHRX?x-CURRENT // relative CURRENT +/- n
    :x-gb.display.cursorX); // absolute
  gb.display.cursorY+=
    (y>=NEXTLINE-NCHRY?y-NEXTLINE+gb.display.fontHeight: // relative NEXTLINE +/- n
    (y>=PREVLINE-NCHRY?y-PREVLINE-gb.display.fontHeight: // relative PREVLINE +/- n
    (y>=CURRENT?y-CURRENT // relative CURRENT +/- n
    :y-gb.display.cursorY))); // absolute
}

void initGame() {
  potion=2; // measures of potion
  prov=10; // provisions
  skiC=skiT=random(1,7)+6; // 1D+6
  staC=staT=random(2,13)+12; // 2D+12
  luC=luT=random(1,7)+6; // 1D+6
  potionType=255; // 0=SKILL ; 1=STAMINA ; 2=LUCK ; 255=Undefined
  par=0;
  lastp=0x7777;
  strcpy_P(eqp, DF_eqp_ini); // equipment
}

byte readNextByteFromEEPROM(int &pos) {
  return EEPROM.read(pos++);
}

word readNextWordFromEEPROM(int &pos) {
  word r=(EEPROM.read(pos++) << 8) & 0xFF00; //MSB
  r+=EEPROM.read(pos++) & 0x00FF; //LSB
  return r;
}

void restoreStatsFromEEPROM() {
  #ifdef debug
  initGame();
  #else
  int a=0; // place ourselves at beginning of EEPROM
  if(readNextWordFromEEPROM(a)!=EEPROM_IDENTIFIER) { initGame(); return; } // initialize stats if no or bad EEPROM identifier
  skiC       = readNextByteFromEEPROM(a);
  skiT       = readNextByteFromEEPROM(a);
  staC       = readNextByteFromEEPROM(a);
  staT       = readNextByteFromEEPROM(a);
  luC        = readNextByteFromEEPROM(a);
  luT        = readNextByteFromEEPROM(a);
  potion     = readNextByteFromEEPROM(a);
  potionType = readNextByteFromEEPROM(a);
  gold       = readNextByteFromEEPROM(a);
  prov       = readNextByteFromEEPROM(a);
  par        = readNextWordFromEEPROM(a);
  lastp      = readNextWordFromEEPROM(a);
  poffset    = readNextWordFromEEPROM(a);
  crline     = readNextWordFromEEPROM(a);
  bmpInPar   = readNextByteFromEEPROM(a);
  for(byte i=0; i<EQPSIZE-1; i++) {
    eqp[i]   = readNextByteFromEEPROM(a);
  } eqp[EQPSIZE-1]=NULL;
  #endif
}

void appendByteToEEPROM(int &pos, byte b) {
  EEPROM.write(pos++, b);
}

void appendWordToEEPROM(int &pos, word w) {
  EEPROM.write(pos++, (w >> 8) & 0x00FF); //MSB
  EEPROM.write(pos++, w & 0x00FF); //LSB
}

void saveStatsToEEPROM() {
  #ifndef debug
  int a=0; // place ourselves at beginning of EEPROM
  appendWordToEEPROM(a,   EEPROM_IDENTIFIER);
  appendByteToEEPROM(a,   skiC);
  appendByteToEEPROM(a,   skiT);
  appendByteToEEPROM(a,   staC);
  appendByteToEEPROM(a,   staT);
  appendByteToEEPROM(a,   luC);
  appendByteToEEPROM(a,   luT);
  appendByteToEEPROM(a,   potion);
  appendByteToEEPROM(a,   potionType);
  appendByteToEEPROM(a,   gold);
  appendByteToEEPROM(a,   prov);
  appendWordToEEPROM(a,   par);
  appendWordToEEPROM(a,   lastp);
  appendWordToEEPROM(a,   poffset);
  appendWordToEEPROM(a,   crline);
  appendByteToEEPROM(a,   bmpInPar);
  for(byte i=0; i<EQPSIZE-1; i++) {
    appendByteToEEPROM(a, eqp[i]); }
  #endif
}

void setup() {
  // put your setup code here, to run once:
  gb.begin();
  gb.setFrameRate(10);
  #ifdef debug
  ldverr=FR_OK;
  #else
  Serial.begin(9600); PFFS.begin(10, rx, tx); // initialize petit_fatfs
  ldverr=pf_open(LDV);
  #endif
  restoreStatsFromEEPROM(); // restore saved stats, or initialize stats if no savegame
  myTitleScreen(PGMT(DF_title1), PGMT(DF_title2), DF_logo); // show title screen
  gb.pickRandomSeed();
} // setup()

void loop() {
  // put your main code here, to run repeatedly:
  if(gb.update()) {
    int choice;

    if(!staC && !inAdvSheet) { // no more STAMINA points -> dead
      gb.display.drawBitmap(37, 0, skull);
      gb.display.cursorY=10;
      gb.display.println(PGMT(DF_dead));

    } else if((byte)txt[0]==0xff) { // still in the game, bitmap being displayed
      if(gb.buttons.pressed(BTN_DOWN) || gb.buttons.pressed(BTN_RIGHT)) { // DOWN or PAGE-DOWN
        gb.display.persistence=false;
        crline=1; // set current line as first line of text (after bitmap)
        poffset=529; // place ourselves after 0xFF marker + 528 bytes of image data (84/8 rounded-up x48 lines)
        readBook();
      }

    } else { // text being displayed (book or adventure sheet)
      printBook();

      // handle user input
      if(gb.buttons.pressed(BTN_DOWN)) { // DOWN
        if(nextCaretOnScreen()!=255)
          pcaret=nextCaretOnScreen();
        else if(nxline) { // we can scroll down
          crline++; // increase absolute current line by 1
          poffset+=nxline; // add relative offset to go to next line
          if(inAdvSheet) showAdventureSheet(); else readBook();
          if(pcaret!=255) { if(pcaret<nxline) pcaret=255; else pcaret-=nxline; }
        } // can scroll down
      } // DOWN

      if(gb.buttons.pressed(BTN_UP)) { // UP
        if(prevCaretOnScreen()!=255)
          pcaret=prevCaretOnScreen();
        else if(crline) { // we can scroll up
          if(inAdvSheet) {
            crline--; showAdventureSheet();
          } else if(bmpInPar && crline==1) {
            backToBook(); // reset paragraph offset/crline (displays bitmap)
          } else seekBook(crline-1); // seek book to find offset for absolute current line minus 1
          pcaret=255; // reset caret for next printBook()
        } // can scroll up
      } // UP

      if(gb.buttons.pressed(BTN_RIGHT)) { // PAGE-DOWN
        if(nxpage) {
          pcaret=255; // reset caret for next printBook()
          crline+=NCHRY; // increase absolute current line by (number.of.lines.per.screen)
          poffset+=nxpage; // add relative offset to go to next page
          if(inAdvSheet) showAdventureSheet(); else readBook();
        }
      } // PAGE-DOWN

      if(gb.buttons.pressed(BTN_LEFT)) { // PAGE-UP
        if(crline) { // we can scroll up
          if(inAdvSheet) {
            crline=max(0, crline-NCHRY);
            showAdventureSheet();
          } else if(bmpInPar && crline<=NCHRY) {
            backToBook(); // reset paragraph offset/crline (displays bitmap)
          } else seekBook(crline-NCHRY); // seek book to find offset for absolute current line minus (nb.of.lines.per.scr)
          pcaret=255; // reset caret for next printBook()
        } // can scroll up
      } // PAGE-UP

    } // still in the game

    if(gb.buttons.pressed(BTN_A) && !staC) { initGame(); backToBook(); } // Restart adventure when out of stamina and pressing A

    if(gb.buttons.pressed(BTN_A) && pcaret!=255) { // ACTION
      if(inAdvSheet) { // [IN ADVENTURE SHEET]

        if(txt[pcaret]=='^') { // Remove equipment
          if(gb.menu(DF_menu4, 2)==0) {
            byte lng; char newitm[ITMSIZE]; // Offers ITMSIZE-1 usable characters, from newitm[0] to newitm[ITMSIZE-2] + final '\0' at newitm[ITMSIZE-1]
            for(lng=0; ; lng++) { newitm[lng]=txt[pcaret+lng]; if(txt[pcaret+lng]=='\n') { newitm[++lng]=NULL; break; } }
            char* start=strstr(eqp, newitm); // returns pointer to 1st occurence of newitm[] in eqp[]
            if(!start) { newitm[--lng]=NULL; start=strstr(eqp, newitm); } // case of last item in the list
            for(byte i=0; ; i++) {
              *(start+i)=*(start+lng+i);
              if(*(start+i)==NULL) break;
            }
            showAdventureSheet();
            pcaret=prevCaretOnScreen(); // because we changed text buffer so the existing caret was lost
          }

        } else if(txt[pcaret]=='>') switch(txt[pcaret+1]) {
          case '\001': case '\002': case '\003': // Change stat
            choice=gb.menu(DF_menu1, 3);
            if(choice<0 || choice==2) break; // cancelled by user
            switch(10*choice+(int)txt[pcaret+1]) {
              case 1: changeStat(skiC); break;
              case 2: changeStat(staC); break;
              case 3: changeStat(luC); break;
              case 11: changeStat(skiT); break;
              case 12: changeStat(staT); break;
              case 13: changeStat(luT); break;
            }
            if(skiC>skiT) skiC=skiT;
            if(staC>staT) staC=staT;
            if(luC>luT) luC=luT;
            showAdventureSheet();
            break;

          case '\004': // Gold
            choice=numKeyboard(gold, 3); // current gold as init number, 3 digits max
            if(choice!=-1) { gold=min(200, choice); showAdventureSheet(); }
            break;

          case '\005': // Provisions
            choice=gb.menu(DF_menu3, 3);
            if(choice==0 && prov>0) { // eat a meal
              prov--; staC+=4;
              if(staC>staT) staC=staT;
              gb.popup(DF_meal,15);
              showAdventureSheet();
            } else if(choice==1) { // add provisions
              choice=numKeyboard(prov, 2); // current provisions as init number, 2 digits max i.e. range 0-99
              if(choice!=-1) { prov=choice; showAdventureSheet(); }
            } // else it was cancelled by user
            break;

          case '\006': // Potion
            if(potionType==255) { // Choose potion at beginning of game
              choice=gb.menu(DF_menu5, 4);
              if(choice>=0 && choice <=2) potionType=choice;
            } else if(potion>0 && gb.menu(DF_menu2, 2)==0) { // Drink potion
              potion--;
              if(potionType==1) { staC=staT; gb.popup(DF_po_sta,15); }
              else if(potionType==2) { luT++; luC=luT; gb.popup(DF_po_luc,15); }
              else { skiC=skiT; gb.popup(DF_po_ski,15); }
            }
            showAdventureSheet();
            break;

          case '\011': // Save game
            inAdvSheet=false;
            poffset=oldpoffset; // restore position in book
            crline=oldcrline;
            pcaret=oldpcaret;
            saveStatsToEEPROM();
            readBook();
            gb.popup(DF_savok,15);
            break;

          case '\012': // Back to last paragraph
            if(lastp!=0x7777) {
              choice=par; par=lastp; lastp=choice; // swap par, lastp
              inAdvSheet=false;
              backToBook();
            }
            break;

          case '\013': // Roll 1 die
            showDice(ROLL_ONE_DIE);
            break;

          case '\014': // Roll 2 dice
            showDice(ROLL_BOTH_DICE);
            break;

          case '\015': // Goto paragraph
            choice=numKeyboard(-1, 3); // no init number, 3 digits max
            if(choice!=-1 && choice<=NPAR) {
              lastp=par; // store old paragraph
              par=choice;
              inAdvSheet=false;
              backToBook();
            }
            break;

          case '\016': // Add equipment
            char newitm[ITMSIZE]=""; // Offers ITMSIZE-1 usable characters, from newitm[0] to newitm[ITMSIZE-2] + final '\0' at newitm[ITMSIZE-1]
            alphaKeyboard(newitm, ITMSIZE-1); // -1 to leave room for final '\0'
            if(strlen(newitm) && strlen(eqp)<EQPSIZE-1) {
              if(strlen(eqp)) strcat(eqp, "\n"); strcat(eqp, "^");
              strncat(eqp, newitm, EQPSIZE-1-strlen(eqp)); // strncat instead of strcat to avoir buffer overflow
              eqp[EQPSIZE-1]=NULL; // final '\0' for protection
              showAdventureSheet();
              pcaret=nextCaretOnScreen(); // because we changed text buffer so the existing caret was lost
            }
            break;
        }

      } else { // [IN BOOK]
        if(txt[pcaret]=='>') { // Goto paragraph
          lastp=par; // store old paragraph
          par=char2int(txt+pcaret+1);
          backToBook();
        }
        else if(txt[pcaret]=='<')
          showDice(TEST_YOUR_LUCK);
        else if(txt[pcaret]=='^') {
          showFight(); readBook(); }
      } // [IN BOOK]
    } // ACTION

    if(gb.buttons.pressed(BTN_B)) { // ADVENTURE SHEET <-> BOOK
      inAdvSheet=!inAdvSheet;
      if(inAdvSheet) { // BOOK -> ADVENTURE SHEET
        gb.display.persistence=false; // in case we are displaying a bitmap
        oldpoffset=poffset; // save position in book
        oldcrline=crline;
        oldpcaret=pcaret;
        poffset=crline=0; pcaret=255; // display beginning of adventure sheet
        showAdventureSheet();
      } else { // ADVENTURE SHEET -> BOOK
        poffset=oldpoffset; // restore position in book
        crline=oldcrline;
        pcaret=oldpcaret;
        readBook();
      }
    }

    if(gb.buttons.pressed(BTN_C)) { // TITLE
      gb.display.persistence=false; // in case we are displaying a bitmap
      if(inAdvSheet) { // restore these 2 vars to save book pos (and not AdvSheet pos) in EEPROM
        poffset=oldpoffset; crline=oldcrline; }
      saveStatsToEEPROM();
      gb.popup(DF_savok,15);
      myTitleScreen(PGMT(DF_title1), PGMT(DF_title2), DF_logo);
    }

  } // if (gb.update())
} // loop()

void changeStat(byte &stat) {
  byte newstat = numKeyboard(stat, 2); // current stat as init number, 2 digits max i.e. range 0-99
  if(newstat!=-1) stat=newstat;
}

long Toc(int idx) { // read paragraph offset in Table of Content on SD
  WORD br; char tmp[4]; // Offers 3 usable characters, from eqp[0] to eqp[2] + final '\0' at eqp[3]
  pf_lseek(3*idx); // go in ToC
  pf_read((void*)&tmp[0], 3, &br); // read 3 bytes
  return 0x10000L*(byte)tmp[0]+0x100L*(byte)tmp[1]+(byte)tmp[2];
}

void backToBook() {
  poffset=crline=0;
  pcaret=255;
  readBook();
}

void readBook() { // read current paragraph Toc(par) at poffset and store into txt[] (buffer the size of the screen)
  if(!poffset) bmpInPar=false;
  #ifdef debug // use dummy gamebook
  int psize=min(TXTSIZE-2, strlen_P(dummy)-poffset);
  strncpy_P(&txt[0], dummy+poffset, psize);
  txt[psize+1]=NULL;
  #else        // open real gamebook on SD
  WORD br;
  long crpoffset=Toc(par)+poffset;
  long nxpoffset=Toc(par+1);
  int psize=min(TXTSIZE-2, nxpoffset-crpoffset);
  pf_lseek(crpoffset); // go to correct offset
  pf_read((void*)&txt[0], psize, &br);
  txt[psize+1]=NULL;
  #endif
  if((byte)txt[0]==0xff) { // it's a bitmap!
    if(!poffset) bmpInPar=true;
    gb.display.persistence=true; // stop clearing screen at every gb.update()
    gb.display.clear();
    txt[1]=84; txt[2]=16; // get bitmap data in 3 blocks of 16 lines
    for(byte y=0; y<48; y+=16) {
      #ifdef debug // use dummy gamebook to get bitmap data
      for(byte i=0; i<11*16; i++) { txt[i+3]=pgm_read_byte(&(dummy[i+1+11*y])); }
      #else // read bitmap data from SD
      pf_lseek(crpoffset+1+11*y);
      pf_read((void*)&txt[3], 11*16, &br);
      #endif
      gb.display.drawBitmap(0, y, txt+1); // and draw it!
    }
  } // it's a bitmap!
}

void seekBook(int tgtline) { // seek current paragraph for target line (used when reading up w/ buttons UP/LEFT)
  byte x,y,nxl[11]; // nxl[] index starts at 0 for first new line (=2nd line on screen) up to (NCHRY-2)+1 for next page offset
  poffset=0; crline=0; // start from the top of the paragraph
  if(bmpInPar) poffset=529; // if there is a bitmap in the paragraph, text starts after (at position 529)
  if(tgtline<=0) { readBook(); return; } // very first line wanted -> we're done (+can't seek before beginning of paragraph)
  do {
    readBook();
    x=1; y=1;
    for(byte i=0; ; i++) { // simulate printing txt[] on screen + find all offsets to new lines (nxl[])
      char c=txt[i];
      if(c==NULL || y>NCHRY) break; // screen has been filled
      x++; if(x>NCHRX || c=='\n') { y++; x=1; nxl[y-2]=i+1; }
    } // for(byte i=0; ; i++)
      if(tgtline<=crline+NCHRY) { // target line is somewhere in current page
        poffset+=nxl[tgtline-crline-1];
        crline=tgtline; // found!
        readBook();
    } else if(crline!=tgtline) { // target line is in another page -> proceed to load next one
          crline+=NCHRY; // go down 1 page
          poffset+=nxl[NCHRY-1]; // prepare offset for next iteration of while() loop
      }
  } while(crline!=tgtline);
}

void printBook() { // print txt[] on screen (current paragraph at poffset) + set next line (nxline) and next page (nxpage) offsets
  nxline=0; nxpage=0;
  for(byte i=0; ; i++) {
    char c=txt[i];
    if(c==NULL) { nxline=0; return; }
    if(gb.display.cursorY>=LCDHEIGHT) { nxpage=i; return; }
    if(IS_SPECIAL_CHARACTER(c)) {
      if(pcaret==255) pcaret=i;
      printCaret(pcaret==i && (gb.frameCount/4)%2);
      if(inAdvSheet && c=='>') i++; // skip special code following caret in adventure sheet
    } else gb.display.print(c);
    if(!nxline && gb.display.cursorY>0) nxline=i+1;
  }
}

void printCaret(boolean condition) {
  if(condition && (gb.frameCount/4)%2) gb.display.print(F("\020")); else gb.display.print(F("\032"));
}

byte prevCaretOnScreen() {
  if(pcaret==255) return 255;
  for(byte i=pcaret-1; ; i--) {
    if(i==255) break;
    char c=txt[i];
    if(IS_SPECIAL_CHARACTER(c)) return i;
  }
  return 255;
}

byte nextCaretOnScreen() {
  if(pcaret==255) return 255;
  for(byte i=pcaret+1; ; i++) {
    char c=txt[i];
    if(i==nxpage || c==NULL) break;
    if(IS_SPECIAL_CHARACTER(c)) return i;
  }
  return 255;
}

