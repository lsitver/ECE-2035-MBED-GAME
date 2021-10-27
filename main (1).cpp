//=================================================================
// The main program file.
//
// Copyright 2020 Georgia Tech.  All rights reserved.
// The materials provided by the instructor in this course are for
// the use of the students currently enrolled in the course.
// Copyrighted course materials may not be further disseminated.
// This file must not be made publicly available anywhere.
//==================================================================
 
// Project includes
#include "globals.h"
#include "hardware.h"
#include "map.h"
#include "graphics.h"
#include "snake.h"
 
#include <math.h>
#include<stdio.h>
 
#define CITY_HIT_MARGIN 1
#define CITY_UPPER_BOUND (SIZE_Y-(LANDSCAPE_HEIGHT+MAX_BUILDING_HEIGHT))
 
// Helper function declarations
void playSound(char* wav);
 
/**
 * The main game state. Must include snake locations and previous locations for
 * drawing to work properly. Other items can be added as needed.
 */
 
/**
 * Given the game inputs, determine what kind of update needs to happen.
 * Possbile return values are defined below.
 */
Snake snake;
 
// Function prototypes
 
/**
 * Given the game inputs, determine what kind of update needs to happen.
 * Possible return values are defined below.
 */
#define NO_RESULT 0
#define NO_ACTION 0
#define ACTION_BUTTON 1
#define MENU_BUTTON 2
#define GO_LEFT 3
#define GO_RIGHT 4
#define GO_UP 5
#define GO_DOWN 6
#define GAME_OVER 7
#define FULL_DRAW 8
#define ATE_GOODIE 55
#define WINNER 9
#define PAUSE_GAME 11
#define HELP_MENU 13
// Function prototypes
int go_right(int x, int y);
int go_left(int x, int y);
int go_up(int x, int y);
int go_down(int x, int y);

int invincibility_button = 0;
int pause = 0;
int help_menu = 0;

int play = 1;
void draw_opening_act();

// Get Actions from User (push buttons & accelerometer)
// Based on push button and accelerometer inputs, determine which action
// needs to be performed (may be no action).
int get_action(GameInputs inputs)
{  
//accessed buttons to return their specific action
    if (!inputs.b2){
        invincibility_button =!invincibility_button;
        }
    if(!inputs.b3){
        pause = !pause;
        return PAUSE_GAME;
        }
    if(!inputs.b1){
        return HELP_MENU;
        }
//set bounds on how far you can move the board one way or another to better control my board
//my ranges for going up and down are opposite of what they should be because my accelerometer was wired differently
   if (inputs.ay >= 0.10 && inputs.ay <= 0.50){
        return GO_DOWN;
        }
    if (inputs.ay < -0.10 && inputs.ay > -0.50){
        return GO_UP;
        }
    if (inputs.ax < -0.10 && inputs.ax > -0.70){
        return GO_RIGHT;
        }
    if (inputs.ax >= 0.10 && inputs.ax <= 0.70){
        return GO_LEFT;
        }
//once you have 0 lives ->  game over
    if(snake.lives <= 0){
        return GAME_OVER;
        }
//if you reach the set max score -> winner
    if(snake.score >= 25){
        return WINNER;
        }
    else{
        return 0;
        }
}

/**
 * Update the game state based on the user action. For example, if the user
 * requests GO_UP, then this function should determine if that is possible by
 * consulting the map, and update the snake position accordingly.
 *
 * Return values are defined below. FULL_DRAW indicates that for this frame,
 * draw_game should not optimize drawing and should draw every tile, even if
 * the snake has not moved.
 */
int update_game(int action)
{
    snake.head_px = snake.head_x;//set current location to previous
    snake.head_py = snake.head_y;
    switch(action) {//checks all cases
        case GO_UP:
            if (go_up(snake.head_px,snake.head_py)){
                return GO_UP;
                }
            else break;
        case GO_LEFT:
            if (go_left(snake.head_px,snake.head_py)){
                return GO_LEFT;
                }
            else break;
        case GO_DOWN:
            if (go_down(snake.head_px,snake.head_py)){
                return GO_DOWN;
                }
            else break;
        case GO_RIGHT:
            if (go_right(snake.head_px,snake.head_py)){
                return GO_RIGHT;
                }
            else break;
        case GAME_OVER:
            return GAME_OVER;
        case WINNER:
            return WINNER;
        case PAUSE_GAME:
            return PAUSE_GAME;
        case HELP_MENU:
            return HELP_MENU;
        default:
            break;
    }
return 0;
}
 
/**
 * Draw the upper status bar.
 */
void draw_upper_status()
{   
    uLCD.color(WHITE);
    uLCD.text_width(1);
    uLCD.text_height(1);
    uLCD.textbackground_color(BLACK);
    uLCD.line(0, 9, 127, 9, GREEN);
    //these if statements make sure their is a blank space for number 0-9 and that space is alotted for all double digit coordinates
    if (snake.locations[0].x<10){
        uLCD.locate(5,0);
        uLCD.printf(" ");
        }
    uLCD.locate(0,0);
    uLCD.printf("Pos:%d",snake.locations[0].x);
    if (snake.locations[0].y<10){
        uLCD.locate(8,0);
        uLCD.printf(" ");
        }
    uLCD.locate(6,0);
    uLCD.printf(",%d", snake.locations[0].y);
    if (snake.locations[0].y<10){
        uLCD.locate(8,0);
        uLCD.printf(" ");
        }
    uLCD.locate(10,0);
    uLCD.printf("Score:%d",snake.score);
}

 
/**
 * Draw the lower status bar.
 */
void draw_lower_status()
{
    uLCD.text_width(1);
    uLCD.text_height(1);
    uLCD.textbackground_color(BLACK);
    uLCD.line(0, 9, 127, 9, 0x008080);
    uLCD.locate(1,15);
    uLCD.printf(" Lives: %d",snake.lives);//displays updated num of lives left
    uLCD.line(0, 118, 127, 118, GREEN);
}
 
/**
 * Draw the border for the map.
 */
void draw_border()
{
    uLCD.filled_rectangle(0,     9, 127,  14, WHITE); // Top
    uLCD.filled_rectangle(0,    13,   2, 114, WHITE); // Left
    uLCD.filled_rectangle(0,   114, 127, 117, WHITE); // Bottom
    uLCD.filled_rectangle(124,  14, 127, 117, WHITE); // Right
}
 
/**
 * Entry point for frame drawing. This should be called once per iteration of
 * the game loop. This draws all tiles on the screen, followed by the status
 * bars. Unless init is nonzero, this function will optimize drawing by only
 * drawing tiles that have changed from the previous frame.
 */

void draw_game(int draw_option)
{
    if(draw_option == 11){//draw Pause Button. I didn't include a background function so that the words will be transparent on the game screen
        uLCD.text_width(2);
        uLCD.text_height(2);
        uLCD.color(GREEN);
    
        uLCD.locate(2,2);
        uLCD.text_bold(TEXTBOLD);
        uLCD.printf("PAUSE");
        uLCD.locate(3,3);
        uLCD.text_bold(TEXTBOLD);
        uLCD.printf("GAME");
        wait(5);
}
    if(draw_option == 13){//draw Key
        
        uLCD.filled_rectangle(0,0,255,255,BLACK);
        uLCD.text_width(2);
        uLCD.text_height(2);
        uLCD.color(GREEN);
    
        uLCD.locate(0,0);
        uLCD.text_bold(TEXTBOLD);
        uLCD.printf("KEY BUFF");
        draw_present(10,30);
        draw_buff1(10,62);
        draw_buff2(10,95);
        
        uLCD.text_width(1);
        uLCD.text_height(1);
        uLCD.color(WHITE);
    
        uLCD.locate(6,4);
        uLCD.printf("-1 length");
        uLCD.locate(6,8);
        uLCD.printf("+5 points");
        uLCD.locate(6,12);
        uLCD.printf("+1 life");
        wait(4);
        uLCD.filled_rectangle(0,0,255,255,BLACK);
        uLCD.text_width(2);
        uLCD.text_height(2);
        uLCD.color(GREEN);
    
        uLCD.locate(0,0);
        uLCD.text_bold(TEXTBOLD);
        uLCD.printf("DEBUFF");
        draw_debuff2(10,30);
        draw_ghost(10,62);
        draw_debuff1(10,95);
        
        uLCD.text_width(1);
        uLCD.text_height(1);
        uLCD.color(WHITE);
    
        uLCD.locate(6,4);
        uLCD.printf("+2 length");
        uLCD.locate(6,8);
        uLCD.printf("-5 points");
        uLCD.locate(6,12);
        uLCD.printf("-1 life");
        
        wait(4);
        uLCD.filled_rectangle(0,0,255,255,BLACK);
        draw_border();
}
        
    if (draw_option == 7) {// draw game over screen
        uLCD.filled_rectangle(0,0,255,255,BLACK);
        uLCD.locate(5,4);
        uLCD.textbackground_color(BLACK);
        uLCD.text_width(1);
        uLCD.text_height(3);
        uLCD.color(RED);
        uLCD.printf("GAME OVER");
        uLCD.text_height(1);
        uLCD.text_width(1);
        uLCD.color(WHITE);
        uLCD.locate(2,11);
        uLCD.printf("Final Score:%d", snake.score);
        uLCD.color(RED);
        uLCD.locate(2,8);
        uLCD.printf("Sofia is still\n  sassy");
        uLCD.color(WHITE);
        uLCD.locate(2,13);
        uLCD.printf("try again :)");
       
        wait(100000);
    }
    if (draw_option == 9) {//draw winner screen
        uLCD.filled_rectangle(0,0,255,255,0x46416C);
        uLCD.locate(5,4);
        uLCD.textbackground_color(0x46416C);
        uLCD.text_width(1);
        uLCD.text_height(3);
        uLCD.color(0xE3E34E);
        uLCD.printf("YOU WON!");
        uLCD.text_height(1);
        uLCD.text_width(1);
        uLCD.color(0xE3E34E);
        uLCD.locate(2,11);
        uLCD.printf("Final Score:%d", snake.score);
        uLCD.color(WHITE);
        uLCD.locate(2,8);
        uLCD.printf("Sofia is nice\n  now");
        uLCD.color(WHITE);
        uLCD.locate(2,13);
        uLCD.printf("play again :)");
       
        wait(10000);
    }
    // Draw game border first
    if(draw_option == FULL_DRAW) 
    {
        draw_border();
        int u = 58;
        int v = 56;
        draw_snake_head_east(u, v);
        draw_snake_body_horizontal(u-11, v);
        draw_snake_tail_horizontal(u-22, v);
        return;
    }
    

    // Iterate over all visible map tiles
    for (int i = -5; i <= 5; i++) { // Iterate over columns of tiles
        for (int j = -4; j <= 4; j++) { // Iterate over one column of tiles
            // Here, we have a given (i,j)
 
            // Compute the current map (x,y) of this tile
            int x = i + snake.head_x;
            int y = j + snake.head_y;
 
            // Compute the previous map (px, py) of this tile
            int px = i + snake.head_px;
            int py = j + snake.head_py;
 
            // Compute u,v coordinates for drawing
            int u = (i+5)*11 + 3;
            int v = (j+4)*11 + 15;
 
            /*if (draw_option && i == 0 && j == 0) {
                draw_snake_head(snake.locations[0].x, snake.locations[0].y);
                draw_snake_body(snake.locations[1].x, snake.locations[1].y);
                for(int part = 2; part < snake.length; part++){
                    draw_snake_tail(snake.locations[part].x, snake.locations[part].y);
                    }
            }*/
    
            // Figure out what to draw
            DrawFunc draw = NULL;
            if (x >= 0 && y >= 0 && x < map_width() && y < map_height()) { // Current (i,j) in the map
                MapItem* curr_item = get_here(x, y);
                MapItem* prev_item = get_here(px, py);
                if (draw_option || curr_item != prev_item) { // Only draw if they're different
                    if (curr_item) { // There's something here! Draw it
                        draw = curr_item->draw;
                    } else { // There used to be something, but now there isn't
                        draw = draw_nothing;
                    }
                } else if (curr_item && curr_item->type == CLEAR) {
                    // This is a special case for erasing things like doors.
                    draw = curr_item->draw; // i.e. draw_nothing
                }
            } else if (draw_option) { // If doing a full draw, but we're out of bounds, draw the walls.
                draw = draw_wall;
            }
 
            // Actually draw the tile
            if (draw) draw(u, v);
        }
    }
 
    // Draw status bars
    draw_upper_status();
    draw_lower_status();
}
 
/**
 * Initialize the main world map. Add walls around the edges, interior chambers,
 * and plants in the background so you can see motion.
 */
void init_main_map()
{
   draw_opening_act();//start screen
    // "Random" plants
    Map* map = set_active_map(0);
    for(int i = map_width() + 3; i < map_area(); i += 39) {
        add_goodie(i % map_width(), i / map_width());
    }
    pc.printf("plants\r\n");
 
    pc.printf("Adding walls!\r\n");
    add_wall(0,              0,              HORIZONTAL, map_width());
    add_wall(0,              map_height()-1, HORIZONTAL, map_width());
    add_wall(0,              0,              VERTICAL,   map_height());
    add_wall(map_width()-1,  0,              VERTICAL,   map_height());
    pc.printf("Walls done!\r\n");
    
    add_snake_head_east(snake.locations[0].x, snake.locations[0].y);
    add_snake_body_horizontal(snake.locations[1].x, snake.locations[1].y);
    add_snake_tail_horizontal(snake.locations[2].x, snake.locations[2].y);
    
    pc.printf("Add extra chamber\r\n");
    add_wall(30, 0, VERTICAL, 10);
    add_wall(30, 10, HORIZONTAL, 10);
    add_wall(39, 0, VERTICAL, 10);
    pc.printf("Added!\r\n");
    
    //added in locations of all buffs/debuffs
    add_ghost(2,5);
    add_ghost(31, 8);
    add_ghost(22,54);
    add_ghost(29,40);
    add_ghost(4,29);
    add_ghost(7,15);
   
   add_present(32,48);
    add_present(17,6);
    add_present(3,19);
   add_present(9,35);
    add_present(57,49);
    add_present(22,3);
    add_present(8,3);
   add_present(41,17);
   
    add_buff1(2, 27);
    add_buff1(33,53);
    add_buff1(15, 3);
    add_buff1(19, 39);
    add_buff1(28,5);
    add_buff1(42, 14);
    add_buff1(35,13);
    add_buff1(29,12);
    add_buff1(50,44);
    
    
    add_buff2(55,47);
    add_buff2(9, 12);
    add_buff2(4,50);
    add_buff2(46, 22);
    add_buff2(26, 33);
    add_buff2(15,25);
    
    add_debuff1(44, 4);
    add_debuff1(58, 49);
    add_debuff1(18, 30);
    add_debuff1(2,10);
    add_debuff1(2,15);
    add_debuff1(26,48);
    add_debuff1(33,7);
    
    add_debuff2(3,6);
    add_debuff2(6,22);
    add_debuff2(37, 58);
    add_debuff2(15, 27);
    add_debuff2(49, 50);
    add_debuff2(53, 13);
    // Add stairs to chamber (map 1)
    //add_stairs(15, 5, 1, 5, 5);
 
//    profile_hashtable();
    print_map();
}
//ADDED FUNCTIONS FOR MOVEMENT
//all four functions are for the 4 different directions N,E,W,S 
int go_right(int x, int y){
    MapItem *itemEast = get_east(x, y); //go east
    if (itemEast->walkable ) { // check if walkable 
        snake.head_x= snake.head_x+1;
        //move x position 1 to the right
        //remove the spot that the snake just passed over and leave it clear
        remove_snake_tail_horizontal(snake.locations[snake.length-1].x, snake.locations[snake.length-1].y);
        //check what type itemEast is and do the following conditions
        if (itemEast->type == GOODIE){
             snake.length += 1;
             snake.score += 1;
             }
        if (itemEast->type == PRESENT){
             if(snake.length>=4){
             remove_snake_tail_horizontal(snake.locations[snake.length-2].x, snake.locations[snake.length-2].y);
             snake.length -= 1;
             }
             }
        if (itemEast->type == BUFF1){
             snake.score += 5;
             snake.lives += 0;
             }
        if (itemEast->type == BUFF2){
            if(snake.lives<=3){
                snake.lives+=1;
                }
                }
        if (itemEast->type == DEBUFF1){
            if(invincibility_button){
                }
            else{
            snake.lives -= 1;
             }
             }
        if (itemEast->type == GHOST){
              if(invincibility_button){
                  }
            else{
              if(snake.score >= 6){
              snake.score -= 5;
              }
              }
             }
        if (itemEast->type == DEBUFF2){
             if(invincibility_button){
                 }
            else{
             snake.length += 2;
             }
             }
        for(int i = snake.length - 1 ; i > 0; i--){//this for loop is going through the snake backwards essentially
            snake.locations[i]= snake.locations[i-1];
            }
        snake.locations[0].x = snake.head_x;//new snake head is now location[0]
        add_snake_head_east(snake.locations[0].x, snake.locations[0].y);
        add_snake_body_horizontal(snake.locations[1].x, snake.locations[1].y);//body is location[1]
        for (int tail = 2; tail<snake.length; tail++){
            add_snake_tail_horizontal(snake.locations[tail].x, snake.locations[tail].y);//add tail
            }
        return 1;
    }else{//if not walkable items
         if (itemEast->type == WALL ){
             if(invincibility_button){
                 }
            else{
             if(snake.lives > 0){
                 snake.lives -= 1;
             if(snake.lives == 0){
                return GAME_OVER;
                }
                }
                }
             }
        if (itemEast->type == SNAKE_BODY ){
            if(invincibility_button){
                 }
            else{
            if(snake.lives > 0){
                 snake.lives -= 1;
             if(snake.lives == 0){
                return GAME_OVER;
                }
                }
                }
             }
        return 0;
        }
    }
int go_left(int x, int y){
     MapItem *itemWest = get_west(x, y); //get item to west
    if (itemWest->walkable) { // check if walkable
        snake.head_x= snake.head_x-1;
         remove_snake_tail_horizontal(snake.locations[snake.length-1].x, snake.locations[snake.length-1].y);
        if (itemWest->type == GOODIE){
             snake.length += 1;
             snake.score += 1;
             }
        if (itemWest->type == PRESENT){
             if(snake.length>=4){
            remove_snake_tail_horizontal(snake.locations[snake.length-2].x, snake.locations[snake.length-2].y);
             snake.length -= 1;
             }
             }
        if (itemWest->type == BUFF1){
             snake.score += 5;
             snake.lives += 0;
             }
        if (itemWest->type == BUFF2){
            if(snake.lives<=3){
                snake.lives+=1;
                }
                }
        if (itemWest->type == DEBUFF2){
             if(invincibility_button){
                 }
            else{
             snake.length += 2;
             }
                }
        if (itemWest->type == GHOST){
              if(invincibility_button){
                  }
            else{
              if(snake.score >= 6){
              snake.score -= 5;
              }
              }
             }
        if (itemWest->type == DEBUFF1 ){
              if(invincibility_button){
                  }
            else{
              snake.lives -= 1;
              }
             }
        for(int i = snake.length - 1 ; i > 0; i--){
            snake.locations[i]= snake.locations[i-1];
            }
        snake.locations[0].x = snake.head_x;
        add_snake_head_west(snake.locations[0].x, snake.locations[0].y);
        add_snake_body_horizontal(snake.locations[1].x, snake.locations[1].y);
        for (int tail = 2; tail<snake.length; tail++){
            add_snake_tail_horizontal(snake.locations[tail].x, snake.locations[tail].y);
            }
        return 1;
    }else{
         if (itemWest->type == WALL ){
            if(invincibility_button){
                 }
            else{
            if(snake.lives > 0){
                 snake.lives -= 1;
             if(snake.lives == 0){
                return GAME_OVER;
                }
                }
                }
             }
            if (itemWest->type == SNAKE_BODY ){
                if(invincibility_button){
                 }
            else{
            if(snake.lives > 0){
                 snake.lives -=1;
             if(snake.lives == 0){
                return GAME_OVER;
                }
                }
                }
             }
        return 0;
        }
    }
int go_up(int x, int y){
    MapItem *itemNorth = get_north(x, y); //get item to north
    if (itemNorth->walkable) { // check if walkable
        snake.head_y= snake.head_y-1;
       remove_snake_tail_vertical(snake.locations[snake.length-1].x, snake.locations[snake.length-1].y);
        if (itemNorth->type == GOODIE){
             snake.length += 1;
             snake.score += 1;
             }
        if (itemNorth->type == PRESENT){
             if(snake.length>=4){
            remove_snake_tail_vertical(snake.locations[snake.length-2].x, snake.locations[snake.length-2].y);
             snake.length -= 1;
             }
             }
        if (itemNorth->type == BUFF1){
             snake.score += 5;
             snake.lives += 0;
             }
        if (itemNorth->type == BUFF2){
            if(snake.lives<=3){
                snake.lives+=1;
                }
                }
        if (itemNorth->type == DEBUFF1){
              if(invincibility_button){
                  }
            else{
              snake.lives -= 1;
              }
             }
        if (itemNorth->type == GHOST){
              if(invincibility_button){
                  }
            else{
              if(snake.score >= 6){
              snake.score -= 5;
              }
              }
             }
        if (itemNorth->type == DEBUFF2){
             if(invincibility_button){
                 }
            else{
             snake.length += 2;
             }
                }
        for(int i = snake.length - 1 ; i > 0; i--){
            snake.locations[i]= snake.locations[i-1];
            }
        snake.locations[0].y = snake.head_y;
        add_snake_head_north(snake.locations[0].x, snake.locations[0].y);
        add_snake_body_vertical(snake.locations[1].x, snake.locations[1].y);
        for (int tail = 2; tail<snake.length; tail++){
            add_snake_tail_vertical(snake.locations[tail].x, snake.locations[tail].y);
            }
        return 1;
    }else{
         if (itemNorth->type == WALL ){
              if(invincibility_button){
                 }
            else{
              if(snake.lives > 0){
                 snake.lives -= 1;
             if(snake.lives == 0){
                return GAME_OVER;
                }
                }
                }
             }
        if (itemNorth->type == SNAKE_BODY){
            if(invincibility_button){
                 }
            else{
            if(snake.lives > 0){
                 snake.lives -=1;
             if(snake.lives == 0){
                return GAME_OVER;
                }
                }
                }
             }
        return 0;
        }
    }
int go_down(int x, int y){
    MapItem *itemSouth = get_south(x, y); //get item to south
    if (itemSouth->walkable) { // check if walkable
        snake.head_y= snake.head_y+1;
        remove_snake_tail_vertical(snake.locations[snake.length-1].x, snake.locations[snake.length-1].y);
        if (itemSouth->type == GOODIE){
             snake.length += 1;
             snake.score += 1;
             }
        if (itemSouth->type == PRESENT){
             if(snake.length>=4){
            remove_snake_tail_vertical(snake.locations[snake.length-2].x, snake.locations[snake.length-2].y);
             snake.length -= 1;
             }
             }
        if (itemSouth->type == BUFF1){
             snake.score += 5;
             snake.lives += 0;
             }
        if (itemSouth->type == BUFF1){
            if(snake.lives<=5){
                snake.lives+=1;
                }
                }
        if (itemSouth->type == GHOST){
              if(invincibility_button){
                  }
            else{
              if(snake.score >= 6){
              snake.score -= 5;
              }
              }
             }
        if (itemSouth->type == DEBUFF1){
                if(invincibility_button){
                    }
                else{
                snake.lives -= 1;
                }
             }
        if (itemSouth->type == DEBUFF2){
                if(invincibility_button){
                    }
                else{    
                    snake.length += 2;
                    }
                }
        for(int i = snake.length - 1 ; i > 0; i--){
            snake.locations[i]= snake.locations[i-1];
            }
        snake.locations[0].y = snake.head_y;
        add_snake_head_south(snake.locations[0].x, snake.locations[0].y);
        add_snake_body_vertical(snake.locations[1].x, snake.locations[1].y);
        for (int tail = 2; tail<snake.length; tail++){
            add_snake_tail_vertical(snake.locations[tail].x, snake.locations[tail].y);
            }
        return 1;
    }else{
         if (itemSouth->type == WALL ){
             if(invincibility_button){
                 }
            else{
             if(snake.lives > 0){
                 snake.lives -= 1;
             if(snake.lives == 0){
                return GAME_OVER;
                }
                }
                }
             }
        if (itemSouth->type == SNAKE_BODY ){
            if(invincibility_button){
                 }
            else{
            if(snake.lives > 0){
                 snake.lives -=1;
             if(snake.lives == 0){
                return GAME_OVER;
                }
                }
                }
             }
        return 0;
        }
    }

/**
 * Program entry point! This is where it all begins.
 * This function or all the parts of the game. Most of your
 * implementation should be elsewhere - this holds the game loop, and should
 * read like a road map for the rest of the code.
 */
int main()
{
    // First things first: initialize hardware
    ASSERT_P(hardware_init() == ERROR_NONE, "Hardware init failed!");
 
    snake_init(&snake);
    // 0. Initialize the maps -- implement this function:
    maps_init();
    init_main_map();
 
    // Initialize game state
    set_active_map(0);
    snake.head_x = snake.head_y = 5;
    // Initial drawing
    draw_game(FULL_DRAW);
    // Main game loop
    while(1) {
        // Timer to measure game update speed
        Timer t;
        t.start();
 
        // 1. Read inputs -- implement this function:
        GameInputs inputs = read_inputs();
        
        // 2. Determine action (move, act, menu, etc.) -- implement this function:
        int action = get_action(inputs);
        
        // 3. Update game -- implement this function:
        int result = update_game(action);
        
        // 3b. Check for game over based on result
        // and if so, handle game over -- implement this.
                
        // 4. Draw screen -- provided:
        draw_game(result);
        
        // Compute update time
        t.stop();
        int dt = t.read_ms();
 
        // Display and wait
        // NOTE: Text is 8 pixels tall
        if (dt < 100) wait_ms(100 - dt);
    }
}
 
void draw_opening_act(){
    uLCD.filled_rectangle(0,0,255,255,BLACK);
    uLCD.textbackground_color(BLACK);
    uLCD.locate(0,0);
    uLCD.color(WHITE);
    uLCD.printf("******************");
    uLCD.locate(0,15);
    uLCD.color(WHITE);
    uLCD.printf("******************");
    uLCD.locate(0,1);
    uLCD.color(0xFFFF00);
    uLCD.printf(" * * * * * * * * *");
    uLCD.locate(0,14);
    uLCD.color(0xFFFF00);
    uLCD.printf(" * * * * * * * * *");
    
    uLCD.text_height(3);
    uLCD.text_width(1);
    uLCD.text_bold(TEXTBOLD);
    uLCD.locate(7,1);
    uLCD.color(0xC8C4FF);
    uLCD.printf("Sofia");
    
    uLCD.text_height(1);
    uLCD.text_width(1);
    uLCD.locate(5,7);
    uLCD.color(WHITE);
    uLCD.printf("the");
    
    uLCD.text_height(1);
    uLCD.text_width(1);
    uLCD.locate(9,7);
    uLCD.text_italic(TEXTITALIC);
    uLCD.color(0xFFFF00);
    uLCD.printf("sassy");
    
    uLCD.text_height(1);
    uLCD.text_width(2);
    uLCD.locate(2,10);
    uLCD.color(GREEN);
    uLCD.printf("SNAKE");
    
    uLCD.text_height(1);
    uLCD.text_width(1);
    uLCD.locate(2,12);
    uLCD.color(WHITE);
    uLCD.printf("by Lilly Sitver");
    wait(3);
    uLCD.filled_rectangle(0,0,255,255,BLACK);
    draw_snake_head_north(100,80);
    draw_snake_body_vertical(100,90);
    draw_snake_tail_vertical(100,100);
    uLCD.text_height(2);
    uLCD.text_width(1);
    uLCD.locate(1,2);
    uLCD.color(WHITE);
    uLCD.printf("Sofia is a\n VERY sassy snake");
    
    wait(1.5);
    uLCD.filled_rectangle(0,0,255,255,BLACK);
    draw_goodie(90,70);
    draw_goodie(50,30);
    draw_goodie(20,60);
    draw_goodie(70,20);
    draw_goodie(90,40);
    draw_buff2(20,30);
    draw_buff2(40,70);
    draw_buff2(80,50);
    draw_buff2(70,20);
    draw_buff2(30,40);
    draw_buff2(30,20);
    draw_buff2(90,70);
    draw_snake_head_north(60,40);
    draw_snake_body_vertical(60,50);
    draw_snake_tail_vertical(60,60);
    uLCD.text_height(1);
    uLCD.text_width(1);
    uLCD.locate(1,12);
    uLCD.color(WHITE);
    uLCD.printf("Sofia LOVES blue");
    uLCD.locate(6,14);
    uLCD.printf("goodies");
    wait(2);
    uLCD.filled_rectangle(0,0,255,255,BLACK);
    uLCD.text_height(1);
    uLCD.text_width(1);
    uLCD.locate(1,1);
    uLCD.color(WHITE);
    uLCD.printf("Help Sofia find\n \n"
                " and eat as\n \n"
                " many blue goodies\n"
                " possible to make\n \n"
                " Sofia less sassy\n \n");
    draw_goodie(10,100);
    draw_goodie(30,100);
    draw_goodie(50,100);
    draw_goodie(70,100);
    draw_goodie(90,100);
    draw_goodie(110,100);
    wait(4.5);
    uLCD.filled_rectangle(0,0,255,255,BLACK);
    uLCD.text_height(1);
    uLCD.text_width(1);
    uLCD.locate(1,4);
    uLCD.color(WHITE);
    uLCD.printf("Move the board\n\n around to move\n\n Sofia");
    uLCD.color(GREEN);
    uLCD.locate(9,10);
    uLCD.printf("N");
    uLCD.locate(7,12);
    uLCD.printf("W   E");
    uLCD.locate(9,14);
    uLCD.printf("S");
    
    draw_snake_head_east(70,10);
    draw_snake_body_horizontal(60,10);
    draw_snake_tail_horizontal(50,10);
    
    wait(2.5);
    uLCD.filled_rectangle(0,0,255,255,BLACK);
    uLCD.text_height(2);
    uLCD.text_width(2);
    uLCD.locate(1,0);
    uLCD.color(GREEN);
    uLCD.printf("BUTTONS");
    uLCD.text_height(1);
    uLCD.text_width(1);
    uLCD.color(WHITE);
    uLCD.locate(1,4);
    uLCD.printf("B1  Invincibility");
    uLCD.locate(1,8);
    uLCD.printf("B2  Game Key");
    uLCD.locate(1,12);
    uLCD.printf("B3  Pause Game");
    wait(4);
    uLCD.filled_rectangle(0,0,255,255,BLACK);
    uLCD.text_height(2);
    uLCD.text_width(1);
    uLCD.color(RED);
    uLCD.locate(1,3);
    uLCD.printf("Sofia is extra\n sassy today");
    draw_snake_head_east(70,30);
    draw_snake_body_horizontal(60,30);
    draw_snake_tail_horizontal(50,30);
    wait(2.5);
    uLCD.filled_rectangle(0,0,255,255,BLACK);
    uLCD.text_height(2);
    uLCD.text_width(1);
    uLCD.color(WHITE);
    uLCD.locate(1,3);
    uLCD.printf("Can you help\n make Sofia nicer?");
    wait(2.5);
    uLCD.filled_rectangle(0,0,255,255,BLACK);
    uLCD.text_height(2);
    uLCD.text_width(1);
    uLCD.color(WHITE);
    uLCD.locate(5,2);
    uLCD.printf("GAME START");
    uLCD.text_height(3);
    uLCD.text_width(1);
    uLCD.color(RED);
    uLCD.locate(6,3);
    uLCD.printf("3");
    wait(1);
    uLCD.filled_rectangle(0,0,255,255,BLACK);
    uLCD.text_height(2);
    uLCD.text_width(1);
    uLCD.color(WHITE);
    uLCD.locate(5,2);
    uLCD.printf("GAME START");
    uLCD.text_height(3);
    uLCD.text_width(1);
    uLCD.color(RED);
    uLCD.locate(6,3);
    uLCD.printf("2");
    wait(.75);
    uLCD.filled_rectangle(0,0,255,255,BLACK);
    uLCD.text_height(2);
    uLCD.text_width(1);
    uLCD.color(WHITE);
    uLCD.locate(5,2);
    uLCD.printf("GAME START");
    uLCD.text_height(3);
    uLCD.text_width(1);
    uLCD.color(RED);
    uLCD.locate(6,3);
    uLCD.printf("1");
    wait(.5);
    uLCD.filled_rectangle(0,0,255,255,BLACK);
    wait(0.2);
    
    }
// Plays a wavfile
void playSound(char* wav)
{
    
}