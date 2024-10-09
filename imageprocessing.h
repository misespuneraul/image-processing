#ifndef IMAGEPROCESSING_H_INCLUDED
#define IMAGEPROCESSING_H_INCLUDED

int ***flip_horizontal(int ***image, int N, int M);
int ***rotate_left(int ***image, int N, int M);
int ***crop(int ***image, int N, int M, int x, int y, int h, int w);
int ***extend(int ***image, int N, int M, int rows, int cols, int new_R, int new_G, int new_B);
int ***paste(int ***image_dst, int N_dst, int M_dst, int *** image_src, int N_src, int M_src, int x, int y);
int ***apply_filter(int ***image, int N, int M, float **filter, int filter_size);
int ***ralloc(int N, int M);

#endif  // IMAGEPROCESSING_H_INCLUDED
