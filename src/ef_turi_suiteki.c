#include "ef_effect_control.h"

static void eTS_init(xyz_t pos, int prio, s16 angle, GAME* game, u16 item_name, s16 arg0, s16 arg1);
static void eTS_ct(eEC_Effect_c* effect, GAME* game, void* ct_arg);
static void eTS_mv(eEC_Effect_c* effect, GAME* game);
static void eTS_dw(eEC_Effect_c* effect, GAME* game);

eEC_PROFILE_c iam_ef_turi_suiteki = {
    // clang-format off
    &eTS_init,
    &eTS_ct,
    &eTS_mv,
    &eTS_dw,
    eEC_IGNORE_DEATH,
    eEC_NO_CHILD_ID,
    eEC_DEFAULT_DEATH_DIST,
    // clang-format on
};

static void eTS_init(xyz_t pos, int prio, s16 angle, GAME* game, u16 item_name, s16 arg0, s16 arg1) {
    // TODO
}

static void eTS_ct(eEC_Effect_c* effect, GAME* game, void* ct_arg) {
    // TODO
}

static void eTS_mv(eEC_Effect_c* effect, GAME* game) {
    // TODO
}

static void eTS_dw(eEC_Effect_c* effect, GAME* game) {
    // TODO
}
