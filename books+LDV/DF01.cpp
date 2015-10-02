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
"Avant de vous lancer\ndans cette aventure,\nil vous faut d'abord\ndeterminer vos\npropres forces et\nfaiblesses. Vous\navez en votre\npossession une epee\net un bouclier,\nainsi qu'un sac a\ndos contenant des\nprovisions\n(nourriture et\nboissons) pour le\nvoyage. Afin de vous\npreparer a votre\nquete, vous vous\netes entraine au\nmaniement de l'epee\net vous vous etes\nexerce avec\nacharnement a\naccroitre votre\nendurance.\nPressez \026 pour\nouvrir la feuille\nd'aventure.\nTest paragraphe:\nallez au >2 ou au\n>123.\n<Tentez votre Chance!\nCombattez:\n^Ennemi numero 1 H06/E05\n";
#endif
// END OF THE PART TO CHANGE WHEN YOU BUILD THE APP WITH A DIFFERENT GAMEBOOK
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
