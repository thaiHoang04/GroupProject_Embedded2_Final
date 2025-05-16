#include <stdio.h>
#include "NuMicro.h"

struct line {
    uint16_t colors[14] = {C_BLACK, C_BLACK, C_BLACK, C_BLACK, C_BLACK, C_BLACK, C_BLACK, C_BLACK, C_BLACK, C_BLACK, C_BLACK, C_BLACK, C_BLACK, C_BLACK};
};

struct playField {
    uint16_t highestScore;
    uint16_t score;
    struct line lineImage[30];
};

volatile struct playField currentPlayField;

void saveCurrentMove(uint16_t currentX, uint16_t currentY, uint16_t currentType, uint16_t currentDirection) {
    currentPlayField.lineImage[currentY / 10].colors[currentX / 10] = currentType;
}