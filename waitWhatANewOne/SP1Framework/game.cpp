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

// Game specific variables here
COORD charLocation;
COORD menuScreen;
//Process *chi = new Process;
int chic;
int ets = 2;
char g_cHealthBar[5] = {char(3), char(3), char(3), char(3), char(3)};
//string g_sHealthBar = "";

// Initialize variables, allocate memory, load data from file, etc. 
// This is called once before entering into your main loop
void init()
{
    // Set precision for floating point output
    elapsedTime = 0.0;

    charLocation.X = console.getConsoleSize().X / 2;
    charLocation.Y = console.getConsoleSize().Y / 2;
    g_cCharLocation2.X = console.getConsoleSize().X / 2;
    g_cCharLocation2.Y = console.getConsoleSize().Y / 2;
    // sets the width, height and the font name to use in the console
    console.setConsoleFont(0, 16, L"jesus");
    splashScreen();
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
    if (chic == 2)
    renderAfter();
}

void moveCharacter()
{
    // Updating the location of the character based on the key press
    if (keyPressed[K_UP] && charLocation.Y > 0)
    {
        //Beep(1440, 30);
        charLocation.Y--;
    }
    if (keyPressed[K_LEFT] && charLocation.X > 0)
    {
        //Beep(1440, 30);
        charLocation.X--;
    }
    if (keyPressed[K_DOWN] && charLocation.Y < console.getConsoleSize().Y - 1)
    {
        //Beep(1440, 30);
        charLocation.Y++;
    }
    if (keyPressed[K_RIGHT] && charLocation.X < console.getConsoleSize().X - 1)
    {
        //Beep(1440, 30);
        charLocation.X++;
    }
    if (keyPressed[K_RIGHT] && charLocation.X >= 44)
    {
        //Beep(1440, 30);
        charLocation.X--;
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

    if (keyPressed[K_D] && g_cCharLocation2.X >= 44)
    {
        //Beep(1440, 30);
        g_cCharLocation2.X--;
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
        chic = ets;
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
    sel.X = 0;
    sel.Y = 4;
}

void renderMap()
{
    // Set up sample colours, and output shadings
    const WORD colors[] = {
        0x1A, 0x2B, 0x3C, 0x4D, 0x5E, 0x6F,
        0xA1, 0xB2, 0xC3, 0xD4, 0xE5, 0xF6
    };

    COORD c;
    for (int i = 0; i < 12; ++i)
    {
        c.X = 0;
        c.Y = 0 + i + 1;
        colour(colors[i]);
        console.writeToBuffer(c, " °±²Û", colors[i]);
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
    console.clearBuffer();
    renderMap();        // renders the map to the buffer first
    renderCharacter();  // renders the character into the buffer
    renderFramerate();  // renders debug information, frame rate, elapsed time, etc
    //if (chic == 2)*/
        renderToScreen();
}