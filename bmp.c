#include <stdio.h>
#include <stdlib.h>

void read_from_bmp(int ***pixel_matrix, int N, int M, const char *path) {
    FILE *file = fopen(path, "rb");
    if (!file) {
        perror("Error opening file");
        return;
    }

    unsigned char header[54];
    fread(header, sizeof(unsigned char), 54, file);

    int padding = (4 - (M * 3) % 4) % 4;

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            unsigned char color[3];
            fread(color, sizeof(unsigned char), 3, file);
            pixel_matrix[N-i-1][j][0] = (int)color[2]; // Red
            pixel_matrix[N-i-1][j][1] = (int)color[1]; // Green
            pixel_matrix[N-i-1][j][2] = (int)color[0]; // Blue
        }
        fseek(file, padding, SEEK_CUR);
    }

    fclose(file);
}

void write_to_bmp(int ***pixel_matrix, int N, int M, const char *path) {
    FILE *file = fopen(path, "wb");
    if (!file) {
        perror("Error opening file");
        return;
    }

    unsigned char header[54] = {
        0x42, 0x4D, // BMP signature
        0, 0, 0, 0, // File size
        0, 0, 0, 0, // Reserved
        54, 0, 0, 0, // Data offset
        40, 0, 0, 0, // Header size
        0, 0, 0, 0, // Width
        0, 0, 0, 0, // Height
        1, 0,       // Planes
        24, 0,      // Bits per pixel
        0, 0, 0, 0, // Compression
        0, 0, 0, 0, // Image size
        0, 0, 0, 0, // X pixels per meter
        0, 0, 0, 0, // Y pixels per meter
        0, 0, 0, 0, // Total colors
        0, 0, 0, 0  // Important colors
    };

    int padding = (4 - (M * 3) % 4) % 4;
    int fileSize = 54 + (3 * M + padding) * N;
    *(int *)&header[2] = fileSize;
    *(int *)&header[18] = M;
    *(int *)&header[22] = N;

    fwrite(header, sizeof(unsigned char), 54, file);

    unsigned char pad[3] = {0, 0, 0};
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            unsigned char color[3];
            color[2] = (unsigned char)(pixel_matrix[N-i-1][j][0]); // Red
            color[1] = (unsigned char)(pixel_matrix[N-i-1][j][1]); // Green
            color[0] = (unsigned char)(pixel_matrix[N-i-1][j][2]); // Blue
            fwrite(color, sizeof(unsigned char), 3, file);
        }
        fwrite(pad, sizeof(unsigned char), padding, file);
    }

    fclose(file);
}