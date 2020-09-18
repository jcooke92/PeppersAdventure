#include <gb/gb.h>
#include "jgbdk/util.c"
#include "sprites/bkgSprites.h"
#include "sprites/sprites.h"

const UWORD g_bkg_sprites_palette[] =
{
    /* Gameboy Color palette 0 */
    bkgSpritesCGBPal0c0, bkgSpritesCGBPal0c1, bkgSpritesCGBPal0c2, bkgSpritesCGBPal0c3
};

const UWORD g_sprites_palette[] = 
{
    /* Gameboy Color palette 0 */
    spritesCGBPal0c0, spritesCGBPal0c1, spritesCGBPal0c2, spritesCGBPal0c3
};

UINT8 g_current_sprite_index = 1;
INT8 g_player_location[] = {0, 0};
BYTE g_player_jumping = 0;
INT8 g_gravity = -2;
INT8 g_current_speed_y = 0;
INT8 g_floor_y_position = 78;

void better_move_sprite(UINT8 nb, INT8 x, INT8 y)
{
    g_player_location[0] = x;
    g_player_location[1] = y;
    move_sprite(nb, g_player_location[0], g_player_location[1]);
}

void better_scroll_sprite(UINT8 nb, INT8 x, INT8 y)
{
    g_player_location[0] = g_player_location[0] + x;
    g_player_location[1] = g_player_location[1] + y;
    scroll_sprite(nb, x, y);
}

void player_idle()
{
    if(g_current_sprite_index != 1) 
    { 
        set_sprite_tile(0, 1);
        g_current_sprite_index = 1;
    }
}

typedef enum DIRECTION
{
    D_LEFT,
    D_UP,
    D_RIGHT,
    D_DOWN
} Direction_t;

void player_run(Direction_t direction)
{
    switch (direction)
    {
        case D_LEFT: 
            set_sprite_prop(0, S_FLIPX);
            better_scroll_sprite(0, -3, 0);
            break;

        case D_RIGHT: 
            set_sprite_prop(0, get_sprite_prop(0) & ~S_FLIPX);
            better_scroll_sprite(0, 3, 0);
            break;

        default: break;
    }

    if(g_current_sprite_index == 3) 
    { 
        set_sprite_tile(0, 4);
        g_current_sprite_index = 4; 
    }
    else 
    { 
        set_sprite_tile(0, 3);
        g_current_sprite_index = 3; 
    }
    better_delay(50);
}

INT8 surface_collision(INT8 projected_y_position)
{
    if(projected_y_position >= g_floor_y_position) 
    { 
        g_player_jumping = 0;
        return g_floor_y_position; 
    }

    return projected_y_position;
}

void player_jump()
{
    set_sprite_tile(0, 4);
    g_current_sprite_index = 4;
    if(g_player_jumping == 0)
    {
        g_player_jumping = 1;
        g_current_speed_y = 8;
    }
    g_current_speed_y = g_current_speed_y + g_gravity;

    INT8 projected_y_position = g_player_location[1] - g_current_speed_y;
    INT8 new_y_position = surface_collision(projected_y_position);

    better_move_sprite(0, g_player_location[0], new_y_position);
    better_delay(50);
}

void init()
{
    // Load background palette
    set_bkg_palette(0, 1, &g_bkg_sprites_palette[0]);

    // Load sprite palette
    set_sprite_palette(0, 1, &g_sprites_palette[0]);
    // Load sprite tiles
    set_sprite_data(0, 7, sprites);
    
    // Load sprite tile 1 into sprite 0
    set_sprite_tile(0, 1);
    // Load sprite palette 0 into sprite 0
    set_sprite_prop(0, 0);
    // Move sprite 0
    better_move_sprite(0, 20, 78);

    DISPLAY_ON;
    SHOW_BKG;
    SHOW_SPRITES;
}

void check_input()
{
    UINT8 input = joypad();
    BYTE input_detected = 1;
    if((input & J_A || g_player_jumping) > 0){ player_jump(); }
    else if(input == J_LEFT) { player_run(D_LEFT); }
    else if(input == J_RIGHT) { player_run(D_RIGHT); }
    else { input_detected = 0;}

    if(input_detected == 0) player_idle(); 
}

void main()
{
    init();
    while(1)
    {
       check_input();
    }
}