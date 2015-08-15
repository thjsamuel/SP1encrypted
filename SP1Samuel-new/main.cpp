// This is the main file to hold everthing together

// Samuel: had no choice but to include iostream
#include <iostream>
// test remove the stream include when done
#include <fstream>
//another trash below
#include <string>
#include "Framework\timer.h"
#include "game.h"
// Samuel: Gonna put sum std::cout and std::cin in this weed farm
using std::cout;
using std::endl;

CStopWatch g_Timer;							// Timer function to keep track of time and the frame rate
bool g_bQuitGame = false;					// Set to true if you want to quit the game
const unsigned char gc_ucFPS = 5;				// FPS of this game
const unsigned int gc_uFrameTime = 1000 / gc_ucFPS;	// time for each frame

//main loop declaration
void mainLoop( void );

// TODO:
// Bug in waitUnitil. it waits for the time from getElapsedTime to waitUntil, but should be insignificant.

// main function - starting function
int main( void )
{
	init();      // initialize your variables
    //menu();
    std::ifstream inData;
    //std::ofstream outData;
    std::string data;

    inData.open("SP1Maze.txt");
    while (!inData.eof())
    {
        std::getline(inData, data);
        cout << data << endl;
    }
    inData.close();
    int z = menu();
    if (z != 2 && z != 3)
    mainLoop();  // main loop
    shutdown();  // do clean up, if any. frees memory.
	
	return 0;
}

//--------------------------------------------------------------
// Purpose	: This main loop calls functions to get input, 
//			update and render the game at a specific frame rate
// Input	: void
// Output	: void
//--------------------------------------------------------------
void mainLoop( void )
{
    g_Timer.startTimer();    // Start timer to calculate how long it takes to render this frame
    while (!g_bQuitGame)      // run this loop until user wants to quit 
	{        
        getInput();                         // get keyboard input
        update(g_Timer.getElapsedTime());   // update the game
        render();                           // render the graphics output to screen
        g_Timer.waitUntil(gc_uFrameTime);   // Frame rate limiter. Limits each frame to a specified time in ms.   
        //displayOptions();
	}    
}
