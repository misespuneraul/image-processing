#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "imageprocessing.h"
#include "bmp.h"

#define MAXPATH 100

int main() {
  char *path = calloc(MAXPATH, sizeof(char));
  char *cmd = calloc(4, sizeof(char));


  int index = 0;
  int cur_img = 0;
  int cur_filter = 0, size = 0;
  int x = 0, y = 0, h = 0, w = 0;
  int index_src = 0, index_dst = 0, iximg = 0, ixfilt = 0;
  int rows = 0, cols = 0, r = 0, g = 0, b = 0;

  int **sizes = (int **)calloc(1, sizeof(int *));
  int *filtersize = (int *)calloc(1, sizeof(int));
  int ****memimg = (int ****)calloc(1, sizeof(int ***));
  float ***filter = (float ***)calloc(1, sizeof(float **));
  scanf("%s", cmd);
  while (strcmp(cmd, "e") != 0) {
    // load
    if (strcmp(cmd, "l") == 0) {
        // citire sizeuri si path imagine curenta
        memimg = (int ****)realloc(memimg, (cur_img + 1) * sizeof(int ***));
        sizes = (int **)realloc(sizes, (cur_img + 1) * sizeof(int *));
        sizes[cur_img] = (int *)calloc(2, sizeof(int));
        scanf("%d %d %s", &sizes[cur_img][0], &sizes[cur_img][1], path);
        // realocare size sa tina si urmatorul
        // realocare memimg sa tina si urmatorul
        // alocare memimg imagine curenta
        memimg[cur_img] = (int ***)calloc(sizes[cur_img][0], sizeof(int **));
        for (int i = 0; i < sizes[cur_img][0]; i++) {
            memimg[cur_img][i] = (int **)calloc(sizes[cur_img][1], sizeof(int *));
            for (int j = 0; j < sizes[cur_img][1]; j++) {
                memimg[cur_img][i][j] = (int *)calloc(3, sizeof(int));
            }
        }
        // citire imagine
        read_from_bmp(memimg[cur_img], sizes[cur_img][0], sizes[cur_img][1], path);
        cur_img++;
    }
    // save
    if (strcmp(cmd, "s") == 0) {
        scanf("%d %s", &index, path);
        write_to_bmp(memimg[index], sizes[index][0], sizes[index][1], path);
    }
    // apply horizontal
    if (strcmp(cmd, "ah") == 0) {
        scanf("%d", &index);
        memimg[index] = flip_horizontal(memimg[index], sizes[index][0], sizes[index][1]);
    }
    // apply rotate
    if (strcmp(cmd, "ar") == 0) {
        scanf("%d", &index);
        int ***imagecopy = rotate_left(memimg[index], sizes[index][0], sizes[index][1]);
        memimg[index] = imagecopy;
        int aux = sizes[index][0];
        sizes[index][0] = sizes[index][1];
        sizes[index][1] = aux;
    }
    // apply crop
    if (strcmp(cmd, "ac") == 0) {
      scanf("%d %d %d %d %d", &index, &x, &y, &w, &h);
      int ***cropped = crop(memimg[index], sizes[index][0], sizes[index][1], x, y, h, w);
      memimg[index] = cropped;
      sizes[index][0] = h;
      sizes[index][1] = w;
    }
    // apply extend
    if (strcmp(cmd, "ae") == 0) {
        scanf("%d %d %d %d %d %d", &index, &rows, &cols, &r, &g, &b);
        int newN = sizes[index][0] + 2 * rows;
        int newM = sizes[index][1] + 2 * cols;
        int ***extended = extend(memimg[index], sizes[index][0], sizes[index][1], rows, cols, r, g, b);
        memimg[index] = extended;
        sizes[index][0] = newN;
        sizes[index][1] = newM;
    }
    // apply paste
    if (strcmp(cmd, "ap") == 0) {
        scanf("%d %d %d %d", &index_dst, &index_src, &x, &y);
        int N_dst = sizes[index_dst][0];
        int M_dst = sizes[index_dst][1];
        int N_src = sizes[index_src][0];
        int M_src = sizes[index_src][1];
        memimg[index_dst] = paste(memimg[index_dst], N_dst, M_dst, memimg[index_src], N_src, M_src, x, y);
    }
    // create filter
    if (strcmp(cmd, "cf") == 0) {
        scanf("%d", &size);
        filtersize = (int *)realloc(filtersize, (cur_filter + 2)*sizeof(int));
        filtersize[cur_filter] = size;
        filter = (float ***)realloc(filter, (cur_filter + 2)*sizeof(float **));
        filter[cur_filter] = (float **)calloc(size, sizeof(float *));
        for (int i = 0; i < size; i ++) {
            filter[cur_filter][i] = (float *)calloc(size, sizeof(float));
        }
        for (int i = 0; i < size; i ++) {
            for (int j = 0; j < size; j ++) {
                scanf("%f", &filter[cur_filter][i][j]);
            }
        }
        cur_filter++;
    }
    // apply filter
    if (strcmp(cmd, "af") == 0) {
        scanf("%d %d", &iximg, &ixfilt);
        int Nen = sizes[iximg][0];
        int Men = sizes[iximg][1];
        memimg[iximg] = apply_filter(memimg[iximg], Nen, Men, filter[ixfilt], filtersize[ixfilt]);
    }
    // delete img
    if (strcmp(cmd, "di") == 0) {
        scanf("%d", &index);
        for (int i = index; i < cur_img - 1; i++) {
            for (int j = 0; j < sizes[i][0]; j++) {
                for (int k = 0; k < sizes[i][1]; k++) {
                    free(memimg[i][j][k]);
                }
                free(memimg[i][j]);
            }
             free(memimg[i]);
            memimg[i] = ralloc(sizes[i + 1][0], sizes[i + 1][1]);
            sizes[i][0] = sizes[i + 1][0];
            sizes[i][1] = sizes[i + 1][1];
            for (int j = 0; j < sizes[i + 1][0]; j ++) {
                for (int k = 0; k <sizes[i + 1][1]; k ++) {
                    for (int l = 0; l < 3; l ++) {
                        memimg[i][j][k][l] = memimg[i + 1][j][k][l];
                    }
                }
            }
        }
        cur_img--;
        for (int i = 0; i < sizes[cur_img][0]; i++) {
            for (int j = 0; j < sizes[cur_img][1]; j++) {
                free(memimg[cur_img][i][j]);
            }
            free(memimg[cur_img][i]);
        }
        free(memimg[cur_img]);
        free(sizes[cur_img]);
    }
    // delete filter
    if (strcmp(cmd, "df") == 0) {
        scanf("%d", &index);
        for (int i = index; i < cur_filter - 1; i ++) {
            for (int j = 0; j < filtersize[i]; j ++) {
                free(filter[i][j]);
            }
            free(filter[i]);
            // curatare
            filter[i] = (float **)calloc(filtersize[i + 1], sizeof(float *));
            for (int j = 0; j < filtersize[i + 1]; j ++) {
                filter[i][j] = (float *)calloc(filtersize[i + 1], sizeof(float));
            }
            // realocare
            for (int j = 0; j < filtersize[i + 1]; j ++) {
                for (int k = 0; k < filtersize[i + 1]; k ++) {
                    filter[i][j][k] = filter[i + 1][j][k];
                }
            }
            filtersize[i] = filtersize[i + 1];
        }  // mutare
        cur_filter--;
        for (int j = 0; j < filtersize[cur_filter]; j ++) {
                free(filter[cur_filter][j]);
            }
        free(filter[cur_filter]);
    }
    scanf("%s", cmd);
  }
  for (int i = 0; i < cur_img; i ++) {
    for (int j = 0; j < sizes[i][0]; j ++) {
        for (int k = 0; k < sizes[i][1]; k ++) {
            free(memimg[i][j][k]);
        }
        free(memimg[i][j]);
    }
    free(memimg[i]);
  }
  free(memimg);
  free(cmd);
  free(path);
  for (int i = 0; i < cur_img; i ++) {
    free(sizes[i]);
  }
      free(sizes);
      for (int i = 0; i < cur_filter; i ++) {
        for (int j = 0; j < filtersize[i]; j ++) {
            free(filter[i][j]);
        }
        free(filter[i]);
      }
      free(filter);
    free(filtersize);
  return 0;
}
