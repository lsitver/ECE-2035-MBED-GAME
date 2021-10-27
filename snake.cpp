// Copyright 2020 Georgia Tech.  All rights reserved.
// The materials provided by the instructor in this course are for
// the use of the students currently enrolled in the course.
// Copyrighted course materials may not be further disseminated.
// This file must not be made publicly available anywhere.

#include "snake.h"

void snake_init (Snake * s)//initialize components of snake struct
{
    s->head_x=5;
    s->head_y=5;
    
    s->head_px=s->head_x-1;
    s->head_py=s->head_y;
    
    s->length=3;
    s->locations[0].x=s->head_x;
    s->locations[0].y=s->head_y;
    s->locations[1].x=s->head_x - 1;
    s->locations[1].y=s->head_y;
    s->locations[2].x=s->head_x - 2;
    s->locations[2].y=s->head_y;
    s->score=0;
    s->lives=3;
}
