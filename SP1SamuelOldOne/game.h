#ifndef _GAME_H
#define _GAME_H

#include "Framework\timer.h"

extern CStopWatch g_swTimer;
extern bool g_bQuitGame;

// Enumeration to store the control keys that your game will have
enum EKEYS
{
	K_UP,
    K_DOWN,
    K_LEFT,
    K_RIGHT,
    K_ESCAPE,
    K_OPTIONS,
	K_COUNT
};

enum Sequence
{
    Play = 1,
    Options = 2,
    // DLC,
    Exit = 3,
    MAX_SEQUENCE,
};

void init		( void );       // initialize your variables, allocate memory, etc
void getInput	( void );		// get input from player
void update		( double dt );	// update the game and the state of the game
void render		( void );       // renders the current state of the game to the console
void shutdown	( void );       // do clean up, free memory
void displayOptions ( void );   // Samuel: displays option screen
int menu       (  int s = 1 );       // Samuel: Menu
void displayMenu( void );       // Samuel: displays menu screen and contains menu cout code
void displayPlay( void );
void displayExit( void );
void userInput  ( Sequence& S ); // Samuel: gets user input

#endif // _GAME_H