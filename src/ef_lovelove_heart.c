#include "ef_effect_control.h"

static void eLoveH_init(xyz_t pos, int prio, s16 angle, GAME* game, u16 item_name, s16 arg0, s16 arg1) {
    // TODO
}

static void eLoveH_ct(eEC_Effect_c* effect, GAME* game, void* ct_arg) {
    // TODO
}

static void eLoveH_mv(eEC_Effect_c* effect, GAME* game) {
    // TODO
}

static void eLoveH_dw(eEC_Effect_c* effect, GAME* game) {
    // TODO
}

eEC_PROFILE_c iam_ef_lovelove_heart = {
    // clang-format off
    &eLoveH_init,
    &eLoveH_ct,
    &eLoveH_mv,
    &eLoveH_dw,
    eEC_IMMEDIATE_DEATH,
    eEC_NO_CHILD_ID,
    eEC_DEFAULT_DEATH_DIST,
    // clang-format on
};
