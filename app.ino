#include <Arduboy2.h>
Arduboy2 arduboy;

const float MOVEMENT_RATE = 0.50;
const int JUMP_INCREASE = 32;
bool b_pressed = false;

PROGMEM const uint8_t player[] = {
    0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0XFF,
};

PROGMEM const uint8_t block[] = {
    0xFF, 0x81, 0x81, 0x81,
    0x81, 0x81, 0x81, 0xFF,
};

struct player_state {
    float xPos;
    float yPos;
    float jumpBase;
    bool jumping :1;
};

player_state p1 = { 0.0, 48.0, 48.0, false };

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
        p1.xPos -= MOVEMENT_RATE;
    }
    if (arduboy.pressed(RIGHT_BUTTON)) {
        p1.xPos += MOVEMENT_RATE;
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

    if (arduboy.pressed(B_BUTTON)) {
        if(p1.jumping) {
            if (p1.jumpBase - JUMP_INCREASE >= p1.yPos) {
                p1.jumping = false;
            } else {
                p1.yPos -= MOVEMENT_RATE;
            }
        } else {
            if(surfaceHeight(p1.xPos) == p1.yPos + 8) {
                p1.jumping = true;
            }
        }
    }

    if (isOverSurface() && ((surfaceHeight(p1.xPos) > p1.yPos + 8))) {
        if (!p1.jumping) {
            p1.yPos += MOVEMENT_RATE;
        }
    }

    arduboy.drawBitmap(p1.xPos, p1.yPos, player, 8, 8, WHITE);

    arduboy.display();
}

bool isOverSurface(void)
{
    // this is specific to the game since we know the layout
    if (((p1.xPos <= 56 || p1.xPos >= 72 || p1.xPos + 8 >= 72)) && (p1.yPos + 8) <= 56) return true;
    if ((p1.xPos > 56 || p1.xPos < 72) && p1.yPos < 24) return true;
    return false;
}

int surfaceHeight(int horizontal)
{
    if (horizontal <= 56 || horizontal >= 72) return 56;
    return 32;
}