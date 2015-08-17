// This is the main file for the game logic and function
//
//
#include "game.h"
#include "Framework\console.h"
#include <iostream>
#include <iomanip>
// Samuel: Going to take in a file so
#include <fstream>
// Also going to use string for getline to work
#include <string>

using std::cout;
using std::endl;
using std::cin;
// Samuel: defining the keys Q and wasd 
#define VK_Q	0x051
#define VK_W	0x057
#define VK_A	0x041
#define VK_S	0x053
#define VK_D	0x044

double	g_dElapsedTime;
double	g_dDeltaTime;
bool	g_abKeyPressed[K_COUNT];
COORD	g_cCharLocation;
COORD	g_cConsoleSize;
// Samuel: Gonna create my own cCharLocation
COORD   g_cCharLocation2;
// Samuel: Gonna create maze coord, hopefully it works out
COORD   g_cMaze; 

//--------------------------------------------------------------
// Purpose	: Initialisation function
// Input	: Void
// Output	: void
//--------------------------------------------------------------
void init( void ) // init is short for initialize
{
    // Set precision for floating point output
    std::cout << std::fixed << std::setprecision(3);

	// set the name of your console window
    SetConsoleTitle(L"SP1 Maze Runner"); // maze runner is just a placeholder

    // Sets the console size, this is the biggest so far.
    setConsoleSize(79, 28);

    // Get console width and height
    CONSOLE_SCREEN_BUFFER_INFO csbi; /* to get buffer info */     

    /* get the number of character cells in the current buffer */ 
    GetConsoleScreenBufferInfo( GetStdHandle( STD_OUTPUT_HANDLE ), &csbi );
    g_cConsoleSize.X = csbi.srWindow.Right + 1;
    g_cConsoleSize.Y = csbi.srWindow.Bottom + 1;

    // set the character to be in the center of the screen.
    g_cCharLocation.X = g_cConsoleSize.X / 3;
    g_cCharLocation.Y = g_cConsoleSize.Y / 3;
    g_cCharLocation2.X = g_cConsoleSize.X / 2;
    g_cCharLocation2.Y = g_cConsoleSize.Y / 2;

    g_dElapsedTime = 0.0;

    char maze[20][20] = {
                    {'-' , '-' , '-' , '-' , '-' , '-' , '-' , '-' , '-' , '-' , '-', '-' , '-' , '-' , '-' , '-' , '-' , '-' , '-' , '-'}

                    ,{'|' , '#' , '#' , '#' , '|' , '#' , '#' , '#' , '#' , '#' , '#', '#' , '#' , '#' , '#' , '#' , '#' , '#' , '#' , '|'}

                    ,{'|' , '#' , '|' , '#' , '|' , '#' , '|' , '-' , '-' , '-' , '-', '-' , '-' , '#' , '-' , '-' , '-' , '-' , '-' , '|'}

                    ,{'|' , '#' , '|' , '#' , '#' , '#' , '|' , '#' , '#' , '#' , '#', '#' , '|' , '#' , '#' , '#' , '#' , '#' , '#' , '|'}

                    ,{'|' , 'O' , '|' , '#' , '|' , '-' , '-' , '#' , '|' , '-' , '|', '#' , '|' , '-' , '-' , '-' , '-' , '-' , '#' , '|'}

                    ,{'|' , '-' , '-' , '-' , '|' , '#' , '|' , '#' , '|' , '#' , '|', '#' , '|' , '#' , '#' , '#' , '#' , '#' , '#' , '|'}

                    ,{'|' , '#' , '#' , '#' , '|' , '#' , '|' , '#' , '#' , '#' , '|', '#' , '|' , '#' , '-' , '-' , '-' , '-' , '-' , '|'}

                    ,{'|' , '#' , '|' , '#' , '|' , '#' , '|' , '-' , '#' , '|' , '-', '#' , '|' , '#' , '|' , '#' , '#' , '#' , '#' , '|'}

                    ,{'|' , '#' , '|' , '#' , '|' , '#' , '|' , '#' , '#' , '|' , '#', '#' , '|' , '#' , '#' , '#' , '-' , '-' , '#' , '|'}

                    ,{'|' , '#' , '|' , '#' , '|' , '#' , '|' , '#' , '-' , '|' , '#', '#' , '|' , '-' , '-' , '-' , '|' , '|' , '#' , '|'}

                    ,{'|' , '#' , '|' , '#' , '#' , '#' , '|' , '#' , '#' , '|' , '#', '#' , '#' , '|' , '#' , '#' , '#' , '|' , '#' , '|'}

                    ,{'|' , '#' , '|' , '#' , '|' , '-' , '-' , '-' , '#' , '|' , '#', '|' , '#' , '|' , '#' , '-' , '-' , '|' , '#' , '|'}

                    ,{'|' , '#' , '|' , '#' , '|' , '#' , '#' , '#' , '#' , '|' , '#', '|' , '#' , '|' , '#' , '|' , '#' , '#' , '#' , '|'}

                    ,{'|' , '#' , '-' , '-' , '|' , '#' , '|' , '-' , '#' , '|' , '#', '|' , '#' , '|' , '#' , '-' , '-' , '#' , '-' , '|'}

                    ,{'|' , '#' , '#' , '#' , '|' , '#' , '|' , '#' , '#' , '|' , '#', '|' , '#' , '|' , '#' , '#' , '#' , '#' , '#' , '|'}

                    ,{'|' , '#' , '|' , '#' , '|' , '#' , '|' , '#' , '#' , '|' , '#', '|' , '#' , '|' , '-' , '-' , '-' , '-' , '#' , '|'}

                    ,{'|' , '#' , '|' , '#' , '#' , '#' , '|' , '#' , '#' , '|' , '#', '|' , '#' , '#' , '#' , '#' , '#' , '#' , '#' , '|'}

                    ,{'|' , '#' , '|' , '#' , '|' , '-' , '-' , '-' , '-' , '-' , '#', '|' , '#' , '|' , '-' , '-' , '-' , '-' , '#' , '|'}

                    ,{'|' , 'X' , '|' , '#' , '|' , '#' , '#' , '#' , '#' , '#' , '#', '|' , '#' , '|' , '#' , '#' , '#' , '#' , '#' , '|'}

                    ,{'-' , '-' , '-' , '-' , '-' , '-' , '-' , '-' , '-' , '-' , '-', '-' , '-' , '-' , '-' , '-' , '-' , '-' , '-' , '-'} 
                        };
}

//--------------------------------------------------------------
// Purpose	: Reset before exiting the program
// Input	: Void
// Output	: void
//--------------------------------------------------------------
void shutdown( void )
{
    // Reset to white text on black background
	colour(FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
}

//--------------------------------------------------------------
// Purpose	: Getting all the key press states
// Input	: Void
// Output	: void
//--------------------------------------------------------------
void getInput( void )
{    
    g_abKeyPressed[K_UP] = isKeyPressed(VK_UP);
    g_abKeyPressed[K_DOWN] = isKeyPressed(VK_DOWN);
    g_abKeyPressed[K_LEFT] = isKeyPressed(VK_LEFT);
    g_abKeyPressed[K_RIGHT] = isKeyPressed(VK_RIGHT);
    g_abKeyPressed[K_ESCAPE] = isKeyPressed(VK_ESCAPE);
    g_abKeyPressed[K_OPTIONS] = isKeyPressed(VK_Q);
    g_abKeyPressed[K_W] = isKeyPressed(VK_W);
    g_abKeyPressed[K_A] = isKeyPressed(VK_A);
    g_abKeyPressed[K_S] = isKeyPressed(VK_S);
    g_abKeyPressed[K_D] = isKeyPressed(VK_D);
}

//--------------------------------------------------------------
// Purpose	: Update function
// Input	: dt = deltatime
// Output	: void
//--------------------------------------------------------------
void update(double dt)
{
    // get the delta time
    g_dElapsedTime += dt;
    g_dDeltaTime = dt;

    // Updating the location of the character based on the key press
	// providing a beep sound whenver we shift the character
    if (g_abKeyPressed[K_UP] && g_cCharLocation.Y > 0)
    {
        Beep(1440, 30);
        for (size_t i = 0; i < 2; ++i)
        g_cCharLocation.Y-=2; 
    }
    if (g_abKeyPressed[K_LEFT] && g_cCharLocation.X > 0)
    {
        Beep(1440, 30);
        g_cCharLocation.X--; 
    }
    if (g_abKeyPressed[K_DOWN] && g_cCharLocation.Y < g_cConsoleSize.Y - 1)
    {
        Beep(1440, 30);
        g_cCharLocation.Y+=5; 
    }
    if (g_abKeyPressed[K_RIGHT] && g_cCharLocation.X < g_cConsoleSize.X - 1)
    {
        Beep(1440, 30);
        g_cCharLocation.X++; 
    }

    if (g_abKeyPressed[K_RIGHT] && g_cCharLocation.X >= 44)
    {
        Beep(1440, 30);
        g_cCharLocation.X--;
    }
    // movement of the other char
    if (g_abKeyPressed[K_W] && g_cCharLocation.Y > 0)
    {
        Beep(1440, 30);
        //for (size_t i = 0; i < 2; ++i)
        g_cCharLocation2.Y--; 
    }
    if (g_abKeyPressed[K_A] && g_cCharLocation.X > 0)
    {
        Beep(1440, 30);
        g_cCharLocation2.X--; 
    }
    if (g_abKeyPressed[K_S] && g_cCharLocation2.Y < g_cConsoleSize.Y - 1)
    {
        Beep(1440, 30);
        g_cCharLocation2.Y++; 
    }
    if (g_abKeyPressed[K_D] && g_cCharLocation2.X < g_cConsoleSize.X - 1)
    {
        Beep(1440, 30);
        g_cCharLocation2.X++; 
    }

    if (g_abKeyPressed[K_D] && g_cCharLocation2.X >= 44)
    {
        Beep(1440, 30);
        g_cCharLocation2.X--;
    }
    // quits the game if player hits the escape key
    if (g_abKeyPressed[K_ESCAPE])
        g_bQuitGame = true;
    // Samuel: Gonna write my own options button
    if (g_abKeyPressed[K_OPTIONS])
        displayOptions();
}

//--------------------------------------------------------------
// Purpose	: Render function is to update the console screen
// Input	: void
// Output	: void
//--------------------------------------------------------------
void render( void )
{
    // clear previous screen
    colour(0x0F);
    cls();

    //render the game

    //render test screen code (not efficient at all)
    const WORD colors[] =   {
	                        0x1A, 0x2B, 0x3C, 0x4D, 0x5E, 0x6F,
	                        0xA1, 0xB2, 0xC3, 0xD4, 0xE5, 0xF6
	                        };
	
	for (int i = 0; i < 12; ++i)
	{
		//gotoXY(3*i,i+1);
        gotoXY(0, 0 + i);
		colour(colors[i]);
		std::cout << "    "; // Samuel: Originally was "WOW"
	}

    // Samuel: The following are part of the small experiment to create a rectangle
	for (int i = 0; i < 12; ++i)
	{
		//gotoXY(3*i,i+1);
        gotoXY(0, 0 - i);
		colour(colors[i]);
		std::cout << "    "; // Samuel: Originally was "WOW"
	}    
    
    for (int i = 0; i < 12; ++i)
    {
        gotoXY(44, 0 + i);
        colour(colors[i]);
        std::cout << "    ";
    }

    for (int i = 0; i < 12; ++i)
    {
        gotoXY(0 + i * 4, 12);
        colour(colors[i]);
        std::cout << "    ";
    }

    /*for (int i = 0; i < 12; ++i)
    {
        gotoXY(15 + i * 3, 12);
        colour(colors[i]);
        std::cout << "rekt";
    }*/

    // Samuel: Experiment ends here

    // render time taken to calculate this frame
    gotoXY(70, 0);
    colour(0x1A);
    std::cout << 1.0 / g_dDeltaTime << "fps" << std::endl;
  
    gotoXY(0, 0);
    colour(0x59);
    std::cout << g_dElapsedTime << "secs" << std::endl;

    // render character
    gotoXY(g_cCharLocation);
    colour(0x0C);
    std::cout << (char)1;

    // Samuel: Gonna render another char
    gotoXY(g_cCharLocation2);
    colour(0x1A);
    std::cout << (char)58;

    std::ifstream inData;
    //std::ofstream outData; Samuel: i can use it to stream to inData first then transport to OutData, giving me a txt vesion of the maze
    std::string data;

    inData.open("SP1MazeAlsoOld.txt");
    while (!inData.eof()) //commented out cause limpeh need for loop
    {
    for (size_t i = 0; i < 20; ++i) // 21 is width of maze
    {
        std::getline(inData, data);
        for (size_t j = 0; j < 20; ++j)
            if (data[j] == 1)
            {
                g_cMaze.X = i;
                g_cMaze.Y = j;
                break;
            }
            //else
              //  continue;
        //cout << data << endl;
            break;
    }
                gotoXY(g_cMaze);
                colour(0xA1);
                std::cout << '1';

    }
    inData.close();
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
    cout << std::setw(g_cConsoleSize.X / 2) << std::setfill('*') << std::right << "MAZE RUNNER" << endl;
    for (float space = 2.5f; space < g_cConsoleSize.X / 2; ++space)
        cout << ' ';    
    cout << "Menu" << endl;

    // loop to loop through the enum sequences
    for (Sequence s = Play; s != MAX_SEQUENCE; s = static_cast<Sequence>(s + 1))
    {
        if (s == 1)
        {
            for (size_t space = 5; space < g_cConsoleSize.X / 2; ++space)
                cout << ' ';
            cout << s << ". " << "Play" << endl;
        }
        else if (s == 2)
        {
            for (size_t space = 5; space < g_cConsoleSize.X / 2; ++space)
                cout << ' ';
            cout << s << ". " << "Options" << endl;
        }
        else if (s == 3)
        {
            for (size_t space = 5; space < g_cConsoleSize.X / 2; ++space)
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
