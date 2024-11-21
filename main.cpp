/**
 * @author Shreyash Purav
 * @date 11/17/24
 * Instructor: Clingan 12:40
 */

#include "FEHLCD.h"
#include "FEHUtility.h"
#include "FEHImages.h"
#include <Windows.h>

void GravityTraining();

class Player
{
private:
    FEHImage rSprite;
    FEHImage rCloak;
    FEHImage lSprite;
    FEHImage lCloak;
    int right = 0;

public:
    Player();
    void drawRight(int x, int y);
    void drawLeft(int x, int y);
    void clearRight(int x, int y);
    void clearLeft(int x, int y);
};

/*
    SDP Training Minigame #2

    There is a circle that jumps but stays in the air.​
    Make the circle fall back down.
*/
void GravityTraining()
{
    float x, y;
    const int y_0 = 180;
    int x_0 = 300, x1 = x_0, y1 = y_0;
    int r = 10;
    float g = 10, v_i = -58.310, t = 0;

    // Draw jumper
    Player astro;
    astro.drawLeft(x1, y1);
    // LCD.SetFontColor(HONEYDEW);
    // LCD.DrawCircle(x1, y1, r);
    // LCD.FillCircle(x1, y1, r);
    LCD.Update();

    // Wait for touch - use x and y
    while (true)
    {
        if ((GetAsyncKeyState(VK_SPACE) | GetAsyncKeyState(VK_LSHIFT)) & 0x8000)
        {
            while (y1 <= y_0)
            {
                // Clear previous jumper
                astro.clearLeft(x1, y1);
                LCD.Update();

                // Change y coordinate
                t += 0.1;
                y1 = y_0 + v_i * t + 0.5 * g * t * t;
                if (x1 > x_0 || (x1 == x_0 && x1 < 150))
                {
                    x1 += 2;
                }
                else
                {
                    x1 -= 2;
                }

                // Draw new jumper
                // LCD.SetFontColor(HONEYDEW);
                // LCD.DrawCircle(x1, y1, r);
                // LCD.FillCircle(x1, y1, r);
                astro.drawLeft(x1, y1);
                Sleep(10);
            }
            x_0 = x1, y1 = y_0, t = 0;
        }
        Sleep(10);
    }
}

int main()
{
    // Clear background
    LCD.SetBackgroundColor(BLACK);
    LCD.Clear();

    GravityTraining();

    while (1)
    {
        LCD.Update();
        // Never end
    }
    return 0;
}

Player::Player()
{
    rSprite.Open("protoustros_01.png");
    lSprite.Open("protoustros_02.png");
    rCloak.Open("protoustros_01_cloak.png");
    lCloak.Open("protoustros_02_cloak.png");
}

void Player::drawRight(int x, int y)
{
    rSprite.Draw(x, y);
}

void Player::drawLeft(int x, int y)
{
    lSprite.Draw(x, y);
}

void Player::clearRight(int x, int y)
{
    rCloak.Draw(x, y);
}

void Player::clearLeft(int x, int y)
{
    lCloak.Draw(x, y);
}