// This is the main file for the game logic and function
//
//
#include "game.h"
#include "Framework\console.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>
// Samuel: I gonna include a string
//#include <string>

using std::cout;
using std::endl;
using std::cin;
using std::string;

// Samuel: defining the keys Q and wasd 
#define VK_Q	0x051
#define VK_W	0x057
#define VK_A	0x041
#define VK_S	0x053
#define VK_D	0x044
#define VK_SPACE 0x20
#define VK_1    0x31
#define VK_2    0x32

// Console object
Console console(80, 25, "SP1 Framework");

double elapsedTime;
double deltaTime;
bool keyPressed[K_COUNT];
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
		{35,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,35},//77
		{35,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,35},//78
		{35,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,32,35},//79
		{35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35}//80

		 };

// Game specific variables here
COORD charLocation;
COORD menuScreen;
//Process *chi = new Process;
int chic;
char g_cHealthBar[5] = {char(3), char(3), char(3), char(3), char(3)};
//string g_sHealthBar = "";

//Ghost1 Start postion is [14][16]
int ghostOneX = 70, ghostOneY = 16; // I is Y, J is X
char ghostCurrent = levelMatrix[ghostOneX][ghostOneY];
char ghostTrail = ghostCurrent[&ghostOneX][&ghostOneY];
//bool isWot = false;

// Initialize variables, allocate memory, load data from file, etc. 
// This is called once before entering into your main loop
void init()
{
    // Set precision for floating point output
    elapsedTime = 0.0;

    charLocation.X = 1; //console.getConsoleSize().X / 2;
    charLocation.Y = 1; //console.getConsoleSize().Y / 2;
    g_cCharLocation2.X = console.getConsoleSize().X / 3;
    g_cCharLocation2.Y = console.getConsoleSize().Y / 3;
    // sets the width, height and the font name to use in the console
    console.setConsoleFont(0, 16, L"Rayner Raffey"); // jesus
    //splashScreen();
    //menu();
    std::ifstream inData;
    //std::ofstream outData;
    std::string data;
    
    inData.open("SP1Maze.txt");
    COORD coor;
    for (int i = 0; i <= 10; ++i)
    {
        std::getline(inData, data);
        coor.X = i;
        coor.Y = i;
        console.writeToBuffer(coor, data, 0x1A);
    }
    inData.close();
    /*for (int n = 0; n < sizeof(g_cHealthBar); ++n)
    {
        g_sHealthBar[n] = g_cHealthBar[n];
    }*/
}

// Do your clean up of memory here
// This is called once just before the game exits
void shutdown()
{
    // Reset to white text on black background
	colour(FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);

    console.clearBuffer();
}
/*
	This function checks if any key had been pressed since the last time we checked
	If a key is pressed, the value for that particular key will be true
	
	Add more keys to the enum in game.h if you need to detect more keys
	To get other VK key defines, right click on the VK define (e.g. VK_UP) and choose "Go To Definition" 
	For Alphanumeric keys, the values are their ascii values (uppercase).
*/
void getInput()
{    
    keyPressed[K_UP] = isKeyPressed(VK_UP);
    keyPressed[K_DOWN] = isKeyPressed(VK_DOWN);
    keyPressed[K_LEFT] = isKeyPressed(VK_LEFT);
    keyPressed[K_RIGHT] = isKeyPressed(VK_RIGHT);
    keyPressed[K_ESCAPE] = isKeyPressed(VK_ESCAPE);
    keyPressed[K_OPTIONS] = isKeyPressed(VK_Q);
    keyPressed[K_W] = isKeyPressed(VK_W);
    keyPressed[K_A] = isKeyPressed(VK_A);
    keyPressed[K_S] = isKeyPressed(VK_S);
    keyPressed[K_D] = isKeyPressed(VK_D);
    keyPressed[K_SPACE] = isKeyPressed(VK_SPACE);
    keyPressed[K_1] = isKeyPressed(VK_1);
    keyPressed[K_2] = isKeyPressed(VK_2);
}

/*
	This is the update function
	double dt - This is the amount of time in seconds since the previous call was made

	Game logic should be done here.
	Such as collision checks, determining the position of your game characters, status updates, etc
	If there are any calls to write to the console here, then you are doing it wrong.

    If your game has multiple states, you should determine the current state, and call the relevant function here.
*/
void update(double dt)
{
    // get the delta time
    elapsedTime += dt;
    deltaTime = dt;

    processUserInput(); // checks if you should change states or do something else with the game, e.g. pause, exit
    moveCharacter();    // moves the character, collision detection, physics, etc
    // sound can be played here too.
    
}

/*
    This is the render loop
    At this point, you should know exactly what to draw onto the screen.
    Just draw it!
    To get an idea of the values for colours, look at console.h and the URL listed there
*/
void render()
{
    clearScreen();      // clears the current screen and draw from scratch 
    menuSelect();
    renderToScreen();
    renderMap();        // renders the map to the buffer first
    renderCharacter();  // renders the character into the buffer
    renderFramerate();  // renders debug information, frame rate, elapsed time, etc
    //renderToScreen();   // dump the contents of the buffer to the screen, one frame worth of game
    if (chic == 1)
    renderAfter();
}

void moveCharacter()
{
    // Updating the location of the character based on the key press
    if (keyPressed[K_UP] && charLocation.Y > 0)
    {
        if (levelMatrix[charLocation.X][(charLocation.Y - 1)] != 35)//if there is no "35" wall above player 1
		{
			//Beep(1440, 30);
			charLocation.Y--; //move upwards
			Ghost();
            //ghostMoveUp();
		}
    }
    if (keyPressed[K_LEFT] && charLocation.X > 0)
    {
        if (levelMatrix[charLocation.X - 1][(charLocation.Y)] != 35)
		{
            //Beep(1440, 30);
            charLocation.X--;
			Ghost();
            //ghostMoveLeft();
        }
    }
    if (keyPressed[K_DOWN] && charLocation.Y < console.getConsoleSize().Y - 1)
    {
        if (levelMatrix[charLocation.X][(charLocation.Y + 1)] != 35)
        {
            //Beep(1440, 30);
            charLocation.Y++;
			Ghost();
            //ghostMoveDown();
        }
    }
    if (keyPressed[K_RIGHT] && charLocation.X < console.getConsoleSize().X - 1)
    {
        if (levelMatrix[charLocation.X + 1][(charLocation.Y)] != 35)
        {
            //Beep(1440, 30);
            charLocation.X++;
			Ghost();
            //ghostMoveRight();
        }
    }
    // movement of the other char
    if (keyPressed[K_W] && g_cCharLocation2.Y > 0)
    {
        //Beep(1440, 30);
        //for (size_t i = 0; i < 2; ++i)
        g_cCharLocation2.Y--; 
    }
    if (keyPressed[K_A] && g_cCharLocation2.X > 0)
    {
        //Beep(1440, 30);
        g_cCharLocation2.X--; 
    }
    if (keyPressed[K_S] && g_cCharLocation2.Y < console.getConsoleSize().Y - 1)
    {
        //Beep(1440, 30);
        g_cCharLocation2.Y++; 
    }
    if (keyPressed[K_D] && g_cCharLocation2.X < console.getConsoleSize().X - 1)
    {
        //Beep(1440, 30);
        g_cCharLocation2.X++; 
    }
}
void processUserInput()
{
    // quits the game if player hits the escape key
    if (keyPressed[K_ESCAPE])
        g_quitGame = true;
    // Samuel: Gonna write my own options button
    if (keyPressed[K_OPTIONS])
        displayOptions();
    
    //*chi = PLAY;
    /*if (keyPressed[K_SPACE])
    {   
        int which = 0;
        cin >> which;
        if (which == 1)
            *chi = PLAY;
        else if (which == 2)
            *chi = OPTIONS;
        else if (which == 3)
            *chi = EXIT;
    }*/
    if (keyPressed[K_1])
        chic = 1;
        //*chi = PLAY;
    if (keyPressed[K_2])
    {
        //*chi = OPTIONS;
        chic = 2;
    }
    //while(g_bQuitGame != true)
    //{
  /*  for(int row = 0; row < ROWMAX; row++)
	{
		for(int col=0; col < COLMAX; col++)
        {
            if (maze[row][col] == 'B')
            {
                //g_cLocation.X = row;
                //g_cLocation.Y = col;
                if (g_cCharLocation.Y <= col)
                {
                    g_cCharLocation.Y++;
                }   
            }
        }
		cout << "\n";
	} */
    //}
}

void clearScreen()
{
    // Clears the buffer with this colour attribute
    console.clearBuffer(0x1F);
}

void menuSelect()
{
    COORD sel;
    sel.X = 0;
    sel.Y = 0;
    console.writeToBuffer(sel, "                         Welcome to the menu!", 0x1A);
    sel.X = 0;
    sel.Y = 1;
    console.writeToBuffer(sel, "                        1. Play              ", 0x1A);
    sel.X = 0;
    sel.Y = 2;
    console.writeToBuffer(sel, "                        2. Options           ", 0x1A);
    sel.X = 0;
    sel.Y = 3;
    console.writeToBuffer(sel, "                        3. Exit              ", 0x1A);
    std::ifstream inData;
    //std::ofstream outData;
    std::string data;
    
    inData.open("SP1Maze.txt");
    COORD coor;
    //while (!inData.eof())
    for (int i = 0; i < 7; ++i)
    {
        std::getline(inData, data);
        coor.X = 0;
        coor.Y = 5 + i;
        console.writeToBuffer(coor, data, 0x1A);
        //console.writeToBuffer(coor, '\n');
    }
    inData.close();
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
        c.X = 0;
        c.Y = 0 + i + 1;
        colour(colors[i]);
        console.writeToBuffer(c, " °±²Û", colors[i]);
    }*/
    for (int x = 0; x <= 79; x++)//rows(top/bottom) go here
    {
         for (int y = 0; y <= 22; y++)//cols(sides) go here
		 {	
            c.X = 0 + x;
			c.Y = 0 + y;
            console.writeToBuffer(c, levelMatrix[x][y], 0x1A);
		 }
    }
    c.X = 0;
    c.Y = 24;
    console.writeToBuffer(c, g_cHealthBar, 0x1A);
}

void renderCharacter()
{
    // Draw the location of the character
    console.writeToBuffer(charLocation, (char)1, 0x0C);
    console.writeToBuffer(g_cCharLocation2, (char)58, 0x1A);
}

void renderFramerate()
{
    COORD c;
    // displays the framerate
    std::ostringstream ss;
    ss << std::fixed << std::setprecision(3);
    ss << 1.0 / deltaTime << "fps";
    c.X = console.getConsoleSize().X - 9;
    c.Y = 0;
    console.writeToBuffer(c, ss.str());

    // displays the elapsed time
    ss.str("");
    ss << elapsedTime << "secs";
    c.X = 0;
    c.Y = 0;
    console.writeToBuffer(c, ss.str(), 0x59);
}
void renderToScreen()
{
    // Writes the buffer to the console, hence you will see what you have written
    console.flushBufferToConsole();
}

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


int menu(int s)
{
    Sequence start = Play;
    while (start != Exit)
    {
        displayMenu();
        userInput(start); // get user input
        switch (start)
        {
        case Play    : displayPlay(); break;
        case Options : displayOptions(); break;
        case Exit    : displayExit(); break;
        }
        return start;
    }
}

void displayMenu()
{
    //for (size_t space = 5; space < g_cConsoleSize.X / 2; ++space)
      //  cout << ' ';
    cout << std::setw(console.getConsoleSize().X / 2) << std::setfill('*') << std::right << "MAZE RUNNER" << endl;
    for (float space = 2.5f; space < console.getConsoleSize().X / 2; ++space)
        cout << ' ';    
    cout << "Menu" << endl;

    // loop to loop through the enum sequences
    for (Sequence s = Play; s != MAX_SEQUENCE; s = static_cast<Sequence>(s + 1))
    {
        if (s == 1)
        {
            for (size_t space = 5; space < console.getConsoleSize().X / 2; ++space)
                cout << ' ';
            cout << s << ". " << "Play" << endl;
        }
        else if (s == 2)
        {
            for (size_t space = 5; space < console.getConsoleSize().X / 2; ++space)
                cout << ' ';
            cout << s << ". " << "Options" << endl;
        }
        else if (s == 3)
        {
            for (size_t space = 5; space < console.getConsoleSize().X / 2; ++space)
                cout << ' ';
            cout << s << ". " << "Exit" << endl;
        }
    }
}

void displayPlay()
{
    cout << "You are now in play" << endl;
}

void displayOptions()
{
    cout << "This is the options screen" << endl;
}

void displayExit()
{
    cout << "Thanks for playing!" << endl;
}

void userInput(Sequence& s)
{
    int input = 0;
    cout << "Make a choice!" << endl; // splashscreen like the one in Minecraft
    cin >> input;
    s = static_cast<Sequence>(input);
}

void splashScreen ()
{
	HANDLE  hConsole;
    int colour = 3;

    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, colour);

/*    cout << "       ###############			   "<< endl;         
    cout << "      ##################		   "<< endl;       
    cout << "    #######################		   "<< endl;         
    cout << "  ##############   ###########				   "<< endl;
    cout << "##############################				   "<< endl;
    cout << "#######################				   "<< endl;
    cout << "####################			   "<< endl;
    cout << "################			   "<< endl;
    cout << "#############			   "<< endl;
    cout << "###########				   "<< endl;   
    cout << "#########					   "<< endl;
    cout << "############			   "<< "Pacman!" << endl;         
    cout << "##################		   "<< endl;       
    cout << "#######################		   "<< endl;         
    cout << "###########################				   "<< endl;
    cout << "##############################				   "<< endl;
    cout << "    #######################				   "<< endl;
    cout << "      ####################			   "<< endl;
    cout << "        ################			   "<< endl;

	cout << "Human Maze ART" << endl << endl; */
    COORD g;
    g.X = 0;
    g.Y = 0;
    cout << "         #######"  << endl;
	cout << "        #########" << endl;
	cout << "      #############" << endl;
	cout << "      #############" << endl;
	cout << "      #############" << endl;
	cout << "       ###########" << endl;
	cout << "        #########" << endl;
	//cout << "                   #" << endl;
	cout << "##########################                       ###################" << endl;
	cout << "##########################                       #                 #" << endl;
	cout << "####   ############   ####                       #   Human Maze!   #" << endl;
	cout << "####   ############    ####                      #                 #" << endl;
	cout << " ####  ############    ####                      ###################" << endl;
	cout << "  #### ############   ####" << endl;
	cout << "       ############" << endl;
	cout << "       ############                             By:" << endl;
	cout << "       ############                                Ezent Chang   Admin No." << endl;
	cout << "       #####  #####                                Yue Xian      Admin No." << endl;
	cout << "       #####  #####                                Samuel Tan    Admin No." << endl;
	cout << "       #####  #####                                Rayner Liew   152591J" << endl;
	cout << "       #####  #####" << endl;                   
	cout << "       #####  #####" << endl;
	cout << "############################" << endl;
	cout << "#                          #" << endl;
	cout << "#     Studio Project 1     #" << endl; 
	cout << "#                          #" << endl;
	cout << "############################" << endl;
    cout << endl;
    //system("pause");
	//cout <<  "Enter your name: ";
	//cin.getline (playerName, 20);
	cout << endl << endl << endl << endl;
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

		if (charLocation.Y < ghostOneY)
	   {

		  //if wall found
		  /*if (!ghostMoveUp())
		  { 

		  }*/
		   ghostMoveUp();
	   }
    
	   //Ghost is moving down
	   else if (charLocation.Y > ghostOneY)
	   {
		  /*if (!ghostMoveDown())
		  {

		  }*/
		  ghostMoveDown();
	   }

	   //Ghost is moving left
	   if (charLocation.X < ghostOneX)
	   {
		  /*if (!ghostMoveLeft())
		  {

		  }*/
		  ghostMoveLeft();
	   }

	   //Ghost is moving right
	   else if (charLocation.X > ghostOneX)
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
	if (charLocation.X == ghostOneX && charLocation.Y == ghostOneY) // else if
    {
	   //lives--;
	   levelMatrix[charLocation.X][charLocation.Y] = 32;
	   levelMatrix[ghostOneX][ghostOneY] = 32;
	   charLocation.Y = 1;
	   charLocation.X = 1;
	   ghostOneY = 16;
	   ghostOneX = 70; // 14
	   //levelMatrix[15][0] = 074;
	   //levelMatrix[16][14] = 87;
    } 
}