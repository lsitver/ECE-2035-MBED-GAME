// Copyright 2020 Georgia Tech.  All rights reserved.
// The materials provided by the instructor in this course are for
// the use of the students currently enrolled in the course.
// Copyrighted course materials may not be further disseminated.
// This file must not be made publicly available anywhere.

#define SNAKE_MAX_LENGTH 50

// Structure of coordinates in the map
typedef struct{
    int x;
    int y;
} Coordinate;

// Snake Structure
typedef struct {
    //no tail body px yx
    int head_x, head_y, head_px, head_py; // Location of the head of the snake
    int length; // length of the snake
    Coordinate locations[SNAKE_MAX_LENGTH]; // Snake body locations
    int score; //Current score of the snake
    int lives;//current number of lives
} Snake;

// Initialize a snake structure
// You want to assign initial values to each of the variables defined above
// in the snake structure.
void snake_init (Snake * snake);