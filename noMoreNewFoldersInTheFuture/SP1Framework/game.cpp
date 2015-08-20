// This is the main file for the game logic and function
//
//
#include "game.h"
#include "Framework\console.h"
#include <iostream>
#include <iomanip>
#include <sstream>

using std::cout;
using std::endl;
using std::cin;
using std::string;

#define VK_W	0x057
#define VK_A	0x041
#define VK_S	0x053
#define VK_D	0x044
#define VK_SPACE 0x20
#define VK_1    0x31
#define VK_2    0x32

double  g_dElapsedTime;
double  g_dDeltaTime;
bool    g_abKeyPressed[K_COUNT];

// Game specific variables here
SGameChar   g_sChar;
EGAMESTATES g_eGameState = S_SPLASHSCREEN;
double  g_dBounceTime; // this is to prevent key bouncing, so we won't trigger keypresses more than once
COORD	g_cCharLocation;
COORD	g_cConsoleSize;
// Samuel: Gonna create my own cCharLocation
COORD   g_cCharLocation2;
// Samuel: Gonna create maze coord, hopefully it works out
COORD   g_cMaze;
int col = 1;
int row = 2;
const int ROWMAX = 11; // 32
const int COLMAX = 16; // 28
// Samuel: A coord for the 2d array
COORD g_cLocation;
char playerName[20];
COORD charLocation;
COORD menuScreen;
//Process *chi = new Process;
int chic;
//char g_cHealthBar[5] = {char(3), char(3), char(3), char(3), char(3)};
//string g_sHealthBar = "";
int i = 5;
char heart = char(3);
int chance = 0;
int unit = 0;
bool spawn = false;

char maze[ROWMAX][COLMAX] =
{
{'B','B','B','B','B','B','B','B','B','B','B','B',' B','B','B','B'},
{'B','M',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','B'},
{'B','B','B','B','B','B','B','B','B','B',' ','B','B','B',' ','B'},
{'B','W',' ',' ',' ',' ',' ',' ',' ','B',' ','B',' ','B',' ','B'},
{'B','B','B','B','B','B','B','B',' ','B','B','B',' ','B',' ','B'},
{'B',' ',' ',' ',' ',' ',' ','B',' ',' ',' ',' ',' ','B',' ','B'},
{'B','B','B','B','B','B','B','B',' ','B','B','B','B','B',' ','B'},
{'B',' ',' ',' ','B',' ',' ',' ',' ','B',' ',' ',' ',' ',' ','B'},
{'B',' ',' ',' ',' ',' ',' ',' ','B','B','B','B',' ','B','B','B'},
{'B',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ','B'},
{'B','B','B','B','B','B','B','B','B','B','B','B',' B','B','B','B'}
};

// Samuel: gonna make a printMaze function

void printMaze()
{
	for(int row = 0; row < ROWMAX; row++)
	{
		for(int col=0; col < COLMAX; col++)
        {
			cout << maze[row][col];
            //if (maze[row][col] == 'B')

        }
		cout << "\n";
	}
}

void runMaze(int row, int col) // finds solution to maze
{
	if( (row>0 && row<ROWMAX) && (col>0 && col<COLMAX)) 
	{
		if( maze[row][col] == 'W' )
			return;

		if( maze[row][col] == ' ') 
		{
			maze[row][col]='*';
			runMaze(row, col+1);
			runMaze(row, col-1);
			runMaze(row-1, col);
			runMaze(row+1, col);
		}
	}
}

const int MAXROWS = 80;
const int MAXCOLUMNS = 23;
char levelMatrix[MAXROWS][MAXCOLUMNS] = 
{	
	{35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35},//1 col^
	{35,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,35},//2
	{35,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,35},//3
	{32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,35},//4
	{32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,35},//5
	{35,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,35},//6
	{35,32,32,32,32,32,32,32,32,65,35,35,35,35,35,35,35,35,35,35,35,35,35},//7
    {35,32,32,32,32,32,32,32,32,65,35,35,35,35,35,35,35,35,35,35,35,35,35},//8
	{35,32,32,32,32,32,32,32,32,65,32,32,32,32,32,32,32,32,32,32,32,32,35},//9
	{35,32,32,32,32,32,32,32,32,65/**/,32,32,32,32,32,32,32,32,32,32,32,32,35},//10
	{35,32,32,32,32,32,32,32,32,65,32,32,32,32,32,32,32,32,32,32,32,32,35},//11
	{35,32,32,32,32,32,32,32,32,65,32,32,32,32,32,32,32,32,32,32,32,32,35},//12
	{35,32,32,32,32,32,32,32,32,65,32,35,32,32,32,32,32,32,32,32,32,32,35},//13
	{35,32,32,32,32,32,32,32,32,65,35,35,35,35/**/,35,35/**/,35,35,35,35,35,35,35},//14
	{35,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,35},//15
	{35,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,35},//16
	{35,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,35},//17
	{35,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,35},//18
	{35,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,35},//19
	{35,32,32,32,32,32,32,32,32,97,35,35,35,35,35,35,35,35,35,35,35,35,35},//20
	{35,32,32,32,32,32,32,32,32,97,32,32,32,32,32,32,32,32,32,32,32,32,35},//21
	{35,32,32,32,32,32,32,32,32,97,32,32,32,32,32,32,32,32,32,32,32,32,35},//22
	{35,32,32,32,32,32,32,32,32,97,32,32,32,32,32,32,32,32,32,32,32,32,35},//23
	{35,32,32,32,32,32,32,32,32,97,32,32,32,32,32,32,32,32,32,32,32,32,35},//24
	{35,32,32,32,32,32,32,32,32,97,32,32,32,32,32,32,32,32,32,32,32,32,35},//25
	{35,32,32,32,32,32,32,32,32,97,32,32,32,32,32,32,32,32,32,32,32,32,35},//26
	{35,32,32,32,32,32,32,32,32,97,32,32,32,32,32,32,32,32,32,32,32,32,35},//27
	{35,32,32,32,32,32,32,32,32,97,35,35,35,35,35,35,35,32,32,32,32,32,35},//28
	{35,32,32,32,32,32,32,32,32,32,32,32,32,32,32,35,35,32,32,32,32,32,35},//29
	{35,32,32,32,32,32,32,32,32,32,32,32,32,32,32,35,35,32,32,32,32,32,35},//30
	{35,32,32,32,32,32,32,32,32,32,32,32,32,32,32,35,35,32,32,32,32,32,35},//31
	{35,32,32,32,32,32,32,32,32,32,32,32,32,32,32,35,35,32,32,32,32,32,35},//32
	{35,32,32,32,32,32,32,32,32,32,32,32,32,32,32,35,35,32,32,32,32,32,35},//33
	{35,32,32,32,32,32,32,32,32,32,32,32,32,32,32,35,35,32,32,32,32,32,35},//34
	{35,32,32,32,32,32,32,32,32,32,32,32,32,32,32,35,35,32,32,32,32,32,35},//35
	{35,35,35,35,35,35,35,35,32,32,32,32,32,32,32,35,35,35,35,35,35,35,35},//36
	{35,35,35,35,35,35,35,35,32,32,32,32,32,32,32,35,35,35,35,35,35,35,35},//37
	{35,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,35,35,32,32,32,35},//38
	{35,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,35,35,32,32,32,35},//39
	{35,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,35,35,32,32,32,35},//40
	{35,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,35,35,32,32,32,35},//41
	{35,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,35,35,32,32,32,35},//42
	{35,32,32,32,32,32,32,35,35,32,32,32,32,32,32,32,32,35,35,32,32,32,35},//43
	{35,32,32,32,32,32,32,35,35,32,32,32,32,32,32,32,32,35,35,32,32,32,35},//44
	{35,32,32,32,32,32,32,35,35,32,32,32,32,32,32,32,32,35,35,32,32,32,35},//45
	{35,32,32,32,32,32,32,35,35,32,32,32,32,32,32,32,32,35,35,32,32,32,35},//46
	{35,32,32,32,32,32,32,35,35,32,32,32,32,32,32,32,32,35,35,32,32,32,35},//47
	{35,32,32,32,32,32,32,35,35,32,32,32,32,32,32,32,32,35,35,32,32,32,35},//48
	{35,32,32,32,32,32,32,35,35,32,32,32,32,32,32,32,32,35,35,32,32,32,35},//49
	{35,32,32,32,32,32,32,35,35,32,32,32,32,32,32,32,32,35,35,32,32,32,35},//50
	{35,32,32,32,32,32,32,35,35,32,32,32,32,32,32,32,32,35,35,32,32,32,35},//51
	{35,32,32,32,32,32,32,35,35,32,32,32,32,32,32,32,32,35,35,32,32,32,35},//52
	{35,32,32,32,32,32,32,35,35,32,32,32,32,32,32,32,32,35,35,32,32,32,35},//53
	{35,32,32,32,32,32,32,35,35,32,32,32,32,32,32,32,32,32,32,32,32,32,35},//54
	{35,32,32,32,32,32,32,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35},//55
	{35,32,32,32,32,32,32,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35},//56
	{35,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,35},//57
	{35,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,35},//58
	{35,32,32,32,32,35,35,35,35,35,35,35,35,35,32,32,32,32,32,32,32,32,35},//59
	{35,32,32,32,32,35,35,32,32,32,32,32,35,35,32,32,32,32,32,32,32,32,35},//60
	{35,32,32,32,32,35,35,32,32,32,32,32,35,35,32,32,32,32,32,32,32,32,35},//61
	{35,32,32,32,32,35,35,32,32,32,32,32,35,35,32,32,32,32,32,32,32,32,35},//62
    {35,32,32,32,32,35,35,32,32,32,32,32,35,35,32,32,32,32,32,32,32,32,35},//63
	{35,32,32,32,32,35,35,32,32,32,32,32,35,35,32,32,32,32,32,32,32,32,35},//64
	{35,32,32,32,32,35,35,32,32,32,32,32,35,35,32,32,32,32,32,32,32,32,35},//65
	{35,32,32,32,32,35,35,32,32,32,32,32,35,35,32,32,32,32,32,32,32,32,35},//66
	{35,32,32,32,32,32,32,32,32,32,32,32,35,35,32,32,32,32,32,32,32,32,35},//67
	{35,32,32,32,32,32,32,32,32,32,32,32,35,35,32,32,32,32,32,32,32,32,35},//68
	{35,32,32,32,32,32,32,32,32,32,32,32,35,35,32,32,32,32,32,32,32,32,35},//69
	{35,35,35,35,35,35,35,35,35,35,35,35,35,35,32,32,32,32,32,32,32,32,35},//70
	{35,35,35,35,35,35,35,35,35,35,35,35,35,35,32,32,32,32,32,32,32,32,35},//71
	{35,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,35},//72
	{35,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,35},//73
	{35,32,32,32,32,32,32,32,32,32,32,32,35,35,35,35,35,35,35,35,35,35,35},//74
	{35,32,32,32,32,32,32,32,32,32,32,32,35,35,35,35,35,35,35,35,35,35,35},//75
	{35,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,35},//76
	{35,32,32,32,32,32,32,32,32,32,32,32,33,33,32,32,32,32,32,32,32,32,35},//77
	{35,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,35},//78
	{35,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,35},//79
	{35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35}//80
};

//Ghost1 Start postion is [14][16]
int ghostOneX = 70, ghostOneY = 16; // I is Y, J is X
char ghostCurrent = levelMatrix[ghostOneX][ghostOneY];
char ghostTrail = ghostCurrent[&ghostOneX][&ghostOneY];
//bool isWot = false;

// Console object
Console g_Console(80, 25, "SP1 Framework");

//--------------------------------------------------------------
// Purpose  : Initialisation function
//            Initialize variables, allocate memory, load data from file, etc. 
//            This is called once before entering into your main loop
// Input    : void
// Output   : void
//--------------------------------------------------------------
void init( void )
{
    // Set precision for floating point output
    g_dElapsedTime = 0.0;
    g_dBounceTime = 0.0;

    // sets the initial state for the game
    g_eGameState = S_SPLASHSCREEN;

    g_sChar.m_cLocation.X = 1;//g_Console.getConsoleSize().X / 2;
    g_sChar.m_cLocation.Y = 1;//g_Console.getConsoleSize().Y / 2;
    g_cCharLocation2.X = g_Console.getConsoleSize().X / 3;
    g_cCharLocation2.Y = g_Console.getConsoleSize().Y / 3;
    g_sChar.m_bActive = true;
    // sets the width, height and the font name to use in the console
    g_Console.setConsoleFont(0, 16, L"JESUS");
}

//--------------------------------------------------------------
// Purpose  : Reset before exiting the program
//            Do your clean up of memory here
//            This is called once just before the game exits
// Input    : Void
// Output   : void
//--------------------------------------------------------------
void shutdown( void )
{
    // Reset to white text on black background
    colour(FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);

    g_Console.clearBuffer();
}

//--------------------------------------------------------------
// Purpose  : Getting all the key press states
//            This function checks if any key had been pressed since the last time we checked
//            If a key is pressed, the value for that particular key will be true
//
//            Add more keys to the enum in game.h if you need to detect more keys
//            To get other VK key defines, right click on the VK define (e.g. VK_UP) and choose "Go To Definition" 
//            For Alphanumeric keys, the values are their ascii values (uppercase).
// Input    : Void
// Output   : void
//--------------------------------------------------------------
void getInput( void )
{    
    g_abKeyPressed[K_UP]     = isKeyPressed(VK_UP);
    g_abKeyPressed[K_DOWN]   = isKeyPressed(VK_DOWN);
    g_abKeyPressed[K_LEFT]   = isKeyPressed(VK_LEFT);
    g_abKeyPressed[K_RIGHT]  = isKeyPressed(VK_RIGHT);
    g_abKeyPressed[K_SPACE]  = isKeyPressed(VK_SPACE);
    g_abKeyPressed[K_ESCAPE] = isKeyPressed(VK_ESCAPE);
    g_abKeyPressed[K_W] = isKeyPressed(VK_W);
    g_abKeyPressed[K_A] = isKeyPressed(VK_A);
    g_abKeyPressed[K_S] = isKeyPressed(VK_S);
    g_abKeyPressed[K_D] = isKeyPressed(VK_D);
    g_abKeyPressed[K_SPACE] = isKeyPressed(VK_SPACE);
    g_abKeyPressed[K_1] = isKeyPressed(VK_1);
    g_abKeyPressed[K_2] = isKeyPressed(VK_2);
}

//--------------------------------------------------------------
// Purpose  : Update function
//            This is the update function
//            double dt - This is the amount of time in seconds since the previous call was made
//
//            Game logic should be done here.
//            Such as collision checks, determining the position of your game characters, status updates, etc
//            If there are any calls to write to the console here, then you are doing it wrong.
//
//            If your game has multiple states, you should determine the current state, and call the relevant function here.
//
// Input    : dt = deltatime
// Output   : void
//--------------------------------------------------------------
void update(double dt)
{
    // get the delta time
    g_dElapsedTime += dt;
    g_dDeltaTime = dt;

    switch (g_eGameState)
    {
        case S_SPLASHSCREEN : splashScreenWait(); // game logic for the splash screen
            break;
        case S_GAME: gameplay(); // gameplay logic when we are in the game
            break;
    }
      
      randSpawn();
}
//--------------------------------------------------------------
// Purpose  : Render function is to update the console screen
//            At this point, you should know exactly what to draw onto the screen.
//            Just draw it!
//            To get an idea of the values for colours, look at console.h and the URL listed there
// Input    : void
// Output   : void
//--------------------------------------------------------------
void render()
{
    clearScreen();      // clears the current screen and draw from scratch 
    switch (g_eGameState)
    {
        case S_SPLASHSCREEN: renderSplashScreen();
            break;
        case S_GAME: renderGame();
            break;
    }
    renderFramerate();  // renders debug information, frame rate, elapsed time, etc
    renderToScreen();   // dump the contents of the buffer to the screen, one frame worth of game
}

void splashScreenWait()    // waits for time to pass in splash screen
{
    if (g_dElapsedTime > 3.0) // wait for 3 seconds to switch to game mode, else do nothing
        g_eGameState = S_GAME;
}

void gameplay()            // gameplay logic
{
    processUserInput(); // checks if you should change states or do something else with the game, e.g. pause, exit
    moveCharacter();    // moves the character, collision detection, physics, etc
                        // sound can be played here too.
    updateTraps();
}

void moveCharacter()
{
    bool bSomethingHappened = false;
    if (g_dBounceTime > g_dElapsedTime)
        return;

    // Updating the location of the character based on the key press
    // providing a beep sound whenver we shift the character
    if (g_abKeyPressed[K_UP] && g_sChar.m_cLocation.Y > 0)
    {
        if (levelMatrix[g_sChar.m_cLocation.X][(g_sChar.m_cLocation.Y - 1)] != 35)//if there is no "35" wall above player 1
        {
            //Beep(1440, 30);
            g_sChar.m_cLocation.Y--;
            bSomethingHappened = true;
            Ghost();
        }
    }
    if (g_abKeyPressed[K_LEFT] && g_sChar.m_cLocation.X > 0)
    {
        if (levelMatrix[g_sChar.m_cLocation.X - 1][(g_sChar.m_cLocation.Y)] != 35)
        {
            //Beep(1440, 30);
            g_sChar.m_cLocation.X--;
            bSomethingHappened = true;
            Ghost();
        }
    }
    if (g_abKeyPressed[K_DOWN] && g_sChar.m_cLocation.Y < g_Console.getConsoleSize().Y - 1)
    {
        if (levelMatrix[g_sChar.m_cLocation.X][(g_sChar.m_cLocation.Y + 1)] != 35)
        {
            //Beep(1440, 30);
            g_sChar.m_cLocation.Y++;
            bSomethingHappened = true;
            Ghost();
        }
    }
    if (g_abKeyPressed[K_RIGHT] && g_sChar.m_cLocation.X < g_Console.getConsoleSize().X - 1)
    {
        if (levelMatrix[g_sChar.m_cLocation.X + 1][(g_sChar.m_cLocation.Y)] != 35)
        {
            //Beep(1440, 30);
            g_sChar.m_cLocation.X++;
            bSomethingHappened = true;
            Ghost();
        }
    }
    // movement of the other char
    if (g_abKeyPressed[K_W] && g_cCharLocation2.Y > 0)
    {
        //Beep(1440, 30);
        //for (size_t i = 0; i < 2; ++i)
        g_cCharLocation2.Y--; 
    }
    if (g_abKeyPressed[K_A] && g_cCharLocation2.X > 0)
    {
        //Beep(1440, 30);
        g_cCharLocation2.X--; 
    }
    if (g_abKeyPressed[K_S] && g_cCharLocation2.Y < g_Console.getConsoleSize().Y - 1)
    {
        //Beep(1440, 30);
        g_cCharLocation2.Y++; 
    }
    if (g_abKeyPressed[K_D] && g_cCharLocation2.X < g_Console.getConsoleSize().X - 1)
    {
        //Beep(1440, 30);
        g_cCharLocation2.X++; 
    }

    if (g_abKeyPressed[K_SPACE])
    {
        g_sChar.m_bActive = !g_sChar.m_bActive;
        bSomethingHappened = true;
    }

    if (bSomethingHappened)
    {
        // set the bounce time to some time in the future to prevent accidental triggers
        g_dBounceTime = g_dElapsedTime + 0.125; // 125ms should be enough
    }
}
void processUserInput()
{
    // quits the game if player hits the escape key
    if (g_abKeyPressed[K_ESCAPE])
        g_bQuitGame = true; 
    if (g_abKeyPressed[K_1])
        chic = 1;
        //*chi = PLAY;
    if (g_abKeyPressed[K_2])
    {
        //*chi = OPTIONS;
        chic = 2;
    }
}

void clearScreen()
{
    // Clears the buffer with this colour attribute
    g_Console.clearBuffer(0x1F);
}

void renderSplashScreen()  // renders the splash screen
{
    COORD c = g_Console.getConsoleSize();
    c.Y /= 3;
    c.X = c.X / 2 - 9;
    g_Console.writeToBuffer(c, "A game in 3 seconds", 0x03);
    c.Y += 1;
    c.X = g_Console.getConsoleSize().X / 2 - 20;
    g_Console.writeToBuffer(c, "Press <Space> to change character colour", 0x09);
    c.Y += 1;
    c.X = g_Console.getConsoleSize().X / 2 - 9;
    g_Console.writeToBuffer(c, "Press 'Esc' to quit", 0x09);
}

void renderGame()
{
    renderMap();        // renders the map to the buffer first
    renderCharacter();  // renders the character into the buffer
}

void renderMap()
{
    // Set up sample colours, and output shadings
    const WORD colors[] = {
        0x1A, 0x2B, 0x3C, 0x4D, 0x5E, 0x6F,
        0xA1, 0xB2, 0xC3, 0xD4, 0xE5, 0xF6
    };

    COORD c;
    /*for (int i = 0; i < 12; ++i)
    {
        c.X = 5 * i;
        c.Y = i + 1;
        colour(colors[i]);
        g_Console.writeToBuffer(c, " °±²Û", colors[i]);
    }*/
    for (int x = 0; x <= 79; x++)//rows(top/bottom) go here
    {
         for (int y = 0; y <= 22; y++)//cols(sides) go here
		 {	
            c.X = 0 + x;
			c.Y = 0 + y;
            g_Console.writeToBuffer(c, levelMatrix[x][y], 0x1A);
		 }
    }
    //g_Console.writeToBuffer(c, g_cHealthBar, 0x1A);
    //renderTraps();
    c.X = 0;
	c.Y = 24;
	if( i > 0)
	{
		g_Console.writeToBuffer(c, heart);
	}
	c.X = 1;
	c.Y = 24;
	if(i >1)
	{
		g_Console.writeToBuffer(c, heart);
	}
	c.X = 2;
	c.Y = 24;
	if(i >2)
	{
		g_Console.writeToBuffer(c, heart);
	}
	c.X = 3;
	c.Y = 24;
	if(i >3)
	{
		g_Console.writeToBuffer(c, heart);
	}
	c.X = 4;
	c.Y = 24;
	if(i > 4)
	{
		g_Console.writeToBuffer(c, heart);
	}
    //renderTraps();
}

void renderCharacter()
{
    // Draw the location of the character
    WORD charColor = 0x0C;
    if (g_sChar.m_bActive)
    {
        charColor = 0x0A;
    }
    g_Console.writeToBuffer(g_sChar.m_cLocation, (char)1, charColor);
    g_Console.writeToBuffer(g_cCharLocation2, (char)58, 0x1A);
}

void renderFramerate()
{
    COORD c;
    // displays the framerate
    std::ostringstream ss;
    ss << std::fixed << std::setprecision(3);
    ss << 1.0 / g_dDeltaTime << "fps";
    c.X = g_Console.getConsoleSize().X - 9;
    c.Y = 0;
    g_Console.writeToBuffer(c, ss.str());

    // displays the elapsed time
    ss.str("");
    ss << g_dElapsedTime << "secs";
    c.X = 0;
    c.Y = 0;
    g_Console.writeToBuffer(c, ss.str(), 0x59);
}
void renderToScreen()
{
    // Writes the buffer to the console, hence you will see what you have written
    g_Console.flushBufferToConsole();
}

void updateTraps()
{
	if(levelMatrix[g_sChar.m_cLocation.X][(g_sChar.m_cLocation.Y)] == 33)
		--i;
}

/*void renderTraps()
{
    COORD c;
    c.X = 0;
	c.Y = 24;
	if( i > 0)
	{
		g_Console.writeToBuffer(c, heart);
	}
	c.X = 1;
	c.Y = 24;
	if(i >1)
	{
		g_Console.writeToBuffer(c, heart);
	}
	c.X = 2;
	c.Y = 24;
	if(i >2)
	{
		g_Console.writeToBuffer(c, heart);
	}
	c.X = 3;
	c.Y = 24;
	if(i >3)
	{
		g_Console.writeToBuffer(c, heart);
	}
	c.X = 4;
	c.Y = 24;
	if(i >= 4)
	{
		g_Console.writeToBuffer(c, heart);
	}
}*/

bool ghostMoveUp()
{
    char ghostNextUp = levelMatrix[ghostOneX][ghostOneY - 1]; // [ghostOneI - 1][ghostOneJ]
      //Check the next upward block to be sure it is not a wall
    if ( ghostNextUp != 65 && ghostNextUp != 35) 
    {
		  levelMatrix[ghostOneX][ghostOneY - 1] = 87; //[ghostOneI - 1][ghostOneJ]
		  levelMatrix[ghostOneX][ghostOneY] = 32; // this was on top 
		  ghostOneY--; // ghostOneJ-- // X--                         
	   return true;
    }
    else
    {
	   return false;
    }
}

bool ghostMoveDown()
{

    char ghostNextDown = levelMatrix[ghostOneX][ghostOneY + 1];
    if ( ghostNextDown != 65 && ghostNextDown != 35) // if not a wall
    {
		  //levelMatrix[ghostOneI][ghostOneJ] = 32;
		  levelMatrix[ghostOneX][ghostOneY + 1] = 87; // [ghostOneI + 1][ghostOneJ]
		  levelMatrix[ghostOneX][ghostOneY] = 32;
		  ghostOneY++; //ghostOneJ++ // X++
	   return true;
    }
    else
    {
	   return false;
    }
}

bool ghostMoveLeft()
{
    char ghostNextLeft = levelMatrix[ghostOneX - 1][ghostOneY];
    if ( ghostNextLeft != 65 && ghostNextLeft != 35)
	   {
			 //levelMatrix[ghostOneI][ghostOneJ] = 32;
			 levelMatrix[ghostOneX - 1][ghostOneY] = 87;
			 levelMatrix[ghostOneX][ghostOneY] = 32;
			 ghostOneX--; //ghostOneI-- // Y--
		   return true;
	   }
        else
	   {
		  return false;
	   }
}

bool ghostMoveRight()
{
    char ghostNextRight = levelMatrix[ghostOneX + 1][ghostOneY];
    if ( ghostNextRight != 65 && ghostNextRight != 35) // if ghost does not touch wall
    {
		  //levelMatrix[ghostOneI][ghostOneJ] = 32;
		  levelMatrix[ghostOneX + 1][ghostOneY] = 87;
		  levelMatrix[ghostOneX][ghostOneY] = 32;
		  ghostOneX++; // ghostOneI++ // Y++
	   return true;
    }
    else
    {
	   return false;
    }

}

void renderAfter()
{
    //clearScreen();
    //console.clearBuffer();
    renderMap();        // renders the map to the buffer first
    renderCharacter();  // renders the character into the buffer
    renderFramerate();  // renders debug information, frame rate, elapsed time, etc
    //if (chic == 2)*/
        renderToScreen();
}

void Ghost()
{
	/*if (isWot)
	{
	   //Ghost is moving up 
		if (charLocation.Y > ghostOneJ)
	   {
		  //if wall found
		  if (!ghostMoveUp())
		  { 
			 //find node path 
		  }
	   }
    
	   //Ghost is moving down
	   else if (charLocation.Y < ghostOneJ)
	   {
		  if (!ghostMoveDown())
		  {

		  }
	   }

	   //Ghost is moving left
	   if (charLocation.X > ghostOneI)
	   {
		  if (!ghostMoveLeft())
		  {

		  }
	   }

	   //Ghost is moving right
		else if (charLocation.X < ghostOneI)
	   {
		  if (!ghostMoveRight())
		  {

		  }
	   }
	} */
    
	   //Ghost is moving up 
   if (spawn == true)
   {
	   if (g_sChar.m_cLocation.Y < ghostOneY)
	   {

		  //if wall found
		  /*if (!ghostMoveUp())
		  { 

		  }*/
		   ghostMoveUp();
	   }
    
	   //Ghost is moving down
	   else if (g_sChar.m_cLocation.Y > ghostOneY)
	   {
		  /*if (!ghostMoveDown())
		  {

		  }*/
		  ghostMoveDown();
	   }

	   //Ghost is moving left
	   if (g_sChar.m_cLocation.X < ghostOneX)
	   {
		  /*if (!ghostMoveLeft())
		  {

		  }*/
		  ghostMoveLeft();
	   }

	   //Ghost is moving right
	   else if (g_sChar.m_cLocation.X > ghostOneX)
	   {
		  /*if (!ghostMoveRight())
		  {

		  }*/
		  ghostMoveRight();
	   }
    

    //you caught the ghost
	/*if (charLocation.X == ghostOneI && charLocation.Y == ghostOneJ)
    {
	   //levelMatrix[ghostOneI][ghostOneJ] = 074;
	   ghostOneI = 16;
	   ghostOneJ = 14;
	   levelMatrix[16][14] = 87;
    }*/
    //Reset the postion of pacman you died 
	if (g_sChar.m_cLocation.X == ghostOneX && g_sChar.m_cLocation.Y == ghostOneY) // else if
    {
	   //lives--;
	   levelMatrix[g_sChar.m_cLocation.X][g_sChar.m_cLocation.Y] = 32;
	   levelMatrix[ghostOneX][ghostOneY] = 32;
	   g_sChar.m_cLocation.Y = 1;
	   g_sChar.m_cLocation.X = 1;
	   ghostOneY = 16;
	   ghostOneX = 70; // 14
	   //levelMatrix[15][0] = 074;
	   //levelMatrix[16][14] = 87;
    } 
   }
}

void randSpawn()
{
    for (int i = 0; i <= unit; i++)
	{
		chance = rand() % 20;
	}
	
	if (unit != 20)
	{
		++unit;
	}

	if (unit == 20)
	{
		unit = 0;
	}
	
    if ((g_sChar.m_cLocation.X > 14)  && (g_sChar.m_cLocation.X < 20) && (g_sChar.m_cLocation.Y == 9) )
	{
	    if (chance == 15)
		{
			spawn = true;
		}
	}
}
