// Copyright 2020 Georgia Tech.  All rights reserved.
// The materials provided by the instructor in this course are for
// the use of the students currently enrolled in the course.
// Copyrighted course materials may not be further disseminated.
// This file must not be made publicly available anywhere.

#include "map.h"

#include "globals.h"
#include "graphics.h"

/**
 * The Map structure. This holds a HashTable for all the MapItems, along with
 * values for the width and height of the Map.
 */
struct Map {
    HashTable* items;
    int w, h;
};

#define NUM_MAPS 1
static Map maps[NUM_MAPS];
static int active_map;

static const MapItem CLEAR_SENTINEL = {
    .type = CLEAR,
    .draw = draw_nothing
};

/**
 * The first step in HashTable access for the map is turning the two-dimensional
 * key information (x, y) into a one-dimensional unsigned integer.
 * This function should uniquely map (x,y) onto the space of unsigned integers.
 */
static unsigned XY_KEY(int X, int Y) {
     return(X+(map_width())*Y);//multiply Y, height, by the width of the map and add X to transform map to 1d by calculating a unique value 
}

/**
 * This is the hash function actually passed into createHashTable. It takes an
 * unsigned key (the output of XY_KEY) and turns it into a hash value (some
 * small non-negative integer).
 */
unsigned map_hash(unsigned key)
{  
    return(key % NUMBUCKETS);
}

void maps_init()
{   
    Map* map = get_active_map();
    HashTable* newmap = createHashTable(map_hash,NUMBUCKETS);
    map->w=WIDTH;
    map->h=HEIGHT;
    map->items= newmap;

}

Map* get_active_map()
{
    return &maps[active_map];
}

Map* set_active_map(int m)
{
    active_map = m;
    return &maps[active_map];
}

void print_map()
{
    char lookup[] = {'W', 'D', 'P', 'A', 'K', 'C', 'N',' ','S'};
    Map* map = get_active_map();
    for(int j = 0; j < map->h; j++)
    {
        for (int i = 0; i < map->w; i++)
        {
            MapItem* item = (MapItem*)getItem(map->items, XY_KEY(i, j));
            if (item) pc.printf("%c", lookup[item->type]);
            else pc.printf(" ");
        }
        pc.printf("\r\n");
    }
}

int map_width()//width
{
Map* map = get_active_map();
return map->w;
}

int map_height()//height
{
Map* map = get_active_map();
return map->h;
}

int map_area()//area = w*h
{
Map* map = get_active_map();
return (map->w)*(map->h);

}

MapItem* get_north(int x, int y)
{
   Map *map = get_active_map();
    int key = XY_KEY(x,y-1); //north is y-1 
    return (MapItem*) getItem(map->items,key); 
}
MapItem* get_south(int x, int y)
{
    Map *map = get_active_map();
    int key = XY_KEY(x,y+1); //south is y+1
    return (MapItem*) getItem(map->items,key);
}

MapItem* get_east(int x, int y)
{
  Map *map = get_active_map();
    int key = XY_KEY(x+1,y); //east is x+1
    return (MapItem*) getItem(map->items,key);  
}

MapItem* get_west(int x, int y)
{
    Map *map = get_active_map();
    int key = XY_KEY(x-1,y); //east is x-1
    return (MapItem*) getItem(map->items,key);

}

MapItem* get_here(int x, int y)
{
    Map *map = get_active_map();
    int key = XY_KEY(x,y);
    return (MapItem*) getItem(map->items,key);
}

void map_erase(int x, int y)//erase items if they are colliding
{
    Map *map = get_active_map();
    int key = XY_KEY(x,y);
    MapItem* isthere = (MapItem*) getItem(map->items,key);
    if(isthere != NULL){
        *isthere=CLEAR_SENTINEL;
    }
}

void add_wall(int x, int y, int dir, int len)
{
    for(int i = 0; i < len; i++)
    {
        MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
        w1->type = WALL;
        w1->draw = draw_wall;
        w1->walkable = false;
        w1->data = NULL;
        unsigned key = (dir == HORIZONTAL) ? XY_KEY(x+i, y) : XY_KEY(x, y+i);
        void* val = insertItem(get_active_map()->items, key, w1);
        if (val) free(val); // If something is already there, free it
    }
}

void add_plant(int x, int y)
{
    MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
    w1->type = PLANT;
    w1->draw = draw_plant;
    w1->walkable = false;
    w1->data = NULL;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    if (val) free(val); // If something is already there, free it
}
void add_buff1(int x, int y)//star
{
    MapItem* b1 = (MapItem*) malloc(sizeof(MapItem));
    b1->type = BUFF1;
    b1->draw = draw_buff1;
    b1->walkable = true;
    b1->data = NULL;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), b1);
    if (val) free(val); // If something is already there, free it
}
void remove_buff1(int x, int y) // clear star once passed
{
    MapItem* b1 = (MapItem*) malloc(sizeof(MapItem));
    b1->type = CLEAR;
    b1->draw = draw_nothing;
    b1->walkable = true;
    b1->data = NULL;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), b1);
    if (val) free(val); // If something is already there, free it
}
void add_buff2(int x, int y)//heart
{
    MapItem* b2 = (MapItem*) malloc(sizeof(MapItem));
    b2->type = BUFF2;
    b2->draw = draw_buff2;
    b2->walkable = true;
    b2->data = NULL;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), b2);
    if (val) free(val); // If something is already there, free it
}
void remove_buff2(int x, int y) // clear heart once passed
{
    MapItem* b2 = (MapItem*) malloc(sizeof(MapItem));
    b2->type = CLEAR;
    b2->draw = draw_nothing;
    b2->walkable = true;
    b2->data = NULL;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), b2);
    if (val) free(val); // If something is already there, free it
}

void add_debuff1(int x, int y)//bomb
{
    MapItem* d1 = (MapItem*) malloc(sizeof(MapItem));
    d1->type = DEBUFF1;
    d1->draw = draw_debuff1;
    d1->walkable = true;
    d1->data = NULL;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), d1);
    if (val) free(val); // If something is already there, free it
}
void remove_debuff1(int x, int y) // clear bomb once passed
{
    MapItem* rb1 = (MapItem*) malloc(sizeof(MapItem));
    rb1->type = CLEAR;
    rb1->draw = draw_nothing;
    rb1->walkable = true;
    rb1->data = NULL;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), rb1);
    if (val) free(val); // If something is already there, free it
}
void add_debuff2(int x, int y)//snail
{
    MapItem* d2 = (MapItem*) malloc(sizeof(MapItem));
    d2->type = DEBUFF2;
    d2->draw = draw_debuff2;
    d2->walkable = true;
    d2->data = NULL;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), d2);
    if (val) free(val); // If something is already there, free it
}
void remove_debuff2(int x, int y) // once snail is passed clear it
{
    MapItem* rb2 = (MapItem*) malloc(sizeof(MapItem));
    rb2->type = CLEAR;
    rb2->draw = draw_nothing;
    rb2->walkable = true;
    rb2->data = NULL;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), rb2);
    if (val) free(val); // If something is already there, free it
}
void add_goodie(int x, int y)
{
    MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
    w1->type = GOODIE;
    w1->draw = draw_goodie;
    w1->walkable = true;
    w1->data = NULL;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    if (val) free(val); // If something is already there, free it
}

void remove_goodie(int x, int y) // clear goodies after passed
{
    MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
    w1->type = CLEAR;
    w1->draw = draw_nothing;
    w1->walkable = false;
    w1->data = NULL;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    if (val) free(val); // If something is already there, free it
}

void add_ghost(int x, int y)//ghost
{
    MapItem* g1 = (MapItem*) malloc(sizeof(MapItem));
    g1->type = GHOST;
    g1->draw = draw_ghost;
    g1->walkable = true;
    g1->data = NULL;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), g1);
    if (val) free(val); // If something is already there, free it
}

void remove_ghost(int x, int y) // clear ghost once passed
{
    MapItem* g1 = (MapItem*) malloc(sizeof(MapItem));
    g1->type = CLEAR;
    g1->draw = draw_nothing;
    g1->walkable = true;
    g1->data = NULL;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), g1);
    if (val) free(val); // If something is already there, free it
}
void add_present(int x, int y)//present
{
    MapItem* p1 = (MapItem*) malloc(sizeof(MapItem));
    p1->type = PRESENT;
    p1->draw = draw_present;
    p1->walkable = true;
    p1->data = NULL;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), p1);
    if (val) free(val); // If something is already there, free it
}
void remove_present(int x, int y) // clear present once passed
{
    MapItem* p1 = (MapItem*) malloc(sizeof(MapItem));
    p1->type = CLEAR;
    p1->draw = draw_nothing;
    p1->walkable = true;
    p1->data = NULL;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), p1);
    if (val) free(val); // If something is already there, free it
}
void add_snake_body_horizontal(int x, int y)//horizontal snake
{
    MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
    w1->type = SNAKE_BODY;
    w1->draw = draw_snake_body_horizontal;
    w1->walkable = false;
    w1->data = NULL;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    if (val) free(val); // If something is already there, free it
}
void add_snake_body_vertical(int x, int y)//vertical snake
{
    MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
    w1->type = SNAKE_BODY;
    w1->draw = draw_snake_body_vertical;
    w1->walkable = false;
    w1->data = NULL;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    if (val) free(val); // If something is already there, free it
}
//the snake head is written 4 times so that it will have the correct orientation when it changes direction
//the snake body+tail is written twice so that it's stripes will have the correct orientation when it changes direction
void add_snake_head_west(int x, int y)
{
    MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
    w1->type = SNAKE_BODY;
    w1->draw = draw_snake_head_west;
    w1->walkable = false;
    w1->data = NULL;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    if (val) free(val); // If something is already there, free it
}

void add_snake_head_east(int x, int y)
{
    MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
    w1->type = SNAKE_BODY;
    w1->draw = draw_snake_head_east;
    w1->walkable = false;
    w1->data = NULL;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    if (val) free(val); // If something is already there, free it
}

void add_snake_head_north(int x, int y)
{
    MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
    w1->type = SNAKE_BODY;
    w1->draw = draw_snake_head_north;
    w1->walkable = false;
    w1->data = NULL;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    if (val) free(val); // If something is already there, free it
}

void add_snake_head_south(int x, int y)
{
    MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
    w1->type = SNAKE_BODY;
    w1->draw = draw_snake_head_south;
    w1->walkable = false;
    w1->data = NULL;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    if (val) free(val); // If something is already there, free it
}
void add_snake_tail_horizontal(int x, int y)
{
    MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
    w1->type = SNAKE_BODY;
    w1->draw = draw_snake_tail_horizontal;
    w1->walkable = false;
    w1->data = NULL;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    if (val) free(val); // If something is already there, free it
}
void add_snake_tail_vertical(int x, int y)
{
    MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
    w1->type = SNAKE_BODY;
    w1->draw = draw_snake_tail_vertical;
    w1->walkable = false;
    w1->data = NULL;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    if (val) free(val); // If something is already there, free it
}
void remove_snake_tail_horizontal(int x, int y) // I'm lazy so overwrite it with a plant
{
    MapItem* s1 = (MapItem*) malloc(sizeof(MapItem));
    s1->type = CLEAR;
    s1->draw = draw_nothing;
    s1->walkable = true;
    s1->data = NULL;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), s1);
    if (val) free(val); // If something is already there, free it
}

void remove_snake_tail_vertical(int x, int y) // I'm lazy so overwrite it with a plant
{
    MapItem* s1 = (MapItem*) malloc(sizeof(MapItem));
    s1->type = CLEAR;
    s1->draw = draw_nothing;
    s1->walkable = true;
    s1->data = NULL;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), s1);
    if (val) free(val); // If something is already there, free it
}