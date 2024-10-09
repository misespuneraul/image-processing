#ifndef BMP_H_INCLUDED
#define BMP_H_INCLUDED

void read_from_bmp(int ***pixel_matrix, int N, int M, const char *path);
void write_to_bmp(int ***pixel_matrix, int N, int M, const char *path);

#endif  // BMP_H_INCLUDED
