#include <tamtypes.h>
#include <kernel.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <unistd.h>
#include <gsKit.h>
#include <dmaKit.h>
#include <audsrv.h>
#include <sbv_patches.h>
#include <fileio.h>

extern unsigned char music_mod[];

static GSGLOBAL *gsGlobal;
static AUDSRV_INFO audsrv_info;

void InitPS2() {
    
    dmaKit_init(D_CTRL_RELE_OFF, D_CTRL_MFD_OFF, D_CTRL_STS_UNSPEC, D_CTRL_RELE_OFF);
    dmaKit_chan_init(DMA_CHANNEL_GIF);
    gsGlobal = gsKit_init_global();
    gsKit_mode_switch(gsGlobal, GS_PERSISTENT);
    gsGlobal->PrimAlphaEnable = GS_SETTING_ON;

    
    gsKit_init_screen(gsGlobal);
    gsKit_set_display_buffer(gsGlobal, 0);

    
    sbv_patch_enable_lmb();
    audsrv_init();
    audsrv_set_volume(MAX_VOLUME);
}

void drawDonut(float A, float B) {
    float i, j;
    int k;
    float z[1760];
    char b[1760];
    memset(b, 32, 1760);
    memset(z, 0, 7040);
    for (j = 0; j < 6.28; j += 0.07) {
        for (i = 0; i < 6.28; i += 0.02) {
            float c = sin(i);
            float d = cos(j);
            float e = sin(A);
            float f = sin(j);
            float g = cos(A);
            float h = d + 2;
            float D = 1 / (c * h * e + f * g + 5);
            float l = cos(i);
            float m = cos(B);
            float n = sin(B);
            float t = c * h * g - f * e;
            int x = 40 + 30 * D * (l * h * m - t * n);
            int y = 12 + 15 * D * (l * h * n + t * m);
            int o = x + 80 * y;
            int N = 8 * ((f * e - c * d * g) * m - c * d * e - f * g - l * d * n);
            if (22 > y && y > 0 && x > 0 && 80 > x && D > z[o]) {
                z[o] = D;
                b[o] = ".,-~:;=!*#$@"[N > 0 ? N : 0];
            }
        }
    }
    gsKit_clear(gsGlobal, GS_SETREG_RGBAQ(0x00, 0x00, 0x00, 0x00, 0x00));
    for (k = 0; k < 1761; k++) {
        if (k % 80 == 0) {
            printf("\n");
        } else {
            printf("%c", b[k]);
        }
    }
    gsKit_sync_flip(gsGlobal);
}

int main(int argc, char **argv) {
    InitPS2();

    
    int ret = audsrv_load_module(AUDSRV_LOAD_MODULE);
    if (ret < 0) {
        printf("Error loading audio module\n");
        return 1;
    }

    

    float A = 0, B = 0;
    while (1) {
        drawDonut(A, B);
        A += 0.00004;
        B += 0.00002;
        usleep(30000);
        printf(" \x1b[44;37m.----------------------------------------------------------------------------.\x1b[40;37m\n");
        printf(" \x1b[44;37m|  PS2 Donut v1.0                                                            |\x1b[40;37m\n");
        printf(" \x1b[44;37m|  Based on the original donut.c by Andy Sloane <andy@a1k0n.net>             |\x1b[40;37m\n");
        printf(" \x1b[44;37m|  GameCube Donut by jornmann <jornmann@duck.com>, PS2 port by Code Copilot  |\x1b[40;37m\n");
        printf(" \x1b[44;37m|  Music by Jogeir Liljedahl                                                 |\x1b[40;37m\n");
        printf(" \x1b[44;37m'----------------------------------------------------------------------------'\x1b[40;37m\n");
    }

    return 0;
}
