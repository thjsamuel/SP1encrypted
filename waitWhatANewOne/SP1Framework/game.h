#ifndef _GAME_H
#define _GAME_H

#include "Framework\timer.h"

extern StopWatch g_timer;
extern bool g_quitGame;

enum Keys
{
    K_UP,
    K_DOWN,
    K_LEFT,
    K_RIGHT,
    K_ESCAPE,
    K_OPTIONS,
    K_W,
    K_A,
    K_S,
    K_D,
    K_PAUSE,
    K_SPACE,
    K_1,
    K_2,
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

enum Process
{
    PLAY = 1,
    OPTIONS,
    EXIT,
    MAX_PROCESS
};

void init();                // initialize your variables, allocate memory, etc
void getInput();            // get input from player
void update(double dt);     // update the game and the state of the game
void render();              // renders the current state of the game to the console
void shutdown();            // do clean up, free memory
void displayOptions ( void );   // Samuel: displays option screen
int menu       (  int s = 1 );       // Samuel: Menu
void displayMenu( void );       // Samuel: displays menu screen and contains menu cout code
void displayPlay( void );
void displayExit( void );
void userInput  ( Sequence& S ); // Samuel: gets user input
void printMaze();
void runMaze(int row, int col); // finds solution to maze
void splashScreen();
void renderAfter();

void moveCharacter();       // moves the character, collision detection, physics, etc
void processUserInput();    // checks if you should change states or do something else with the game, e.g. pause, exit
void clearScreen();         // clears the current screen and draw from scratch 
void menuSelect();          // Samuel: The menu;
void renderMap();           // renders the map to the buffer first
void renderCharacter();     // renders the character into the buffer
void renderFramerate();     // renders debug information, frame rate, elapsed time, etc
void renderToScreen();      // dump the contents of the buffer to the screen, one frame worth of game

#endif // _GAME_H