#include <gb/gb.h>
#include "sprites/bkgSprites.h"
#include "sprites/sprites.h"

#define delay betterDelay

const UWORD bkgSpritesPalette[] =
{
    /* Gameboy Color palette 0 */
    bkgSpritesCGBPal0c0, bkgSpritesCGBPal0c1, bkgSpritesCGBPal0c2, bkgSpritesCGBPal0c3
};

const UWORD spritesPalette[] = 
{
    /* Gameboy Color palette 0 */
    spritesCGBPal0c0, spritesCGBPal0c1, spritesCGBPal0c2, spritesCGBPal0c3
};

UINT8 currentSpriteIndex = 1;

void betterDelay(UINT8 numloops)
{
    numloops = numloops / 8;
    UINT8 i;
    for(i = 0; i< numloops; i++) { wait_vbl_done(); }
}

void idleAnimation()
{
    if(currentSpriteIndex == 1) { currentSpriteIndex = 2; }
    else { currentSpriteIndex = 1; }

    set_sprite_tile(0, currentSpriteIndex);
}

typedef enum DIRECTION
{
    D_LEFT,
    D_UP,
    D_RIGHT,
    D_DOWN
} Direction_t;

void runAnimation(Direction_t direction)
{
    switch (direction)
    {
        case D_LEFT: 
            set_sprite_prop(0, S_FLIPX);
            scroll_sprite(0, -5, 0);
            break;

        case D_RIGHT: 
            set_sprite_prop(0, get_sprite_prop(0) & ~S_FLIPX);
            scroll_sprite(0, 5, 0);
            break;

        default: break;
    }

    if(currentSpriteIndex == 3) { currentSpriteIndex = 4; }
    else { currentSpriteIndex = 3; }
    set_sprite_tile(0, currentSpriteIndex);
}

void init()
{
    // Load background palette
    set_bkg_palette(0, 1, &bkgSpritesPalette[0]);

    // Load sprite palette
    set_sprite_palette(0, 1, &spritesPalette[0]);
    // Load sprite tiles
    set_sprite_data(0, 7, sprites);
    
    // Load sprite tile 1 into sprite 0
    set_sprite_tile(0, 1);
    // Load sprite palette 0 into sprite 0
    set_sprite_prop(0, 0);
    // Move sprite 0
    move_sprite(0, 20, 78);

    DISPLAY_ON;
    SHOW_BKG;
    SHOW_SPRITES;
}

void joypadInput()
{
    UINT8 input = joypad();
    UINT8 inputDetected = 1;
    switch(input)
    {
        case J_LEFT: runAnimation(D_LEFT); break;
        case J_RIGHT: runAnimation(D_RIGHT); break;

        default: inputDetected = 0; break;
    }

    if(inputDetected == 0) idleAnimation(); 
}

void main()
{
    init();
    while(1)
    {
       joypadInput();
       delay(120);
    }
}