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

typedef struct _AKAO_INSTRUMENT {
    /* 0x00 */ u_int      addr;
    /* 0x04 */ u_int      loop_addr;
    /* 0x08 */ u_char     ar;
    /* 0x09 */ u_char     dr;
    /* 0x0A */ u_char     sl;
    /* 0x0B */ u_char     sr;
    /* 0x0C */ u_char     rr;
    /* 0x0D */ u_char     a_mode;
    /* 0x0E */ u_char     s_mode;
    /* 0x0F */ u_char     r_mode;
    /* 0x10 */ u_int      pitch[12];
} AKAO_INSTRUMENT;

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
    /* 0x18 */ u_char    *vibrato_addr;
    /* 0x1C */ short     *tremolo_addr;
    /* 0x20 */ short     *pan_addr;
    /* 0x24 */ u_int      overlay_voice;
    /* 0x28 */ u_int      alternate_voice;
    /* 0x2C */ u_int      master_volume;
    /* 0x30 */ u_int      pitch_of_note;
    /* 0x34 */ char       pad1[0x4];
    /* 0x38 */ u_int      effect_flags;
    /* 0x3C */ char       pad2[0x8];
    /* 0x44 */ u_int      volume;
    /* 0x48 */ int        volume_slope;
    /* 0x4C */ char       pad3[0x6];
    /* 0x52 */ u_short    field_52;
    /* 0x54 */ u_short    field_54;
    /* 0x56 */ u_short    delta_time_counter;
    /* 0x58 */ u_short    instrument;
    /* 0x5A */ char       pad4[0x2];
    /* 0x5C */ u_short    volume_slide_length_counter;
    /* 0x5E */ u_short    overlay_balance_slide_length_counter;
    /* 0x60 */ u_short    pan;
    /* 0x62 */ u_short    pan_slide_length;
    /* 0x64 */ char       pad5[0x2];
    /* 0x66 */ u_short    octave;
    /* 0x68 */ u_short    portamento_length;
    /* 0x6A */ u_short    previous_note_number;
    /* 0x6C */ u_short    portamento_speed;
    /* 0x6E */ u_short    legato_flags;
    /* 0x70 */ char       pad6[0x2];
    /* 0x72 */ u_short    vibrato_delay;
    /* 0x74 */ u_short    vibrato_delay_counter;
    /* 0x76 */ u_short    vibrato_rate;
    /* 0x78 */ u_short    vibrato_rate_counter;
    /* 0x7A */ u_short    vibrato_form;
    /* 0x7C */ u_short    vibrato_max_amplitude;
    /* 0x7E */ u_short    vibrato_depth;
    /* 0x80 */ u_short    vibrato_depth_slide_length_counter;
    /* 0x82 */ short      vibrato_depth_slope;
    /* 0x84 */ char       pad7[0x2];
    /* 0x86 */ u_short    tremolo_delay;
    /* 0x88 */ u_short    tremolo_delay_counter;
    /* 0x8A */ u_short    tremolo_rate;
    /* 0x8C */ u_short    tremolo_rate_counter;
    /* 0x8E */ u_short    tremolo_form;
    /* 0x90 */ u_short    tremolo_depth;
    /* 0x92 */ u_short    tremolo_depth_slide_length_counter;
    /* 0x94 */ short      tremolo_depth_slope;
    /* 0x96 */ char       pad8[0x2];
    /* 0x98 */ u_short    pan_lfo_rate;
    /* 0x9A */ u_short    pan_lfo_rate_counter;
    /* 0x9C */ u_short    pan_lfo_form;
    /* 0x9E */ u_short    pan_lfo_depth;
    /* 0xA0 */ u_short    pan_lfo_depth_slide_length_counter;
    /* 0xA2 */ short      pan_lfo_slope;
    /* 0xA4 */ u_short    noise_delay;
    /* 0xA6 */ u_short    freq_modulate_delay;
    /* 0xA8 */ char       pad9[0x10];
    /* 0xB8 */ u_short    loop_depth;
    /* 0xBA */ u_short    loop_delay[4];
    /* 0xC2 */ u_short    previous_delta_time;
    /* 0xC4 */ u_short    forced_delta_time;
    /* 0xC6 */ u_short    overlay_balance;
    /* 0xC8 */ short      overlay_balance_slope;
    /* 0xCA */ short      pan_slope;
    /* 0xCC */ short      transpose;
    /* 0xCE */ short      tuning;
    /* 0xD0 */ u_short    note;
    /* 0xD2 */ short      portamento_amount;
    /* 0xD4 */ short      previous_transpose;
    /* 0xD6 */ short      vibrato_lfo_amplitude;
    /* 0xD8 */ short      tremolo_lfo_amplitude;
    /* 0xDA */ short      pan_lfo_amplitude;
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
    /* 0x3C */ u_int      reverb_type;
    /* 0x40 */ u_int      reverb_depth;
    /* 0x44 */ int        reverb_depth_slope;
    /* 0x48 */ u_short    tempo_slide_length;
    /* 0x4A */ u_short    song_id;
    /* 0x4C */ u_short    last_condition;
    /* 0x4E */ u_short    condition;
    /* 0x50 */ u_short    reverb_depth_slide_length;
    /* 0x52 */ u_short    noise_clock;
    /* 0x54 */ u_short    field_54;
    /* 0x56 */ u_short    beats_per_measure;
    /* 0x58 */ u_short    beat;
    /* 0x5A */ u_short    ticks_per_beat;
    /* 0x5C */ u_short    tick;
    /* 0x5E */ u_short    measure;
} AKAO_PLAYER;

static AKAO_INSTRUMENT AkaoInstruments[128];
static AKAO_PLAYER AkaoPlayer;
static u_int       AkaoSpuControl; /* same as AkaoPlayer.spucnt */
static AKAO_TRACK AkaoTracks[24];

static u_short AkaoNoiseClock;

static int D_80062F04;

static u_int D_80099FCC;
static u_int D_80099FEC;
static u_int D_80099FF0;
static u_int D_80099FF4;

void *AkaoVibratoForms[16];

#define AKAO_EFFECT_VIBRATO 0x01
#define AKAO_EFFECT_TREMOLO 0x02
#define AKAO_EFFECT_PAN_LFO 0x04
#define AKAO_EFFECT_DRUM    0x08
#define AKAO_EFFECT_FREQMOD 0x10
#define AKAO_EFFECT_VOLMOD  0x20
#define AKAO_EFFECT_ALL     (AKAO_EFFECT_VIBRATO | AKAO_EFFECT_TREMOLO | AKAO_EFFECT_PAN_LFO | AKAO_EFFECT_DRUM | AKAO_EFFECT_FREQMOD | AKAO_EFFECT_VOLMOD)

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
void AkaoSetTrackInstrument(AKAO_TRACK *track, u_short num);

INCLUDE_ASM("asm/nonmatchings/akao", func_800318BC);

INCLUDE_ASM("asm/nonmatchings/akao", func_80031A70);

/* command.c */

void AkaoCmdSetTempo(AKAO_TRACK *track, AKAO_PLAYER *player, u_int mask)
{
    player->tempo = *track->addr++ << 16;
    player->tempo |= *track->addr++ << 24;

    player->tempo_slide_length = 0;
}

void AkaoCmdSetTempoSlide(AKAO_TRACK *track, AKAO_PLAYER *player, u_int mask)
{
    int length;
    int tempo;
    int current;

    length = *track->addr++;
    player->tempo_slide_length = length;
    if (length == 0)
    {
        player->tempo_slide_length = 256;
    }

    tempo = *track->addr++ << 16;
    tempo |= *track->addr++ << 24;

    current = player->tempo & 0xFFFF0000;
    player->tempo_slope = (tempo - current) / player->tempo_slide_length;
    player->tempo = current;
}

void AkaoCmdSetReverbDepth(AKAO_TRACK *track, AKAO_PLAYER *player, u_int mask)
{
    int depth;

    depth = *track->addr++ << 16;
    depth |= *track->addr++ << 24;

    player->spucnt |= 0x80;

    player->reverb_depth = depth;
    player->reverb_depth_slide_length = 0;
}

void AkaoCmdSetReverbSlide(AKAO_TRACK *track, AKAO_PLAYER *player, u_int mask)
{
    int length;
    int depth;
    int current;

    length = *track->addr++;
    player->reverb_depth_slide_length = length;
    if (length == 0)
    {
        player->reverb_depth_slide_length = 256;
    }

    depth = *track->addr++ << 16;
    depth |= *track->addr++ << 24;

    current = player->reverb_depth & 0xFFFF0000;
    player->reverb_depth_slope = (depth - current) / player->reverb_depth_slide_length;
    player->reverb_depth = current;
}

void AkaoCmdSetMasterVolume(AKAO_TRACK *track, AKAO_PLAYER *player, u_int mask)
{
    track->master_volume = *track->addr++;
    track->attr.update_flags |= 0x3;
}

void AkaoCmdSetVolume(AKAO_TRACK *track, AKAO_PLAYER *player, u_int mask)
{
    track->volume = (signed char)*track->addr++ << 23;
    track->volume_slide_length_counter = 0;
    track->attr.update_flags |= 0x3;
}

void AkaoCmdSetVolumeSlide(AKAO_TRACK *track, AKAO_PLAYER *player, u_int mask)
{
    int length;
    int slope;
    int current;

    length = *track->addr++;
    track->volume_slide_length_counter = length;
    if (length == 0)
    {
        track->volume_slide_length_counter = 256;
    }

    slope = (signed char)*track->addr++ << 23;

    current = track->volume & 0xFFFF0000;
    track->volume_slope = (slope - current) / track->volume_slide_length_counter;
    track->volume = current;
}

void AkaoCmdSetOverlayVoiceOn(AKAO_TRACK *track, AKAO_PLAYER *player, u_int mask)
{
    u_int   used;
    u_short voice;
    int     index;
    int     inst1, inst2;

    if (!(track->effect_flags & 0x100))
    {
        used = player->voice_mask | player->overlay_mask | player->alternate_mask;
        voice = D_80062F04 ? 24 : 0;
        index = 1;

        while (1)
        {
            if ((used & index) == 0)
            {
                break;
            }

            voice++;
            index <<= 1;

            if ((index & 0xFFFFFF) == 0)
            {
                return;
            }
        }
    }
    else
    {
        voice = track->overlay_voice;

        if (track->overlay_voice >= 24)
        {
            index = 1 << (track->overlay_voice - 24);
        }
        else
        {
            index = 1 << track->overlay_voice;
        }
    }

    if (index & 0xFFFFFF)
    {
        player->overlay_mask |= index;
        track->overlay_voice = voice;
        track->effect_flags |= 0x100;

        inst1 = *track->addr++;
        inst2 = *track->addr++;

        AkaoSetTrackInstrument(track, inst1);
        AkaoSetTrackInstrument(&AkaoTracks[voice], inst2);
    }
}

void AkaoCmdSetOverlayVoiceOff(AKAO_TRACK *track, AKAO_PLAYER *player, u_int mask)
{
    u_short voice;

    voice = track->overlay_voice;
    if (D_80062F04 != 0)
    {
        voice -= 24;
    }

    if (track->effect_flags & 0x100)
    {
        track->effect_flags &= ~0x100;
        player->overlay_mask &= ~(1 << voice);
    }
}

void AkaoCmdSetOverlayBalance(AKAO_TRACK *track, AKAO_PLAYER *player, u_int mask)
{
    track->overlay_balance = *track->addr++ << 8;
    track->overlay_balance_slide_length_counter = 0;

    if (track->effect_flags & 0x100)
    {
        track->attr.update_flags |= 0x3;
    }
}

void AkaoCmdSetOverlayBalanceSlide(AKAO_TRACK *track, AKAO_PLAYER *player, u_int mask)
{
    int   length;
    int   balance;
    short current;

    length = *track->addr++;
    track->overlay_balance_slide_length_counter = length;
    if (length == 0)
    {
        track->overlay_balance_slide_length_counter = 256;
    }

    balance = *track->addr++ << 8;

    current = track->overlay_balance & 0xFF00;
    track->overlay_balance_slope = (balance - current) / track->overlay_balance_slide_length_counter;
    track->overlay_balance = current;
}

void AkaoCmdSetPan(AKAO_TRACK *track, AKAO_PLAYER *player, u_int mask)
{
    track->pan = *track->addr++ << 8;
    track->pan_slide_length = 0;
    track->attr.update_flags |= 0x3;
}

void AkaoCmdSetPanSlide(AKAO_TRACK *track, AKAO_PLAYER *player, u_int mask)
{
    int length;
    int pan;

    length = *track->addr++;
    track->pan_slide_length = length;
    if (length == 0)
    {
        track->pan_slide_length = 256;
    }

    pan = track->pan & 0xFF00;
    track->pan_slope = ((*track->addr++ << 8) - pan) / track->pan_slide_length;
    track->pan = pan;
}

void AkaoCmdSetOctave(AKAO_TRACK *track, AKAO_PLAYER *player, u_int mask)
{
    track->octave = *track->addr++;
}

void AkaoCmdIncOctave(AKAO_TRACK *track, AKAO_PLAYER *player, u_int mask)
{
    track->octave = (track->octave + 1) & 0xF;
}

void AkaoCmdDecOctave(AKAO_TRACK *track, AKAO_PLAYER *player, u_int mask)
{
    track->octave = (track->octave - 1) & 0xF;
}

void AkaoCmdLoadInstrument(AKAO_TRACK *track, AKAO_PLAYER *player, u_int mask)
{
    int              next;
    u_short          voice;
    AKAO_INSTRUMENT *instr;

    next = *track->addr++;

    voice = track->overlay_voice;
    if (D_80062F04 != 0)
    {
        voice -= 24;
    }

    if (track->effect_flags & 0x100)
    {
        player->overlay_mask &= ~(1 << voice);
        track->effect_flags &= ~0x100;
    }

    if ((track->field_54 != 0) || !(mask & player->active & D_80099FCC))
    {
        track->attr.update_flags |= 0x10;
        track->pitch_of_note = (track->pitch_of_note * AkaoInstruments[next].pitch[0]) / AkaoInstruments[track->instrument].pitch[0];
    }

    if (track->effect_flags & 0x200)
    {
        instr = &AkaoInstruments[next];

        track->instrument = next;
        track->attr.addr = instr->addr;
        track->attr.loop_addr = instr->loop_addr;
        track->attr.ar = instr->ar;
        track->attr.dr = instr->dr;
        track->attr.sl = instr->sl;
        track->attr.sr = instr->sr;
        track->attr.a_mode = instr->a_mode;
        track->attr.s_mode = instr->s_mode;
        track->attr.update_flags |= 0x1BB80;
    }
    else
    {
        AkaoSetTrackInstrument(track, next);
    }
}

void AkaoCmdLoadInstrumentSoft(AKAO_TRACK *track, AKAO_PLAYER *player, u_int mask)
{
    int              next;
    AKAO_INSTRUMENT *instr;

    next = *track->addr++;
    instr = &AkaoInstruments[next];

    if ((track->field_54 != 0) || !(mask & player->active & D_80099FCC))
    {
        track->attr.update_flags |= 0x10;
        track->pitch_of_note = (track->pitch_of_note * instr->pitch[0]) / AkaoInstruments[track->instrument].pitch[0];
    }

    track->instrument = next;
    track->attr.addr = 0x76FE0;
    track->attr.loop_addr = instr->loop_addr;
    track->attr.ar = instr->ar;
    track->attr.dr = instr->dr;
    track->attr.sl = instr->sl;
    track->attr.sr = instr->sr;
    track->attr.a_mode = instr->a_mode;
    track->attr.s_mode = instr->s_mode;

    if (track->effect_flags & 0x200)
    {
        track->attr.update_flags |= 0x1BB80;
    }
    else
    {
        track->attr.rr = instr->rr;
        track->attr.r_mode = instr->r_mode;
        track->attr.update_flags |= 0x1FF80;
    }
}

void AkaoCmdResetADSR(AKAO_TRACK *track, AKAO_PLAYER *player, u_int mask)
{
    AKAO_INSTRUMENT *instr;
    AKAO_TRACK      *overlay;

    instr = &AkaoInstruments[track->instrument];
    track->attr.ar = instr->ar;
    track->attr.dr = instr->dr;
    track->attr.sl = instr->sl;
    track->attr.sr = instr->sr;
    track->attr.rr = instr->rr;
    track->attr.a_mode = instr->a_mode;
    track->attr.s_mode = instr->s_mode;
    track->attr.r_mode = instr->r_mode;
    track->attr.update_flags |= 0xFF00;

    if (track->effect_flags & 0x100)
    {
        overlay = &AkaoTracks[track->overlay_voice];
        overlay->attr.ar = track->attr.ar;
        overlay->attr.dr = track->attr.dr;
        overlay->attr.sl = track->attr.sl;
        overlay->attr.sr = track->attr.sr;
        overlay->attr.rr = track->attr.rr;
        overlay->attr.a_mode = track->attr.a_mode;
        overlay->attr.s_mode = track->attr.s_mode;
        overlay->attr.r_mode = track->attr.r_mode;
    }
}

void AkaoCmdSetTranspose(AKAO_TRACK *track, AKAO_PLAYER *player, u_int mask)
{
    track->transpose = (signed char)*track->addr++;
}

void AkaoCmdAdjustTranspose(AKAO_TRACK *track, AKAO_PLAYER *player, u_int mask)
{
    track->transpose += (signed char)*track->addr++;
}

void AkaoCmdSetPortamento(AKAO_TRACK *track, AKAO_PLAYER *player, u_int mask)
{
    int length;

    length = *track->addr++;
    track->portamento_length = length;
    if (length == 0)
    {
        track->portamento_length = 256;
    }

    track->portamento_amount = (signed char)*track->addr++;
}

void AkaoCmdSetPortamentoOn(AKAO_TRACK *track, AKAO_PLAYER *player, u_int mask)
{
    u_char speed;

    speed = *track->addr++;
    track->portamento_speed = speed;
    if (speed == 0)
    {
        track->portamento_speed = 256;
    }

    track->previous_transpose = 0;
    track->previous_note_number = 0;
    track->legato_flags = 1;
}

void AkaoCmdSetPortamentoOff(AKAO_TRACK *track, AKAO_PLAYER *player, u_int mask)
{
    track->portamento_speed = 0;
}

void AkaoCmdSetFineTune(AKAO_TRACK *track, AKAO_PLAYER *player, u_int mask)
{
    track->tuning = (signed char)*track->addr++;
}

void AkaoCmdAdjustFineTune(AKAO_TRACK *track, AKAO_PLAYER *player, u_int mask)
{
    track->tuning += (signed char)*track->addr++;
}

void AkaoCmdSetVibratoOn(AKAO_TRACK *track, AKAO_PLAYER *player, u_int mask)
{
    int vib_depth;
    int rate;
    int pitch;
    int depth;
    int amplitude;

    track->effect_flags |= AKAO_EFFECT_VIBRATO;

    if (track->field_54 != 0)
    {
        track->vibrato_delay = 0;

        vib_depth = *track->addr++;
        if (vib_depth != 0)
        {
            track->vibrato_depth = vib_depth << 8;
        }
    }
    else
    {
        track->vibrato_delay = *track->addr++;
    }

    rate = *track->addr++;
    track->vibrato_rate = rate;
    if (rate == 0)
    {
        track->vibrato_rate = 256;
    }

    track->vibrato_form = *track->addr++;

    pitch = track->pitch_of_note & 0xFFFF;
    depth = (track->vibrato_depth & 0x7F00) >> 8;

    if (track->vibrato_depth & 0x8000)
    {
        amplitude = (depth * pitch) >> 7;
    }
    else
    {
        amplitude = (depth * ((pitch * 15) >> 8)) >> 7;
    }

    track->vibrato_max_amplitude = amplitude;

    track->vibrato_addr = AkaoVibratoForms[track->vibrato_form];

    track->vibrato_delay_counter = track->vibrato_delay;
    track->vibrato_rate_counter = 1;
}

void AkaoCmdSetVibratoDepth(AKAO_TRACK *track, AKAO_PLAYER *player, u_int mask)
{
    int pitch;
    int depth;
    int amplitude;

    track->vibrato_depth = *track->addr++ << 8;

    pitch = track->pitch_of_note;
    depth = (track->vibrato_depth & 0x7F00) >> 8;

    if (track->vibrato_depth & 0x8000)
    {
        amplitude = (depth * pitch) >> 7;
    }
    else
    {
        amplitude = (depth * ((pitch * 15) >> 8)) >> 7;
    }

    track->vibrato_max_amplitude = amplitude;
}

void AkaoCmdSetVibratoSlide(AKAO_TRACK *track, AKAO_PLAYER *player, u_int mask)
{
    int length;

    length = *track->addr++;
    if (length == 0)
    {
        length = 256;
    }

    track->vibrato_depth_slope = ((*track->addr++ << 8) - track->vibrato_depth) / length;
    track->vibrato_depth_slide_length_counter = length;
}

void AkaoCmdSetVibratoOff(AKAO_TRACK* track, AKAO_PLAYER* player, u_int mask)
{
    track->effect_flags &= ~AKAO_EFFECT_VIBRATO;

    track->vibrato_lfo_amplitude = 0;
    track->attr.update_flags |= 0x10;
}

void AkaoCmdSetTremoloOn(AKAO_TRACK *track, AKAO_PLAYER *player, u_int mask)
{
    int depth;
    int rate;

    track->effect_flags |= AKAO_EFFECT_TREMOLO;

    if (track->field_54 != 0)
    {
        track->tremolo_delay = 0;

        depth = *track->addr++;
        if (depth != 0)
        {
            track->tremolo_depth = depth << 8;
        }
    }
    else
    {
        track->tremolo_delay = *track->addr++;
    }

    rate = *track->addr++;
    track->tremolo_rate = rate;
    if (rate == 0)
    {
        track->tremolo_rate = 256;
    }

    track->tremolo_form = *track->addr++;
    track->tremolo_addr = AkaoVibratoForms[track->tremolo_form];

    track->tremolo_delay_counter = track->tremolo_delay;
    track->tremolo_rate_counter = 1;
}

void AkaoCmdSetTremoloDepth(AKAO_TRACK *track, AKAO_PLAYER *player, u_int mask)
{
    track->tremolo_depth = *track->addr++ << 8;
}

void AkaoCmdSetTremoloSlide(AKAO_TRACK *track, AKAO_PLAYER *player, u_int mask)
{
    int length;

    length = *track->addr++;
    if (length == 0)
    {
        length = 256;
    }

    track->tremolo_depth_slope = ((*track->addr++ << 8) - track->tremolo_depth) / length;
    track->tremolo_depth_slide_length_counter = length;
}

void AkaoCmdSetTremoloOff(AKAO_TRACK *track, AKAO_PLAYER *player, u_int mask)
{
    track->effect_flags &= ~AKAO_EFFECT_TREMOLO;

    track->tremolo_lfo_amplitude = 0;
    track->attr.update_flags |= 0x3;
}

void AkaoCmdSetPanLFOOn(AKAO_TRACK *track, AKAO_PLAYER *player, u_int mask)
{
    int rate;

    track->effect_flags |= AKAO_EFFECT_PAN_LFO;

    rate = *track->addr++;
    track->pan_lfo_rate = rate;
    if (rate == 0)
    {
        track->pan_lfo_rate = 256;
    }

    track->pan_lfo_form = *track->addr++;
    track->pan_addr = AkaoVibratoForms[track->pan_lfo_form];

    track->pan_lfo_rate_counter = 1;
}

void AkaoCmdSetPanLFODepth(AKAO_TRACK *track, AKAO_PLAYER *player, u_int mask)
{
    track->pan_lfo_depth = *track->addr++ << 7;
}

void AkaoCmdSetPanLFOSlide(AKAO_TRACK *track, AKAO_PLAYER *player, u_int mask)
{
    int length;

    length = *track->addr++;
    if (length == 0)
    {
        length = 256;
    }

    track->pan_lfo_slope = ((*track->addr++ << 7) - track->pan_lfo_depth) / length;
    track->pan_lfo_depth_slide_length_counter = length;
}

void AkaoCmdSetPanLFOOff(AKAO_TRACK* track, AKAO_PLAYER* player, u_int mask)
{
    track->effect_flags &= ~AKAO_EFFECT_PAN_LFO;

    track->pan_lfo_amplitude = 0;
    track->attr.update_flags |= 0x3;
}

void AkaoCmdSetNoiseOn(AKAO_TRACK *track, AKAO_PLAYER *player, u_int mask)
{
    if (track->field_54 == 0)
    {
        player->noise_mask |= mask;
    }
    else
    {
        D_80099FEC |= mask;
    }

    AkaoSpuControl |= 0x10;

    _AkaoNoiseVoice();
}

void AkaoCmdSetNoiseOff(AKAO_TRACK *track, AKAO_PLAYER *player, u_int mask)
{
    if (track->field_54 == 0)
    {
        player->noise_mask &= ~mask;
    }
    else
    {
        D_80099FEC &= ~mask;
    }

    AkaoSpuControl |= 0x10;

    _AkaoNoiseVoice();
    track->noise_delay = 0;
}

void AkaoCmdSetFreqModulateOn(AKAO_TRACK *track, AKAO_PLAYER *player, u_int mask)
{
    if (track->field_54 == 0)
    {
        player->pitch_lfo_mask |= mask;
    }
    else if ((mask & 0x555555) == 0)
    {
        D_80099FF4 |= mask;
    }

    _AkaoPitchLFOVoice();
}

void AkaoCmdSetFreqModulateOff(AKAO_TRACK *track, AKAO_PLAYER *player, u_int mask)
{
    if (track->field_54 == 0)
    {
        player->pitch_lfo_mask &= ~mask;
    }
    else
    {
        D_80099FF4 &= ~mask;
    }

    _AkaoPitchLFOVoice();
    track->freq_modulate_delay = 0;
}

void AkaoCmdSetReverbOn(AKAO_TRACK *track, AKAO_PLAYER *player, u_int mask)
{
    if (track->field_54 == 0)
    {
        player->reverb_mask |= mask;
    }
    else
    {
        D_80099FF0 |= mask;
    }

    _AkaoReverbVoice();
}

void AkaoCmdSetReverbOff(AKAO_TRACK *track, AKAO_PLAYER *player, u_int mask)
{
    if (track->field_54 == 0)
    {
        player->reverb_mask &= ~mask;
    }
    else
    {
        D_80099FF0 &= ~mask;
    }

    _AkaoReverbVoice();
}

void AkaoCmdSetLegatoOn(AKAO_TRACK *track, AKAO_PLAYER *player, u_int mask)
{
    track->legato_flags = 1;
}

void AkaoCmdSetLegatoOff(AKAO_TRACK *track, AKAO_PLAYER *player, u_int mask)
{
    /* do nothing */
}

void AkaoCmdSetFullNoteOn(AKAO_TRACK *track, AKAO_PLAYER *player, u_int mask)
{
    track->legato_flags = 4;
}

void AkaoCmdSetFullNoteOff(AKAO_TRACK *track, AKAO_PLAYER *player, u_int mask)
{
    /* do nothing */
}

void AkaoCmdSetNoiseClock(AKAO_TRACK *track, AKAO_PLAYER *player, u_int mask)
{
    int clock;

    clock = *track->addr++;
    if (track->field_54 == 0)
    {
        if (clock & 0xC0)
        {
            player->noise_clock += clock & 0x3F;
            player->noise_clock &= 0x3F;
        }
        else
        {
            player->noise_clock = clock;
        }
    }
    else
    {
        if (clock & 0xC0)
        {
            AkaoNoiseClock += clock & 0x3F;
            AkaoNoiseClock &= 0x3F;
        }
        else
        {
            AkaoNoiseClock = clock;
        }
    }

    /* TODO: AkaoPlayer.spucnt, doesn't match when using struct */
    AkaoSpuControl |= 0x10;
}

void AkaoCmdSetAttackRate(AKAO_TRACK *track, AKAO_PLAYER *player, u_int mask)
{
    track->attr.ar = *track->addr++;
    track->attr.update_flags |= 0x900;

    if (track->effect_flags & 0x100)
    {
        AkaoTracks[track->overlay_voice].attr.ar = track->attr.ar;
    }
}

void AkaoCmdSetDecayRate(AKAO_TRACK *track, AKAO_PLAYER *player, u_int mask)
{
    track->attr.dr = *track->addr++;
    track->attr.update_flags |= 0x1000;

    if (track->effect_flags & 0x100)
    {
        AkaoTracks[track->overlay_voice].attr.dr = track->attr.dr;
    }
}

void AkaoCmdSetSustainLevel(AKAO_TRACK *track, AKAO_PLAYER *player, u_int mask)
{
    track->attr.sl = *track->addr++;
    track->attr.update_flags |= 0x8000;

    if (track->effect_flags & 0x100)
    {
        AkaoTracks[track->overlay_voice].attr.sl = track->attr.sl;
    }
}

void AkaoCmdSetSustainRate(AKAO_TRACK *track, AKAO_PLAYER *player, u_int mask)
{
    track->attr.sr = *track->addr++;
    track->attr.update_flags |= 0x2200;

    if (track->effect_flags & 0x100)
    {
        AkaoTracks[track->overlay_voice].attr.sr = track->attr.sr;
    }
}

void AkaoCmdSetReleaseRate(AKAO_TRACK *track, AKAO_PLAYER *player, u_int mask)
{
    track->attr.rr = *track->addr++;
    track->attr.update_flags |= 0x4400;

    if (track->effect_flags & 0x100)
    {
        AkaoTracks[track->overlay_voice].attr.rr = track->attr.rr;
    }
}

void AkaoCmdSetAttackMode(AKAO_TRACK *track, AKAO_PLAYER *player, u_int mask)
{
    track->attr.a_mode = *track->addr++;
    track->attr.update_flags |= 0x100;

    if (track->effect_flags & 0x100)
    {
        AkaoTracks[track->overlay_voice].attr.a_mode = track->attr.a_mode;
    }
}

void AkaoCmdSetSustainMode(AKAO_TRACK *track, AKAO_PLAYER *player, u_int mask)
{
    track->attr.s_mode = *track->addr++;
    track->attr.update_flags |= 0x200;

    if (track->effect_flags & 0x100)
    {
        AkaoTracks[track->overlay_voice].attr.s_mode = track->attr.s_mode;
    }
}

void AkaoCmdSetReleaseMode(AKAO_TRACK *track, AKAO_PLAYER *player, u_int mask)
{
    track->attr.r_mode = *track->addr++;
    track->attr.update_flags |= 0x400;

    if (track->effect_flags & 0x100)
    {
        AkaoTracks[track->overlay_voice].attr.r_mode = track->attr.r_mode;
    }
}

void AkaoCmdSetAlternateVoiceOn(AKAO_TRACK *track, AKAO_PLAYER *player, u_int mask)
{
    u_int used;
    int   i;
    int   bit;

    track->attr.rr = *track->addr++;
    if (track->effect_flags & 0x200)
    {
        return;
    }

    used = player->voice_mask | player->overlay_mask | player->alternate_mask;
    for (i = 0, bit = 1; bit & 0xFFFFFF; i++, bit <<= 1)
    {
        if ((used & bit) == 0)
        {
            break;
        }
    }

    if (bit & 0xFFFFFF)
    {
        player->alternate_mask |= bit;
        track->alternate_voice = i & 0xFFFF;
        track->effect_flags |= 0x200;
    }
}

void AkaoCmdSetAlternateVoiceOff(AKAO_TRACK *track, AKAO_PLAYER *player, u_int mask)
{
    player->alternate_mask &= ~(1 << track->alternate_voice);
    track->effect_flags &= ~0x200;

    track->attr.rr = AkaoInstruments[track->instrument].rr;
    track->attr.update_flags |= 0x4400;
}

void AkaoCmdLoopStart(AKAO_TRACK *track, AKAO_PLAYER *player, u_int mask)
{
    track->loop_depth = (track->loop_depth + 1) & 0x3;
    track->loop_addr[track->loop_depth] = track->addr;
    track->loop_delay[track->loop_depth] = 0;
}

void AkaoCmdLoopEndCount(AKAO_TRACK *track, AKAO_PLAYER *player, u_int mask)
{
    int count;

    count = *track->addr++;
    if (count == 0)
    {
        count = 256;
    }

    if (++track->loop_delay[track->loop_depth] != count)
    {
        track->addr = track->loop_addr[track->loop_depth];
    }
    else
    {
        track->loop_depth = (track->loop_depth - 1) & 0x3;
    }
}

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
        offset |= *track->addr++ << 8;
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
        offset |= *track->addr++ << 8;
        track->addr += offset;

        track->loop_depth = (track->loop_depth - 1) & 0x3;
    }
}

void AkaoCmdLoopEnd(AKAO_TRACK *track, AKAO_PLAYER *player, u_int mask)
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
    offset |= *track->addr++ << 8;
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

void AkaoCmdSetDecayRateSustainLevel(AKAO_TRACK *track, AKAO_PLAYER *player, u_int mask)
{
    AkaoCmdSetDecayRate(track, player, mask);
    AkaoCmdSetSustainLevel(track, player, mask);
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

    AkaoCmdSetNoiseOn(track, player, mask);
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

    AkaoCmdSetFreqModulateOn(track, player, mask);
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
    track->effect_flags &= ~AKAO_EFFECT_ALL;
    AkaoCmdSetNoiseOff(track, player, mask);
    AkaoCmdSetFreqModulateOff(track, player, mask);
    AkaoCmdSetReverbOff(track, player, mask);
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
    offset |= *track->addr++ << 8;
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
        offset |= *track->addr++ << 8;

        track->addr += offset;
        player->last_condition = cond;
    }
    else
    {
        track->addr += 2;
    }
}

/* TODO: NOT MATCHING! */
void AkaoCmdFinishChannel(AKAO_TRACK *track, AKAO_PLAYER *player, u_int mask)
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

void AkaoCmdUnused(AKAO_TRACK *track, AKAO_PLAYER *player, u_int mask)
{
    /* unused command, so just finish the channel */
    AkaoCmdFinishChannel(track, player, mask);
}
