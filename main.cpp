#include "FEHLCD.h"
#include "FEHUtility.h"
#include "FEHImages.h"
#include "Windows.h"
#include <array>
#include <vector>
#include <iostream>

using namespace std;

#define CHAR_HEIGHT 17
#define CHAR_WIDTH 12
#define GRAVITY 4
array<int, 4> MAIN_PLATFORM = {40, 220, 280, 239};

/*
    Buttons and Utility
*/

bool ScreenBoundsCheck(array<int, 4> box)
{
    bool x_check = box[0] < 0 || box[2] > LCD_WIDTH;
    bool y_check = box[1] < 0 || box[3] > LCD_HEIGHT;
    return x_check || y_check;
}

bool BoundsCheck(int x, int y, array<int, 4> box)
{
    bool x_check = x > box[0] && x < box[2];
    bool y_check = y > box[1] && y < box[3];
    return x_check && y_check;
}

bool BoundsCheckBox(array<int, 4> box1, array<int, 4> box2)
{
    int box1_x1 = box1[0], box1_y1 = box1[1], box1_x2 = box1[2], box1_y2 = box1[3];
    int box2_x1 = box2[0], box2_y1 = box2[1], box2_x2 = box2[2], box2_y2 = box2[3];
    bool x1_check = (box1_x1 >= box2_x1 && box1_x1 <= box2_x2) || (box2_x1 >= box1_x1 && box2_x1 <= box1_x2);
    bool x2_check = (box1_x2 >= box2_x2 && box1_x2 <= box2_x2) || (box2_x2 >= box1_x2 && box2_x2 <= box1_x2);
    bool x_check = x1_check || x2_check;
    bool y1_check = (box1_y1 >= box2_y1 && box1_y1 <= box2_y2) || (box2_y1 >= box1_y1 && box2_y1 <= box1_y2);
    bool y2_check = (box1_y2 >= box2_y2 && box1_y2 <= box2_y2) || (box2_y2 >= box1_y2 && box2_y2 <= box1_y2);
    bool y_check = y1_check || y2_check;
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
void Menu(), Play(), Credits(), Instructions(), Statistics(), Quit();

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
    void (*functions[5])() = {Play, Credits, Instructions, Statistics, Quit};

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

class Bullet
{
private:
    float x, y, r = 1;
    unsigned int color = YELLOW;
    bool right_check, visible = false;

public:
    void Create(float x1, float y1, bool right)
    {
        x = x1, y = y1;
        right_check = right;
        visible = true;
    }
    void Draw()
    {
        if (ScreenBoundsCheck(Collision()))
        {
            visible = false;
        }
        if (visible)
        {
            LCD.DrawCircle(x, y, r);
            if (right_check)
            {
                x += 5;
            }
            else
            {
                x -= 5;
            }
        }
    }
    void Kill()
    {
        visible = false;
    }
    bool Available()
    {
        return !visible;
    }
    array<int, 4> Collision()
    {
        return {x - r, y - r, x + r, y + r};
    }
};

class Player
{
private:
    int health = 5;
    float x, y, w, h, bullet_timer = 11;
    float t = 0, v = -7;
    bool right_check = true, jump_check = false, shoot_check = false, dead_check = false;
    FEHImage player_right, player_left, player_dead;
    Bullet b1;

public:
    Player()
    {
        x = 153;
        y = 170;
        w = 15;
        h = 50;
        player_left.Open("protoustros_01.png");
        player_right.Open("protoustros_02.png");
        player_right.Draw(x, y);
    }
    void moveRight()
    {
        right_check = true;
        if (x + w + 2 <= LCD_WIDTH)
        {
            x += 2;
        }
    }
    void moveLeft()
    {
        right_check = false;
        if (x - 2 >= 0)
        {
            x -= 2;
        }
    }
    void jump()
    {
        if (!jump_check)
        {
            jump_check = true;
            t = 0.1;
            y += v * t + 0.5 * GRAVITY * t * t;
        }
    }
    void shoot()
    {
        if (bullet_timer > 50)
        {
            b1.Create(x, y + h / 3, right_check);
            bullet_timer = 0;
        }
    }

    array<int, 4> Collision()
    {
        return {x, y, x + w, y + h};
    }
    void Draw()
    {
        // draw shot bullet
        b1.Draw();
        bullet_timer += 1;
        // calculate change in y coordinates
        if (!BoundsCheckBox(Collision(), MAIN_PLATFORM))
        {
            if (ScreenBoundsCheck(Collision()))
            {
                Hit();
                x = 153;
                y = 160;
            }
            t += 0.1;
            if (jump_check)
            {
                y += v * t + 0.5 * GRAVITY * t * t;
            }
            else
            {
                y += 0.5 * GRAVITY * t * t;
            }
        }
        else
        {
            jump_check = false;
            t = 0;
            if (BoundsCheckBox(MAIN_PLATFORM, Collision()))
            {
                y = 170;
            }
        }
        // check if player is dead
        if (Death())
        {
            player_dead.Draw(x - h / 2, y + h - 15);
        }
        else
        {
            if (right_check)
            {
                player_right.Draw(x, y);
            }
            else
            {
                player_left.Draw(x, y);
            }
        }
    }
    void Hit()
    {
        health--;
    }
    bool Death()
    {
        if (health == 0)
        {
            dead_check = true;
        }
        return dead_check;
    }

    array<int, 4> BulletCollision()
    {
        return b1.Collision();
    }

    void BulletKill()
    {
        b1.Kill();
    }

    bool BulletCheck()
    {
        return !b1.Available();
    }
};

class Enemy
{
protected:
    float x, y, w, h, health;
    float t = 0;
    bool right_check, dead_check;
    FEHImage enemy_right, enemy_left, enemy_dead;

public:
    array<int, 4> Collision()
    {
        return {x, y, x + w, y + h};
    }
    void Hit()
    {
        health--;
    }

    void Draw()
    {
        // check if player is dead
        // calculate change in y coordinates
        if (!BoundsCheckBox(Collision(), MAIN_PLATFORM))
        {
            t += 0.1;
            y += 0.5 * GRAVITY * t * t;
        }
        if (!Dead())
        {
            t = 0;
            y = 170;
            if (right_check)
            {
                enemy_right.Draw(x++, y);
            }
            else
            {
                enemy_left.Draw(x--, y);
            }
        }
    }
    bool Dead()
    {
        if (health == 0 || (ScreenBoundsCheck(Collision())))
        {
            dead_check = true;
        }
        return dead_check;
    }
};

// Enemy Type 1 with 1 health and 1 speed
class Enemy1 : public Enemy
{
public:
    Enemy1(bool right)
    {
        w = 15;
        h = 50;
        enemy_left.Open("protoustros_01.png");
        enemy_right.Open("protoustros_02.png");
        dead_check = false;
        right_check = right;
        health = 1;
        y = 0;
        if (right)
        {
            x = 40;
        }
        else
        {
            x = 280 - w;
        }
    }
};

vector<Enemy> enemies;

void SpawnEnemy1()
{
    bool random_num = rand() % 2;
    Enemy1 new_enemy(random_num);
    enemies.push_back(new_enemy);
}

void Play()
{
    LCD.Clear();
    Player player;
    int kill_count = 0;
    int time_0 = time(NULL);
    bool second_over = true;
    while (true)
    {
        LCD.Clear();
        LCD.SetFontColor(WHITE);
        LCD.DrawRectangle(40, 220, 240, 19);
        LCD.FillRectangle(40, 220, 240, 19);
        if (GetAsyncKeyState(0x41) & 0x8000)
        {
            player.moveLeft();
        }
        if (GetAsyncKeyState(0x44) & 0x8000)
        {
            player.moveRight();
        }
        if (GetAsyncKeyState(0x57) & 0x8000)
        {
            player.jump();
        }
        if (GetAsyncKeyState(0x11) & 0x8000)
        {
            player.shoot();
        }
        player.Draw();
        if ((time(NULL) - time_0) % 7 == 0 && second_over)
        {
            SpawnEnemy1();
            second_over = false;
        }
        if ((time(NULL) - time_0) % 7 == 1)
        {
            second_over = true;
        }
        for (int i = 0; i < enemies.size(); i++)
        {
            if (enemies[i].Dead())
            {
                enemies.erase(enemies.begin() + i--);
            }
        }
        for (Enemy &enemy : enemies)
        {
            enemy.Draw();
            if (BoundsCheckBox(player.BulletCollision(), enemy.Collision()) && player.BulletCheck())
            {
                enemy.Hit();
                player.BulletKill();
            }
        }
        Sleep(10);
    }
    // Random Enemy Generation push
}