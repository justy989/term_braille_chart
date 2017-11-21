#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <ncurses.h>

#define ROWS_PER_CELL 4
#define COLUMNS_PER_CELL 2

typedef struct{
     uint8_t* cells;
     int32_t width;
     int32_t height;
}BackBuffer_t;

bool back_buffer_init(BackBuffer_t* back_buffer, int32_t terminal_width, int32_t terminal_height);
void back_buffer_free(BackBuffer_t* back_buffer);
bool back_buffer_set_pixel(BackBuffer_t* back_buffer, int32_t x, int32_t y, bool on);
void back_buffer_draw(BackBuffer_t* back_buffer, WINDOW* window);
int32_t back_buffer_pixel_width(BackBuffer_t* back_buffer);
int32_t back_buffer_pixel_height(BackBuffer_t* back_buffer);
