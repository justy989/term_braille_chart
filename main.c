#include <stdio.h>
#include <ncurses.h>
#include <locale.h>
#include <math.h>
#include <unistd.h>

#include "braille_buffer.h"
#include "chart.h"

double algo_prices[] = {
     155.85,
     155.91,
     156.05,
     156.08,
     156.17,
     156.04,
     155.84,
     156.03,
     156.01,
     155.97,
     155.75,
     155.86,
     155.95,
     155.99,
     155.95,
     156.00,
     155.96,
     156.01,
     155.98,
     156.02,
     155.95,
     155.94,
     155.95,
     155.87,
     155.84,
     155.86,
     155.93,
     156.04,
     156.15,
     156.26,
     156.21,
     156.25,
     156.24,
     156.25,
     156.30,
     156.40,
     156.35,
     156.47,
     156.47,
     156.62,
     156.67,
     156.64,
     156.82,
     156.89,
     156.90,
     156.85,
     156.70,
     156.69,
     156.62,
     156.76,
     156.80,
     156.80,
     156.78,
     156.79,
     156.92,
     156.79,
     156.81,
     156.87,
     156.87,
     156.83,
     156.76,
     156.74,
     156.82,
     156.86,
     156.85,
     156.80,
     156.73,
     156.62,
     156.54,
     156.59,
     156.56,
     156.54,
     156.58,
     156.60,
     156.56,
     156.65,
     156.61,
     156.64,
     156.66,
     156.62,
     156.51,
     156.58,
     156.60,
     156.69,
     156.71,
     156.75,
     156.74,
     156.73,
     156.67,
     156.71,
     156.74,
     156.71,
     156.72,
     156.65,
     156.73,
     156.73,
     156.69,
     156.76,
     156.70,
     156.73,
     156.81,
     156.80,
     156.81,
     156.85,
     156.81,
     156.78,
     156.79,
     156.69,
     156.69,
     156.64,
     156.59,
     156.59,
     156.69,
     156.62,
     156.62,
     156.55,
     156.56,
     156.60,
     156.61,
     156.61,
     156.57,
     156.58,
     156.64,
     156.70,
     156.67,
     156.67,
     156.59,
     156.66,
     156.62,
     156.61,
     156.63,
     156.58,
     156.56,
     156.62,
     156.63,
     156.57,
     156.59,
     156.56,
     156.57,
     156.59,
     156.57,
     156.59,
     156.53,
     156.58,
     156.56,
     156.56,
     156.64,
     156.62,
     156.66,
     156.67,
     156.67,
     156.64,
     156.65,
     156.68,
     156.67,
     156.66,
     156.72,
     156.71,
     156.69,
     156.68,
     156.69,
     156.65,
     156.64,
     156.63,
     156.62,
     156.59,
     156.61,
     156.66,
     156.66,
     156.65,
     156.70,
     156.72,
     156.71,
     156.67,
     156.68,
     156.66,
     156.68,
     156.71,
     156.73,
     156.68,
     156.73,
     156.71,
     156.69,
     156.68,
     156.67,
     156.65,
     156.65,
     156.66,
     156.67,
     156.67,
     156.67,
     156.66,
     156.66,
     156.67,
     156.74,
     156.75,
     156.79,
     156.71,
     156.68,
     156.66,
     156.65,
     156.66,
     156.70,
     156.72,
     156.71,
     156.69,
     156.66,
     156.69,
     156.63,
     156.64,
     156.67,
     156.69,
     156.70,
     156.70,
     156.71,
     156.70,
     156.69,
     156.68,
     156.73,
     156.76,
     156.78,
     156.77,
     156.77,
     156.75,
     156.77,
     156.73,
     156.76,
     156.75,
     156.70,
     156.69,
     156.71,
     156.72,
     156.70,
     156.66,
     156.64,
     156.66,
     156.66,
     156.64,
     156.61,
     156.54,
     156.61,
     156.63,
     156.63,
     156.63,
     156.60,
     156.55,
     156.60,
     156.60,
     156.53,
     156.59,
     156.55,
     156.57,
     156.51,
     156.50,
     156.48,
     156.48,
     156.51,
     156.52,
     156.51,
     156.51,
     156.57,
     156.59,
     156.57,
     156.58,
     156.55,
     156.58,
     156.58,
     156.58,
     156.56,
     156.49,
     156.43,
     156.53,
     156.54,
     156.53,
     156.59,
     156.56,
     156.55,
     156.55,
     156.57,
     156.57,
     156.57,
     156.56,
     156.52,
     156.55,
     156.55,
     156.58,
     156.63,
     156.68,
     156.73,
     156.70,
     156.70,
     156.67,
     156.63,
     156.51,
     156.45,
     156.47,
     156.46,
     156.45,
     156.46,
     156.48,
     156.54,
     156.52,
     156.46,
     156.47,
     156.47,
     156.46,
     156.47,
     156.43,
     156.45,
     156.47,
     156.45,
     156.49,
     156.51,
     156.54,
     156.52,
     156.48,
     156.49,
     156.48,
     156.48,
     156.45,
     156.39,
     156.42,
     156.44,
     156.46,
     156.45,
     156.44,
     156.48,
     156.51,
     156.54,
     156.57,
     156.51,
     156.57,
     156.56,
     156.55,
     156.56,
     156.57,
     156.59,
     156.56,
     156.52,
     156.53,
     156.55,
     156.54,
     156.58,
     156.63,
     156.57,
     156.55,
     156.55,
     156.59,
     156.62,
     156.58,
     156.57,
     156.58,
     156.57,
     156.53,
     156.53,
     156.51,
     156.48,
     156.51,
     156.51,
     156.48,
     156.54,
     156.59,
     156.56,
     156.65,
     156.62,
     156.64,
     156.61,
     156.62,
     156.60,
     156.63,
     156.58,
     156.53,
     156.52,
     156.49,
     156.50,
     156.51,
     156.49,
     156.48,
     156.45,
     156.43,
     156.45,
     156.46,
     156.46,
     156.46,
     156.52,
     156.49,
     156.49,
     156.54,
     156.59,
     156.61,
};

double algo_hft_prices[] = {
     155.77,
     155.90,
     156.05,
     156.03,
     156.13,
     156.01,
     155.82,
     156.01,
     155.96,
     155.92,
     155.72,
     155.86,
     155.92,
     155.98,
     155.90,
     155.98,
     155.94,
     155.99,
     155.98,
     156.00,
     155.91,
     155.90,
     155.94,
     155.85,
     155.83,
     155.82,
     155.89,
     156.03,
     156.12,
     156.22,
     156.19,
     156.20,
     156.23,
     156.21,
     156.28,
     156.35,
     156.33,
     156.46,
     156.47,
     156.58,
     156.65,
     156.64,
     156.82,
     156.89,
     156.90,
     156.81,
     156.68,
     156.67,
     156.58,
     156.74,
     156.76,
     156.76,
     156.77,
     156.78,
     156.88,
     156.74,
     156.76,
     156.86,
     156.86,
     156.78,
     156.75,
     156.73,
     156.81,
     156.82,
     156.81,
     156.76,
     156.68,
     156.61,
     156.53,
     156.58,
     156.54,
     156.52,
     156.56,
     156.58,
     156.55,
     156.61,
     156.59,
     156.63,
     156.61,
     156.57,
     156.50,
     156.57,
     156.58,
     156.67,
     156.70,
     156.73,
     156.72,
     156.72,
     156.65,
     156.71,
     156.73,
     156.66,
     156.71,
     156.64,
     156.69,
     156.71,
     156.66,
     156.74,
     156.69,
     156.68,
     156.80,
     156.76,
     156.76,
     156.81,
     156.79,
     156.76,
     156.78,
     156.64,
     156.64,
     156.61,
     156.58,
     156.58,
     156.68,
     156.58,
     156.60,
     156.51,
     156.54,
     156.56,
     156.60,
     156.60,
     156.53,
     156.57,
     156.63,
     156.68,
     156.63,
     156.62,
     156.57,
     156.64,
     156.60,
     156.59,
     156.62,
     156.54,
     156.54,
     156.58,
     156.62,
     156.56,
     156.55,
     156.53,
     156.53,
     156.55,
     156.55,
     156.57,
     156.53,
     156.53,
     156.55,
     156.51,
     156.62,
     156.60,
     156.65,
     156.63,
     156.65,
     156.60,
     156.64,
     156.64,
     156.66,
     156.62,
     156.71,
     156.70,
     156.68,
     156.63,
     156.67,
     156.60,
     156.63,
     156.62,
     156.60,
     156.54,
     156.57,
     156.61,
     156.65,
     156.64,
     156.65,
     156.70,
     156.70,
     156.65,
     156.67,
     156.64,
     156.67,
     156.67,
     156.68,
     156.66,
     156.71,
     156.69,
     156.64,
     156.66,
     156.63,
     156.63,
     156.63,
     156.62,
     156.62,
     156.64,
     156.63,
     156.64,
     156.64,
     156.66,
     156.73,
     156.73,
     156.77,
     156.67,
     156.66,
     156.64,
     156.64,
     156.65,
     156.65,
     156.68,
     156.69,
     156.64,
     156.61,
     156.68,
     156.59,
     156.62,
     156.65,
     156.64,
     156.65,
     156.69,
     156.69,
     156.66,
     156.67,
     156.65,
     156.72,
     156.71,
     156.74,
     156.73,
     156.76,
     156.74,
     156.76,
     156.68,
     156.75,
     156.70,
     156.69,
     156.67,
     156.69,
     156.68,
     156.69,
     156.65,
     156.62,
     156.62,
     156.62,
     156.62,
     156.56,
     156.52,
     156.60,
     156.59,
     156.59,
     156.59,
     156.55,
     156.53,
     156.59,
     156.58,
     156.51,
     156.54,
     156.54,
     156.56,
     156.47,
     156.48,
     156.47,
     156.47,
     156.50,
     156.47,
     156.47,
     156.46,
     156.56,
     156.57,
     156.53,
     156.56,
     156.51,
     156.56,
     156.53,
     156.53,
     156.55,
     156.45,
     156.42,
     156.49,
     156.52,
     156.51,
     156.54,
     156.52,
     156.53,
     156.53,
     156.56,
     156.56,
     156.56,
     156.55,
     156.50,
     156.54,
     156.54,
     156.53,
     156.61,
     156.63,
     156.69,
     156.68,
     156.69,
     156.66,
     156.58,
     156.50,
     156.44,
     156.42,
     156.42,
     156.43,
     156.43,
     156.44,
     156.49,
     156.50,
     156.45,
     156.42,
     156.46,
     156.45,
     156.45,
     156.42,
     156.41,
     156.45,
     156.41,
     156.47,
     156.49,
     156.52,
     156.47,
     156.47,
     156.47,
     156.47,
     156.44,
     156.40,
     156.37,
     156.41,
     156.43,
     156.44,
     156.40,
     156.39,
     156.47,
     156.46,
     156.50,
     156.55,
     156.49,
     156.52,
     156.54,
     156.50,
     156.54,
     156.52,
     156.54,
     156.54,
     156.51,
     156.52,
     156.50,
     156.52,
     156.57,
     156.59,
     156.52,
     156.54,
     156.54,
     156.54,
     156.61,
     156.54,
     156.56,
     156.57,
     156.56,
     156.52,
     156.49,
     156.50,
     156.46,
     156.49,
     156.50,
     156.46,
     156.49,
     156.55,
     156.55,
     156.63,
     156.60,
     156.62,
     156.59,
     156.60,
     156.56,
     156.59,
     156.57,
     156.48,
     156.51,
     156.44,
     156.49,
     156.47,
     156.47,
     156.47,
     156.44,
     156.42,
     156.40,
     156.41,
     156.45,
     156.41,
     156.51,
     156.44,
     156.44,
     156.54,
     156.58,
     156.60,
};

double algo_quantities[] = {
     1006,
     688,
     372,
     704,
     782,
     1050,
     242,
     734,
     352,
     467,
     448,
     457,
     490,
     313,
     122,
     273,
     274,
     143,
     428,
     416,
     301,
     226,
     246,
     412,
     627,
     291,
     288,
     424,
     381,
     595,
     274,
     340,
     325,
     340,
     625,
     296,
     313,
     236,
     1304,
     588,
     419,
     1126,
     908,
     656,
     595,
     660,
     318,
     276,
     575,
     571,
     369,
     165,
     199,
     319,
     258,
     401,
     221,
     437,
     195,
     237,
     247,
     264,
     509,
     137,
     125,
     384,
     368,
     609,
     352,
     414,
     464,
     300,
     354,
     190,
     173,
     357,
     207,
     189,
     215,
     261,
     210,
     330,
     138,
     380,
     564,
     185,
     156,
     218,
     333,
     246,
     326,
     187,
     299,
     386,
     235,
     225,
     566,
     214,
     139,
     442,
     416,
     113,
     169,
     192,
     214,
     36,
     240,
     283,
     173,
     479,
     95,
     254,
     140,
     89,
     204,
     345,
     241,
     499,
     146,
     202,
     336,
     214,
     495,
     362,
     147,
     211,
     197,
     294,
     128,
     66,
     219,
     194,
     246,
     233,
     190,
     92,
     81,
     101,
     324,
     104,
     118,
     118,
     159,
     103,
     102,
     358,
     252,
     85,
     80,
     183,
     85,
     72,
     53,
     67,
     256,
     145,
     157,
     72,
     76,
     42,
     95,
     59,
     111,
     73,
     106,
     55,
     163,
     115,
     53,
     278,
     228,
     32,
     53,
     77,
     89,
     51,
     165,
     182,
     102,
     195,
     82,
     318,
     125,
     58,
     213,
     100,
     8,
     62,
     22,
     245,
     62,
     144,
     203,
     305,
     643,
     537,
     171,
     232,
     590,
     128,
     172,
     158,
     245,
     63,
     144,
     169,
     119,
     85,
     266,
     535,
     73,
     155,
     86,
     15,
     133,
     100,
     117,
     299,
     169,
     179,
     248,
     23,
     111,
     187,
     174,
     139,
     19,
     193,
     410,
     209,
     62,
     105,
     213,
     96,
     105,
     58,
     63,
     117,
     721,
     272,
     43,
     258,
     65,
     91,
     123,
     90,
     85,
     154,
     115,
     136,
     97,
     207,
     325,
     23,
     88,
     100,
     43,
     58,
     81,
     117,
     291,
     393,
     397,
     87,
     93,
     120,
     168,
     51,
     121,
     195,
     243,
     171,
     57,
     84,
     104,
     121,
     98,
     215,
     87,
     73,
     77,
     151,
     131,
     53,
     70,
     90,
     177,
     243,
     215,
     114,
     166,
     152,
     224,
     420,
     72,
     64,
     263,
     281,
     1068,
     316,
     56,
     196,
     112,
     488,
     149,
     57,
     136,
     98,
     121,
     139,
     246,
     129,
     112,
     39,
     103,
     4,
     113,
     49,
     124,
     325,
     169,
     95,
     148,
     38,
     86,
     240,
     138,
     257,
     18,
     202,
     170,
     48,
     158,
     52,
     302,
     153,
     189,
     109,
     209,
     96,
     156,
     205,
     224,
     267,
     153,
     96,
     103,
     187,
     176,
     112,
     323,
     109,
     316,
     64,
     193,
     280,
     157,
     251,
     158,
     238,
     169,
     233,
     577,
     285,
     202,
     183,
     175,
     213,
     142,
     392,
     200,
     335,
     182,
     362,
     369,
     276,
     355,
     493,
     360,
     464,
     721,
     569,
     709,
     809,
     1113,
     765,
     965,
     665,
     1029,
     3752,
};

bool price_axis_label_format_func(int32_t index, int32_t max, char* label, int32_t byte_count, void* user_data){
     Chart_t* chart = user_data;
     double range = chart->y_max - chart->y_min;
     double value = chart->y_min + ((double)(max - index) / (double)(max)) * range;
     value -= fmod(value, 0.05);
     snprintf(label, byte_count, "$%.2f", value);
     return true;
}

bool quantity_axis_label_format_func(int32_t index, int32_t max, char* label, int32_t byte_count, void* user_data){
     Chart_t* chart = user_data;
     double range = chart->y_max - chart->y_min;
     double value = chart->y_min + ((double)(max - index) / (double)(max)) * range;
     value -= fmod(value, 50.0);
     snprintf(label, byte_count, "%'d", (int)(value));
     return true;
}

bool time_axis_label_format_func(int32_t index, int32_t max, char* label, int32_t byte_count, void* user_data){
     double total_minutes = (double)(max);
     int32_t minute = 30 + total_minutes * ((double)(index) / (double)(max));
     int32_t hour = 9 + (minute / 60);
     minute %= 60;
     snprintf(label, byte_count, "%02d:%02d", hour, minute);
     return true;
}

int main(){
     setlocale(LC_ALL, "");

     WINDOW* chart_window = NULL;

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

     chart_window = newwin(80, 210, 0, 0);

     init_pair(1, COLOR_BLUE, -1);
     init_pair(2, COLOR_RED, -1);
     init_pair(3, COLOR_GREEN, -1);
     init_pair(4, COLOR_MAGENTA, -1);
     init_pair(5, COLOR_YELLOW, -1);

     Chart_t price_chart = {CHART_TYPE_LINE, 155.6, 157.0};
     Chart_t quantity_chart = {CHART_TYPE_BAR, 0.0, 3752.0};
     Chart_t lost_chart = {CHART_TYPE_LINE, 0.0, 2600.0};
     int32_t data_start = 50;
     int32_t data_end = 100;
     const int32_t data_entries = sizeof(algo_prices) / sizeof(algo_prices[0]);
     int32_t move_speed = 1;

     double price_differences[data_entries];
     double price_difference = 0;
     for(int32_t i = 0; i < data_entries; i++){
          price_difference += (algo_prices[i] * algo_quantities[i]) - (algo_hft_prices[i] * algo_quantities[i]);
          price_differences[i] = price_difference;
     }

     ChartView_t chart_view = {};
     chart_view.data_entry_count = data_entries;
     chart_view.left_axis_label_format_func = price_axis_label_format_func;
     chart_view.bottom_axis_label_format_func = time_axis_label_format_func;
     chart_view.right_axis_label_format_func = quantity_axis_label_format_func;
     chart_view.left_axis_label_format_func_data = &price_chart;
     chart_view.bottom_axis_label_format_func_data = NULL;
     chart_view.right_axis_label_format_func_data = &quantity_chart;
     chart_view.space_between_bottom_axis_labels = 3;
     chart_view.lines_between_right_axis_labels = 5;
     chart_view.lines_between_left_axis_labels = 5;

     chart_view_resize(&chart_view, 210, 80, 8, 5);

     bool done = false;
     while(true){
          int key = getch();
          switch(key){
          default:
               break;
          case 'q':
               done = true;
               break;
          case KEY_LEFT:
               if(data_start > 0){
                    data_start -= move_speed;
                    data_end -= move_speed;
               }
               werase(chart_window);
               break;
          case KEY_RIGHT:
               if(data_end < data_entries){
                    data_start += move_speed;
                    data_end += move_speed;
               }
               werase(chart_window);
               break;
          case KEY_UP:
          {
               int32_t diff = data_end - data_start;
               diff /= 4;
               data_end -= diff;
               data_start += diff;
               if(data_end < data_start) data_end = data_start;
               werase(chart_window);
          } break;
          case KEY_DOWN:
          {
               int32_t diff = data_end - data_start;
               data_end += diff;
               data_start -= diff;
               if(data_start < 0){
                    data_start = 0;
                    data_end = diff * 2;
               }
               if(data_end >= data_entries){
                    data_end = data_entries - 1;
                    data_start = (data_entries - 1) - diff * 2;
               }
               if(data_start < 0) data_start = 0;
               werase(chart_window);
          } break;
          }

          if(done) break;

          {
               chart_view_clear_charts(&chart_view);

               chart_view.data_start_index = data_start;
               chart_view.data_end_index = data_end;

               chart_view_add_chart(&chart_view, price_chart, algo_prices, 1);
               chart_view_add_chart(&chart_view, price_chart, algo_hft_prices, 2);
               chart_view_add_chart(&chart_view, lost_chart, price_differences, 5);
               chart_view_add_chart(&chart_view, quantity_chart, algo_quantities, 3);

               // TODO: this gets cleared when we call chart_view_resize(), what should we do about this?
               chart_view.braille_buffer_combined.color_pair = 4;

               chart_view_draw(&chart_view, chart_window);
          }

          move(0, 0);
          wrefresh(chart_window);

          usleep(100000);
     }

     endwin();
     return 0;
}
