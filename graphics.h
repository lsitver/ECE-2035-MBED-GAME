// Copyright 2020 Georgia Tech.  All rights reserved.
// The materials provided by the instructor in this course are for
// the use of the students currently enrolled in the course.
// Copyrighted course materials may not be further disseminated.
// This file must not be made publicly available anywhere.

#ifndef GRAPHICS_H
#define GRAPHICS_H

/**
 * Takes a string image and draws it to the screen. The string is 121 characters
 * long, and represents an 11x11 tile in row-major ordering (across, then down,
 * like a regular multi-dimensional array). The available colors are:
 *      R = Red
 *      Y = Yellow
 *      G = Green
 *      D = Brown ("dirt")
 *      5 = Light grey (50%)
 *      3 = Dark grey (30%)
 *      Any other character is black
 * More colors can be easily added by following the pattern already given.
 */
void draw_img(int u, int v, const char* img);

/**
 * DrawFunc functions. 
 * These can be used as the MapItem draw functions.
 */
 
void draw_nothing(int u, int v);
void draw_plant(int u, int v);
void draw_wall(int u, int v);
void draw_goodie(int u, int v);
void draw_snake_body_horizontal(int u, int v);
void draw_snake_body_vertical(int u, int v);
void draw_snake_head_south(int u, int v);
void draw_snake_head_north(int u, int v);
void draw_snake_head_east(int u, int v);
void draw_snake_head_west(int u, int v);
void draw_snake_tail_horizontal(int u, int v);
void draw_snake_tail_vertical(int u, int v);
void draw_buff1(int u, int v);
void draw_buff2(int u, int v);
void draw_debuff1(int u, int v);
void draw_debuff2(int u, int v);
void draw_ghost(int u, int v);
void draw_present(int u, int v);


#endif // GRAPHICS_H