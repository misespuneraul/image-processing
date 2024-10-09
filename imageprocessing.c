#include <stdio.h>
#include <stdlib.h>
#include "imageprocessing.h"

#define MAXCOLOR 255

int ***ralloc(int N, int M) {
  int ***image = (int ***)calloc(N, sizeof(int **));
  for (int i = 0; i < N; i++) {
    image[i] = (int **)calloc(M, sizeof(int *));
    for (int j = 0; j < M; j++) {
      image[i][j] = (int *)calloc(3, sizeof(int));
    }
  }
  return image;
}


int ***flip_horizontal(int ***image, int N, int M) {
  int aux = 0;
  for (int i = 0; i < (M / 2); i ++) {
    for (int j = 0; j < N; j ++) {
      for (int k = 0; k < 3; k ++) {
        aux = image[j][i][k];
        image[j][i][k] = image[j][M - 1 - i][k];
        image[j][M - 1 - i][k] = aux;
      }
    }
  }
  return image;
}


int ***rotate_left(int ***image, int N, int M) {
  int ***imagecopy = (int ***)calloc(M, sizeof(int **));
  for (int i = 0; i < M; i++) {
    imagecopy[i] = (int **)calloc(N, sizeof(int *));
    for (int j = 0; j < N; j++) {
      imagecopy[i][j] = (int *)calloc(3, sizeof(int));
    }
  }
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < M; j++) {
      for (int k = 0; k < 3; k++) {
        imagecopy[M - 1 - j][i][k] = image[i][j][k];
      }
    }
  }
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < M; j++) {
      free(image[i][j]);
    }
    free(image[i]);
  }
  free(image);
  return imagecopy;
}

int ***crop(int ***image, int N, int M, int x, int y, int h, int w) {
  int ***cropped = (int ***)malloc(h * sizeof(int **));
  for (int i = 0; i < h; i++) {
    cropped[i] = (int **)malloc(w * sizeof(int *));
    for (int j = 0; j < w; j++) {
      cropped[i][j] = (int *)malloc(3 * sizeof(int));
    }
  }
  for (int i = 0; i < h; i++) {
    for (int j = 0; j < w; j++) {
      for (int k = 0; k < 3; k++) {
        cropped[i][j][k] = image[y + i][x + j][k];
      }
    }
  }
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < M; j++) {
      free(image[i][j]);
    }
    free(image[i]);
  }
  free(image);
  return cropped;
}

int ***extend(int ***image, int N, int M, int rows, int cols, int new_R, int new_G, int new_B) {
  int ***extended = (int ***)malloc((N + 2 * rows) * sizeof(int **));
  for (int i = 0; i < N + 2 * rows; i++) {
    extended[i] = (int **)malloc((M + 2 * cols) * sizeof(int *));
    for (int j = 0; j < M + 2 * cols; j++) {
      extended[i][j] = (int *)malloc(3 * sizeof(int));
    }
  }
  int newN = N + 2 * rows;
  int newM = M + 2 * cols;
  for (int i = 0; i < newN; i++) {
    for (int j = 0; j < newM; j++) {
      if (i < rows || j < cols || i >= rows + N || j >= cols + M) {
        extended[i][j][0] = new_R;
        extended[i][j][1] = new_G;
        extended[i][j][2] = new_B;
      } else {
        for (int k = 0; k < 3; k++) {
          extended[i][j][k] = image[i - rows][j - cols][k];
        }
      }
    }
  }
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < M; j++) {
      free(image[i][j]);
    }
    free(image[i]);
  }
  free(image);
  return extended;
}

int ***paste(int ***image_dst, int N_dst, int M_dst, int *** image_src, int N_src, int M_src, int x, int y) {
  int limitN = 0, limitM = 0;
  if (y + N_src > N_dst) {
    limitN = N_dst;
  } else {
    limitN = y + N_src;
  }
  if (x + M_src > M_dst) {
    limitM = M_dst;
  } else {
    limitM = x + M_src;
  }
  for (int i = y; i < limitN; i++) {
    for (int j = x; j < limitM; j++) {
      for (int k = 0; k < 3; k ++) {
        image_dst[i][j][k] = image_src[i - y][j - x][k];
      }
    }
  }
  return image_dst;
}

int ***apply_filter(int ***image, int N, int M, float **filter, int filter_size) {
  int ***imagecopy = (int ***)malloc(N * sizeof(int **));
  for (int i = 0; i < N; i++) {
    imagecopy[i] = (int **)malloc(M * sizeof(int *));
    for (int j = 0; j < M; j++) {
      imagecopy[i][j] = (int *)malloc(3 * sizeof(int));
    }
  }
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < M; j++) {
      for (int k = 0; k < 3; k++) {
        imagecopy[i][j][k] = 0;
        float aux = 0;
        for (int i1 = i - (filter_size / 2); i1 <= i + (filter_size / 2); i1++) {
          for (int j1 = j - (filter_size / 2); j1 <= j + (filter_size / 2); j1++) {
            if (i1 < 0 || j1 < 0 || i1 >= N || j1 >= M) {
              aux = aux + 0;
            } else {
              aux = aux + (float)(image[i1][j1][k]) * filter[i1 + filter_size / 2 - i][j1 + filter_size / 2 - j];
            }
          }
        }
        imagecopy[i][j][k] = (int) (aux);
        if (imagecopy[i][j][k] < 0) {
          imagecopy[i][j][k] = 0;
        }
        if (imagecopy[i][j][k] > MAXCOLOR) {
          imagecopy[i][j][k] = MAXCOLOR;
        }
      }
    }
  }
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < M; j++) {
      free(image[i][j]);
    }
    free(image[i]);
  }
  free(image);
  return imagecopy;
}
