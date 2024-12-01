#include "FEHLCD.h"
#include "FEHUtility.h"
#include "FEHImages.h"
#include "Windows.h"
#include "Windows.h"
#include <array>
#include <vector>
#include <iostream>

using namespace std;

#define CHAR_HEIGHT 17
#define CHAR_WIDTH 12
#define GRAVITY 4

/*
    Buttons and Utility
*/
struct Box
{
    int x1, y1, x2, y2; // left, top, right, bottom

    Box(int x1_0, int y1_0, int x2_0, int y2_0)
    {
        x1 = x1_0, y1 = y1_0, x2 = x2_0, y2 = y2_0;
    }

    bool PointIntersection(int x, int y)
    {
        return x >= x1 && x <= x2 && y >= y1 && y <= y2;
    }

    bool BoxIntersection(Box box2)
    {
        return (x2 >= box2.x1) && (x1 <= box2.x2) && (y2 >= box2.y1) && (y1 <= box2.y2);
    }

    bool ScreenIntersection()
    {
        return x1 < 0 || x2 > LCD_WIDTH || y2 > LCD_HEIGHT;
    }
};

Box main_platform(40, 220, 280, 239);

class Button
{
private:
    int x, y, w, h, text_length, x_t, y_t;
    unsigned int border_color = WHITE, fill_color = BLACK, text_color = WHITE;
    char text[30];
    void (*function)();

public:
    Button(int w1 = 1, int h1 = 1) : w(w1), h(h1) {}

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
        x = x_prop * LCD_WIDTH - w / 2;
    }

    void SetYProp(float y_prop)
    {
        y = y_prop * LCD_HEIGHT - h / 2;
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

    void SetText(char str[])
    {
        text_length = strlen(str);
        strcpy(text, str);
    }

    void SetWidth(int w1)
    {
        w = w1;
    }

    void SetHeight(int h1)
    {
        h = h1;
    }

    void SetFunction(void funct())
    {
        function = funct;
    }

    void Create()
    {
        LCD.SetFontColor(fill_color);
        LCD.FillRectangle(x, y, w, h);
        LCD.SetFontColor(border_color);
        LCD.DrawRectangle(x, y, w, h);
        x_t = x + (w - (text_length * CHAR_WIDTH)) / 2, y_t = y + (h - CHAR_HEIGHT) / 2;
        LCD.SetFontColor(text_color);
        LCD.WriteAt(text, x_t, y_t);
    }

    void Check(float x_press, float y_press)
    {
        Box button(x, y, x + w, y + h);
        if (button.PointIntersection(x_press, y_press))
        {
            function();
        }
    }
};

void WriteCenter(char str[], int y)
{
    int length = strlen(str) * CHAR_WIDTH;
    int x = (LCD_WIDTH / 2) - length / 2;
    LCD.WriteAt(str, x, y);
}

// Blueprints
void Menu(), Play(), Credits(), Instructions(), Statistics(), Quit();

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

    int w, h = 25, y_diff = 10, y_start = 40;
    float x_prop = 0.5;
    char labels[5][30] = {"Play", "Credits", "Instructions", "Statistics", "Quit"};
    void (*functions[5])() = {Play, Credits, Instructions, Statistics, Quit};

    Button menu_buttons[5];

    for (int b = 0; b < 5; b++)
    {
        w = (strlen(labels[b]) + 2) * CHAR_WIDTH;
        menu_buttons[b].SetWidth(w);
        menu_buttons[b].SetHeight(h);
        menu_buttons[b].SetXProp(x_prop);
        menu_buttons[b].SetYCoord(y_start + b * (h + y_diff));
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

// This is where the fun begins

class Bullet
{
private:
    float x, y, r = 1;
    unsigned int color = YELLOW;
    bool right_check, visible_check = false;

public:
    void Create(float x1, float y1, bool right)
    {
        x = x1, y = y1;
        right_check = right;
        visible_check = true;
    }

    Box CollisionBox()
    {
        return Box(x - r, y - r, x + r, y + r);
    }

    void Draw()
    {
        if (CollisionBox().ScreenIntersection())
        {
            visible_check = false;
        }
        if (visible_check)
        {
            LCD.SetFontColor(color);
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
        visible_check = false;
    }

    bool Available()
    {
        return !visible_check;
    }
};

class Player
{
private:
    const int total_hit_points = 5;
    int hit_points = total_hit_points;
    float x, y, w, h;
    float t = 0, v_jump = -7, bullet_timer = 40;
    bool right_check = true, jump_check = false, shoot_check = false, dead_check = false;
    bool step_check = false, step_num = 0;
    FEHImage player_left_0, player_left_1, player_left_2;
    FEHImage player_right_0, player_right_1, player_right_2;
    FEHImage player_hearts, player_hits;
    Bullet b1;

public:
    Player()
    {
        w = 15, h = 55;
        x = 153, y = 220 - h;
        player_left_0.Open("astro_left_0.png");
        player_left_1.Open("astro_left_1.png");
        player_left_2.Open("astro_left_2.png");
        player_right_0.Open("astro_right_0.png");
        player_right_1.Open("astro_right_1.png");
        player_right_2.Open("astro_right_2.png");

        player_hearts.Open("heart_full.png");
        player_hits.Open("heart_empty.png");
    }

    Box CollisionBox()
    {
        return Box(x, y, x + w, y + h);
    }

    void moveRight()
    {
        right_check = true;
        step_check = true;
        x += 2;
    }

    void moveLeft()
    {
        right_check = false;
        step_check = true;
        x -= 2;
    }

    void jump()
    {
        if (!jump_check)
        {
            jump_check = true;
            t = 0.1;
            y += v_jump * t + 0.5 * GRAVITY * t * t;
        }
    }

    void shoot()
    {
        if (bullet_timer > 50)
        {
            if (right_check)
            {

                b1.Create(x + w, y + 28, true);
            }
            else
            {

                b1.Create(x, y + 28, false);
            }
            bullet_timer = 0;
        }
    }

    void Hit()
    {
        hit_points--;
    }

    bool Dead()
    {
        return hit_points == 0;
    }

    void Draw()
    {
        // draw shot bullet
        b1.Draw();
        bullet_timer++;
        // calculate change in y coordinates
        if (!CollisionBox().BoxIntersection(main_platform))
        {
            t += 0.1;
            if (jump_check)
            {
                y += v_jump * t + 0.5 * GRAVITY * t * t;
            }
            else
            {
                y += 0.5 * GRAVITY * t * t;
            }

            if (CollisionBox().ScreenIntersection())
            {
                Hit();
                x = 153;
                y = 220 - h - 10;
            }
        }
        else
        {
            jump_check = false;
            t = 0;
            y = 220 - h;
        }

        // check if player is dead
        if (hit_points != 0)
        {
            if (right_check)
            {
                if (step_check)
                {
                    step_check = false;
                    if (step_num)
                    {
                        step_num = false;
                        player_right_1.Draw(x, y);
                    }
                    else
                    {
                        step_num = true;
                        player_right_2.Draw(x, y);
                    }
                }
                else
                {
                    player_right_0.Draw(x, y);
                }
            }
            else
            {
                if (step_check)
                {
                    step_check = false;
                    if (step_num)
                    {
                        step_num = false;
                        player_left_1.Draw(x, y);
                    }
                    else
                    {
                        step_num = true;
                        player_left_2.Draw(x, y);
                    }
                }
                else
                {
                    player_left_0.Draw(x, y);
                }
            }
        }

        // Draw health indicating hearts
        LCD.SetFontColor(DARKSLATEGRAY);
        LCD.DrawRectangle(1, 232, 318, 7);
        LCD.FillRectangle(1, 232, 318, 7);
        for (int h = 0; h < total_hit_points; h++)
        {
            if (h < hit_points)
            {
                player_hearts.Draw(7 * h + 1, 232);
            }
            else
            {
                player_hits.Draw(7 * h + 1, 232);
            }
        }
    }

    Box BulletCollisionBox()
    {
        return b1.CollisionBox();
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
    bool right_check, hit_check = false;
    FEHImage enemy_left_1, enemy_left_2, enemy_right_1, enemy_right_2, enemy_dead;

public:
    Box CollisionBox()
    {
        return Box(x, y, x + w, y + h);
    }

    void Hit()
    {
        health--;
    }

    void PlayerHit()
    {
        hit_check = true;
    }

    bool CheckPlayerHit()
    {
        return hit_check;
    }

    bool Dead()
    {
        return (health == 0 || (CollisionBox().ScreenIntersection()));
    }

    virtual void Draw() = 0;
};

// Enemy Type 1 with 1 health and 1 speed
class Enemy1 : public Enemy
{
private:
    float t = 0;
    int t_frames = 0;
    bool frames_check = false;

public:
    Enemy1(bool right)
    {
        w = 20;
        h = 40;
        enemy_left_1.Open("xenos_left_1.png");
        enemy_left_2.Open("xenos_left_2.png");
        enemy_right_1.Open("xenos_right_1.png");
        enemy_right_2.Open("xenos_right_2.png");
        right_check = right;
        health = 1;
        y = 0 - h;
        if (right)
        {
            x = 15;
        }
        else
        {
            x = 320 - w - 15;
        }
    }
    void Draw() override
    {
        if (!CollisionBox().BoxIntersection(main_platform))
        {
            t += 0.1;
            y += 0.5 * GRAVITY * t * t;
        }
        else
        {
            t = 0;
            y = 220 - h;
        }
        if (!Dead())
        {

            frames_check = t_frames / 10 != 0;
            if (right_check)
            {
                if (frames_check)
                {
                    enemy_right_1.Draw(x, y);
                }
                else
                {
                    enemy_right_2.Draw(x, y);
                }
                x += 0.25;
            }
            else
            {
                if (frames_check)
                {
                    enemy_left_1.Draw(x, y);
                }
                else
                {
                    enemy_left_2.Draw(x, y);
                }
                x -= 0.25;
            }
            t_frames = (t_frames + 1) % 20;
        }
    }
};

vector<Enemy *> enemies;

void SpawnEnemy1()
{
    bool random_num = rand() % 2;
    Enemy1 *new_enemy = new Enemy1(random_num);
    enemies.push_back(new_enemy);
}

void Play()
{
    LCD.Clear();
    Player player;
    int kill_count = 0, final_time = 0;
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
            if (enemies[i]->Dead())
            {
                enemies.erase(enemies.begin() + i--);
            }
        }

        for (Enemy *enemy : enemies)
        {
            enemy->Draw();
            if (enemy->CollisionBox().BoxIntersection(player.BulletCollisionBox()) && player.BulletCheck())
            {
                kill_count++;
                enemy->Hit();
                player.BulletKill();
            }
            if (enemy->CollisionBox().BoxIntersection(player.CollisionBox()) && !enemy->CheckPlayerHit())
            {
                enemy->PlayerHit();
                player.Hit();
            }
        }

        if (player.Dead())
        {
            final_time = time(NULL) - time_0;
            enemies.clear();
            break;
        }

        Sleep(10);
    }

    int score = kill_count * 7 + final_time;
    int high_score, total_kill_count, best_time;

    ifstream high_score_in;
    high_score_in.open("statistics.txt");
    high_score_in >> high_score;
    high_score_in >> total_kill_count;
    high_score_in >> best_time;
    high_score_in.close();

    if (score > high_score)
    {
        high_score = score;
    }
    if (final_time > best_time)
    {
        best_time = final_time;
    }
    total_kill_count += kill_count;

    ofstream high_score_out;
    high_score_out.open("statistics.txt");
    high_score_out << high_score << endl;
    high_score_out << total_kill_count << endl;
    high_score_out << best_time;
    high_score_out.close();

    LCD.Clear();
    LCD.SetFontColor(WHITE);

    char score_text[30];
    char high_score_text[30];
    char kills_text[30];
    char time_text[30];

    sprintf(score_text, "Score: %i", score);
    sprintf(high_score_text, "High Score: %i", high_score);
    sprintf(kills_text, "Kills: %i", kill_count);
    sprintf(time_text, "Time: %i", final_time);

    WriteCenter(score_text, 20);
    WriteCenter(high_score_text, 40);
    WriteCenter(kills_text, 60);
    WriteCenter(time_text, 80);

    BackButtonCheck();
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
    WriteCenter("Use WAS to move and CTRL", 50);
    WriteCenter("to shoot. Kill ", 70);
    WriteCenter("enemies and survive", 90);
    WriteCenter("as long as possible", 110);
    WriteCenter("to get a high score.", 130);
    WriteCenter("You have 5 hearts.", 150);

    // Create and check a back button to return to the main menu
    BackButtonCheck();
}

void Statistics()
{
    /*
      Reset the background color, clear the screen,
      then write out the statistics.
    */
    int high_score, total_kill_count, best_time;

    ifstream high_score_in;
    high_score_in.open("statistics.txt");
    high_score_in >> high_score >> total_kill_count >> best_time;
    high_score_in.close();

    char high_score_text[30], total_kill_count_text[30], best_time_text[30];
    sprintf(high_score_text, "HIGH SCORE:  %i", high_score);
    sprintf(total_kill_count_text, "TOTAL KILLS: %i", total_kill_count);
    sprintf(best_time_text, "BEST TIME:   %i", best_time);

    LCD.SetBackgroundColor(BLACK);
    LCD.Clear();
    LCD.SetFontColor(WHITE);
    WriteCenter("STATS", 10);
    WriteCenter("__________________________", 20);
    LCD.WriteAt(high_score_text, 50, 40);
    LCD.WriteAt(total_kill_count_text, 50, 60);
    LCD.WriteAt(best_time_text, 50, 80);

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