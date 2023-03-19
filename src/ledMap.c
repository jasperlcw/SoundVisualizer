#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUM_ROW 7
#define NUM_COL 5

// Matrtices are declared as [row][column]
// Each number takes up NUM_ROW rows and NUM_COL columns
static int LED_numZero[NUM_ROW][NUM_COL] = {
    { 0, 7, 7, 7, 0 },
    { 7, 0, 0, 0, 7 },
    { 7, 0, 0, 7, 7 },
    { 7, 0, 7, 0, 7 },
    { 7, 7, 0, 0, 7 },
    { 7, 0, 0, 0, 7 },
    { 0, 7, 7, 7, 0 }
};

static int LED_numOne[NUM_ROW][NUM_COL] = {
    { 0, 0, 7, 0, 0 },
    { 0, 7, 7, 0, 0 },
    { 0, 0, 7, 0, 0 },
    { 0, 0, 7, 0, 0 },
    { 0, 0, 7, 0, 0 },
    { 0, 0, 7, 0, 0 },
    { 0, 7, 7, 7, 0 }
};

static int LED_numTwo[NUM_ROW][NUM_COL] = {
    { 0, 7, 7, 7, 0 },
    { 7, 0, 0, 0, 7 },
    { 0, 0, 0, 0, 7 },
    { 0, 7, 7, 7, 0 },
    { 7, 0, 0, 0, 0 },
    { 7, 0, 0, 0, 0 },
    { 7, 7, 7, 7, 7 }
};

static int LED_numThree[NUM_ROW][NUM_COL] = {
    { 7, 7, 7, 7, 0 },
    { 0, 0, 0, 0, 7 },
    { 0, 0, 0, 0, 7 },
    { 0, 7, 7, 7, 0 },
    { 0, 0, 0, 0, 7 },
    { 0, 0, 0, 0, 7 },
    { 7, 7, 7, 7, 0 }
};

static int LED_numFour[NUM_ROW][NUM_COL] = {
    { 0, 0, 7, 0, 7 },
    { 0, 7, 0, 0, 7 },
    { 7, 0, 0, 0, 7 },
    { 7, 7, 7, 7, 7 },
    { 0, 0, 0, 0, 7 },
    { 0, 0, 0, 0, 7 },
    { 0, 0, 0, 0, 7 }
};

static int LED_numFive[NUM_ROW][NUM_COL] = {
    { 7, 7, 7, 7, 7 },
    { 7, 0, 0, 0, 0 },
    { 7, 7, 7, 7, 0 },
    { 0, 0, 0, 0, 7 },
    { 0, 0, 0, 0, 7 },
    { 7, 0, 0, 0, 7 },
    { 0, 7, 7, 7, 0 }
};

static int LED_numSix[NUM_ROW][NUM_COL] = {
    { 0, 0, 7, 7, 7 },
    { 0, 7, 0, 0, 0 },
    { 7, 0, 0, 0, 0 },
    { 7, 7, 7, 7, 0 },
    { 7, 0, 0, 0, 7 },
    { 7, 0, 0, 0, 7 },
    { 0, 7, 7, 7, 0 }
};

static int LED_numSeven[NUM_ROW][NUM_COL] = {
    { 7, 7, 7, 7, 7 },
    { 0, 0, 0, 0, 7 },
    { 0, 0, 0, 0, 7 },
    { 0, 0, 0, 7, 0 },
    { 0, 0, 7, 0, 0 },
    { 0, 7, 0, 0, 0 },
    { 7, 0, 0, 0, 0 }
};

static int LED_numEight[NUM_ROW][NUM_COL] = {
    { 0, 7, 7, 7, 0 },
    { 7, 0, 0, 0, 7 },
    { 7, 0, 0, 0, 7 },
    { 0, 7, 7, 7, 0 },
    { 7, 0, 0, 0, 7 },
    { 7, 0, 0, 0, 7 },
    { 0, 7, 7, 7, 0 }
};

static int LED_numNine[NUM_ROW][NUM_COL] = {
    { 0, 7, 7, 7, 0 },
    { 7, 0, 0, 0, 7 },
    { 7, 0, 0, 0, 7 },
    { 0, 7, 7, 7, 7 },
    { 0, 0, 0, 0, 7 },
    { 0, 0, 0, 7, 0 },
    { 7, 7, 7, 0, 0 }
};

static int LED_numBlank[NUM_ROW][NUM_COL] = {
    { 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0 }
};

int** LED_getNumberDisplay(const int number)
{
    int **toReturn = malloc(NUM_ROW * sizeof(int*));
    for (int i = 0; i < NUM_ROW; i++) {
        toReturn[i] = malloc(NUM_COL * sizeof(int));
        memset(toReturn[i], 0, NUM_COL);
    }

    if (number == 0) {
        for (int row = 0; row < NUM_ROW; row++) {
            for (int col = 0; col < NUM_COL; col++) {
                toReturn[row][col] = LED_numZero[row][col];
            }
        }
    }
    else if (number == 1)
    {
        for (int row = 0; row < NUM_ROW; row++) {
            for (int col = 0; col < NUM_COL; col++) {
                toReturn[row][col] = LED_numOne[row][col];
            }
        }
    }
    else if (number == 2)
    {
        for (int row = 0; row < NUM_ROW; row++) {
            for (int col = 0; col < NUM_COL; col++) {
                toReturn[row][col] = LED_numTwo[row][col];
            }
        }
    }
    else if (number == 3)
    {
        for (int row = 0; row < NUM_ROW; row++) {
            for (int col = 0; col < NUM_COL; col++) {
                toReturn[row][col] = LED_numThree[row][col];
            }
        }
    }
    else if (number == 4)
    {
        for (int row = 0; row < NUM_ROW; row++) {
            for (int col = 0; col < NUM_COL; col++) {
                toReturn[row][col] = LED_numFour[row][col];
            }
        }
    }
    else if (number == 5)
    {
        for (int row = 0; row < NUM_ROW; row++) {
            for (int col = 0; col < NUM_COL; col++) {
                toReturn[row][col] = LED_numFive[row][col];
            }
        }
    }
    else if (number == 6)
    {
        for (int row = 0; row < NUM_ROW; row++) {
            for (int col = 0; col < NUM_COL; col++) {
                toReturn[row][col] = LED_numSix[row][col];
            }
        }
    }
    else if (number == 7)
    {
        for (int row = 0; row < NUM_ROW; row++) {
            for (int col = 0; col < NUM_COL; col++) {
                toReturn[row][col] = LED_numSeven[row][col];
            }
        }
    }
    else if (number == 8)
    {
        for (int row = 0; row < NUM_ROW; row++) {
            for (int col = 0; col < NUM_COL; col++) {
                toReturn[row][col] = LED_numEight[row][col];
            }
        }
    }
    else if (number == 9)
    {
        for (int row = 0; row < NUM_ROW; row++) {
            for (int col = 0; col < NUM_COL; col++) {
                toReturn[row][col] = LED_numNine[row][col];
            }
        }
    }
    else {
        printf("DEBUG: Cannot find a mapping to an LED representation for the number %d\n", number);
        for (int row = 0; row < NUM_ROW; row++) {
            for (int col = 0; col < NUM_COL; col++) {
                toReturn[row][col] = LED_numBlank[row][col];
            }
        }
    }

    return toReturn;
}

void LEDMap_freeDisplayArray(int **toFree)
{
    for (int i = 0; i < NUM_ROW; i++)
    {
        free(toFree[i]);
    }
    free(toFree);
}
