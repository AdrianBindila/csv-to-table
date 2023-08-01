#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <stdlib.h>

#define MAX_LEN 255
#define MAX_COL 256
#define HEADER_SEP '~'
#define FOOTER_SEP '='

void printSeparators(int width, char separator) {
    for (int i = 0; i < width; i++) {
        printf("%c", separator);
    }
}

int main(__attribute__((unused)) int argc, char *argv[]) {

    //check correct args
    if (argv[1] == NULL) {
        printf("Usage: csv-to-table file.csv");
        return 1;
    } else if (strcmp(argv[1], "--help") == 0) {
        printf("This program takes a .csv file and pretty prints an ASCII table of it");
        return 1;
    }

    const char *fileName = argv[1];

    //check correct extension
    const char *extension = strrchr(fileName, '.');
    if (strcmp(extension, ".csv") != 0) {
        printf("Please specify a .csv file!");
        return 1;
    }

    FILE *fd = fopen(fileName, "r");

    if (fd == NULL) {
        printf("File %s does not exist!", fileName);
        return 1;
    }

    char *buf = malloc(sizeof(char) * MAX_LEN);
    int *maxWidths = calloc(MAX_COL, sizeof(int));
    int noCols = 0;
    //First pass - find the max widths per cell and overall width
    while (fgets(buf, MAX_LEN, fd)) {
        int i = 0;
        const char *cell = strtok(buf, ",\n");
        while (cell != NULL) {
            int len = (int) strlen(cell);
            if (maxWidths[i] < len) {
                maxWidths[i++] = len + 2;
                noCols = i;
            }
            cell = strtok(NULL, ",\n");
        }
    }
    printf("\n");

    //Compute overall widths
    int sumWidths = 2;
    for (int i = 0; i < noCols; i++)
        sumWidths += maxWidths[i] + 2;

    rewind(fd);

    int row = 0;
    //Second pass - align text to max widths per column, pad rest with spaces
    while (fgets(buf, MAX_LEN, fd)) {
        int col = 0;
        if (row == 0) printSeparators(sumWidths, HEADER_SEP);

        printf("\n|");
        char *cell = strtok(buf, ",\n");
        while (cell != NULL) {
            int spaceRem = maxWidths[col++] - (int) strlen(cell);
            int left;
            int right;

            if (spaceRem % 2 == 0) {
                left = right = spaceRem / 2;
            } else {
                left = right = spaceRem / 2;
                left++;
            }

            printf("|%*c", left, ' ');
            printf("%s", cell);
            printf("%*c|", right, ' ');

            cell = strtok(NULL, ",\n");
        }
        printf("|");

        if (row == 0) {
            printf("\n");
            printSeparators(sumWidths, HEADER_SEP);
        }
        row++;
    }
    printf("\n");
    printSeparators(sumWidths,FOOTER_SEP);
    printf("\n");

    free(maxWidths);
    free(buf);
    fclose(fd);
    return 0;
}
