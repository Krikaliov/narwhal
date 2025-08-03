#ifndef MAIN_H
#define MAIN_H 1

/***************
 * @name NARVWHAL
 * @author Krikal
 ***************/

#define TXT_NOIR 0x0
#define TXT_BLEU_3 0x1
#define TXT_VERT_3 0x2
#define TXT_BLEU_GRIS 0x3.
#define TXT_MARRON 0x4
#define TXT_VIOLET 0x5
#define TXT_KAKI 0x6
#define TXT_GRIS_1 0x7
#define TXT_GRIS_2 0x8
#define TXT_BLEU_1 0x9
#define TXT_VERT_1 0xA
#define TXT_CYAN 0xB
#define TXT_ROUGE 0xC
#define TXT_ROSE 0xD
#define TXT_JAUNE 0xE
#define TXT_BLANC 0xF

/*Ç --> 128
é --> 130
â --> 131
à --> 133
å --> 134
ç --> 135
ê --> 136
è --> 138
É --> 144
æ --> 145
ô --> 147
ö --> 148
û --> 150
ù --> 151
*/

/* Traîtement de fichiers :
le début du fichier :ios::beg;
la fin du fichier :ios::end;
la position actuelle :ios::cur.
*/

#include <fstream>
#include <cmath>
#include <ctime>

#include <windows.h>
#include <conio.h>
#include <process.h>

#include <SFML/Network.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

#define WIN_WIDTH 1366 //(px)
#define WIN_HEIGHT 768 //(px)
#define WIN_TICK 0.016667f //(s)

#define WIN_H 672 //(px)
#define WIN_W 1008 //(px)

#define CURSOR_NORMAL {0, 0, 25, 26} //(px^4)
#define CURSOR_ONTEXT {25, 0, 10, 26} //(px^4)

#define LAUNCHER_NORMAL 0 //(qtt)
#define LAUNCHER_TYPING 1 //(qtt)

#define PSEUDO_MAXSIZE 25 //(qtt)

#define MAX_FIREBALL 64
#define MAX_MONSTER 32
#define MAX_BONUS 8

#define MONSTER_INIT_SPEED 0.01f //(rad/fps)
#define MONSTER_ACCELERATION 0.0002f //(rad/fps^2)

#define FIREBALL_SPEED 25 //(px/fps)

#define MAX_EPOLAR 4
#define EPOLAR_ACCELERATION 0.48f //(px/fps^2)
#define EPOLAR_SPEED_ANIM 100 //(Hz.10^3)
#define EPOLAR_INIT_SPEED 3.0f //(px/fps)
#define EPOLAR_INIT_HEALTH 25
#define EPOLAR_GAIN_HEALTH 10
#define EPOLAR_PHASES 5
#define EPOLAR_PHASETIME_0 1000 //(ms)
#define EPOLAR_PHASETIME_1 3000 //(ms)
#define EPOLAR_PHASETIME_2 2000 //(ms)
#define EPOLAR_PHASETIME_3 3000 //(ms)
#define EPOLAR_PHASETIME_4 2000 //(ms)
#define EPOLAR_PHASETIME_ACC 0.92 //(%)

#define NORMAL_SPEED_ANIM 100 //(Hz.10^3)
#define HIGH_SPEED_ANIM 30 //(Hz.10^3)
#define FBEFFECT_DURATION 5000 //(ms)

#define MAX_BACKGROUND_ADVANCEMENT 1300-768 //(px)
// Background rocks : 0, 70, 146, 223, 300, 380, 458
#define MAX_ROCKS 64

#define MONSTER_SPAWN_NBR 8

#define MAX_SOUNDS 64

#define HEALTHBAR_SIZE 179 //(px)
#define HEALTHBAR_HEIGHT 6 //(px)

#define BUT_RETOUR_JEU 0
#define BUT_TOUCHES 1
#define BUT_MUSIQUE 2
#define BUT_RETOUR_MAIN 3
#define BUT_KEY_FORWARD 4
#define BUT_KEY_BACKWARD 5
#define BUT_KEY_GODOWN 6
#define BUT_KEY_GOUP 7
#define BUT_KEY_FIREBALL 8
#define BUT_KEY_ACCERELATE 9
#define BUT_KEY_FPSDISP 10
#define BUT_MUSIC_UP 11
#define BUT_MUSIC_DOWN 12
#define BUT_SOUND_UP 13
#define BUT_SOUND_DOWN 14
#define BUT_MUSIC_CHOICE 15
#define BUT_MUSIC_REPEAT 16
#define NBR_BUTTONS 17

#define JAUGE_SON 0
#define JAUGE_MUSIQUE 1
#define NBR_JAUGES 2

#define BUT_MOUSEON 76
#define BUT_MOUSEOUT 0

#define MENU_MAIN 0
#define MENU_MUSIQUE 1
#define MENU_TOUCHES 2
#define MENU_TOUCHES_FORWARD 3
#define MENU_TOUCHES_BACKWARD 4
#define MENU_TOUCHES_FIREBALL 5
#define MENU_TOUCHES_FPSDISP 6
#define MENU_TOUCHES_ACCELERATE 7
#define MENU_TOUCHES_GOUP 8
#define MENU_TOUCHES_GODOWN 9

#define KEYB_FORWARD 0
#define KEYB_BACKWARD 1
#define KEYB_GODOWN 2
#define KEYB_GOUP 3
#define KEYB_FIREBALL 4
#define KEYB_ACCELERATE 5
#define KEYB_FPSDISP 6
#define NBR_KEYS 7

#endif // MAIN_H
