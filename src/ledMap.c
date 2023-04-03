#include "include/ledMap.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

void LEDMap_getNumberDisplay(const int number, int timeMatrix[LED_TIME_ROW][LED_TIME_COL])
{
    if (number == 0) {
        for (int row = 0; row < LED_TIME_ROW; row++) {
            for (int col = 0; col < LED_TIME_COL; col++) {
                timeMatrix[row][col] = LED_numZero[row][col];
            }
        }
    }
    else if (number == 1)
    {
        for (int row = 0; row < LED_TIME_ROW; row++) {
            for (int col = 0; col < LED_TIME_COL; col++) {
                timeMatrix[row][col] = LED_numOne[row][col];
            }
        }
    }
    else if (number == 2)
    {
        for (int row = 0; row < LED_TIME_ROW; row++) {
            for (int col = 0; col < LED_TIME_COL; col++) {
                timeMatrix[row][col] = LED_numTwo[row][col];
            }
        }
    }
    else if (number == 3)
    {
        for (int row = 0; row < LED_TIME_ROW; row++) {
            for (int col = 0; col < LED_TIME_COL; col++) {
                timeMatrix[row][col] = LED_numThree[row][col];
            }
        }
    }
    else if (number == 4)
    {
        for (int row = 0; row < LED_TIME_ROW; row++) {
            for (int col = 0; col < LED_TIME_COL; col++) {
                timeMatrix[row][col] = LED_numFour[row][col];
            }
        }
    }
    else if (number == 5)
    {
        for (int row = 0; row < LED_TIME_ROW; row++) {
            for (int col = 0; col < LED_TIME_COL; col++) {
                timeMatrix[row][col] = LED_numFive[row][col];
            }
        }
    }
    else if (number == 6)
    {
        for (int row = 0; row < LED_TIME_ROW; row++) {
            for (int col = 0; col < LED_TIME_COL; col++) {
                timeMatrix[row][col] = LED_numSix[row][col];
            }
        }
    }
    else if (number == 7)
    {
        for (int row = 0; row < LED_TIME_ROW; row++) {
            for (int col = 0; col < LED_TIME_COL; col++) {
                timeMatrix[row][col] = LED_numSeven[row][col];
            }
        }
    }
    else if (number == 8)
    {
        for (int row = 0; row < LED_TIME_ROW; row++) {
            for (int col = 0; col < LED_TIME_COL; col++) {
                timeMatrix[row][col] = LED_numEight[row][col];
            }
        }
    }
    else if (number == 9)
    {
        for (int row = 0; row < LED_TIME_ROW; row++) {
            for (int col = 0; col < LED_TIME_COL; col++) {
                timeMatrix[row][col] = LED_numNine[row][col];
            }
        }
    }
    else {
        printf("DEBUG: Cannot find a mapping to an LED representation for the number %d\n", number);
        for (int row = 0; row < LED_TIME_ROW; row++) {
            for (int col = 0; col < LED_TIME_COL; col++) {
                timeMatrix[row][col] = LED_numBlank[row][col];
            }
        }
    }
}
