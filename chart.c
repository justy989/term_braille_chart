#include "chart.h"

#include <stdlib.h>
#include <string.h>
#include <math.h>

static int32_t double_round(double d){
     return (int32_t)(d + 0.5);
}

static void get_buffer_dimensions_from_window_dimensions(int32_t window_width, int32_t window_height,
                                                         int32_t left_axis_label_width, int32_t right_axis_label_width,
                                                         int32_t* buffer_width, int32_t* buffer_height){
     *buffer_width = window_width - (left_axis_label_width + right_axis_label_width);
     *buffer_height = window_height - 2;
}

void chart_view_resize(ChartView_t* chart_view, int32_t window_width, int32_t window_height,
                       int32_t left_axis_label_width, int32_t right_axis_label_width){
     int32_t buffer_width = 0;
     int32_t buffer_height = 0;
     get_buffer_dimensions_from_window_dimensions(window_width, window_height, left_axis_label_width, right_axis_label_width,
                                                  &buffer_width, &buffer_height);
     for(int32_t i = 0; i < chart_view->braille_buffer_count; i++){
          BrailleBuffer_t* buffer = chart_view->braille_buffers + i;
          braille_buffer_free(buffer);
          braille_buffer_init(buffer, buffer_width, buffer_height);
          buffer->x_offset = left_axis_label_width;
     }

     braille_buffer_free(&chart_view->braille_buffer_combined);
     braille_buffer_init(&chart_view->braille_buffer_combined, buffer_width, buffer_height);
     chart_view->braille_buffer_combined.x_offset = left_axis_label_width;

     chart_view->width = window_width;
     chart_view->height = window_height;
     chart_view->left_axis_label_width = left_axis_label_width;
     chart_view->right_axis_label_width = right_axis_label_width;
}

void chart_view_free(ChartView_t* chart_view){
     free(chart_view->braille_buffers);
     memset(chart_view, 0, sizeof(*chart_view));
}

static void draw_chart_on_braille_buffer(Chart_t* chart, double* data, BrailleBuffer_t* braille_buffer,
                                         int32_t start_index, int32_t stop_index, int32_t total_entry_count){
     switch(chart->type){
     default:
          break;
     case CHART_TYPE_LINE:
     {
          double y_range = chart->y_max - chart->y_min;
          int32_t chart_height = braille_buffer_pixel_height(braille_buffer);
          int32_t chart_width = braille_buffer_pixel_width(braille_buffer);
          int32_t prev_x = -1;
          int32_t prev_y = -1;
          int32_t entry_count = (stop_index - start_index) + 1;
          for(int32_t i = start_index; i <= stop_index; i++){
               if(i < 0) continue;
               if(i >= total_entry_count) continue;
               double d = (data[i] - chart->y_min) / y_range;
               int32_t y = chart_height - double_round((double)(chart_height) * d);

               d = (double)(i - start_index) / (double)(entry_count);
               int32_t x = double_round((double)(chart_width) * d);

               braille_buffer_line(braille_buffer, prev_x, prev_y, x, y);

               prev_x = x;
               prev_y = y;
          }
     } break;
     case CHART_TYPE_BAR:
     {
          double y_range = chart->y_max - chart->y_min;
          int32_t chart_height = braille_buffer_pixel_height(braille_buffer);
          int32_t chart_width = braille_buffer_pixel_width(braille_buffer);
          int32_t entry_count = (stop_index - start_index) + 1;
          int32_t bar_width = chart_width / entry_count;
          for(int32_t i = start_index; i <= stop_index; i++){
               if(i < 0) continue;
               if(i >= total_entry_count) continue;
               double d = (data[i] - chart->y_min) / y_range;
               int32_t cur_y = chart_height - double_round((double)(chart_height) * d);
               d = (double)(i - start_index) / (double)(entry_count);
               int32_t x = double_round((double)(chart_width) * d);
               for(int32_t j = chart_height - 1; j >= cur_y; j--){
                    for(int32_t k = 0; k <= bar_width; k++){
                         braille_buffer_set_pixel(braille_buffer, x + k, j, true);
                    }
               }
          }
     } break;
     }
}

void chart_view_add_chart(ChartView_t* chart_view, Chart_t chart, double* data, int color_pair){
     int32_t new_buffer_count = chart_view->braille_buffer_count + 1;
     chart_view->braille_buffers = realloc(chart_view->braille_buffers, new_buffer_count * sizeof(*chart_view->braille_buffers));
     BrailleBuffer_t* buffer = chart_view->braille_buffers + chart_view->braille_buffer_count;
     memset(buffer, 0, sizeof(*buffer));
     buffer->x_offset = chart_view->left_axis_label_width;
     int32_t buffer_width = 0;
     int32_t buffer_height = 0;
     get_buffer_dimensions_from_window_dimensions(chart_view->width, chart_view->height,
                                                  chart_view->left_axis_label_width, chart_view->right_axis_label_width,
                                                  &buffer_width, &buffer_height);
     braille_buffer_init(buffer, buffer_width, buffer_height);
     draw_chart_on_braille_buffer(&chart, data, buffer, chart_view->data_start_index, chart_view->data_end_index,
                                  chart_view->data_entry_count);
     buffer->color_pair = color_pair;

     for(int32_t b = 0; b < chart_view->braille_buffer_count; b++){
          for(int32_t j = 0; j < chart_view->braille_buffer_combined.height; j++){
               for(int32_t i = 0; i < chart_view->braille_buffer_combined.width; i++){
                    uint8_t* cell_a = braille_buffer_get_cell(buffer, i, j);
                    uint8_t* cell_b = braille_buffer_get_cell(chart_view->braille_buffers + b, i, j);

                    if(*cell_a && *cell_b){
                         uint8_t* cell_all = braille_buffer_get_cell(&chart_view->braille_buffer_combined, i, j);
                         *cell_all |= *cell_a | *cell_b;
                    }
               }
          }
     }

     chart_view->braille_buffer_count = new_buffer_count;
}

void chart_view_clear_charts(ChartView_t* chart_view){
     for(int32_t b = 0; b < chart_view->braille_buffer_count; b++){
          braille_buffer_clear(chart_view->braille_buffers + b);
     }

     braille_buffer_clear(&chart_view->braille_buffer_combined);
}

void chart_view_draw(ChartView_t* chart_view, WINDOW* window){
     // draw left y axis labels
     if(chart_view->left_axis_label_format_func){
          wstandend(window);
          int32_t line_skip = 0;
          int32_t label_draw_height = chart_view->braille_buffer_combined.height;
          for(int32_t i = 0; i < label_draw_height; i++){
               if(line_skip == chart_view->lines_between_left_axis_labels || i == (label_draw_height - 1) || i == 0){
                    int32_t label_bytes = chart_view->left_axis_label_width + 1;
                    char label[label_bytes];
                    memset(label, 0, label_bytes);
                    chart_view->left_axis_label_format_func(i, label_draw_height, label, label_bytes,
                                                            chart_view->left_axis_label_format_func_data);
                    mvwaddstr(window, i, 0, label);
                    line_skip = 0;
               }else{
                    line_skip++;
               }
          }
     }

     // draw right y axis labels
     if(chart_view->right_axis_label_format_func){
          wstandend(window);
          int32_t line_skip = 0;
          int32_t label_draw_height = chart_view->braille_buffer_combined.height;
          for(int32_t i = 0; i < label_draw_height; i++){
               if(line_skip == chart_view->lines_between_right_axis_labels || i == (label_draw_height - 1) || i == 0){
                    int32_t label_bytes = chart_view->right_axis_label_width + 1;
                    char label[label_bytes];
                    memset(label, 0, label_bytes);
                    chart_view->right_axis_label_format_func(i, label_draw_height, label, label_bytes,
                                                             chart_view->right_axis_label_format_func_data);
                    mvwaddstr(window, i, chart_view->width - chart_view->right_axis_label_width, label);
                    line_skip = 0;
               }else{
                    line_skip++;
               }
          }
     }

     // draw bottom x axis labels
     wstandend(window);
     int32_t data_range = (chart_view->data_end_index - chart_view->data_start_index) + 1;
     for(int32_t i = 0; i < chart_view->braille_buffer_combined.width;){
          int32_t label_bytes = 128;
          char label[label_bytes];
          memset(label, 0, label_bytes);
          double d = (double)(i) / (double)(chart_view->braille_buffer_combined.width);
          int32_t index = chart_view->data_start_index + double_round((double)(data_range) * d);
          chart_view->bottom_axis_label_format_func(index, chart_view->data_entry_count, label, label_bytes, chart_view->bottom_axis_label_format_func_data);
          int32_t len = strlen(label);
          mvwprintw(window, chart_view->height - 1, i + chart_view->left_axis_label_width, label);
          i += len + chart_view->space_between_bottom_axis_labels;
     }

     // draw charts
     for(int32_t i = 0; i < chart_view->braille_buffer_count; i++){
          BrailleBuffer_t* braille_buffer = chart_view->braille_buffers + i;
          wstandend(window);
          wattron(window, COLOR_PAIR(braille_buffer->color_pair));
          braille_buffer_draw(braille_buffer, window);
     }

     wstandend(window);
     wattron(window, COLOR_PAIR(chart_view->braille_buffer_combined.color_pair));
     braille_buffer_draw(&chart_view->braille_buffer_combined, window);
}
