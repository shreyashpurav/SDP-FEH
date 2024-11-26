/**
 * @author Shreyash Purav
 * @date 11/17/24
 * Instructor: Clingan 12:40
 */

#include "FEHLCD.h"
#include "FEHUtility.h"
#include "FEHImages.h"
#include <Windows.h>
#include <array>
#include <vector>
#include <iostream>

using namespace std;

#define CHAR_HEIGHT 17
#define CHAR_WIDTH 12
int map_num = 0;

/*
    Buttons and Utility
*/

bool BoundsCheck(int x, int y, array<int, 4> box)
{
    bool x_check = x >= box[0] && x <= box[2];
    bool y_check = y >= box[1] && y <= box[3];
    return x_check && y_check;
}

class Button
{
private:
    int x, y, width, height, text_length, x_t, y_t;
    unsigned int border_color = WHITE, fill_color = BLACK, text_color = WHITE;
    char text[30];
    void (*function)();

public:
    Button(int w = 1, int h = 1) : width(w), height(h) {}

    void SetXCoord(int x1)
    {
        x = x1;
    }

    void SetYCoord(int y1)
    {
        y = y1;
    }

    void SetXProp(float x_prop)
    {
        x = x_prop * LCD_WIDTH - width / 2;
    }

    void SetYProp(float y_prop)
    {
        y = y_prop * LCD_HEIGHT - height / 2;
    }

    void SetBorderColor(unsigned int color)
    {
        border_color = color;
    }

    void SetFillColor(unsigned int color)
    {
        fill_color = color;
    }

    void SetTextColor(unsigned int color)
    {
        text_color = color;
    }

    void SetText(const char *str)
    {
        text_length = strlen(str);
        strcpy(text, str);
    }

    void SetWidth(int w)
    {
        width = w;
    }

    void SetHeight(int h)
    {
        height = h;
    }

    void SetFunction(void funct())
    {
        function = funct;
    }

    void Create()
    {
        LCD.SetFontColor(fill_color);
        LCD.FillRectangle(x, y, width, height);
        LCD.SetFontColor(border_color);
        LCD.DrawRectangle(x, y, width, height);
        x_t = x + (width - (text_length * CHAR_WIDTH)) / 2, y_t = y + (height - CHAR_HEIGHT) / 2;
        LCD.SetFontColor(text_color);
        LCD.WriteAt(text, x_t, y_t);
    }

    void Check(float x_press, float y_press)
    {
        array<int, 4> button = {x, y, x + width, y + height};
        if (BoundsCheck(x_press, y_press, button))
        {
            function();
        }
    }
};

void WriteCenter(const char *str, int y)
{
    int length = strlen(str) * CHAR_WIDTH;
    int x = (LCD_WIDTH / 2) - length / 2;
    LCD.WriteAt(str, x, y);
}

// Blueprints
void Menu(), GenerateNextMap(), Credits(), Instructions(), Statistics(), Quit();

/*
    SDP Menu Creation

    Create a menu screen with 5 buttons the user can press. The options are
    Play, "Statistics", "Instructions", "Credits", or Quit.
    Each of the 3 info pages have a Back Button to return to Menu.
*/
void Menu()
{
    LCD.SetBackgroundColor(BLACK);
    LCD.Clear();

    int width, height = 25, y_diff = 10, y_start = 40;
    float x_prop = 0.5;
    const char *labels[5] = {"Play", "Credits", "Instructions", "Statistics", "Quit"};
    void (*functions[5])() = {GenerateNextMap, Credits, Instructions, Statistics, Quit};

    Button menu_buttons[5];

    for (int b = 0; b < 5; b++)
    {
        width = (strlen(labels[b]) + 2) * CHAR_WIDTH;
        menu_buttons[b].SetWidth(width);
        menu_buttons[b].SetHeight(height);
        menu_buttons[b].SetXProp(x_prop);
        menu_buttons[b].SetYCoord(y_start + b * (height + y_diff));
        menu_buttons[b].SetText(labels[b]);
        menu_buttons[b].SetBorderColor(LIGHTGRAY);
        menu_buttons[b].SetFillColor(MEDIUMBLUE);
        menu_buttons[b].SetTextColor(LIGHTGRAY);
        menu_buttons[b].SetFunction(functions[b]);
        menu_buttons[b].Create();
    }
    while (true)
    {
        int x, y;
        while (!LCD.Touch(&x, &y))
        {
        }
        while (LCD.Touch(&x, &y))
        {
        }
        for (int c = 0; c < 5; c++)
        {
            menu_buttons[c].Check(x, y);
        }
    }
}

void BackButtonCheck()
{
    Button backButton;
    backButton.SetWidth(100);
    backButton.SetHeight(25);
    backButton.SetXProp(0.2);
    backButton.SetYProp(0.9);
    backButton.SetText("Back");
    backButton.SetBorderColor(LIGHTGRAY);
    backButton.SetFillColor(MEDIUMBLUE);
    backButton.SetTextColor(LIGHTGRAY);
    backButton.SetFunction(Menu);
    backButton.Create();
    float x, y;
    while (true)
    {
        while (!LCD.Touch(&x, &y))
        {
        }
        while (LCD.Touch(&x, &y))
        {
        }
        backButton.Check(x, y);
    }
}

class Player
{
private:
    float xPlayer, yPlayer, t, y_0, v_i, g;
    bool right;

public:
    FEHImage playerLeft, playerRight;
    Player()
    {
        xPlayer = 100;
        yPlayer = 100;
        t = 0;
        v_i = -30;
        g = 10;
        right = true;
        playerLeft.Open("protoustros_01.png");
        playerRight.Open("protoustros_02.png");
        playerRight.Draw(xPlayer, yPlayer);
    }
    void moveRight()
    {
        LCD.Clear();
        xPlayer += 2;
        playerRight.Draw(xPlayer, yPlayer);
        right = true;
    }
    void moveLeft()
    {
        LCD.Clear();
        xPlayer -= 2;
        playerLeft.Draw(xPlayer, yPlayer);
        right = false;
    }
    void jump()
    {
        jumping = true;
        y_0 = yPlayer;
        while (yPlayer <= y_0)
        {
            // Clear previous jumper
            LCD.Clear();

            // Change y coordinate
            t += 0.1;
            if (y_0 + v_i * t + 0.5 * g * t * t < y_0)
            {
                yPlayer = y_0 + v_i * t + 0.5 * g * t * t;
            }
            else
            {
                yPlayer = y_0;
                if (right == true)
                {
                    playerRight.Draw(xPlayer, yPlayer);
                }
                else
                {
                    playerLeft.Draw(xPlayer, yPlayer);
                }
                break;
            }
            LCD.Clear();
            if (right == true)
            {
                playerRight.Draw(xPlayer, yPlayer);
            }
            else
            {
                playerLeft.Draw(xPlayer, yPlayer);
            }

            if (GetAsyncKeyState(0x44) & 0x8000)
            {
                moveRight();
            }
            if (GetAsyncKeyState(0x41) & 0x8000)
            {
                moveLeft();
            }
            if ((GetAsyncKeyState(0x11)) & 0x8000)
            {
                shoot();
            }

            Sleep(10);
        }
        LCD.Update();
        yPlayer = y_0;
        t = 0;
        jumping = false;
    }
    void shoot()
    {
        float xBullet, yBullet;
        xBullet = xPlayer + 20;
        yBullet = yPlayer + 20;
        LCD.Clear();
        LCD.SetFontColor(WHITE);
        LCD.DrawCircle(xBullet, yBullet, 2);
        while (xBullet < 319)
        {
            if (GetAsyncKeyState(0x44) & 0x8000)
            {
                moveRight();
            }
            if (GetAsyncKeyState(0x41) & 0x8000)
            {
                moveLeft();
            }
            if ((GetAsyncKeyState(0x57)) & 0x8000)
            {
                jumping = true;
            }

            if (xBullet + 10 > 319)
            {
                LCD.Clear();
                break;
            }
            xBullet += 5;
            if (jumping == true)
            {
                LCD.Clear();
                // Change y coordinate
                t += 0.1;
                if (y_0 + v_i * t + 0.5 * g * t * t < y_0)
                {
                    yPlayer = y_0 + v_i * t + 0.5 * g * t * t;
                }
                else
                {
                    yPlayer = y_0;
                    if (right == true)
                    {
                        playerRight.Draw(xPlayer, yPlayer);
                    }
                    else
                    {
                        playerLeft.Draw(xPlayer, yPlayer);
                    }
                    LCD.Update();
                    yPlayer = y_0;
                    t = 0;
                    jumping = false;
                }
            }
            LCD.Clear();
            playerRight.Draw(xPlayer, yPlayer);
            LCD.DrawCircle(xBullet, yBullet, 2);
            Sleep(10);
        }
        playerRight.Draw(xPlayer, yPlayer);
    }
};

// Create the blueprint for the button functions
void GenerateNextMap();
void Credits();
void Instructions();
void Statistics();
void Quit();

void GenerateNextMap()
{
    LCD.SetBackgroundColor(BLACK);
    LCD.Clear();
    Player player;
    while (true)
    {

        if (GetAsyncKeyState(0x44) & 0x8000)
        {
            player.moveRight();
        }
        if (GetAsyncKeyState(0x41) & 0x8000)
        {
            player.moveLeft();
        }
        if ((GetAsyncKeyState(0x57)) & 0x8000)
        {
            player.jump();
        }
        if ((GetAsyncKeyState(0x11)) & 0x8000)
        {
            player.shoot();
        }
        Sleep(10);
    }
    // float x, y;
    // float x_trash, y_trash;
}

void Credits()
{
    /*
      Reset the background color, clear the screen,
      then write out the credits.
    */
    LCD.SetBackgroundColor(BLACK);
    LCD.Clear();
    LCD.SetFontColor(WHITE);
    WriteCenter("Created by Shreyash", 80);
    WriteCenter("Purav and Jack Hanlon", 100);

    // Create and check a back button to return to the main menu
    BackButtonCheck();
}

void Instructions()
{
    /*
      Reset the background color, clear the screen,
      then write out the instructions.
    */
    LCD.SetBackgroundColor(BLACK);
    LCD.Clear();
    LCD.SetFontColor(WHITE);
    WriteCenter("INSTRUCTIONS", 10);
    WriteCenter("__________________________", 20);
    WriteCenter("Use WASD to move and CTRL", 50);
    WriteCenter("to shoot. Move through", 70);
    WriteCenter("the levels without", 90);
    WriteCenter("dying and reach the end", 110);
    WriteCenter("to move onto the next", 130);
    WriteCenter("level. Complete all ", 150);
    WriteCenter("levels to beat the game.", 170);

    // Create and check a back button to return to the main menu
    BackButtonCheck();
}

void Statistics()
{
    /*
      Reset the background color, clear the screen,
      then write out the statistics.
    */
    LCD.SetBackgroundColor(BLACK);
    LCD.Clear();
    LCD.SetFontColor(WHITE);
    WriteCenter("STATS", 10);
    WriteCenter("__________________________", 20);
    LCD.WriteAt("KILLS: 0", 50, 40);
    LCD.WriteAt("LEVELS COMPLETED: 0", 50, 60);
    LCD.WriteAt("GREAT ENEMIES FELL: 1", 50, 80);

    /*
      Create and check a back button to return to the main menu
    */
    BackButtonCheck();
}

void Quit()
{
    LCD.SetBackgroundColor(BLACK);
    LCD.Clear();
    exit(0);
}

int main()
{
    // Clear background
    LCD.SetBackgroundColor(BLACK);
    LCD.Clear();

    Menu();

    while (1)
    {
        LCD.Update();
        // Never end
    }
    return 0;
}

// This is where the fun begins

/*
    Map Generation here!
*/

class Platform
{
private:
    int x, y, width, height;
    unsigned int color;

public:
    Platform(int x1, int y1, int w, int h, unsigned int color1) : x(x1), y(y1), width(w), height(h), color(color1)
    {
        Draw();
    }

    void Draw()
    {
        LCD.SetFontColor(color);
        LCD.DrawRectangle(x, y, width, height);
        LCD.FillRectangle(x, y, width, height);
    }

    array<int, 4> Collision()
    {
        array<int, 4> box = {x, y, x + width, y + height};
        return box;
    }
};

class Tile
{
private:
    int num;
    array<int, 4> connections;
    vector<Platform> platforms;

public:
    Tile(int n) : num(n) {}

    void AddPlatform(Platform platform)
    {
        platforms.push_back(platform);
    }

    void Draw()
    {
        for (int i = 0; i < platforms.size(); i++)
        {
            platforms[i].Draw();
        }
    }

    bool CollisionCheck(int x, int y)
    {
        bool check = false;
        for (int i = 0; i < platforms.size(); i++)
        {
            if (BoundsCheck(x, y, platforms[i].Collision()))
            {
                check = true;
            }
        }
        return check;
    }

    int Num()
    {
        return num;
    }

    /*
      0 = left, 1 = up, 2 = right, 3 = down
    */
    int NextTile(int direction)
    {
        return connections[direction];
    }
};

class Map
{
private:
    int num;
    int current_tile = 0;
    vector<Tile> tiles;

public:
    void AddTile(Tile tile)
    {
        tiles.push_back(tile);
    }
    void DrawNextTile()
    {
    }
    // if sprite is
};

void GenerateNextMap()
{
    LCD.Clear();
    BackButtonCheck();
}
