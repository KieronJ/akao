#include "akao.h"

#include <sys/types.h>
#include <libspu.h>

/* transfer.c */

static volatile short AkaoTransferBusy;

void _AkaoTransferCallback(void)
{
    SpuSetTransferCallback(NULL);
    AkaoTransferBusy = 0;
}

void _AkaoSetTransferCallback(void)
{
    AkaoTransferBusy = 1;
    SpuSetTransferCallback(_AkaoTransferCallback);
}

void AkaoTransferWrite(void *addr, u_long size)
{
    _AkaoSetTransferCallback();
    SpuWrite(addr, size);
}

void AkaoTransferRead(void *addr, u_long size)
{
    _AkaoSetTransferCallback();
    SpuRead(addr, size);
}

void AkaoTransferFlush(void)
{
    while (AkaoTransferBusy)
    {
        /* do nothing */
    }
}

/* sndmain.c */

typedef struct _AKAO_VOICE {
    /* 0x00 */ u_int      voice;
    /* 0x04 */ u_int      update_flags;
    /* 0x08 */ u_int      addr;
    /* 0x0C */ u_int      loop_addr;
    /* 0x10 */ u_int      a_mode;
    /* 0x14 */ u_int      s_mode;
    /* 0x18 */ u_int      r_mode;
    /* 0x1C */ u_short    pitch;
    /* 0x1E */ u_short    ar;
    /* 0x20 */ u_short    dr;
    /* 0x22 */ u_short    sl;
    /* 0x24 */ u_short    sr;
    /* 0x26 */ u_short    rr;
    /* 0x28 */ SpuVolume  volume;
} AKAO_VOICE;

typedef struct _AKAO_TRACK {
    /* 0x00 */ u_char    *addr;
    /* 0x04 */ u_char    *loop_addr[4];
    /* 0x14 */ u_char    *drum_addr;
    /* 0x18 */ char       pad1[0xC];
    /* 0x24 */ u_int      overlay_voice;
    /* 0x28 */ u_int      alternate_voice;
    /* 0x2C */ char       pad2[0xC];
    /* 0x38 */ u_int      effect_flags;
    /* 0x3C */ char       pad3[0x18];
    /* 0x54 */ u_short    field_54;
    /* 0x56 */ char       pad4[0x18];
    /* 0x6E */ u_short    legato_flags;
    /* 0x70 */ char       pad5[0x34];
    /* 0xA4 */ u_short    noise_delay;
    /* 0xA6 */ u_short    freq_modulate_delay;
    /* 0x70 */ char       pad6[0x10];
    /* 0xB8 */ u_short    loop_depth;
    /* 0xBA */ u_short    loop_delay[4];
    /* 0xC4 */ char       pad7[0x1A];
    /* 0xDC */ AKAO_VOICE attr;
} AKAO_TRACK;

typedef struct _AKAO_PLAYER {
    /* 0x00 */ u_int      channel_mode;
    /* 0x04 */ u_int      voice_mask;
    /* 0x08 */ u_int      keyons;
    /* 0x0C */ u_int      active;
    /* 0x10 */ u_int      keyoffs;
    /* 0x14 */ u_int      active_old;
    /* 0x18 */ u_int      tempo;
    /* 0x1C */ int        tempo_slope;
    /* 0x20 */ u_int      time;
    /* 0x24 */ u_int      overlay_mask;
    /* 0x28 */ u_int      alternate_mask;
    /* 0x2C */ u_int      noise_mask;
    /* 0x30 */ u_int      reverb_mask;
    /* 0x34 */ u_int      pitch_lfo_mask;
    /* 0x38 */ u_int      spucnt;
    /* 0x3C */ char       pad2[0xE];
    /* 0x4A */ u_short    song_id;
    /* 0x4C */ char       pad3[0x8];
    /* 0x54 */ u_short    field_54;
    /* 0x56 */ u_short    beats_per_measure;
    /* 0x58 */ u_short    beat;
    /* 0x5A */ u_short    ticks_per_beat;
    /* 0x5C */ u_short    tick;
    /* 0x5E */ u_short    measure;
} AKAO_PLAYER;

extern AKAO_PLAYER AkaoPlayer;
extern AKAO_TRACK  AkaoTracks[24];

extern int D_80062F04;

extern u_int D_80099FCC;
extern u_int D_80099FEC;
extern u_int D_80099FF0;
extern u_int D_80099FF4;

#define AKAO_EFFECT_DRUM    0x08
#define AKAO_EFFECT_FREQMOD 0x10
#define AKAO_EFFECT_VOLMOD  0x20

INCLUDE_ASM("asm/nonmatchings/akao", func_800294BC);

INCLUDE_ASM("asm/nonmatchings/akao", func_800297A4);

INCLUDE_ASM("asm/nonmatchings/akao", func_80029818);

INCLUDE_ASM("asm/nonmatchings/akao", func_8002988C);

INCLUDE_ASM("asm/nonmatchings/akao", func_80029998);

INCLUDE_ASM("asm/nonmatchings/akao", func_800299C8);

INCLUDE_ASM("asm/nonmatchings/akao", func_80029A50);

INCLUDE_ASM("asm/nonmatchings/akao", func_80029AF0);

INCLUDE_ASM("asm/nonmatchings/akao", func_80029B78);

INCLUDE_ASM("asm/nonmatchings/akao", func_80029BAC);

INCLUDE_ASM("asm/nonmatchings/akao", func_80029C48);

INCLUDE_ASM("asm/nonmatchings/akao", func_80029E98);

INCLUDE_ASM("asm/nonmatchings/akao", func_80029F44);

INCLUDE_ASM("asm/nonmatchings/akao", func_8002A094);

INCLUDE_ASM("asm/nonmatchings/akao", func_8002A28C);

INCLUDE_ASM("asm/nonmatchings/akao", func_8002A43C);

INCLUDE_ASM("asm/nonmatchings/akao", func_8002A510);

INCLUDE_ASM("asm/nonmatchings/akao", func_8002A6C4);

INCLUDE_ASM("asm/nonmatchings/akao", func_8002A748);

INCLUDE_ASM("asm/nonmatchings/akao", func_8002A798);

INCLUDE_ASM("asm/nonmatchings/akao", func_8002A7E8);

INCLUDE_ASM("asm/nonmatchings/akao", func_8002A958);

INCLUDE_ASM("asm/nonmatchings/akao", func_8002AABC);

INCLUDE_ASM("asm/nonmatchings/akao", func_8002AFB8);

INCLUDE_ASM("asm/nonmatchings/akao", func_8002B1A8);

INCLUDE_ASM("asm/nonmatchings/akao", func_8002B1F8);

INCLUDE_ASM("asm/nonmatchings/akao", func_8002B2F8);

INCLUDE_ASM("asm/nonmatchings/akao", func_8002B3B4);

INCLUDE_ASM("asm/nonmatchings/akao", func_8002B5A8);

INCLUDE_ASM("asm/nonmatchings/akao", func_8002B608);

INCLUDE_ASM("asm/nonmatchings/akao", func_8002B668);

INCLUDE_ASM("asm/nonmatchings/akao", func_8002B6AC);

INCLUDE_ASM("asm/nonmatchings/akao", func_8002B730);

INCLUDE_ASM("asm/nonmatchings/akao", func_8002B7E0);

INCLUDE_ASM("asm/nonmatchings/akao", func_8002B8B4);

INCLUDE_ASM("asm/nonmatchings/akao", func_8002B904);

INCLUDE_ASM("asm/nonmatchings/akao", func_8002B958);

INCLUDE_ASM("asm/nonmatchings/akao", func_8002B9AC);

INCLUDE_ASM("asm/nonmatchings/akao", func_8002BA08);

INCLUDE_ASM("asm/nonmatchings/akao", func_8002BA5C);

INCLUDE_ASM("asm/nonmatchings/akao", func_8002BA98);

INCLUDE_ASM("asm/nonmatchings/akao", func_8002BB20);

INCLUDE_ASM("asm/nonmatchings/akao", func_8002BBB4);

INCLUDE_ASM("asm/nonmatchings/akao", func_8002BBEC);

INCLUDE_ASM("asm/nonmatchings/akao", func_8002BC58);

INCLUDE_ASM("asm/nonmatchings/akao", func_8002BCCC);

INCLUDE_ASM("asm/nonmatchings/akao", func_8002BD04);

INCLUDE_ASM("asm/nonmatchings/akao", func_8002BDCC);

INCLUDE_ASM("asm/nonmatchings/akao", func_8002BE2C);

INCLUDE_ASM("asm/nonmatchings/akao", func_8002BE8C);

INCLUDE_ASM("asm/nonmatchings/akao", func_8002BEB4);

INCLUDE_ASM("asm/nonmatchings/akao", func_8002BEDC);

INCLUDE_ASM("asm/nonmatchings/akao", func_8002BF04);

INCLUDE_ASM("asm/nonmatchings/akao", func_8002BF2C);

INCLUDE_ASM("asm/nonmatchings/akao", func_8002BF54);

INCLUDE_ASM("asm/nonmatchings/akao", func_8002BF7C);

INCLUDE_ASM("asm/nonmatchings/akao", func_8002BFA4);

INCLUDE_ASM("asm/nonmatchings/akao", func_8002BFCC);

INCLUDE_ASM("asm/nonmatchings/akao", func_8002C004);

INCLUDE_ASM("asm/nonmatchings/akao", func_8002C0CC);

INCLUDE_ASM("asm/nonmatchings/akao", func_8002C12C);

INCLUDE_ASM("asm/nonmatchings/akao", func_8002C18C);

INCLUDE_ASM("asm/nonmatchings/akao", func_8002C1B4);

INCLUDE_ASM("asm/nonmatchings/akao", func_8002C1DC);

INCLUDE_ASM("asm/nonmatchings/akao", func_8002C204);

INCLUDE_ASM("asm/nonmatchings/akao", func_8002C22C);

INCLUDE_ASM("asm/nonmatchings/akao", func_8002C254);

INCLUDE_ASM("asm/nonmatchings/akao", func_8002C27C);

INCLUDE_ASM("asm/nonmatchings/akao", func_8002C2A4);

INCLUDE_ASM("asm/nonmatchings/akao", func_8002C2CC);

INCLUDE_ASM("asm/nonmatchings/akao", func_8002C300);

INCLUDE_ASM("asm/nonmatchings/akao", func_8002C3A8);

INCLUDE_ASM("asm/nonmatchings/akao", func_8002C408);

INCLUDE_ASM("asm/nonmatchings/akao", func_8002C468);

INCLUDE_ASM("asm/nonmatchings/akao", func_8002C490);

INCLUDE_ASM("asm/nonmatchings/akao", func_8002C4B8);

INCLUDE_ASM("asm/nonmatchings/akao", func_8002C4E0);

INCLUDE_ASM("asm/nonmatchings/akao", func_8002C508);

INCLUDE_ASM("asm/nonmatchings/akao", func_8002C530);

INCLUDE_ASM("asm/nonmatchings/akao", func_8002C558);

INCLUDE_ASM("asm/nonmatchings/akao", func_8002C580);

INCLUDE_ASM("asm/nonmatchings/akao", func_8002C5A8);

INCLUDE_ASM("asm/nonmatchings/akao", func_8002C5C8);

INCLUDE_ASM("asm/nonmatchings/akao", func_8002C634);

INCLUDE_ASM("asm/nonmatchings/akao", func_8002C6A8);

INCLUDE_ASM("asm/nonmatchings/akao", func_8002C6C8);

INCLUDE_ASM("asm/nonmatchings/akao", func_8002C734);

INCLUDE_ASM("asm/nonmatchings/akao", func_8002C7A8);

INCLUDE_ASM("asm/nonmatchings/akao", func_8002C7C8);

INCLUDE_ASM("asm/nonmatchings/akao", func_8002C7E8);

INCLUDE_ASM("asm/nonmatchings/akao", func_8002C81C);

INCLUDE_ASM("asm/nonmatchings/akao", func_8002C850);

INCLUDE_ASM("asm/nonmatchings/akao", func_8002C884);

INCLUDE_ASM("asm/nonmatchings/akao", func_8002C8C4);

INCLUDE_ASM("asm/nonmatchings/akao", func_8002C8DC);

INCLUDE_ASM("asm/nonmatchings/akao", func_8002C9E4);

INCLUDE_ASM("asm/nonmatchings/akao", func_8002CA84);

INCLUDE_ASM("asm/nonmatchings/akao", func_8002CB78);

INCLUDE_ASM("asm/nonmatchings/akao", func_8002CC18);

INCLUDE_ASM("asm/nonmatchings/akao", func_8002CC44);

INCLUDE_ASM("asm/nonmatchings/akao", func_8002CCBC);

INCLUDE_ASM("asm/nonmatchings/akao", func_8002CCCC);

INCLUDE_ASM("asm/nonmatchings/akao", func_8002CCDC);

INCLUDE_ASM("asm/nonmatchings/akao", func_8002CDD0);

INCLUDE_ASM("asm/nonmatchings/akao", func_8002CEC0);

INCLUDE_ASM("asm/nonmatchings/akao", func_8002CF1C);

INCLUDE_ASM("asm/nonmatchings/akao", func_8002CF78);

void func_8002CF98(void)
{
    /* do nothing */
}

INCLUDE_ASM("asm/nonmatchings/akao", func_8002CFA0);

INCLUDE_ASM("asm/nonmatchings/akao", func_8002CFC0);

INCLUDE_ASM("asm/nonmatchings/akao", func_8002D1E4);

INCLUDE_ASM("asm/nonmatchings/akao", func_8002D2D4);

INCLUDE_ASM("asm/nonmatchings/akao", func_8002D410);

INCLUDE_ASM("asm/nonmatchings/akao", func_8002D4A0);

INCLUDE_ASM("asm/nonmatchings/akao", func_8002D530);

INCLUDE_ASM("asm/nonmatchings/akao", func_8002D668);

INCLUDE_ASM("asm/nonmatchings/akao", func_8002D7A0);

INCLUDE_ASM("asm/nonmatchings/akao", func_8002D8E8);

INCLUDE_ASM("asm/nonmatchings/akao", func_8002DA30);

INCLUDE_ASM("asm/nonmatchings/akao", func_8002DA7C);

INCLUDE_ASM("asm/nonmatchings/akao", func_8002DF88);

INCLUDE_ASM("asm/nonmatchings/akao", func_8002E1A8);

INCLUDE_ASM("asm/nonmatchings/akao", func_8002E23C);

INCLUDE_ASM("asm/nonmatchings/akao", func_8002E428);

INCLUDE_ASM("asm/nonmatchings/akao", func_8002E478);

INCLUDE_ASM("asm/nonmatchings/akao", func_8002E954);

INCLUDE_ASM("asm/nonmatchings/akao", func_8002ED34);

INCLUDE_ASM("asm/nonmatchings/akao", func_8002F24C);

INCLUDE_ASM("asm/nonmatchings/akao", func_8002F738);

INCLUDE_ASM("asm/nonmatchings/akao", func_8002F848);

INCLUDE_ASM("asm/nonmatchings/akao", func_8002FDA0);

INCLUDE_ASM("asm/nonmatchings/akao", func_8002FE48);

INCLUDE_ASM("asm/nonmatchings/akao", func_8002FF4C);
void _AkaoNoiseVoice(void);

INCLUDE_ASM("asm/nonmatchings/akao", func_80030038);
void _AkaoReverbVoice(void);

INCLUDE_ASM("asm/nonmatchings/akao", func_80030148);
void _AkaoPitchLFOVoice(void);

INCLUDE_ASM("asm/nonmatchings/akao", func_80030234);

INCLUDE_ASM("asm/nonmatchings/akao", func_80030380);

INCLUDE_ASM("asm/nonmatchings/akao", func_800308D4);

INCLUDE_ASM("asm/nonmatchings/akao", func_80030E7C);

INCLUDE_ASM("asm/nonmatchings/akao", func_80031820);

INCLUDE_ASM("asm/nonmatchings/akao", func_800318BC);

INCLUDE_ASM("asm/nonmatchings/akao", func_80031A70);

INCLUDE_ASM("asm/nonmatchings/akao", func_80031AB0);

INCLUDE_ASM("asm/nonmatchings/akao", func_80031AFC);

INCLUDE_ASM("asm/nonmatchings/akao", func_80031BA0);

INCLUDE_ASM("asm/nonmatchings/akao", func_80031BE4);

INCLUDE_ASM("asm/nonmatchings/akao", func_80031C88);

INCLUDE_ASM("asm/nonmatchings/akao", func_80031CB0);

INCLUDE_ASM("asm/nonmatchings/akao", func_80031CE0);

INCLUDE_ASM("asm/nonmatchings/akao", func_80031D6C);

INCLUDE_ASM("asm/nonmatchings/akao", func_80031E98);

INCLUDE_ASM("asm/nonmatchings/akao", func_80031EEC);

INCLUDE_ASM("asm/nonmatchings/akao", func_80031F30);

INCLUDE_ASM("asm/nonmatchings/akao", func_80031FC0);

INCLUDE_ASM("asm/nonmatchings/akao", func_80031FF0);

INCLUDE_ASM("asm/nonmatchings/akao", func_80032078);

INCLUDE_ASM("asm/nonmatchings/akao", func_80032094);

INCLUDE_ASM("asm/nonmatchings/akao", func_800320AC);

INCLUDE_ASM("asm/nonmatchings/akao", func_800320C4);

INCLUDE_ASM("asm/nonmatchings/akao", func_80032274);

INCLUDE_ASM("asm/nonmatchings/akao", func_800323CC);

INCLUDE_ASM("asm/nonmatchings/akao", func_800324D8);

INCLUDE_ASM("asm/nonmatchings/akao", func_80032500);

INCLUDE_ASM("asm/nonmatchings/akao", func_8003252C);

INCLUDE_ASM("asm/nonmatchings/akao", func_8003257C);

INCLUDE_ASM("asm/nonmatchings/akao", func_800325B8);

INCLUDE_ASM("asm/nonmatchings/akao", func_800325C0);

INCLUDE_ASM("asm/nonmatchings/akao", func_800325E8);

INCLUDE_ASM("asm/nonmatchings/akao", func_80032614);

INCLUDE_ASM("asm/nonmatchings/akao", func_80032718);

INCLUDE_ASM("asm/nonmatchings/akao", func_80032770);

INCLUDE_ASM("asm/nonmatchings/akao", func_800327E0);

INCLUDE_ASM("asm/nonmatchings/akao", func_80032804);

INCLUDE_ASM("asm/nonmatchings/akao", func_800328D4);

INCLUDE_ASM("asm/nonmatchings/akao", func_800328F8);

INCLUDE_ASM("asm/nonmatchings/akao", func_80032968);

INCLUDE_ASM("asm/nonmatchings/akao", func_8003298C);

INCLUDE_ASM("asm/nonmatchings/akao", func_80032A04);

INCLUDE_ASM("asm/nonmatchings/akao", func_80032A28);

INCLUDE_ASM("asm/nonmatchings/akao", func_80032A98);

INCLUDE_ASM("asm/nonmatchings/akao", func_80032ABC);

INCLUDE_ASM("asm/nonmatchings/akao", func_80032B30);

INCLUDE_ASM("asm/nonmatchings/akao", func_80032BB4);

INCLUDE_ASM("asm/nonmatchings/akao", func_80032C20);

INCLUDE_ASM("asm/nonmatchings/akao", func_80032C8C);

INCLUDE_ASM("asm/nonmatchings/akao", func_80032CE8);

INCLUDE_ASM("asm/nonmatchings/akao", func_80032D44);

void func_80032D50(void)
{
    /* do nothing */
}

INCLUDE_ASM("asm/nonmatchings/akao", func_80032D58);

void func_80032D64(void)
{
    /* do nothing */
}

INCLUDE_ASM("asm/nonmatchings/akao", func_80032D6C);

INCLUDE_ASM("asm/nonmatchings/akao", func_80032E08);

INCLUDE_ASM("asm/nonmatchings/akao", func_80032E6C);

INCLUDE_ASM("asm/nonmatchings/akao", func_80032ED0);

INCLUDE_ASM("asm/nonmatchings/akao", func_80032F34);

INCLUDE_ASM("asm/nonmatchings/akao", func_80032F98);

INCLUDE_ASM("asm/nonmatchings/akao", func_80032FFC);

INCLUDE_ASM("asm/nonmatchings/akao", func_80033060);

INCLUDE_ASM("asm/nonmatchings/akao", func_800330C4);

INCLUDE_ASM("asm/nonmatchings/akao", func_80033128);

INCLUDE_ASM("asm/nonmatchings/akao", func_800331CC);

INCLUDE_ASM("asm/nonmatchings/akao", func_80033224);

INCLUDE_ASM("asm/nonmatchings/akao", func_80033264);

void AkaoCmdJumpCount(AKAO_TRACK *track, AKAO_PLAYER *player, u_int mask)
{
    int   count;
    short offset;

    count = *track->addr++;
    if (count == 0)
    {
        count = 256;
    }

    if ((track->loop_count[track->loop_depth] + 1) != count)
    {
        track->addr += 2;
    }
    else
    {
        offset = *track->addr++;
        offset |= (*track->addr++ << 8);
        track->addr += offset;
    }
}

void AkaoCmdLoopBreak(AKAO_TRACK *track, AKAO_PLAYER *player, u_int mask)
{
    int   count;
    short offset;

    count = *track->addr++;
    if (count == 0)
    {
        count = 256;
    }

    if ((track->loop_delay[track->loop_depth] + 1) != count)
    {
        track->addr += 2;
    }
    else
    {
        offset = *track->addr++;
        offset |= (*track->addr++ << 8);
        track->addr += offset;

        track->loop_depth = (track->loop_depth - 1) & 0x3;
    }
}

void AkaoCmdLoop(AKAO_TRACK *track, AKAO_PLAYER *player, u_int mask)
{
    track->loop_delay[track->loop_depth]++;
    track->addr = track->loop_addr[track->loop_depth];
}

void AkaoCmdSetNoteLength(AKAO_TRACK *track, AKAO_PLAYER *player, u_int mask)
{
    u_char delta;

    delta = *track->addr++;
    track->delta_time_counter = delta | (delta << 8);
    track->forced_delta_time = 0;
    track->previous_delta_time = delta;
}

void AkaoCmdIncNoteLength(AKAO_TRACK *track, AKAO_PLAYER *player, u_int mask)
{
    short delta;

    delta = (signed char)*track->addr++;
    if (delta != 0)
    {
        delta += track->previous_delta_time;
        if (delta < 1)
        {
            delta = 1;
        }
        else if (delta > 255)
        {
            delta = 255;
        }
    }

    track->forced_delta_time = delta;
}

void AkaoCmdDrumOn(AKAO_TRACK *track, AKAO_PLAYER *player, u_int mask)
{
    short offset;

    offset = *track->addr++;
    offset |= (*track->addr++ << 8);
    track->drum_addr = track->addr + offset;

    track->effect_flags |= AKAO_EFFECT_DRUM;
}

void AkaoCmdDrumOff(AKAO_TRACK *track, AKAO_PLAYER *player, u_int mask)
{
    track->effect_flags &= ~AKAO_EFFECT_DRUM;
}

void AkaoCmdSetTimeSignature(AKAO_TRACK *track, AKAO_PLAYER *player, u_int mask)
{
    player->ticks_per_beat = *track->addr++;
    player->beats_per_measure = *track->addr++;
    player->tick = 0;
    player->beat = 0;
}

void AkaoCmdSetMeasure(AKAO_TRACK *track, AKAO_PLAYER *player, u_int mask)
{
    player->measure = *track->addr++;
    player->measure |= *track->addr++ << 8;
}

void AkaoCmdSetUnknown(AKAO_TRACK *track, AKAO_PLAYER *player, u_int mask)
{
    player->field_54 = 1;
}

void AkaoCmdSetDRSL(AKAO_TRACK *track, AKAO_PLAYER *player, u_int mask)
{
    func_80032E6C(track, player, mask);
    func_80032ED0(track, player, mask);
}

void AkaoCmdNoiseDelay(AKAO_TRACK *track, AKAO_PLAYER *player, u_int mask)
{
    int delay;

    delay = *track->addr++;
    if (delay == 0)
    {
        track->noise_delay = 257;
    }
    else
    {
        track->noise_delay = delay + 1;
    }

    func_80032ABC(track, player, mask);
}

void AkaoCmdNoiseToggle(AKAO_TRACK *track, AKAO_PLAYER *player, u_int mask)
{
    int delay;

    delay = *track->addr++;
    if (delay == 0)
    {
        track->noise_delay = 257;
    }
    else
    {
        track->noise_delay = delay + 1;
    }
}

void AkaoCmdFreqModulateDelay(AKAO_TRACK *track, AKAO_PLAYER *player, u_int mask)
{
    int delay;

    delay = *track->addr++;
    if (delay == 0)
    {
        track->freq_modulate_delay = 257;
    }
    else
    {
        track->freq_modulate_delay = delay + 1;
    }

    func_80032BB4(track, player, mask);
}

void AkaoCmdFreqModulateToggle(AKAO_TRACK *track, AKAO_PLAYER *player, u_int mask)
{
    int delay;

    delay = *track->addr++;
    if (delay == 0)
    {
        track->freq_mod_delay = 257;
    }
    else
    {
        track->freq_mod_delay = delay + 1;
    }
}

void AkaoCmdResetEffects(AKAO_TRACK *track, AKAO_PLAYER *player, u_int mask)
{
    track->effect_flags &= ~(AKAO_EFFECT_VOLMOD | AKAO_EFFECT_FREQMOD | 0x7);
    func_80032B30(track, player, mask);
    func_80032C20(track, player, mask);
    func_80032CE8(track, player, mask);
    track->legato_flags &= ~0x5;
}

void AkaoCmdFreqModulateOn(AKAO_TRACK *track, AKAO_PLAYER *player, u_int mask)
{
    track->effect_flags |= AKAO_EFFECT_FREQMOD;
}

void AkaoCmdFreqModulateOff(AKAO_TRACK *track, AKAO_PLAYER *player, u_int mask)
{
    track->effect_flags &= ~AKAO_EFFECT_FREQMOD;
}

void AkaoCmdVolModulateOn(AKAO_TRACK *track, AKAO_PLAYER *player, u_int mask)
{
    track->effect_flags |= AKAO_EFFECT_VOLMOD;
}

void AkaoCmdVolModulateOff(AKAO_TRACK *track, AKAO_PLAYER *player, u_int mask)
{
    track->effect_flags &= ~AKAO_EFFECT_VOLMOD;
}

void AkaoCmdJump(AKAO_TRACK *track, AKAO_PLAYER *player, u_int mask)
{
    short offset;

    offset = *track->addr++;
    offset |= (*track->addr++ << 8);
    track->addr += offset;
}

void AkaoCmdJumpCond(AKAO_TRACK *track, AKAO_PLAYER *player, u_int mask)
{
    int   cond;
    short offset;

    cond = *track->addr++;
    if (player->condition != 0 && cond <= player->condition)
    {
        offset = *track->addr++;
        offset |= (*track->addr++ << 8);

        track->addr += offset;
        player->last_condition = cond;
    }
    else
    {
        track->addr += 2;
    }
}

/* NOT MATCHING! */
void AkaoFinishChannel(AKAO_TRACK *track, AKAO_PLAYER *player, u_int mask)
{
    u_int voice;

    if (track->field_54 == 0)
    {
        player->voice_mask = (mask ^ 0xFFFFFF) & player->voice_mask;

        if (player->voice_mask == 0)
        {
            player->song_id = 0;
        }

        player->noise_mask     = (mask ^ 0xFFFFFF) & player->noise_mask;
        player->reverb_mask    = (mask ^ 0xFFFFFF) & player->reverb_mask;
        player->pitch_lfo_mask = (mask ^ 0xFFFFFF) & player->pitch_lfo_mask;

        if (track->effect_flags & 0x100)
        {
            voice = track->overlay_voice;
            if (D_80062F04 != 0)
            {
                voice -= 24;
            }

            player->overlay_mask = ~(1 << voice) & player->overlay_mask;
        }

        if (track->effect_flags & 0x200)
        {
            player->alternate_mask = ~(1 << track->alternate_voice) & player->alternate_mask;
        }
    }
    else
    {
        D_80099FCC = (mask ^ 0xFF0000) & D_80099FCC;
        D_80099FEC = (mask ^ 0xFF0000) & D_80099FEC;
        D_80099FF0 = (mask ^ 0xFF0000) & D_80099FF0;
        D_80099FF4 = (mask ^ 0xFF0000) & D_80099FF4;

        AkaoPlayer.keyons  = ~mask & AkaoPlayer.keyons;
        AkaoPlayer.active  = ~mask & AkaoPlayer.active;
        AkaoPlayer.keyoffs = ~mask & AkaoPlayer.keyoffs;

        AkaoTracks[track->alternate_voice].attr.update_flags |= 0x1FF80;
    }

    track->effect_flags = 0;
    AkaoPlayer.spucnt |= 0x10;

    _AkaoNoiseVoice();
    _AkaoReverbVoice();
    _AkaoPitchLFOVoice();
}

void func_80033A70(AKAO_TRACK *track, AKAO_PLAYER *player, u_int mask)
{
    AkaoFinishChannel(track, player, mask);
}
