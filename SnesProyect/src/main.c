#include <snes.h>

//===VARIABLES AND DATA===//////
extern char Player1, Player1_end, palPlayer1, palPlayer1_end;
extern unsigned char background, background_end, pbackground, pbackground_end, tbackground, tbackground_end;
extern char tileset, tilesetend, tilesetpal;
extern char map, tilesetdef, tilesetatt;
#define FRAMES_PER_ANIMATION 7

//========FUCTIONS============//
void BackgroundAndMap();
void PlayerInit();
void Update();
void Controls();
bool NotIDLE = FALSE;

//===========================//

//==Player Var==//
typedef struct{
        s16 x, y;
        u16 gfx_frame;
        u16 anim_frame;
        u8 state;
        u8 flipx;
    } Player_1;

//==PLAYERINITPOS==//
Player_1 player1 = {50, 180};
//==STATE ANIM==//
    enum SpriteState{W_IDLE = 0,W_MOVE = 1};

//==SCREEN DIMENSIONS==//
    enum{
        SCREEN_TOP = 0,
        SCREEN_BOTTOM = 224,
        SCREEN_LEFT = 0,
        SCREEN_RIGHT = 256
    };
// Remember that sprites are interleave with 128 pix width
const char sprTiles[16] ={0x00, 0x02, 0x04, 0x06, 0x08, 0x0A, 0x0C, 0x0E, 0x20, 0x22, 0x24, 0x26,  0x28, 0x2A,0x2C, 0x2E};

//==MAIN==//
int main(void){
//-Initialize SNES
    consoleInit();
    PlayerInit();
    BackgroundAndMap();
//-Initialize Player, Map, etc.
//===LOOP===//
while (1){
        Update();
        Controls();
        WaitForVBlank();
    }
    return 0;
}

//==Init Sprites And Text==//
void BackgroundAndMap(){
//-Init background
    bgInitTileSet(2, &tbackground, &pbackground, 0, (&tbackground_end - &tbackground),16, BG_4COLORS, 0x2000);// Pal 1 is a petty colour
    bgInitMapSet(2, &background, (&background_end - &background), SC_32x32, 0x0400);
//-Init Map
    bgInitTileSet(3, &tileset, &tilesetpal, 0, (&tilesetend - &tileset), 16 * 2, BG_16COLORS, 0x4800);
    bgSetMapPtr(3, 0x4000, SC_64x32);

    setMode(BG_MODE1, 0);
    bgSetDisable(0);
    bgSetDisable(1);
    setScreenOn();

    // Load map in memory and update it regarding current location of the sprite
    mapLoad((u8 *)&map, (u8 *)&tilesetdef, (u8 *)&tilesetatt);
    mapUpdateCamera(player1.x, player1.y);
}

//Execute all object in update
void Update(){
    mapUpdate();
    mapVblank();
}

//===Init Player Sprites and Variables===//
void PlayerInit(){
    oamInitGfxSet(&Player1, (&Player1_end - &Player1), &palPlayer1, (&palPlayer1_end - &palPlayer1), 0, 0x8000, OBJ_SIZE16_L32);
    oamSet(0, player1.x, player1.y, 0, 0, 0, 0, 0);
    oamSetEx(0, OBJ_SMALL, OBJ_SHOW);
    oamSetVisible(0, OBJ_SHOW);
}

//==CONTROL===//
void Controls(){
    unsigned short pad0, i;
    pad0 = padsCurrent(0);
    // Refresh pad values in VBL and Get current #0 pad
        if (pad0){
            // Update sprite with current pad
            if (pad0 & KEY_UP){
                if (player1.y >= SCREEN_TOP){
                    player1.y--;
                    player1.state = W_MOVE;
                    NotIDLE=TRUE;
                }
            }if (pad0 & KEY_LEFT){
                if (player1.x >= SCREEN_LEFT){
                    player1.x--;
                    player1.state = W_MOVE;
                    player1.flipx = 1;
                    NotIDLE=TRUE;
                }
            }if (pad0 & KEY_RIGHT){
                if (player1.x <= SCREEN_RIGHT){
                    player1.x++;
                    player1.state = W_MOVE;
                    player1.flipx = 0;
                    NotIDLE=TRUE;
                }
            }if (pad0 & KEY_DOWN){
                if (player1.y <= SCREEN_BOTTOM)
                    player1.y++;
                    player1.state = W_MOVE;
                    NotIDLE=TRUE;
            }
        }if(NotIDLE==FALSE){
            player1.state = W_IDLE;
        }
        //LOOP ANIMATION//
        if (player1.anim_frame >= FRAMES_PER_ANIMATION)
            player1.anim_frame = 0;

        // Now, get current sprite in current animation
            player1.anim_frame++;
            player1.gfx_frame = sprTiles[player1.anim_frame + player1.state * FRAMES_PER_ANIMATION];
            oamSet(0, player1.x, player1.y, 0, player1.flipx, 0, player1.gfx_frame, 0);
}
