#include "include/ledMap.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void copyColorHelper(int dest[LED_TIME_ROW][LED_TIME_COL], const int copyFrom[LED_TIME_ROW][LED_TIME_COL], const int color);

// Matrtices are declared as [row][column]
// Each number takes up LED_TIME_ROW rows and LED_TIME_COL columns
static int LED_numZero[LED_TIME_ROW][LED_TIME_COL] = {
    { 0, 7, 7, 7, 0 },
    { 7, 0, 0, 0, 7 },
    { 7, 0, 0, 7, 7 },
    { 7, 0, 7, 0, 7 },
    { 7, 7, 0, 0, 7 },
    { 7, 0, 0, 0, 7 },
    { 0, 7, 7, 7, 0 }
};

static int LED_numOne[LED_TIME_ROW][LED_TIME_COL] = {
    { 0, 0, 7, 0, 0 },
    { 0, 7, 7, 0, 0 },
    { 0, 0, 7, 0, 0 },
    { 0, 0, 7, 0, 0 },
    { 0, 0, 7, 0, 0 },
    { 0, 0, 7, 0, 0 },
    { 0, 7, 7, 7, 0 }
};

static int LED_numTwo[LED_TIME_ROW][LED_TIME_COL] = {
    { 0, 7, 7, 7, 0 },
    { 7, 0, 0, 0, 7 },
    { 0, 0, 0, 0, 7 },
    { 0, 7, 7, 7, 0 },
    { 7, 0, 0, 0, 0 },
    { 7, 0, 0, 0, 0 },
    { 7, 7, 7, 7, 7 }
};

static int LED_numThree[LED_TIME_ROW][LED_TIME_COL] = {
    { 7, 7, 7, 7, 0 },
    { 0, 0, 0, 0, 7 },
    { 0, 0, 0, 0, 7 },
    { 0, 7, 7, 7, 0 },
    { 0, 0, 0, 0, 7 },
    { 0, 0, 0, 0, 7 },
    { 7, 7, 7, 7, 0 }
};

static int LED_numFour[LED_TIME_ROW][LED_TIME_COL] = {
    { 0, 0, 7, 0, 7 },
    { 0, 7, 0, 0, 7 },
    { 7, 0, 0, 0, 7 },
    { 7, 7, 7, 7, 7 },
    { 0, 0, 0, 0, 7 },
    { 0, 0, 0, 0, 7 },
    { 0, 0, 0, 0, 7 }
};

static int LED_numFive[LED_TIME_ROW][LED_TIME_COL] = {
    { 7, 7, 7, 7, 7 },
    { 7, 0, 0, 0, 0 },
    { 7, 7, 7, 7, 0 },
    { 0, 0, 0, 0, 7 },
    { 0, 0, 0, 0, 7 },
    { 7, 0, 0, 0, 7 },
    { 0, 7, 7, 7, 0 }
};

static int LED_numSix[LED_TIME_ROW][LED_TIME_COL] = {
    { 0, 0, 7, 7, 7 },
    { 0, 7, 0, 0, 0 },
    { 7, 0, 0, 0, 0 },
    { 7, 7, 7, 7, 0 },
    { 7, 0, 0, 0, 7 },
    { 7, 0, 0, 0, 7 },
    { 0, 7, 7, 7, 0 }
};

static int LED_numSeven[LED_TIME_ROW][LED_TIME_COL] = {
    { 7, 7, 7, 7, 7 },
    { 0, 0, 0, 0, 7 },
    { 0, 0, 0, 0, 7 },
    { 0, 0, 0, 7, 0 },
    { 0, 0, 7, 0, 0 },
    { 0, 7, 0, 0, 0 },
    { 7, 0, 0, 0, 0 }
};

static int LED_numEight[LED_TIME_ROW][LED_TIME_COL] = {
    { 0, 7, 7, 7, 0 },
    { 7, 0, 0, 0, 7 },
    { 7, 0, 0, 0, 7 },
    { 0, 7, 7, 7, 0 },
    { 7, 0, 0, 0, 7 },
    { 7, 0, 0, 0, 7 },
    { 0, 7, 7, 7, 0 }
};

static int LED_numNine[LED_TIME_ROW][LED_TIME_COL] = {
    { 0, 7, 7, 7, 0 },
    { 7, 0, 0, 0, 7 },
    { 7, 0, 0, 0, 7 },
    { 0, 7, 7, 7, 7 },
    { 0, 0, 0, 0, 7 },
    { 0, 0, 0, 7, 0 },
    { 7, 7, 7, 0, 0 }
};

static int LED_numBlank[LED_TIME_ROW][LED_TIME_COL] = {
    { 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0 }
};

void LEDMap_getNumberDisplay(const int number, const int color, int timeMatrix[LED_TIME_ROW][LED_TIME_COL])
{
    if (number == 0) {
        copyColorHelper(timeMatrix, LED_numZero, color);
    }
    else if (number == 1)
    {
        copyColorHelper(timeMatrix, LED_numOne, color);
    }
    else if (number == 2)
    {
        copyColorHelper(timeMatrix, LED_numTwo, color);
    }
    else if (number == 3)
    {
        copyColorHelper(timeMatrix, LED_numThree, color);
    }
    else if (number == 4)
    {
        copyColorHelper(timeMatrix, LED_numFour, color);
    }
    else if (number == 5)
    {
        copyColorHelper(timeMatrix, LED_numFive, color);
    }
    else if (number == 6)
    {
        copyColorHelper(timeMatrix, LED_numSix, color);
    }
    else if (number == 7)
    {
        copyColorHelper(timeMatrix, LED_numSeven, color);
    }
    else if (number == 8)
    {
        copyColorHelper(timeMatrix, LED_numEight, color);
    }
    else if (number == 9)
    {
        copyColorHelper(timeMatrix, LED_numNine, color);
    }
    else {
        printf("DEBUG: Cannot find a mapping to an LED representation for the number %d\n", number);
        copyColorHelper(timeMatrix, LED_numBlank, color);
    }
}

static void copyColorHelper(int dest[LED_TIME_ROW][LED_TIME_COL], const int copyFrom[LED_TIME_ROW][LED_TIME_COL], const int color)
{
    for (int row = 0; row < LED_TIME_ROW; row++) {
        for (int col = 0; col < LED_TIME_COL; col++) {
            if (copyFrom[row][col] != 0) {
                dest[row][col] = color;
            } else {
                dest[row][col] = 0; // equivalent to copyFrom[row][col] which is black
            }
        }
    }
}
