#include "FEHLCD.h"
#include "FEHUtility.h"
#include "FEHImages.h"
#include "Windows.h"
#include <time.h>
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
// Shreyash
struct Box
{
    /*
    Box Struct (Shreyash)

    This struct creates a collision box for different objects and
    checks if clicks or other boxes intersect with the box.
    */
    int x1, y1, x2, y2; // left, top, right, bottom

    // Constructor for the struct that sets the coordinates entered to the value of the top left and bottom right corners of the hit box
    Box(int x1_0, int y1_0, int x2_0, int y2_0)
    {
        x1 = x1_0, y1 = y1_0, x2 = x2_0, y2 = y2_0;
    }

    // Checks if the point x, y falls within the box, if it does true is returned
    bool PointIntersection(int x, int y)
    {
        return x >= x1 && x <= x2 && y >= y1 && y <= y2;
    }

    // Takes in the object box2, checks if two boxes collide, if they do, true is returned
    bool BoxIntersection(Box box2)
    {
        return (x2 >= box2.x1) && (x1 <= box2.x2) && (y2 >= box2.y1) && (y1 <= box2.y2);
    }

    // Checks if the end of the screen is reached
    bool ScreenIntersection()
    {
        return x1 < 0 || x2 > LCD_WIDTH || y2 > LCD_HEIGHT;
    }
};

// Creates the main platform as a box object
Box main_platform(40, 220, 280, 239);

class Button
{
    /*
    Button Class (Shreyash)

    This class is made to create the buttons on the menu. It takes their coordinates
    and any text associated and displays them to the screen. It also takes the button's
    function and when the button is pressed, the proper function is called.
    */
private:
    // Initializes the varibles for the x and y coordinates of the box, text, font colors, text array, and a function pointer
    int x, y, w, h, text_length, x_t, y_t;
    unsigned int border_color = WHITE, fill_color = BLACK, text_color = WHITE;
    char text[30];
    void (*function)();

public:
    // Constructor the button class and sets the values of width and height to 1
    Button(int w1 = 1, int h1 = 1) : w(w1), h(h1) {}

    // Takes in an x value and sets x coordinate to the value passed into the function
    void SetXCoord(int x1)
    {
        x = x1;
    }

    // Takes in an x value and sets x coordinate to the value passed into the function
    void SetYCoord(int y1)
    {
        y = y1;
    }

    // Takes in an x proportion and sets x prop to the value passed into the function
    void SetXProp(float x_prop)
    {
        // Takes the proportion (values 0-1) and multiplies it by LCD width then subtracts the width of the button / 2
        x = x_prop * LCD_WIDTH - w / 2;
    }

    // Takes in a y proportion and sets y prop to the value passed into the function
    void SetYProp(float y_prop)
    {
        // Takes the proportion (values 0-1) and multiplies it by LCD width then subtracts the height of the button / 2
        y = y_prop * LCD_HEIGHT - h / 2;
    }

    // Sets border color to color value passed into function
    void SetBorderColor(unsigned int color)
    {
        border_color = color;
    }

    // Sets fill color to color value passed into function
    void SetFillColor(unsigned int color)
    {
        fill_color = color;
    }

    // Sets text color to color value passed into function
    void SetTextColor(unsigned int color)
    {
        text_color = color;
    }

    // Takes in a char array and copies it into the text variable. Notes the length of the string.
    void SetText(char str[])
    {
        text_length = strlen(str);
        strcpy(text, str);
    }

    // Sets the width of the button to the width entered into the function
    void SetWidth(int w1)
    {
        w = w1;
    }

    // Sets the height of the button to the height entered into the function
    void SetHeight(int h1)
    {
        h = h1;
    }

    // Tells the button what its function is. When the button is pressed, the function taken in will be called
    void SetFunction(void funct())
    {
        function = funct;
    }

    // Displays the button to the screen
    void Create()
    {
        // Sets font color and draws the rectangle and fills it
        LCD.SetFontColor(fill_color);
        LCD.FillRectangle(x, y, w, h);
        // Sets font color and draws the border of the rectangle
        LCD.SetFontColor(border_color);
        LCD.DrawRectangle(x, y, w, h);
        // Centers the text inside the button
        x_t = x + (w - (text_length * CHAR_WIDTH)) / 2, y_t = y + (h - CHAR_HEIGHT) / 2;
        LCD.SetFontColor(text_color);
        LCD.WriteAt(text, x_t, y_t);
    }

    // Checks to see if a click was inside the button's area
    void Check(float x_press, float y_press)
    {
        // Creates a button object at the coordinates of the button the function is checking
        Box button(x, y, x + w, y + h);
        // Takes in x and y coordinates of click and checks to see if the click falls inside the button being checked
        if (button.PointIntersection(x_press, y_press))
        {
            // If the click was inside the button, the function for the button being checked is called
            function();
        }
    }
};

// Writes char array str[] in the center of the screen at height y (Shreyash)
void WriteCenter(char str[], int y)
{
    // Calculates how wide the text will be on the screen
    int length = strlen(str) * CHAR_WIDTH;
    // Finds the position x where the text will be centered and displays it to the screen
    int x = (LCD_WIDTH / 2) - length / 2;
    LCD.WriteAt(str, x, y);
}

// Blueprints
void Menu(), Play(), Credits(), Instructions(), Statistics(), Quit();

// Checks to see if the back button is pressed (Shreyash)
void BackButtonCheck()
{
    // Creates a back button at the button of the screen
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
    // Waits until the button is pressed
    while (true)
    {
        // Waits until the screen is clicked and tracks the coordinates of the click
        while (!LCD.Touch(&x, &y))
        {
        }
        // Waits until the click is released
        while (LCD.Touch(&x, &y))
        {
        }
        // Checks if the back button was pressed at this x and y coordinate
        backButton.Check(x, y);
    }
}

void Menu()
{
    /*
    SDP Menu Creation (Jack)

    Create a menu screen with 5 buttons the user can press. The options are
    Play, "Statistics", "Instructions", "Credits", or Quit.
    Each of the 3 info pages have a Back Button to return to Menu.
    */
    LCD.SetBackgroundColor(BLACK);
    LCD.Clear();

    LCD.SetFontColor(WHITE);
    WriteCenter("Astraia: A Last Stand", 10);
    WriteCenter("__________________________", 20);

    int w, h = 25, y_diff = 10, y_start = 50;
    float x_prop = 0.5;
    char labels[5][30] = {"Play", "Credits", "Instructions", "Statistics", "Quit"};
    void (*functions[5])() = {Play, Credits, Instructions, Statistics, Quit};

    Button menu_buttons[5];

    // Uses a for loop to create the buttons used for the menu
    for (int b = 0; b < 5; b++)
    {
        w = (strlen(labels[b]) + 2) * CHAR_WIDTH;
        menu_buttons[b].SetWidth(w);
        menu_buttons[b].SetHeight(h);
        menu_buttons[b].SetXProp(x_prop);
        menu_buttons[b].SetYCoord(y_start + b * (h + y_diff));
        menu_buttons[b].SetText(labels[b]);
        menu_buttons[b].SetBorderColor(LIGHTGRAY);
        menu_buttons[b].SetFillColor(DARKSLATEGRAY);
        menu_buttons[b].SetTextColor(LIGHTGRAY);
        menu_buttons[b].SetFunction(functions[b]);
        menu_buttons[b].Create();
    }
    // Runs until the user presses a button
    while (true)
    {
        // Creates values that track where the user presses on the screen
        int x, y;
        // Waits until a press is detected and stores the x, y coordinates in the ints x, y
        while (!LCD.Touch(&x, &y))
        {
        }
        // Waits until the press is released
        while (LCD.Touch(&x, &y))
        {
        }
        // Checks all 5 menu buttons to see if the x and y coordinates are within the coordinates of the button and executes their function if they were pressed
        for (int c = 0; c < 5; c++)
        {
            menu_buttons[c].Check(x, y);
        }
    }
}

// This is where the fun begins

class Bullet
{
    /*
    Bullet Class Creation (Jack)

    Creates the class for the bullets that sets their generic
    values and ensures that only one bullet is present on the
    screen at one time. It also tracks collisions with enemies
    and no longer draws the bullet if it collides.
    */
private:
    // The coordinates and the radius are initialized. The color is set to yellow. There are bools to see if the bullet is facing right and visible.
    float x, y, r = 1;
    unsigned int color = YELLOW;
    bool right_check, visible_check = false;

public:
    // The constructor for the bullet class takes in the x and y coordinates of player and checks if the player is moving right
    void Create(float x1, float y1, bool right)
    {
        // Sets the values of the bullet to the player's coordinates with a slight offset so the bullet appears outside the player. Sets the right check and visibility.
        x = x1, y = y1;
        right_check = right;
        visible_check = true;
    }

    // Returns the values of the top left and bottom right x, y coordinates
    Box CollisionBox()
    {
        return Box(x - r, y - r, x + r, y + r);
    }

    void Draw()
    {
        // If bullet goes off screen, it stops being visible
        if (CollisionBox().ScreenIntersection())
        {
            visible_check = false;
        }
        // LCD got cleared and bullet needs to be redrawn. It is also having it's position updated.
        if (visible_check)
        {
            LCD.SetFontColor(color);
            LCD.DrawCircle(x, y, r);
            // If bullet is moving right increment it by 5, otherwise decrement it by 5
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

    // If bullet collides with an enemy make it no longer visible
    void Kill()
    {
        visible_check = false;
    }

    // Returns if player is able to shoot another bullet
    bool Available()
    {
        return !visible_check;
    }
};

class Player
{
    /*
    Player Class Creation (Jack)

    Creates the class for the player that begins with
    initializing variables that track the player's state.
    The class then tracks the keyboard inputs to move
    the player across the screen and shoot bullets.
    The class also draws the player based on their orientation
    and animates the player's movement. It tracks if the player takes
    damage, their hit box, and returns if they're dead.

    */
private:
    /*
    Sets the amount of hearts the player has, their current health, their position,
    their current states including direction, jumping, shooting, death, and hits.
    The images for the players left and right animations as well as the hearts are imported
    and a bullet object is created.
    */
    const int total_health = 5;
    int health = total_health;
    float x, y, w, h;
    float t = 0, v_jump = -7, bullet_timer = 20, hit_timer = 0;
    bool right_check = true, jump_check = false, shoot_check = false, dead_check = false, hit_check = false;
    bool step_check = false, step_num = 0;
    FEHImage player_left_0, player_left_1, player_left_2, player_left_hit;
    FEHImage player_right_0, player_right_1, player_right_2, player_right_hit;
    FEHImage player_hearts, player_hits;
    Bullet b1;

public:
    // This constructor opens all possible images for the player and sets their basic values for coordinates
    Player()
    {
        w = 15, h = 55;
        x = 153, y = 220 - h;
        player_left_0.Open("astro_left_0.png");
        player_left_1.Open("astro_left_1.png");
        player_left_2.Open("astro_left_2.png");
        player_left_hit.Open("astro_left_hit.png");
        player_right_0.Open("astro_right_0.png");
        player_right_1.Open("astro_right_1.png");
        player_right_2.Open("astro_right_2.png");
        player_right_hit.Open("astro_right_hit.png");

        player_hearts.Open("heart_full.png");
        player_hits.Open("heart_empty.png");
    }

    // Returns the value of the hit box
    Box CollisionBox()
    {
        return Box(x, y, x + w, y + h);
    }

    // If the "D"/right-arrow key is pressed the player's x coordinate is increased
    void moveRight()
    {
        right_check = true;
        step_check = true;
        x += 2;
        // If the character is jumping the x coordinate will increase extra to allow for dodging enemies
        if (jump_check)
        {
            x += 1;
        }
    }

    // If the "A"/left-arrow key is pressed the player's x coordinate is decreased
    void moveLeft()
    {
        right_check = false;
        step_check = true;
        x -= 2;
        // If the character is jumping the x coordinate will decrease extra to allow for dodging enemies
        if (jump_check)
        {
            x -= 1;
        }
    }

    // If the "W"/up-arrow key is pressed the player's y coordinate is increased
    void jump()
    {
        // Ensures that the player can't jump while already in the air
        if (!jump_check)
        {
            jump_check = true;
            t = 0.1;
            y += v_jump * t + 0.5 * GRAVITY * t * t;
        }
    }

    // If the "CTRL" key is pressed the player shoots a bullet
    void shoot()
    {
        // If the bullet has been moving for 30 iterations or less
        if (bullet_timer > 30)
        {
            // Creates the bullet to the right and shoots it right
            if (right_check)
            {

                b1.Create(x + w, y + 28, true);
            }
            // Creates the bullet to the left and shoots it left
            else
            {

                b1.Create(x, y + 28, false);
            }
            bullet_timer = 0;
        }
    }

    // If the player is hit by the enemy, the health decreases by 1 heart and that enemy can no longer hurt the player
    void Hit()
    {
        health--;
        hit_check = true;
    }

    // Checks to see if the player is dead and returns true if they are
    bool Dead()
    {
        return health == 0;
    }

    void Draw()
    {
        // draw shot bullet
        b1.Draw();
        bullet_timer++;
        // calculate change in y coordinates

        // If the player is not touching the platform
        if (!CollisionBox().BoxIntersection(main_platform))
        {
            t += 0.1;
            if (jump_check)
            {
                // Equation for gravity derived from kinematics
                y += v_jump * t + 0.5 * GRAVITY * t * t;
            }
            // Allows the player to fall off the platform if they aren't jumping
            else
            {
                y += 0.5 * GRAVITY * t * t;
            }

            // If player goes off end of screen
            if (CollisionBox().ScreenIntersection())
            {
                // Decrement their health and respawn them in the middle of the screen
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
        if (health > 0)
        {
            if (right_check)
            {
                // If player is hit and the hit animation is within 10 iterations (100ms)
                if (hit_check && hit_timer < 10)
                {
                    hit_timer++;
                    player_right_hit.Draw(x, y);
                }
                else
                {
                    hit_check = false;
                    hit_timer = 0;
                    if (step_check)
                    {
                        step_check = false;
                        if (step_num)
                        {
                            // Lifts the first leg and causes legs to alternate
                            step_num = false;
                            player_right_1.Draw(x, y);
                        }
                        else
                        {
                            // Lifts the second leg and causes legs to alternate
                            step_num = true;
                            player_right_2.Draw(x, y);
                        }
                    }
                    // Doesn't keep stepping after player has moved
                    else
                    {
                        player_right_0.Draw(x, y);
                    }
                }
            }
            else
            {
                // If player is hit and the hit animation is within 10 iterations (100ms)
                if (hit_check && hit_timer < 10)
                {
                    hit_timer++;
                    player_left_hit.Draw(x, y);
                }
                else
                {
                    hit_check = false;
                    if (step_check)
                    {
                        step_check = false;
                        if (step_num)
                        {
                            // Lifts the first leg and causes legs to alternate
                            step_num = false;
                            player_left_1.Draw(x, y);
                        }
                        else
                        {
                            // Lifts the second leg and causes legs to alternate
                            step_num = true;
                            player_left_2.Draw(x, y);
                        }
                    }
                    // Doesn't keep stepping after player has moved
                    else
                    {
                        player_left_0.Draw(x, y);
                    }
                }
            }
        }

        // Draw health indicating hearts
        LCD.SetFontColor(DARKSLATEGRAY);
        LCD.DrawRectangle(1, 232, 318, 7);
        LCD.FillRectangle(1, 232, 318, 7);
        // Draw hearts
        for (int h = 0; h < total_health; h++)
        {
            // Draw red hearts
            if (h < health)
            {
                player_hearts.Draw(7 * h + 1, 232);
            }
            // Draw brown hearts
            else
            {
                player_hits.Draw(7 * h + 1, 232);
            }
        }
    }

    // Returns top left and bottom right x,y coordinates of bullet
    Box BulletCollisionBox()
    {
        return b1.CollisionBox();
    }

    // Stops drawing bullet and allows you to shoot another bullet
    void BulletKill()
    {
        b1.Kill();
    }

    // Checks if a bullet is available to allow player to shoot again. Should also check if bullet exists.
    bool BulletCheck()
    {
        return !b1.Available();
    }
};

// Shreyash
/*
  Enemy Class is a parent class that contains all the common behavior for all enemy types.
  Contains functions CollisionBox(), Hit(), PlayerHit(), CheckPlayerHit(), Dead(), Death(),
  and Draw().
*/
class Enemy
{
// Uses protected to ensure that extending classes are able to access variables
protected:
    // dead_counter tracks which picture in the 3-picture death animation to show
    // dead_timer spaces the death animation transitions out to make the animation human visible
    // t_frames spaces the moving animation out
    // frames_speed controls how long the moving animations last
    int dead_counter = 0, dead_timer = 0, t_frames = 0, frames_speed;
    // x, y store the coordinates of the top-left corner of the sprite
    // t is used to control the gravity effects on the sprite
    float x, y, w, h, health, speed, t = 0;
    // right_check stores the direction the sprite faces: true = right, false = left
    // hit_check stores whether the enemy has hit the player or not (can only hit once)
    // frames_check uses t_frames and frames_speed to calculate which image to display for motion
    bool right_check, hit_check = false, frames_check = false;
    // Moving enemy images
    FEHImage enemy_left_1, enemy_left_2, enemy_right_1, enemy_right_2;
    // Death animation images
    FEHImage enemy_dead_left_1, enemy_dead_left_2, enemy_dead_left_3;
    FEHImage enemy_dead_right_1, enemy_dead_right_2, enemy_dead_right_3;

public:
    /*
      Returns an instance of the Box struct, containing the bounding box of the enemy.
    */
    Box CollisionBox()
    {
        return Box(x, y, x + w, y + h);
    }

    /*
      Decrements the health of the enemy. (returns nothing)
    */
    void Hit()
    {
        health--;
    }

    /*
      Sets hit_check to true, i.e., the enemy has hit the player. (returns nothing)
    */
    void PlayerHit()
    {
        hit_check = true;
    }

    /*
      Returns whether or not the enemy has hit the player.
    */
    bool CheckPlayerHit()
    {
        return hit_check;
    }

    /*
      Returns whether or not the Death Animation is complete.
    */
    bool Dead()
    {
        return dead_counter == 3;
    }

    /*
      If the enemy has fallen off the platform, skips the death animation.
      If enemy has 0 health left (or less), plays the death animation,
      closing all images upon completion.
      Else if neither is true, enemy is not dead, so death_check is set to false.
      Returns whether or not enemy is dead via death_check.
    */
    bool Death()
    {
        bool death_check = true;
        if (CollisionBox().ScreenIntersection())
        {
            dead_counter = 3;
        }
        else if (health <= 0)
        {
            if (dead_counter < 3)
            {
                if (dead_timer == 0)
                {
                    dead_counter++;
                    if (dead_counter == 1)
                    {
                        if (right_check)
                        {
                            enemy_dead_right_1.Draw(x, y);
                        }
                        else
                        {
                            enemy_dead_left_1.Draw(x, y);
                        }
                    }
                    else if (dead_counter == 2)
                    {
                        if (right_check)
                        {
                            enemy_dead_right_2.Draw(x, y);
                        }
                        else
                        {
                            enemy_dead_left_2.Draw(x, y);
                        }
                    }
                    else if (dead_counter == 3)
                    {
                        if (right_check)
                        {
                            enemy_dead_right_3.Draw(x, y);
                        }
                        else
                        {
                            enemy_dead_left_3.Draw(x, y);
                        }
                    }
                }
                dead_timer = (dead_timer + 1) % 10;
            }
            if (dead_counter == 3)
            {
                enemy_right_1.Close();
                enemy_right_2.Close();
                enemy_left_1.Close();
                enemy_left_2.Close();
                enemy_dead_right_1.Close();
                enemy_dead_right_2.Close();
                enemy_dead_right_3.Close();
                enemy_dead_left_1.Close();
                enemy_dead_left_2.Close();
                enemy_dead_left_3.Close();
            }
        }
        else
        {
            death_check = false;
        }
        return death_check;
    }

    /*
      Draws the enemy and updates coordinates according to motion logic. (returns nothing)
    */
    void Draw()
    {
        // If the enemy is not touching the platform, calculates the change in y according
        // to game physics.
        if (!CollisionBox().BoxIntersection(main_platform))
        {
            t += 0.1;
            y += 0.5 * GRAVITY * t * t;
        }
        else
        {
            t = 0;
            // Sets y to just above the platform to ensure the enemy doesn't get stuck in it
            // because of high velocity falling.
            y = 220 - h;
        }

        // Checks if enemy is dead or not; processes death animation logic by calling Death()
        if (!Death())
        {
            // Changes the sprite of the enemy every frames_speed no. of update cycles
            // Updates the x coordinates by the speed specified
            frames_check = t_frames / frames_speed != 0;
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
                x += speed;
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
                x -= speed;
            }
            // Increments the t_frames frame timer in the base that is twice as much as the frames_speed
            // because there are two sprites to cycle between
            t_frames = (t_frames + 1) % (frames_speed * 2);
        }
    }
};

// Shreyash
/*
  Child class extending Enemy, controls the specific behavior of the regular enemy.
*/
class EnemyRegular : public Enemy
{
public:
    /*
      Constructor.
      Parameter right can be used to control which direction the enemy will come from.
      Opens the appropriate images for the regular enemy type.
      Sets the width and height according to the images being opened.
      Sets the health to 1 (requires 1 hit to die).
      Sets speed to 0.5 (medium).
      Sets the initial spawn points to above the screen so that the enemy falls down from either side.
    */
    EnemyRegular(bool right)
    {
        w = 20;
        h = 40;
        enemy_left_1.Open("xenos_left_1.png");
        enemy_left_2.Open("xenos_left_2.png");
        enemy_right_1.Open("xenos_right_1.png");
        enemy_right_2.Open("xenos_right_2.png");
        enemy_dead_right_1.Open("xenos_dead_right_1.png");
        enemy_dead_left_1.Open("xenos_dead_left_1.png");
        enemy_dead_right_2.Open("xenos_dead_right_2.png");
        enemy_dead_left_2.Open("xenos_dead_left_2.png");
        enemy_dead_right_3.Open("xenos_dead_right_3.png");
        enemy_dead_left_3.Open("xenos_dead_left_3.png");
        right_check = right;
        health = 1;
        speed = 0.5;
        frames_speed = 10;
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
};

/*
  Child class extending Enemy, controls the specific behavior of the armored enemy.
*/
class EnemyArmored : public Enemy
{
public:
    /*
      Constructor.
      Parameter right can be used to control which direction the enemy will come from.
      Opens the appropriate images for the armored enemy type.
      Sets the width and height according to the images being opened.
      Sets the health to 5 (requires 5 hits to die).
      Sets speed to 0.3 (slow).
      Sets the initial spawn points to above the screen so that the enemy falls down from either side.
    */
    EnemyArmored(bool right)
    {
        w = 20;
        h = 40;
        enemy_left_1.Open("xenos_left_armored_1.png");
        enemy_left_2.Open("xenos_left_armored_2.png");
        enemy_right_1.Open("xenos_right_armored_1.png");
        enemy_right_2.Open("xenos_right_armored_2.png");
        enemy_dead_right_1.Open("xenos_dead_right_1.png");
        enemy_dead_left_1.Open("xenos_dead_left_1.png");
        enemy_dead_right_2.Open("xenos_dead_right_2.png");
        enemy_dead_left_2.Open("xenos_dead_left_2.png");
        enemy_dead_right_3.Open("xenos_dead_right_3.png");
        enemy_dead_left_3.Open("xenos_dead_left_3.png");
        right_check = right;
        health = 5;
        speed = 0.3;
        frames_speed = 10;
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
};

/*
  Child class extending Enemy, controls the specific behavior of the fast enemy.
*/
class EnemyFast : public Enemy
{
public:
    /*
      Constructor.
      Parameter right can be used to control which direction the enemy will come from.
      Opens the appropriate images for the fast enemy type.
      Sets the width and height according to the images being opened.
      Sets the health to 1 (requires 1 hit to die).
      Sets speed to 1 (fast).
      Sets the initial spawn points to above the screen so that the enemy falls down from either side.
    */
    EnemyFast(bool right)
    {
        w = 20;
        h = 40;
        enemy_left_1.Open("xenos_left_fast_1.png");
        enemy_left_2.Open("xenos_left_fast_2.png");
        enemy_right_1.Open("xenos_right_fast_1.png");
        enemy_right_2.Open("xenos_right_fast_2.png");
        enemy_dead_right_1.Open("xenos_dead_right_1.png");
        enemy_dead_left_1.Open("xenos_dead_left_1.png");
        enemy_dead_right_2.Open("xenos_dead_right_2.png");
        enemy_dead_left_2.Open("xenos_dead_left_2.png");
        enemy_dead_right_3.Open("xenos_dead_right_3.png");
        enemy_dead_left_3.Open("xenos_dead_left_3.png");
        right_check = right;
        health = 1;
        speed = 1;
        frames_speed = 5;
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
};

// A global vector that contains all the enemy objects
vector<Enemy *> enemies;

/*
  Helper function to randomly spawn the regular enemy from left or right.
*/
void SpawnEnemyRegular()
{
    bool random_num = rand() % 2;
    EnemyRegular *new_enemy = new EnemyRegular(random_num);
    enemies.push_back(new_enemy);
}

/*
  Helper function to randomly spawn the armored enemy from left or right.
*/
void SpawnEnemyArmored()
{
    bool random_num = rand() % 2;
    EnemyArmored *new_enemy = new EnemyArmored(random_num);
    enemies.push_back(new_enemy);
}

/*
  Helper function to randomly spawn the fast enemy from left or right.
*/
void SpawnEnemyFast()
{
    bool random_num = rand() % 2;
    EnemyFast *new_enemy = new EnemyFast(random_num);
    enemies.push_back(new_enemy);
}

// Jack
// This is the function that runs the main game
void Play()
{
    // Clears the main menu and sets initial values
    LCD.Clear();
    Player player;
    FEHImage Background;
    Background.Open("background.png");
    int kill_count = 0, final_time = 0;
    int time_0 = time(NULL), time_c;
    bool second_over_regular = true;
    bool second_over_armored = true;
    bool second_over_fast = true;
    bool second_over_regular_2 = true;
    bool second_over_armored_2 = true;
    bool second_over_fast_2 = true;
    while (true)
    {
        // Gets the current time and draws the background
        time_c = time(NULL) - time_0;
        LCD.Clear();
        Background.Draw(1, 1);
        LCD.SetFontColor(WHITE);
        LCD.DrawRectangle(40, 220, 240, 19);
        LCD.FillRectangle(40, 220, 240, 19);

        // If "A" or left arrow key is clicked, the moveLeft function is called
        if ((GetAsyncKeyState(0x41) | GetAsyncKeyState(0x25)) & 0x8000)
        {
            player.moveLeft();
        }

        // If "D" or right arrow key is clicked, the moveRight function is called
        if ((GetAsyncKeyState(0x44) | GetAsyncKeyState(0x27)) & 0x8000)
        {
            player.moveRight();
        }

        // If "W" or up arrow key is clicked, the jump function is called
        if ((GetAsyncKeyState(0x57) | GetAsyncKeyState(0x26)) & 0x8000)
        {
            player.jump();
        }

        // If "CTRL" is clicked, the shoot function is called
        if (GetAsyncKeyState(0x11) & 0x8000)
        {
            player.shoot();
        }
        player.Draw();

        // If 4 seconds have passed, new regular enemy is spawned
        if (time_c % 4 == 0 && second_over_regular)
        {
            SpawnEnemyRegular();
            second_over_regular = false;
        }
        if (time_c % 4 == 1)
        {
            second_over_regular = true;
        }

        // After 20 seconds, every 7 seconds, the armored enemy will spawn
        if (time_c > 20 && time_c % 7 == 0 && second_over_armored)
        {
            SpawnEnemyArmored();
            second_over_armored = false;
        }
        if (time_c % 7 == 1)
        {
            second_over_armored = true;
        }

        // After 45 seconds, and every 10 seconds, the fast enemy will spawn
        if (time_c > 45 && time_c % 10 == 0 && second_over_fast)
        {
            SpawnEnemyFast();
            second_over_fast = false;
        }
        if (time_c % 10 == 1)
        {
            second_over_fast = true;
        }

        // After 60 seconds, the regular enemy will spawn every 3 seconds
        if (time_c > 60 && time_c % 3 == 0 && second_over_regular_2)
        {
            SpawnEnemyRegular();
            second_over_regular_2 = false;
        }
        if (time_c % 3 == 1)
        {
            second_over_regular_2 = true;
        }

        // After 90 seconds, every 8 seconds, the armored enemy will spawn
        if (time_c > 90 && time_c % 8 == 0 && second_over_armored_2)
        {
            SpawnEnemyArmored();
            second_over_armored_2 = false;
        }
        if (time_c % 8 == 1)
        {
            second_over_armored_2 = true;
        }

        // After 150 seconds, and every 5 seconds, the fast enemy will spawn
        if (time_c > 150 && time_c % 5 == 0 && second_over_fast_2)
        {
            SpawnEnemyFast();
            second_over_fast_2 = false;
        }
        if (time_c % 5 == 1)
        {
            second_over_fast_2 = true;
        }

        // Removed dead enemies from the vector
        for (int i = 0; i < enemies.size(); i++)
        {
            if (enemies[i]->Dead())
            {
                enemies.erase(enemies.begin() + i--);
            }
        }

        // Runs for entire enemy vector
        for (Enemy *enemy : enemies)
        {
            enemy->Draw();
            // If enemy collides with the bullet and the bullet is supposed to be there
            if (enemy->CollisionBox().BoxIntersection(player.BulletCollisionBox()) && player.BulletCheck())
            {
                kill_count++;
                enemy->Hit();
                player.BulletKill();
            }

            // If the enemy collides with the player and hasn't already hit the player before
            if (enemy->CollisionBox().BoxIntersection(player.CollisionBox()) && !enemy->CheckPlayerHit())
            {
                enemy->PlayerHit();
                player.Hit();
            }
        }

        // If player has no more lives, the final time is noted for the stats and the game is stopped
        if (player.Dead())
        {
            final_time = time(NULL) - time_0;
            enemies.clear();
            break;
        }

        // Slight pause to keep the while loop from overloading
        Sleep(10);
    }

    // Score is calculated
    int score = kill_count * 7 + final_time;
    int high_score, total_kill_count, best_time;

    // Reading values from stats file
    ifstream high_score_in;
    high_score_in.open("statistics.txt");
    high_score_in >> high_score;
    high_score_in >> total_kill_count;
    high_score_in >> best_time;
    high_score_in.close();

    // If player gets a new highscore, replace old highscore
    if (score > high_score)
    {
        high_score = score;
    }
    if (final_time > best_time)
    {
        best_time = final_time;
    }
    total_kill_count += kill_count;

    // Writing the values to the highscore out file
    ofstream high_score_out;
    high_score_out.open("statistics.txt");
    high_score_out << high_score << endl;
    high_score_out << total_kill_count << endl;
    high_score_out << best_time;
    high_score_out.close();

    // Clear screen and display the score
    LCD.Clear();
    LCD.SetFontColor(WHITE);

    char score_text[30];
    char high_score_text[30];
    char kills_text[30];
    char time_text[30];

    // Stores the text as a string
    sprintf(score_text, "Score: %i", score);
    sprintf(high_score_text, "High Score: %i", high_score);
    sprintf(kills_text, "Kills: %i", kill_count);
    sprintf(time_text, "Time: %i", final_time);

    // Displays the high scores to the screen and the current scores as well
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
    WriteCenter("Use W A S or arrow keys to", 50);
    WriteCenter("move and CTRL to shoot.", 70);
    WriteCenter("Kill enemies and survive", 90);
    WriteCenter("as long as possible to get", 110);
    WriteCenter("a high score.", 130);
    WriteCenter("You have 5 hearts.", 150);
    WriteCenter("Good luck!", 150);

    // Create and check a back button to return to the main menu
    BackButtonCheck();
}

void Statistics()
{
    /*
      Read in the values from the high score file. Stores them as strings
      and displays them to the screen.
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
    // This creates a button that quits the program and closes the simulator
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
