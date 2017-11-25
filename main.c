#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include <locale.h>
#include <math.h>

#include "back_buffer.h"

// NOTE: bresenham line algo
void line(BackBuffer_t* back_buffer, int32_t x_0, int32_t y_0, int32_t x_1, int32_t y_1){
     int32_t dy = (y_0 < y_1) ? 1 : -1;

     if(x_0 == x_1){
          for(int32_t y = y_0; y <= y_1; y += dy){
               back_buffer_set_pixel(back_buffer, x_0, y, true);
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
          back_buffer_set_pixel(back_buffer, x, y, true);
          error += delta_error;
          while(error >= 0.5f){
               y += dy;
               error -= 1.0f;
          }
     }
}

typedef struct{
     double price;
     int64_t timestamp;
}PriceSample_t;

typedef struct{
     PriceSample_t* samples;
     int64_t count;
     int64_t allocated;
}PriceData_t;

bool price_data_init(PriceData_t* price_data, int64_t allocated_samples){
     price_data->samples = calloc(allocated_samples, sizeof(*price_data->samples));
     if(!price_data->samples) return false;
     price_data->count = 0;
     price_data->allocated = allocated_samples;
     return true;
}

bool price_data_insert(PriceData_t* price_data, double price, int64_t timestamp){
     int64_t new_count = price_data->count + 1;
     int64_t allocated = price_data->allocated;

     // double heuristic
     while(new_count < allocated) allocated *= 2;

     if(allocated != price_data->allocated){
          price_data->samples = realloc(price_data->samples, allocated * sizeof(*price_data->samples));
          if(!price_data->samples) return false;
          price_data->allocated = allocated;
     }

     PriceSample_t* sample = price_data->samples + price_data->count;
     sample->price = price;
     sample->timestamp = timestamp;

     price_data->count = new_count;

     return true;
}

void price_data_free(PriceData_t* price_data){
     free(price_data->samples);
     memset(price_data->samples, 0, sizeof(*price_data));
}

typedef struct{
     PriceData_t* price_data;
     double min_price;
     double max_price;
     int64_t min_timestamp;
     int64_t max_timestamp;
     BackBuffer_t back_buffer;
}PriceDataView_t;

void price_data_view_set(PriceDataView_t* price_data_view, double min_price, double max_price, int64_t min_timestamp, int64_t max_timestamp,
                         PriceData_t* price_data, int64_t width, int64_t height){
     price_data_view->price_data = price_data;

     price_data_view->min_price = min_price;
     price_data_view->max_price = max_price;
     price_data_view->min_timestamp = min_timestamp;
     price_data_view->max_timestamp = max_timestamp;

     back_buffer_init(&price_data_view->back_buffer, width, height);
}

int main(){
     setlocale(LC_ALL, "");

     {
          initscr();
          nodelay(stdscr, TRUE);
          keypad(stdscr, TRUE);
          cbreak();
          noecho();
          raw();
          set_escdelay(0);

          if(has_colors() == FALSE){
               printf("Your terminal doesn't support colors. what year do you live in?\n");
               return 1;
          }

          start_color();
          use_default_colors();
     }

     init_pair(1, COLOR_GREEN, -1);
     attron(COLOR_PAIR(1));

     // PriceData_t aapl_data = {};
     // PriceDataView_t aapl_view = {};

     bool done = false;
     while(true){
          int key = getch();
          switch(key){
          default:
               break;
          case 'q':
               done = true;
               break;
          }

          if(done) break;

          // back_buffer_draw(&back_buffer, stdscr);
          move(0, 0);
          refresh();
     }

     endwin();
     return 0;
}
