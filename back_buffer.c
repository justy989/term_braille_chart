#include "back_buffer.h"

#include <stdlib.h>
#include <string.h>

bool back_buffer_init(BackBuffer_t* back_buffer, int32_t terminal_width, int32_t terminal_height){
     if(back_buffer->cells) back_buffer_free(back_buffer);
     int32_t total_cells = terminal_width * terminal_height;
     back_buffer->cells = calloc(total_cells, sizeof(*back_buffer->cells));
     if(!back_buffer->cells) return false;
     back_buffer->width = terminal_width;
     back_buffer->height = terminal_height;
     return true;
}

void back_buffer_free(BackBuffer_t* back_buffer){
     free(back_buffer->cells);
     memset(back_buffer, 0, sizeof(*back_buffer));
}

static uint8_t* get_cell(BackBuffer_t* back_buffer, int32_t x, int32_t y){
     if(x < 0 || y < 0 || x >= back_buffer->width || y >= back_buffer->height) return NULL;
     int32_t index = y * back_buffer->width + x;
     return back_buffer->cells + index;
}

bool back_buffer_set_pixel(BackBuffer_t* back_buffer, int32_t x, int32_t y, bool on){
     if(x < 0 || y < 0) return false;

     int32_t max_x = back_buffer->width * COLUMNS_PER_CELL;
     int32_t max_y = back_buffer->height * ROWS_PER_CELL;
     if(x >= max_x || y >= max_y) return false;

     int32_t cell_x = x / COLUMNS_PER_CELL;
     int32_t cell_y = y / ROWS_PER_CELL;
     int32_t pixel_x = x % COLUMNS_PER_CELL;
     int32_t pixel_y = y % ROWS_PER_CELL;

     static const uint8_t map[ROWS_PER_CELL][COLUMNS_PER_CELL] = {
          {0x1, 0x8},
          {0x2, 0x10},
          {0x4, 0x20},
          {0x40, 0x80}
     };

     uint8_t* cell = get_cell(back_buffer, cell_x, cell_y);

     if(on){
          *cell |= map[pixel_y][pixel_x];
     }else{
          *cell &= ~map[pixel_y][pixel_x];
     }

     return true;
}

#define UTF8_SIZE 4

bool utf8_encode(int32_t u, char* string, int64_t string_len, int32_t* bytes_written){
     if(u < 0x80){
          if(string_len < 1) return false;
          *bytes_written = 1;

          // leave as-is
          string[0] = u;
     }else if(u < 0x0800){
          if(string_len < 2) return false;
          *bytes_written = 2;

          // u = 00000000 00000000 00000abc defghijk

          // 2 bytes
          // first byte:  110abcde
          string[0] = 0xC0 | ((u >> 6) & 0x1f);

          // second byte: 10fghijk
          string[1] = 0x80 | (u & 0x3f);
     }else if(u < 0x10000){
          if(string_len < 3) return false;
          *bytes_written = 3;

          // u = 00000000 00000000 abcdefgh ijklmnop

          // 3 bytes
          // first byte:  1110abcd
          string[0] = 0xE0 | ((u >> 12) & 0x0F);

          // second byte: 10efghij
          string[1] = 0x80 | ((u >> 6) & 0x3F);

          // third byte:  10klmnop
          string[2] = 0x80 | (u & 0x3F);
     }else if(u < 0x110000){
          if(string_len < 4) return false;
          *bytes_written = 4;

          // u = 00000000 000abcde fghijklm nopqrstu

          // 4 bytes
          // first byte:  11110abc
          string[0] = 0xF0 | ((u >> 18) & 0x07);
          // second byte: 10defghi
          string[1] = 0x80 | ((u >> 12) & 0x3F);
          // third byte:  10jklmno
          string[2] = 0x80 | ((u >> 6) & 0x3F);
          // fourth byte: 10pqrstu
          string[3] = 0x80 | (u & 0x3F);
     }

     return true;
}

void back_buffer_draw(BackBuffer_t* back_buffer, WINDOW* window){
     const int string_len = UTF8_SIZE;
     char string[string_len + 1];
     for(int32_t j = 0; j < back_buffer->height; j++){
          for(int32_t i = 0; i < back_buffer->width; i++){
               uint8_t* cell = get_cell(back_buffer, i, j);
               if(cell && *cell){
                    int32_t written = 0;
                    memset(string, 0, string_len + 1);
                    if(utf8_encode(0x2800 + *cell, string, string_len, &written)){
                         mvwaddstr(window, j, i, string);
                    }
               }
          }
     }
}

int32_t back_buffer_pixel_width(BackBuffer_t* back_buffer){
     return back_buffer->width * COLUMNS_PER_CELL;
}

int32_t back_buffer_pixel_height(BackBuffer_t* back_buffer){
     return back_buffer->height * ROWS_PER_CELL;
}
