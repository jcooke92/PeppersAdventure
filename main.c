#include <gb/gb.h>
#include "jgbdk/util.c"
#include "sprites/sprites.h"
#include "sprites/bkg_sprites.h"

const UWORD g_bkg_sprites_palette[] =
{
    /* Gameboy Color palette 0 */
    bkg_spritesCGBPal0c0, bkg_spritesCGBPal0c1, bkg_spritesCGBPal0c2, bkg_spritesCGBPal0c3,

    /* Gameboy Color palette 1 */
    bkg_spritesCGBPal1c0, bkg_spritesCGBPal1c1, bkg_spritesCGBPal1c2, bkg_spritesCGBPal1c3,

    /* Gameboy Color palette 2 */
    bkg_spritesCGBPal2c0, bkg_spritesCGBPal2c1, bkg_spritesCGBPal2c2, bkg_spritesCGBPal2c3,

    /* Gameboy Color palette 3 */
    bkg_spritesCGBPal3c0, bkg_spritesCGBPal3c1, bkg_spritesCGBPal3c2, bkg_spritesCGBPal3c3
};

const UWORD g_sprites_palette[] = 
{
    /* Gameboy Color palette 0 */
    spritesCGBPal0c0, spritesCGBPal0c1, spritesCGBPal0c2, spritesCGBPal0c3
};

UINT8 g_current_sprite_index = 1;
INT8 g_player_location[] = {0, 0};
UINT8 g_player_jumping = 0;
INT8 g_gravity = -2;
INT8 g_current_speed_y = 0;
INT8 g_floor_y_position = 78;
INT8 g_projected_y_position = 0;
INT8 g_new_y_position = 0;
UINT8 g_input_detected = 0;
UINT8 g_input = 0;

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

void main()
{
    // init
    // Load background palette
    // set_bkg_palette(0, 1, &g_bkg_sprites_palette[0]);

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
    // end init
    while(1)
    {
        g_input = joypad();
        g_input_detected = 1;
        if((g_input & J_A || g_player_jumping) > 0)
        { 
            // player jump
            set_sprite_tile(0, 4);
            g_current_sprite_index = 4;
            if(g_player_jumping == 0)
            {
                g_player_jumping = 1;
                g_current_speed_y = 8;
            }
            g_current_speed_y = g_current_speed_y + g_gravity;
            g_projected_y_position = g_player_location[1] - g_current_speed_y;
            g_new_y_position = g_projected_y_position;
            
            if(g_projected_y_position >= g_floor_y_position) 
            { 
                g_player_jumping = 0;
                g_new_y_position = g_floor_y_position;
            }        

            better_move_sprite(0, g_player_location[0], g_new_y_position);
            better_delay(50);
        }
        else if(g_input == J_LEFT) 
        { 
            // player walk left
            set_sprite_prop(0, S_FLIPX);
            better_scroll_sprite(0, -3, 0);

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
        else if(g_input == J_RIGHT) 
        { 
            //player walk right
            set_sprite_prop(0, get_sprite_prop(0) & ~S_FLIPX);
            better_scroll_sprite(0, 3, 0);
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
        else 
        { 
            g_input_detected = 0;
        }

        if(g_input_detected == 0) 
        {
            // player idle
            if(g_current_sprite_index != 1) 
            { 
                set_sprite_tile(0, 1);
                g_current_sprite_index = 1;
            }
        }
    }
}