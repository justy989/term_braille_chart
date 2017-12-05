#pragma once

#include "braille_buffer.h"

typedef enum{
     CHART_TYPE_LINE,
     CHART_TYPE_BAR,
}ChartType_t;

typedef struct{
     ChartType_t type;
     double y_min;
     double y_max;
}Chart_t;

typedef bool ChartLabelFormatFunc(int32_t, int32_t, char*, int32_t, void*);

typedef struct{
     BrailleBuffer_t* braille_buffers;
     int32_t braille_buffer_count;
     BrailleBuffer_t braille_buffer_combined;
     int32_t width;
     int32_t height;
     int32_t data_start_index;
     int32_t data_end_index;
     int32_t data_entry_count;
     ChartLabelFormatFunc* left_axis_label_format_func;
     ChartLabelFormatFunc* bottom_axis_label_format_func;
     ChartLabelFormatFunc* right_axis_label_format_func;
     void* left_axis_label_format_func_data;
     void* bottom_axis_label_format_func_data;
     void* right_axis_label_format_func_data;
     int32_t space_between_bottom_axis_labels;
     int32_t lines_between_left_axis_labels;
     int32_t lines_between_right_axis_labels;
     int32_t left_axis_label_width;
     int32_t right_axis_label_width;
}ChartView_t;

void chart_view_resize(ChartView_t* chart_view, int32_t window_width, int32_t window_height,
                       int32_t left_axis_label_width, int32_t right_axis_label_width);
void chart_view_free(ChartView_t* chart_view);
void chart_view_add_chart(ChartView_t* chart_view, Chart_t chart, double* data, int color_pair);
void chart_view_clear_charts(ChartView_t* chart_view);
void chart_view_draw(ChartView_t* chart_view, WINDOW* window);
