#include "ef_effect_control.h"

static void eNebo_init(xyz_t pos, int prio, s16 angle, GAME* game, u16 item_name, s16 arg0, s16 arg1);
static void eNebo_ct(eEC_Effect_c* effect, GAME* game, void* ct_arg);
static void eNebo_mv(eEC_Effect_c* effect, GAME* game);
static void eNebo_dw(eEC_Effect_c* effect, GAME* game);

eEC_PROFILE_c iam_ef_neboke = {
    // clang-format off
    &eNebo_init,
    &eNebo_ct,
    &eNebo_mv,
    &eNebo_dw,
    eEC_IMMEDIATE_DEATH,
    eEC_NO_CHILD_ID,
    eEC_DEFAULT_DEATH_DIST,
    // clang-format on
};

static void eNebo_init(xyz_t pos, int prio, s16 angle, GAME* game, u16 item_name, s16 arg0, s16 arg1) {
    // TODO
}

static void eNebo_ct(eEC_Effect_c* effect, GAME* game, void* ct_arg) {
    // TODO
}

static void eNebo_mv(eEC_Effect_c* effect, GAME* game) {
    // TODO
}

static void eNebo_dw(eEC_Effect_c* effect, GAME* game) {
    // TODO
}
