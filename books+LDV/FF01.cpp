//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// THIS IS THE PART TO CHANGE WHEN YOU BUILD THE APP WITH A DIFFERENT GAMEBOOK:
#define NPAR 401 // Number of paragraphs in this Gamebook
#define LDV "FF01.LDV"
#define CANCEL "Cancel"
PROGMEM const char DF_title1[]="FIGHTING FANTASY 01";
PROGMEM const char DF_title2[]="The Warlock Of Firetop Mountain";
PROGMEM const char DF_eqp_ini[]="^sword\n^leather armour\n^rucksack\n^lantern";
PROGMEM const char keyboard[]="qwertyuiopasdfghjkl<zxcvbnm_\030"; // Used by alphaKeyboard.ino
// Adventure sheet constants
PROGMEM const char DF_adv[]="[ADVENTURE SHEET]";
PROGMEM const char DF_ski[]="Skill:";
PROGMEM const char DF_sta[]="Stamina:";
PROGMEM const char DF_luc[]="Luck:";
PROGMEM const char DF_gold1[]="Gold:";
PROGMEM const char DF_gold2[]=" gp";
PROGMEM const char DF_prov[]="Provisions:";
PROGMEM const char DF_meal[]="Stamina +4";
PROGMEM const char DF_pot255[]="Choose ";
PROGMEM const char DF_potion[]="Potion ";
PROGMEM const char DF_pot0[]="of Skill";
PROGMEM const char DF_pot1[]="of Strength";
PROGMEM const char DF_pot2[]="of Fortune";
PROGMEM const char DF_po_ski[]="Skill @initial lvl!";
PROGMEM const char DF_po_sta[]="Stamina @init. lvl!";
PROGMEM const char DF_po_luc[]="Luck @initial lvl+1";
PROGMEM const char DF_axn[]="[ACTIONS]";
PROGMEM const char DF_save[]="Save game";
PROGMEM const char DF_savok[]="Game saved";
PROGMEM const char DF_back[]="Previous paragraph";
PROGMEM const char DF_roll1[]="Roll one die";
PROGMEM const char DF_roll2[]="Roll both dice";
PROGMEM const char DF_goto[]="Turn to...";
PROGMEM const char DF_about[]="About";
PROGMEM const char DF_eqp[]="[EQUIPMENT]";
PROGMEM const char DF_add[]="Add an equipment";
// Game constants
PROGMEM const char DF_tyl[]="Test your Luck";
PROGMEM const char DF_lucky[]="LUCKY";
PROGMEM const char DF_unlucky[]="UNLUCKY";
PROGMEM const char DF_fight[]="Fight!";
PROGMEM const char DF_flee[]="Escape battle";
PROGMEM const char DF_you[]="YOU";
PROGMEM const char DF_fski[]="ski";
PROGMEM const char DF_fsta[]="STA:";
PROGMEM const char DF_dead[]="You are dead...\nPress \025 to\nrestart the adventure";
// Menus
PROGMEM const char DF_m1s1[]="Change current stat";
PROGMEM const char DF_m1s2[]="Change initial level";
PROGMEM const char DF_m2s1[]="Drink one measure";
PROGMEM const char DF_m3s1[]="Eat one meal";
PROGMEM const char DF_m3s2[]="Add provision(s)";
PROGMEM const char DF_m4s1[]="Remove equipment";
PROGMEM const char DF_m5s1[]="Potion of Skill";
PROGMEM const char DF_m5s2[]="Potion of Strength";
PROGMEM const char DF_m5s3[]="Potion of Fortune";
// About
PROGMEM const char DF_engine[]="Gamebookuino engine";
PROGMEM const char DF_pcsoft[]="PC software LDV maker";
#ifdef debug // declare dummy gamebook
PROGMEM const char dummy[]=
"Before embarking on\nyour adventure, you\nmust first determine\nyour own strengths\nand weaknesses. You\nhave in your\npossession a sword\nand a shield\ntogether with a\nrucksack containing\nprovisions (food and\ndrink) for the trip.\nYou have been\npreparing for your\nquest by training\nyourself in\nswordplay and\nexercising\nvigorously to build\nup your stamina.\nPress \026 to open\nthe adventure sheet.\nChange paragraph:\nturn to >2 or to\n>123.\n<Test your Luck!\nFight:\n^Enemy number 1 s06/S05\n";
#endif
// END OF THE PART TO CHANGE WHEN YOU BUILD THE APP WITH A DIFFERENT GAMEBOOK
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
