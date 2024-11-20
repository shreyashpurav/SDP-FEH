#include "FEHLCD.h"
#include "FEHUtility.h"
#include <Windows.h>
using namespace std;

void createMenu();

/*
    SDP Menu Creation
  
    This creates a menu with 5 buttons the user can press. The options are
    "Play", "Statistics", "Instructions", "Credits", or "Quit".
*/

void generateNextMap();

//Create the blueprint for the Credits function
void rollCredits();

void displayInstructions();

void loadStats();

void quit();

class Button
{
    private:
        float x_pos1, y_pos1, x_pos2, y_pos2;
        char label[10];

    public:
        Button(int x1, int y1, int x2, int y2);
        bool pressed(float x_press, float y_press);
};

Button::Button(int x1, int y1, int x2, int y2)
{
    LCD.SetFontColor(WHITE);
    LCD.DrawRectangle(x1,y1,x2-x1,y2-y1);
    LCD.FillRectangle(x1,y1,x2-x1,y2-y1);
    x_pos1 = x1;
    y_pos1 = y1;
    x_pos2 = x2;
    y_pos2 = y2;
}

bool Button::pressed(float x_press, float y_press)
{
    if(x_press >= x_pos1 && x_press <= x_pos2 && y_press <= y_pos2 && y_press >= y_pos1)
    {
        return true;
    }
}

void createMenu() 
{
    LCD.SetBackgroundColor(BLACK);
    LCD.Clear();
    LCD.SetFontColor(WHITE);

    
    //This code creates all the buttons for the main menu
    Button playButton(80, 80, 240, 100);
    Button creditsButton(80, 110, 240, 130);
    Button instructionsButton(80, 140, 240, 160);
    Button statsButton(80, 170, 240, 190);
    Button quitButton(80, 200, 240, 220);

    //Write the text inside the buttons
    LCD.SetFontColor(BLACK);
    LCD.WriteAt("Play", 135, 83);
    LCD.WriteAt("Credits", 120, 113);
    LCD.WriteAt("Instructions", 90, 143);
    LCD.WriteAt("Statistics", 100, 173);
    LCD.WriteAt("Quit", 135, 203);

    //Create variables to determine the position of the click
    float x, y;
    float x_trash, y_trash;

    //Wait for the user to click the screen
    while(!LCD.Touch(&x, &y)){};
    //Wait for the user to release the press
    while(LCD.Touch(&x_trash, &y_trash)){};

    //Determine if the user pressed the credits button

    // if(GetKeyState(0x10) & 0x8000)
    // {
    //     rollCredits();
    // }
    

    // return false;
    if (playButton.pressed(x, y))
    {
        generateNextMap();
    }
    else if (creditsButton.pressed(x, y))
    {
        rollCredits();
    }
    else if (instructionsButton.pressed(x, y))
    {
        displayInstructions();
    }
    else if (statsButton.pressed(x, y))
    {
        loadStats();
    }
    else if (quitButton.pressed(x, y))
    {
        quit();
    }

}


void generateNextMap()
{
    LCD.SetBackgroundColor(BLACK);
    LCD.Clear();

    //Create a back button to return to the main menu
    Button backButton(20, 190, 80, 210);
    LCD.SetFontColor(BLACK);
    LCD.WriteAt("Back", 25, 193);

    float x, y;
    float x_trash, y_trash;

    //Wait for the user to click the screen
    while(!LCD.Touch(&x, &y)){};
    //Wait for the user to release the press
    while(LCD.Touch(&x_trash, &y_trash)){};
    if (backButton.pressed(x, y))
    {
        createMenu();
    }
}

void rollCredits()
{
    /*
        This function resets the background color and clears the
        screen. It then write out the credits to the screen.
    */
    LCD.SetBackgroundColor(BLACK);
    LCD.Clear();
    LCD.SetFontColor(WHITE);
    LCD.WriteAt("Created by Shreyash", 40, 80);
    LCD.WriteAt("Purav and Jack Hanlon", 30, 100);
    
    //Create a back button to return to the main menu
    Button backButton(20, 190, 80, 210);
    LCD.SetFontColor(BLACK);
    LCD.WriteAt("Back", 25, 193);

    float x, y;
    float x_trash, y_trash;

    //Wait for the user to click the screen
    while(!LCD.Touch(&x, &y)){};
    //Wait for the user to release the press
    while(LCD.Touch(&x_trash, &y_trash)){};
    if (backButton.pressed(x, y))
    {
        createMenu();
    }
}

void displayInstructions()
{
    LCD.SetBackgroundColor(BLACK);
    LCD.Clear();

    //Create a back button to return to the main menu
    Button backButton(20, 190, 80, 210);
    LCD.SetFontColor(BLACK);
    LCD.WriteAt("Back", 25, 193);

    float x, y;
    float x_trash, y_trash;

    //Wait for the user to click the screen
    while(!LCD.Touch(&x, &y)){};
    //Wait for the user to release the press
    while(LCD.Touch(&x_trash, &y_trash)){};
    if (backButton.pressed(x, y))
    {
        createMenu();
    }
}

void loadStats()
{
    LCD.SetBackgroundColor(BLACK);
    LCD.Clear();

    //Create a back button to return to the main menu
    Button backButton(20, 190, 80, 210);
    LCD.SetFontColor(BLACK);
    LCD.WriteAt("Back", 25, 193);

    float x, y;
    float x_trash, y_trash;

    //Wait for the user to click the screen
    while(!LCD.Touch(&x, &y)){};
    //Wait for the user to release the press
    while(LCD.Touch(&x_trash, &y_trash)){};
    if (backButton.pressed(x, y))
    {
        createMenu();
    }
}

void quit()
{
    LCD.SetBackgroundColor(BLACK);
    LCD.Clear();

}

int main() {
    // Clear background
    LCD.SetBackgroundColor(BLACK);
    LCD.Clear();

    createMenu();

    while (1) {
        LCD.Update();
        // Never end
    }
    return 0;
}