#include "ef_effect_control.h"

static void eSwing_Rod_init(xyz_t pos, int prio, s16 angle, GAME* game, u16 item_name, s16 arg0, s16 arg1) {
    // TODO
}

static void eSwing_Rod_ct(eEC_Effect_c* effect, GAME* game, void* ct_arg) {
    // TODO
}

static void eSwing_Rod_mv(eEC_Effect_c* effect, GAME* game) {
    // TODO
}

static void eSwing_Rod_dw(eEC_Effect_c* effect, GAME* game) {
    // TODO
}

eEC_PROFILE_c iam_ef_swing_rod = {
    // clang-format off
    &eSwing_Rod_init,
    &eSwing_Rod_ct,
    &eSwing_Rod_mv,
    &eSwing_Rod_dw,
    eEC_IGNORE_DEATH,
    eEC_NO_CHILD_ID,
    eEC_DEFAULT_DEATH_DIST,
    // clang-format on
};
