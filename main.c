#include <stdio.h>
#include <ncurses.h>
#include <locale.h>

#include "back_buffer.h"

typedef struct{
     int32_t x;
     int32_t y;
}Point_t;

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

     BackBuffer_t back_buffer = {};
     back_buffer_init(&back_buffer, 16, 16);

#if 0
     bool flag = false;
     for(int32_t j = 0; j < back_buffer_pixel_height(&back_buffer); j++){
          for(int32_t i = 0; i < back_buffer_pixel_width(&back_buffer); i++){
               flag = !flag;
               if(flag){
                    back_buffer_set_pixel(&back_buffer, j, i, true);
               }
          }
          flag = !flag;
     }

#else
     Point_t art[] = {
          {3, 4},
          {4, 5},
          {4, 6},
          {3, 6},
          {3, 7},
          {3, 8},
          {3, 9},
          {3, 10},
          {2, 7},
          {2, 8},
          {1, 8},
          {1, 9},
          {1, 10},
          {4, 11},
          {5, 11},
          {4, 8},
          {4, 9},
          {5, 6},
          {5, 7},
          {5, 8},
          {5, 9},
          {6, 6},
          {6, 7},
          {6, 8},
          {6, 9},
          {7, 6},
          {7, 7},
          {7, 8},
          {7, 9},
          {7, 11},
          {8, 5},
          {8, 6},
          {8, 8},
          {8, 9},
          {8, 11},
          {9, 4},
          {9, 6},
          {9, 7},
          {9, 8},
          {9, 9},
          {9, 10},
          {10, 7},
          {10, 8},
          {11, 8},
          {11, 9},
          {11, 10},
     };

     int32_t pixel_count = sizeof(art) / sizeof(art[0]);
     for(int32_t i = 0; i < pixel_count; i++){
          back_buffer_set_pixel(&back_buffer, art[i].x, art[i].y, true);
     }
#endif

     init_pair(1, COLOR_GREEN, -1);
     attron(COLOR_PAIR(1));

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

          back_buffer_draw(&back_buffer, stdscr);
          move(0, 0);
          refresh();
     }

     endwin();
     return 0;
}
