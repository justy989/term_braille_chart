#include "braille_buffer.h"

#include <stdlib.h>
#include <string.h>
#include <math.h>

bool braille_buffer_init(BrailleBuffer_t* braille_buffer, int32_t terminal_width, int32_t terminal_height){
     if(braille_buffer->cells) braille_buffer_free(braille_buffer);
     int32_t total_cells = terminal_width * terminal_height;
     braille_buffer->cells = calloc(total_cells, sizeof(*braille_buffer->cells));
     if(!braille_buffer->cells) return false;
     braille_buffer->width = terminal_width;
     braille_buffer->height = terminal_height;
     return true;
}

void braille_buffer_free(BrailleBuffer_t* braille_buffer){
     free(braille_buffer->cells);
     memset(braille_buffer, 0, sizeof(*braille_buffer));
}

uint8_t* braille_buffer_get_cell(BrailleBuffer_t* braille_buffer, int32_t x, int32_t y){
     if(x < 0 || y < 0 || x >= braille_buffer->width || y >= braille_buffer->height) return NULL;
     int32_t index = y * braille_buffer->width + x;
     return braille_buffer->cells + index;
}

bool braille_buffer_set_pixel(BrailleBuffer_t* braille_buffer, int32_t x, int32_t y, bool on){
     if(x < 0 || y < 0) return false;

     int32_t max_x = braille_buffer->width * BRAILLE_COLUMNS_PER_CELL;
     int32_t max_y = braille_buffer->height * BRAILLE_ROWS_PER_CELL;
     if(x >= max_x || y >= max_y) return false;

     int32_t cell_x = x / BRAILLE_COLUMNS_PER_CELL;
     int32_t cell_y = y / BRAILLE_ROWS_PER_CELL;
     int32_t pixel_x = x % BRAILLE_COLUMNS_PER_CELL;
     int32_t pixel_y = y % BRAILLE_ROWS_PER_CELL;

     static const uint8_t map[BRAILLE_ROWS_PER_CELL][BRAILLE_COLUMNS_PER_CELL] = {
          {0x1, 0x8},
          {0x2, 0x10},
          {0x4, 0x20},
          {0x40, 0x80}
     };

     uint8_t* cell = braille_buffer_get_cell(braille_buffer, cell_x, cell_y);

     if(on){
          *cell |= map[pixel_y][pixel_x];
     }else{
          *cell &= ~map[pixel_y][pixel_x];
     }

     return true;
}

void braille_buffer_clear(BrailleBuffer_t* braille_buffer){
     for(int32_t j = 0; j < braille_buffer->height; j++){
          for(int32_t i = 0; i < braille_buffer->width; i++){
               uint8_t* cell = braille_buffer_get_cell(braille_buffer, i, j);
               *cell = 0;
          }
     }
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

void braille_buffer_draw(BrailleBuffer_t* braille_buffer, WINDOW* window){
     const int string_len = UTF8_SIZE;
     char string[string_len + 1];
     for(int32_t j = 0; j < braille_buffer->height; j++){
          for(int32_t i = 0; i < braille_buffer->width; i++){
               uint8_t* cell = braille_buffer_get_cell(braille_buffer, i, j);
               if(cell && *cell){
                    int32_t written = 0;
                    memset(string, 0, string_len + 1);
                    if(utf8_encode(0x2800 + *cell, string, string_len, &written)){
                         mvwaddstr(window, braille_buffer->y_offset + j, braille_buffer->x_offset + i, string);
                    }
               }
          }
     }
}

int32_t braille_buffer_pixel_width(BrailleBuffer_t* braille_buffer){
     return braille_buffer->width * BRAILLE_COLUMNS_PER_CELL;
}

int32_t braille_buffer_pixel_height(BrailleBuffer_t* braille_buffer){
     return braille_buffer->height * BRAILLE_ROWS_PER_CELL;
}

// NOTE: bresenham line algo
void braille_buffer_line(BrailleBuffer_t* braille_buffer, int32_t x_0, int32_t y_0, int32_t x_1, int32_t y_1){
     int32_t dy = (y_0 < y_1) ? 1 : -1;

     if(x_0 == x_1){
          for(int32_t y = y_0; y <= y_1; y += dy){
               braille_buffer_set_pixel(braille_buffer, x_0, y, true);
          }
          return;
     }

     double delta_x = x_1 - x_0;
     double delta_y = y_1 - y_0;
     double delta_error = fabs(delta_y / delta_x);
     double error = 0.0f;
     int32_t y = y_0;
     int32_t dx = (x_0 < x_1) ? 1 : -1;
     for(int32_t x = x_0; x != x_1; x += dx){
          braille_buffer_set_pixel(braille_buffer, x, y, true);
          error += delta_error;
          while(error >= 0.5f){
               y += dy;
               error -= 1.0f;
          }
     }
}
