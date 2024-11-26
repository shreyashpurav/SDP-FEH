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
#include <math.h>
#include <cstdlib>
#include <ctime>

using namespace std;

#define CHAR_HEIGHT 17
#define CHAR_WIDTH 12
int map_num = 0;
int kills = 0;
float enemySpeed = 1;

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

class Enemy
{
private:
    float xEnemy, yEnemy;
    bool right, alive;

public:
    FEHImage enemyLeft, enemyRight;
    Enemy()
    {
        int range = 2;
        int randomNum = rand() % range;
        enemyRight.Open("protoustros_02.png");
        enemyLeft.Open("protoustros_01.png");
        if (randomNum == 0)
        {
            xEnemy = 20;
            yEnemy = 100;
            right = true;
            alive = true;
            enemyRight.Draw(xEnemy, yEnemy);
        }
        else
        {
            xEnemy = 300;
            yEnemy = 100;
            right = false;
            alive = true;
            enemyLeft.Draw(xEnemy, yEnemy);
        }
    }
    float getX()
    {
        return xEnemy;
    }
    float getY()
    {
        return yEnemy;
    }
    void setX(float x)
    {
        xEnemy = x;
    }
    void setY(float y)
    {
        yEnemy = y;
    }
    bool Alive()
    {
        return alive;
    }
    void setAlive(bool a)
    {
        alive = a;
    }
    bool getRight()
    {
        return right;
    }
    void setRight(bool answer)
    {
        right = answer;
    }
    void drawEnemy(Enemy name)
    {
        if (name.Alive())
        {
            if (name.getRight())
            {
                name.enemyRight.Draw(name.getX(), name.getY());
            }
            else
            {
                name.enemyLeft.Draw(name.getX(), name.getY());
            }
        }
    }
    void move()
    {
        if (alive)
        {
            if (right && xEnemy < 319)
            {
                xEnemy += enemySpeed;
            }
            else if (!right && xEnemy > 0)
            {
                xEnemy -= enemySpeed;
            }
        }
    }
};

vector<Enemy> enemies;
float spawnTime = 1000;

class Player
{
private:
    float xPlayer, yPlayer, t, y_0, v_i, g;
    bool right, jumping;

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
        jumping = false;
        playerLeft.Open("protoustros_01.png");
        playerRight.Open("protoustros_02.png");
        playerRight.Draw(xPlayer, yPlayer);
    }
    float getX()
    {
        return xPlayer;
    }
    float getY()
    {
        return yPlayer;
    }
    bool getRight()
    {
        return right;
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
        if (right == true)
        {
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
                // if ((GetAsyncKeyState(0x57)) & 0x8000)
                // {
                //     jumping = true;
                // }
                if (xBullet + 8 > 319)
                {
                    LCD.Clear();
                    break;
                }
                // if (enemy1.Alive())
                // {
                //     if (abs(xBullet - enemy1.getX()) < 10)
                //     {
                //         LCD.Clear();
                //         enemy1.setAlive(0);
                //         kills++;
                //         enemy1.setY(20);
                //         enemy1.drawEnemy(enemy1);
                //         enemy2.drawEnemy(enemy2);
                //         break;
                //     }
                // }
                // if (enemy2.Alive())
                // {
                //     if (abs(xBullet - enemy2.getX()) < 10)
                //     {
                //         LCD.Clear();
                //         enemy2.setAlive(0);
                //         kills++;
                //         enemy2.setY(20);
                //         enemy1.drawEnemy(enemy1);
                //         enemy2.drawEnemy(enemy2);
                //         break;
                //     }
                // }

                // Code inspired by code found on Stack Overflow
                // https://stackoverflow.com/questions/12702561/iterate-through-a-c-vector-using-a-for-loop
                for (Enemy &enemy : enemies)
                {
                    if (enemy.Alive() && abs(xBullet - enemy.getX()) < 10)
                    {
                        enemy.setAlive(0);
                        kills++;
                        break;
                    }
                }

                xBullet += 5;

                // if (jumping == true)
                // {
                //     LCD.Clear();
                //     // Change y coordinate
                //     t += 0.1;
                //     if (y_0 + v_i * t + 0.5 * g * t * t < y_0)
                //     {
                //         yPlayer = y_0 + v_i * t + 0.5 * g * t * t;
                //     }
                //     else
                //     {
                //         yPlayer = y_0;
                //         if (right == true)
                //         {
                //             playerRight.Draw(xPlayer, yPlayer);
                //         }
                //         else
                //         {
                //             playerLeft.Draw(xPlayer, yPlayer);
                //         }
                //         LCD.Update();
                //         yPlayer = y_0;
                //         t = 0;
                //         jumping = false;
                //     }
                // }

                for (Enemy &enemy : enemies)
                {
                    LCD.Clear();
                    enemy.move();
                    if (enemy.Alive())
                    {
                        enemy.drawEnemy(enemy);
                    }
                }
                playerRight.Draw(xPlayer, yPlayer);
                LCD.DrawCircle(xBullet, yBullet, 2);
                Sleep(10);
            }
            if (right == true)
            {
                playerRight.Draw(xPlayer, yPlayer);
            }
            else
            {
                playerLeft.Draw(xPlayer, yPlayer);
            }
        }
        else
        {
            xBullet = xPlayer - 10;
            while (xBullet > 0)
            {
                if (GetAsyncKeyState(0x44) & 0x8000)
                {
                    moveRight();
                }
                if (GetAsyncKeyState(0x41) & 0x8000)
                {
                    moveLeft();
                }
                // if ((GetAsyncKeyState(0x57)) & 0x8000)
                // {
                //     jumping = true;
                // }
                if (xBullet - 8 < 0)
                {
                    LCD.Clear();
                    break;
                }
                // if (enemy1.Alive())
                // {
                //     if (abs(xBullet - enemy1.getX()) < 10)
                //     {
                //         LCD.Clear();
                //         enemy1.setAlive(0);
                //         kills++;
                //         enemy1.setY(20);
                //         enemy1.drawEnemy(enemy1);
                //         enemy2.drawEnemy(enemy2);
                //         break;
                //     }
                // }
                // if (enemy2.Alive())
                // {
                //     if (abs(xBullet - enemy2.getX()) < 10)
                //     {
                //         LCD.Clear();
                //         enemy2.setAlive(0);
                //         kills++;
                //         enemy2.setY(20);
                //         enemy1.drawEnemy(enemy1);
                //         enemy2.drawEnemy(enemy2);
                //         break;
                //     }
                // }

                for (Enemy &enemy : enemies)
                {
                    if (enemy.Alive() && abs(xBullet - enemy.getX()) < 10)
                    {
                        enemy.setAlive(0);
                        kills++;
                        break;
                    }
                }

                xBullet -= 5;

                // if (jumping == true)
                // {
                //     LCD.Clear();
                //     // Change y coordinate
                //     t += 0.1;
                //     if (y_0 + v_i * t + 0.5 * g * t * t < y_0)
                //     {
                //         yPlayer = y_0 + v_i * t + 0.5 * g * t * t;
                //     }
                //     else
                //     {
                //         yPlayer = y_0;
                //         if (right == true)
                //         {
                //             playerRight.Draw(xPlayer, yPlayer);
                //         }
                //         else
                //         {
                //             playerLeft.Draw(xPlayer, yPlayer);
                //         }
                //         LCD.Update();
                //         yPlayer = y_0;
                //         t = 0;
                //         jumping = false;
                //     }
                // }
                for (Enemy &enemy : enemies)
                {
                    LCD.Clear();
                    enemy.move();
                    if (enemy.Alive())
                    {
                        enemy.drawEnemy(enemy);
                    }
                }
                playerLeft.Draw(xPlayer, yPlayer);
                LCD.DrawCircle(xBullet, yBullet, 2);
                Sleep(10);
            }
            if (right == true)
            {
                playerRight.Draw(xPlayer, yPlayer);
            }
            else
            {
                playerLeft.Draw(xPlayer, yPlayer);
            }
        }
    }
};

// Create the blueprint for the button functions
void GenerateNextMap();
void Credits();
void Instructions();
void Statistics();
void Quit();
void spawnEnemy();

void spawnEnemy()
{
    Enemy newEnemy;
    int randomNum = rand() % 2;
    if (randomNum == 0)
    {
        newEnemy.setX(20);
        newEnemy.setY(100);
        newEnemy.setRight(true);
    }
    else
    {
        newEnemy.setX(300);
        newEnemy.setY(100);
        newEnemy.setRight(false);
    }
    enemies.push_back(newEnemy);
}

void GenerateNextMap()
{
    LCD.SetBackgroundColor(BLACK);
    LCD.Clear();
    Player player;
    float gameTime = 0;

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
        // if ((GetAsyncKeyState(0x57)) & 0x8000)
        // {
        //     player.jump();
        // }
        if ((GetAsyncKeyState(0x11)) & 0x8000)
        {
            player.shoot();
        }

        gameTime += 10;
        if (gameTime > spawnTime)
        {
            spawnEnemy();
            gameTime = 0;
            enemySpeed += 0.05;
        }

        for (Enemy &enemy : enemies)
        {
            enemy.move();
            LCD.Clear();
            if (enemy.Alive())
            {
                enemy.drawEnemy(enemy);
            }
            if (player.getRight())
            {
                player.playerRight.Draw(player.getX(), player.getY());
            }
            else
            {
                player.playerLeft.Draw(player.getX(), player.getY());
            }
        }

        Sleep(10);
    }
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
