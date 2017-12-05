#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <ncurses.h>

#define BRAILLE_ROWS_PER_CELL 4
#define BRAILLE_COLUMNS_PER_CELL 2

typedef struct{
     uint8_t* cells;
     int32_t width;
     int32_t height;
     int32_t x_offset;
     int32_t y_offset;
     int color_pair;
}BrailleBuffer_t;

bool braille_buffer_init(BrailleBuffer_t* braille_buffer, int32_t terminal_width, int32_t terminal_height);
void braille_buffer_free(BrailleBuffer_t* braille_buffer);
uint8_t* braille_buffer_get_cell(BrailleBuffer_t* braille_buffer, int32_t x, int32_t y);
bool braille_buffer_set_pixel(BrailleBuffer_t* braille_buffer, int32_t x, int32_t y, bool on);
void braille_buffer_clear(BrailleBuffer_t* braille_buffer);
void braille_buffer_draw(BrailleBuffer_t* braille_buffer, WINDOW* window);
int32_t braille_buffer_pixel_width(BrailleBuffer_t* braille_buffer);
int32_t braille_buffer_pixel_height(BrailleBuffer_t* braille_buffer);