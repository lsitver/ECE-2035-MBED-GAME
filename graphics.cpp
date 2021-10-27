// Copyright 2020 Georgia Tech.  All rights reserved.
// The materials provided by the instructor in this course are for
// the use of the students currently enrolled in the course.
// Copyrighted course materials may not be further disseminated.
// This file must not be made publicly available anywhere.

#include "graphics.h"

#include "globals.h"

void draw_nothing(int u, int v)
{
    uLCD.filled_rectangle(u, v, u+10, v+10, BLACK);
}

void draw_img(int u, int v, const char* img)
{
    int colors[11*11];
    for (int i = 0; i < 11*11; i++)
    {
        if (img[i] == 'R') colors[i] = RED;
        else if (img[i] == 'Y') colors[i] = 0xFFFF00; // Yellow
        else if (img[i] == 'G') colors[i] = GREEN;
        else if (img[i] == 'D') colors[i] = 0xD2691E; // "Dirt"
        else if (img[i] == '5') colors[i] = LGREY; // 50% grey
        else if (img[i] == '3') colors[i] = 0x000001; //black
        else if (img[i] == 'W') colors[i] = 0xFFFFFF;//white
        else if (img[i] == 'B') colors[i] = 0x0000FF;//blue
        else if (img[i] == '1') colors[i] = 0xC8C4FF ;//lavender
        else if (img[i] == 'O') colors[i] = 0xFF9900; //orange
        else if (img[i] == 'P') colors[i] = 0x9933FF; //purple
        else if (img[i] == '2') colors[i] = 0x623369; //dark purple
        else if (img[i] == 'I') colors[i] = 0xFFCAF4; //light pink
        else if (img[i] == '6') colors[i] = 0xD0FFCA; //light green
        else if (img[i] == 'Z') colors[i] = 0xFF8DA7; //brick
        
        
        else colors[i] = BLACK;
    }
    uLCD.BLIT(u, v, 11, 11, colors);
    wait_us(250); // Recovery time!
}

void draw_wall(int u, int v)
{
    const char* wall="222222222222ZZ2ZZ2ZZ2Z2ZZ2ZZ2ZZ2Z22222222222ZZ2ZZ2ZZ2ZZ22222222222Z2ZZ2ZZ2ZZ2Z2ZZ2ZZ2ZZ2222222222222ZZ2ZZ2ZZ2ZZ";
    draw_img(u,v,wall);
}

void draw_plant(int u, int v)
{
    const char* plant ="355555555535WW33333WW553WW333WW35533WW3WW3355333W3W333553333W333355333W3W3335533WW3WW33553WW333WW355WW33333WW535555555553";
    draw_img(u,v, plant);
}

void draw_goodie(int u, int v)
{
    const char* goodie ="BBBBBBBBBBBB111111111BB1BBBBBBB1BB1B11111B1BB1B1BBB1B1BB1B1B1B1B1BB1B1BBB1B1BB1B11111B1BB1BBBBBBB1BB111111111BBBBBBBBBBBB";
    draw_img(u,v,goodie);
}
void draw_buff1(int u, int v)//star
{
    const char* buff1 ="3333333333333333Y333333333YYY33333YYYYYYYYY333YYYYYYY33333YYYYY33333YYYYYYY333YYYYYYYYY33333YYY333333333Y3333333333333333";
    draw_img(u,v,buff1);
}

void draw_buff2(int u, int v)//heart
{
    const char* buff2 ="3333333333333RR333RR333RRRR3RRRR33RRRR3RRRR33RRRRRRRRR33RRRRRRRRR33RRRRRRRRR333RRRRRRR33333RRRRR3333333RRR333333333R33333";
    draw_img(u,v,buff2);
}
void draw_debuff1(int u, int v)//bomb
{
    const char* debuff1 = "RRRRRRRRRRRRRRRRRWRRRRRRRRRWRWRRRRRRRR3RRYRRRRRR333RRRRRRR33333RRRRR3333333RRRR3333333RRRRRR333RRRRRRRRR3RRYRRRRRRRRRRRRR";
    draw_img(u,v,debuff1);
}

void draw_debuff2(int u, int v)//snail
{
    const char* debuff2 ="3333333333332323333333322233333332III2I333332I2I232223332II2266623332II266262332II262622332662I22I2332III2662633322222222";
    draw_img(u,v,debuff2);
}


void draw_snake_body_vertical(int u, int v)
{
    const char* snakebodyV = "BGYORPBGYORBGYORPBGYORBGYORPBGYORBGYORPBGYORBGYORPBGYORBGYORPBGYORBGYORPBGYORBGYORPBGYORBGYORPBGYORBGYORPBGYORBGYORPBGYOR";
    draw_img(u,v,snakebodyV);
}

void draw_snake_body_horizontal(int u, int v)
{
    const char* snakebodyH = "BBBBBBBBBBBGGGGGGGGGGGYYYYYYYYYYYOOOOOOOOOOORRRRRRRRRRRPPPPPPPPPPPBBBBBBBBBBBGGGGGGGGGGGYYYYYYYYYYYOOOOOOOOOOORRRRRRRRRRR";
    draw_img(u,v,snakebodyH);
}


void draw_snake_head_south(int u, int v)
{
    const char* snakeheadsouth = "1111111111111WW111WW1111W31113W11111111111113111111111333311111333333111113333333YYY33333333Y3Y3333333YY3YY333333Y333Y333";
    draw_img(u,v,snakeheadsouth);
}

void draw_snake_head_north(int u, int v )
{
    const char* snakeheadnorth = "333Y333Y333333YY3YY3333333Y3Y33333333YYY33333331111133333311111333311111111131111111111111W31113W1111WW111WW1111111111111";
    draw_img(u,v,snakeheadnorth);
}

void draw_snake_head_east(int u, int v )
{
    const char* snakeheadeast = "11113333333111113333331WW113333331W3111133YY1111111YYY31111111Y3331111111YYY31W3111133YY1WW113333331111133333311113333333";
    draw_img(u,v,snakeheadeast);
}

void draw_snake_head_west(int u, int v)
{
    const char* snakeheadwest = "333333311113333333111133333311ww1YY3311113W13YYY1111111333Y11111113YYY1111111YY3311113W133333311WW13333331111133333331111";
    draw_img(u,v,snakeheadwest);
}

void draw_snake_tail_vertical(int u, int v)
{
    const char* snaketailvertical = "BGYORPBGYORBGYORPBGYORBGYORPBGYORBGYORPBGYORBGYORPBGYORBGYORPBGYORBGYORPBGYORBGYORPBGYORBGYORPBGYORBGYORPBGYORBGYORPBGYOR";
    draw_img(u,v,snaketailvertical);
    
}

void draw_snake_tail_horizontal(int u, int v)
{
    const char* snaketailhorizontal = "BBBBBBBBBBBGGGGGGGGGGGYYYYYYYYYYYOOOOOOOOOOORRRRRRRRRRRPPPPPPPPPPPBBBBBBBBBBBGGGGGGGGGGGYYYYYYYYYYYOOOOOOOOOOORRRRRRRRRRR";
    draw_img(u,v,snaketailhorizontal);
    
}

void draw_ghost(int u, int v)
{
    const char* ghost = "3333WWW33333333WWW3333333WWWWW333333W3W3W333333W3W3W333333WWWWW333333WW3WW333333WWWWW333333WWWWW333333WWWWW333333W3W3W333";
    draw_img(u,v,ghost);
}

void draw_present(int u, int v)
{
    const char* present = "333YY3YY333333YY3YY3333333YYY3333333IIYII333333IIYII333333IIYII333333YYYYY333333IIYII333333IIYII333333IIYII333333IIYII333";
    draw_img(u,v,present);
}

