#include <Arduboy2.h>
Arduboy2 arduboy;

const float MOVEMENT_RATE = 0.25;
float xPos = 0.0;
float yPos = 48.0;

PROGMEM const uint8_t player[] = {
    0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0XFF,
};

PROGMEM const uint8_t block[] = {
    0xFF, 0x81, 0x81, 0x81,
    0x81, 0x81, 0x81, 0xFF,
};

bool isOverSurface(void);
int surfaceHeight(int);

void setup()
{
    arduboy.begin();
    arduboy.clear();
}

void loop()
{
    arduboy.clear();

    if (arduboy.pressed(LEFT_BUTTON)) {
        xPos -= MOVEMENT_RATE;
    }
    if (arduboy.pressed(RIGHT_BUTTON)) {
        xPos += MOVEMENT_RATE;
    }

    for(int i = 0; i < 128; i += 8) {
        if (i >= 56 && i <= 71) continue;
        arduboy.drawBitmap(i, 56, block, 8, 8, WHITE);
    }

    for(int i = 0; i < 2; i++) {
        int platformX = 56;
        int platformY = 32;
        arduboy.drawBitmap(platformX + (8 * i), platformY, block, 8, 8, WHITE);
    }

    if (!isOverSurface() || surfaceHeight(xPos) > (yPos + 8)) {
        yPos += MOVEMENT_RATE;
    }

    arduboy.drawBitmap(xPos, yPos, player, 8, 8, WHITE);

    arduboy.display();
}

bool isOverSurface(void)
{
    // this is specific to the game since we know the layout
    if (((xPos <= 56 || xPos >= 72 || xPos + 8 >= 72)) && (yPos + 8) <= 56) return true;
    if ((xPos > 56 || xPos < 72) && yPos < 24) return true;
    return false;
}

int surfaceHeight(int horizontal)
{
    if (horizontal <= 56 || horizontal >= 72) return 56;
    return 32;
}