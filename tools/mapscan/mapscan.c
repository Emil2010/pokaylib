#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

typedef struct {
    uint8_t R;
    uint8_t G;
    uint8_t B;
} Pixel;

typedef struct {
    Pixel **PXS;
    uint16_t width;
    uint16_t height;
    uint16_t tilesNumber;
} Image;

void getDimensions(const char *filename, Image *img);
void parsePixels(const char *filename, Image *img);
void slicingTiles(Image *img, Pixel ****tiles);
void freeImg(Image *img);
void freeTiles(int width, int height, Pixel ***tiles);
bool compareTile(Pixel **mapPX, Pixel **tilePX);
void compareMap(Pixel ***map, Pixel ***tiles, int Nmap, int Ntiles, Image img, const char *outputfile);
void writePNG(Image mapImg, Image tilesetImg, const char *tilesetfile, const char *outputfile);

uint16_t tileSize; // Global

// in Aseprite, export in CSS and rename to .dat, delete the first lines of information and the ; } at the end
int main(int argc, char *argv[])
{
    if (argc != 4) {
        printf("Usage: %s <map>.dat <tileset>.dat <tile size>\n", argv[0]);
        return 1; // Return error if tile size is not provided
    }

    tileSize = atoi(argv[3]);
    printf("Tile size: %d*%d px\n", tileSize, tileSize);

    const char *mapfile = argv[1];
    const char *tilesetfile = argv[2];

    Image tilesetImg;
    Image mapImg;
    Pixel ***tilesetTiles;
    Pixel ***mapTiles;

    // rename the output from <map>.css to <map>.dat
    size_t mapfile_length = strlen(mapfile);
    char *outputfile = (char *)malloc(mapfile_length + 1); // +1 for null terminator

    // Copy the map filename and replace .css with .dat
    strcpy(outputfile, mapfile);
    char *map_extension = strrchr(outputfile, '.');
    if (map_extension) {
        strcpy(map_extension, ".dat");
    }

    getDimensions(tilesetfile, &tilesetImg);
    getDimensions(mapfile, &mapImg);

    parsePixels(tilesetfile, &tilesetImg);
    parsePixels(mapfile, &mapImg);

    writePNG(mapImg, tilesetImg, tilesetfile, outputfile);

    slicingTiles(&tilesetImg, &tilesetTiles);
    slicingTiles(&mapImg, &mapTiles);
    compareMap(mapTiles, tilesetTiles, mapImg.tilesNumber, tilesetImg.tilesNumber, mapImg, outputfile);
    //compareTile(mapTiles[1], tilesetTiles[0]);

    freeImg(&tilesetImg);
    freeImg(&mapImg);
    freeTiles(mapImg.width, mapImg.height, mapTiles);
    freeTiles(tilesetImg.width, tilesetImg.height, tilesetTiles);

    return 0;
}

void getDimensions(const char *filename, Image *img)
{
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Error: Could not open file [%s]\n", filename);
        return;
    }

    int maxX = 0, maxY = 0;
    char line[50];
    while (fgets(line, sizeof(line), file)) {
        int x, y;
        sscanf(line, "%dpx %dpx", &x, &y);
        if (x > maxX) maxX = x;
        if (y > maxY) maxY = y;
    }

    img->width = maxX + 1;
    img->height = maxY + 1;
    img->tilesNumber = img->width/tileSize * img->height/tileSize;

    printf("[%s] is %d*%d px -> %d tiles\n", filename, img->width, img->height, img->tilesNumber);

    img->PXS = (Pixel **)malloc(img->width * sizeof(Pixel *));
    for (int i = 0; i < img->width; i++) {
        img->PXS[i] = (Pixel *)malloc(img->height * sizeof(Pixel));
        for (int j = 0; j < img->height; j++) {
            img->PXS[i][j].R = 0;
            img->PXS[i][j].G = 0;
            img->PXS[i][j].B = 0;
        }
    }

    fclose(file);
}

void parsePixels(const char *filename, Image *img)
{
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Error: Could not open file [%s]\n", filename);
        return;
    }

    int x, y;
    char str[32];
    uint32_t rgb;
    while (!feof(file)) {
        fscanf(file, "%dpx %dpx #%06X,", &x, &y, &rgb);
        img->PXS[x][y].R = (rgb >> 16) & 0xFF;
        img->PXS[x][y].G = (rgb >> 8) & 0xFF;
        img->PXS[x][y].B = rgb & 0xFF;
        // printf("px @ [%d, %d] : RGB(\e[31m%03u\e[0m,\e[32m%03u\e[0m,\e[34m%03u\e[0m) #%02x%02x%02x\n", x, y, img->PXS[x][y].R, img->PXS[x][y].G, img->PXS[x][y].B, img->PXS[x][y].R, img->PXS[x][y].G, img->PXS[x][y].B);
    }

    fclose(file);
}

void slicingTiles(Image *img, Pixel ****tiles)
{
    int tilesX = img->width / tileSize;
    int tilesY = img->height / tileSize;
    int tilesNumber = tilesX * tilesY;

    *tiles = (Pixel ***)malloc(tilesNumber * sizeof(Pixel **));
    for (int i = 0; i < tilesNumber; i++) {
        (*tiles)[i] = (Pixel **)malloc(tileSize * sizeof(Pixel *));
        for (int j = 0; j < tileSize; j++) {
            (*tiles)[i][j] = (Pixel *)malloc(tileSize * sizeof(Pixel));
        }
    }

    for (int ty = 0; ty < tilesY; ty++) {
        for (int tx = 0; tx < tilesX; tx++) {
            for (int x = 0; x < tileSize; x++) {
                for (int y = 0; y < tileSize; y++) {
                    (*tiles)[ty * tilesX + tx][x][y] = img->PXS[tx * tileSize + x][ty * tileSize + y];
                }
            }
        }
    }
}

void freeImg(Image *img)
{
    for (int i = 0; i < img->width; i++) {
        free(img->PXS[i]);
    }
    free(img->PXS);
}

void freeTiles(int width, int height, Pixel ***tiles)
{
    int tilesX = width / tileSize;
    int tilesY = height / tileSize;
    int tilesNumber = tilesX * tilesY;

    for (int i = 0; i < tilesNumber; i++) {
        for (int j = 0; j < tileSize; j++) {
            free(tiles[i][j]);
        }
        free(tiles[i]);
    }
    free(tiles);
}

bool compareTile(Pixel **mapPX, Pixel **tilePX)
{
    int match = 0;

    for (int x = 0; x < tileSize; x++) {
        for (int y = 0; y < tileSize; y++) {
            // printf("map [%d, %d] RGB(\e[31m%03u\e[0m,\e[32m%03u\e[0m,\e[34m%03u\e[0m) #%02x%02x%02x\n", x, y, mapPX[x][y].R, mapPX[x][y].G, mapPX[x][y].B, mapPX[x][y].R, mapPX[x][y].G, mapPX[x][y].B);
            // printf("tile[%d, %d] RGB(\e[31m%03u\e[0m,\e[32m%03u\e[0m,\e[34m%03u\e[0m) #%02x%02x%02x ", x, y, tilePX[x][y].R, tilePX[x][y].G, tilePX[x][y].B, tilePX[x][y].R, tilePX[x][y].G, tilePX[x][y].B);

            if ((mapPX[x][y].R == tilePX[x][y].R) &&
                (mapPX[x][y].G == tilePX[x][y].G) &&
                (mapPX[x][y].B == tilePX[x][y].B)) {
                    match++;
                    //printf("\e[32mokay\e[0m\n\n");
                    //printf("█");
            } else {
                //printf("\e[31mnuh huh\e[0m\n\n");
                //printf("░");
                return false;
            }
        }
        //putchar('\n');
    }
    if (match == tileSize * tileSize) { // all the pixel matched
        return true;
    }

    return false;
}

void compareMap(Pixel ***map, Pixel ***tiles, int Nmap, int Ntiles, Image img, const char *outputfile)
{
    int W = img.width/tileSize;
    FILE *output = fopen(outputfile, "a");    // append

    for (int tmap = 0; tmap < Nmap; tmap++) { // {0..8640}
        printf("\e[35mTesting map[%04d]\e[0m\n", tmap);
        for (int t = 0; t < Ntiles; t++) {    // {0...103}
            printf("\e[33m> with tileset[%03d]\e[0m ", t);
            if (compareTile(map[tmap], tiles[t]) == true) {
                printf("\e[32mmatch !\e[0m [%04d][%03d]\n", tmap, t);
                fprintf(output, "%03d ", t);
                t = 0;
                break;
            } else {
                printf("\e[31mno match\e[0m\n");
            }
        }
        if ((tmap + 1) % W == 0)
            fprintf(output, "\n");
    }
}

void writePNG(Image mapImg, Image tilesetImg, const char *tilesetfile, const char *outputfile)
{
    // Allocate memory for the tileset filename with .png extension
    size_t tilesetfile_length = strlen(tilesetfile);
    char *tilesetfile_png = (char *)malloc(tilesetfile_length + 1); // +1 for null terminator

    if (!tilesetfile_png) {
        perror("Failed to allocate memory");
        return;
    }

    // Copy the tileset filename and replace .css with .png
    strcpy(tilesetfile_png, tilesetfile);
    char *extension = strrchr(tilesetfile_png, '.');
    if (extension) {
        strcpy(extension, ".png");
    }

    FILE *output = fopen(outputfile, "w");
    if (!output) {
        printf("Error: Could not open file [%s]\n", outputfile);
        free(tilesetfile_png);
        return;
    }

    fprintf(output, "%03d %03d %03d %03d %03d\n", mapImg.width/tileSize, mapImg.height/tileSize, tilesetImg.tilesNumber, tileSize, 1);
    fprintf(output, "assets/sprites/tilesets/%s\n", tilesetfile_png);
    fclose(output);
    free(tilesetfile_png);
}
