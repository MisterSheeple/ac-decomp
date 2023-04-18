#include "m_font.h"

#include "audio.h"
#include "m_lib.h"
#include "gbi_extensions.h"
#include "dataobject.h"
#include "game.h"
#include "graph.h"
#include "types.h"
#include "libultra/gu.h"
#include "sys_matrix.h"
#include "MSL_C/w_math.h"
#include "m_rcp.h"
#include "libc/math.h"

#define mFont_CC_FONT   0, 0, 0, PRIMITIVE, PRIMITIVE, 0, TEXEL0, 0
#define mFont_CC_NOFONT 0, 0, 0, PRIMITIVE, 0, PRIMITIVE, TEXEL0, PRIMITIVE

/* @unused static u8* mFont_Get_end_load_texture() */

extern void mFont_ct() {
  // code removed probably
}

static int mMsg_CutLeftSpace(u8* str, int str_len) {
  // This should've been renamed or moved to m_message.h
  
  u8* dst;
  u8* src;
  int i, j;

  for (i = 0; i < str_len; i++) {
    if (str[i] != CHAR_SPACE) {
      break;
    }
  }

  if (i == str_len) {
    return 0;
  }
  else if (i == 0) {
    return str_len;
  }

  dst = str;
  src = str + i;
  for (j = i; j < str_len; j++) {
    *dst++ = *src++;
  }

  for (j = 0; j < i; j++) {
    *dst++ = CHAR_SPACE;
  }

  return str_len - i;
}

static u8 mFont_suji_data[] = "0123456789";

static int mFont_suji_check(u8 c) {
  u8* suji = mFont_suji_data;
  int i;

  for (i = 0; i < 10; i++) {
    if (c == suji[i]) {
      return TRUE;
    }
  }

  return FALSE;
}

extern int mFont_UnintToString(
  u8* str,
  int figure,
  u32 num,
  int figure_start,
  int left_cut,
  int fill_zero,
  int separator
) {
  int i;
  int j;
  u32 cur_num;
  int next;
    
  int temp2;
  int last_figure;

  int l = figure - figure_start;
  u8 fill_char = fill_zero ? CHAR_ZERO : CHAR_SPACE ;
  u8 left_fill_char = fill_zero && !left_cut ? CHAR_ZERO : CHAR_SPACE;

  cur_num = num;
  for (last_figure = figure - 1; last_figure >= l; last_figure--) {
    next = cur_num / 10;
    temp2 = cur_num - next * 10;
    if (cur_num == 0) {   
      if (num == 0 && last_figure == figure - 1) {
        str[last_figure] = CHAR_ZERO;
      }
      else {
        str[last_figure] = fill_char;
      }
    }
    else {
      str[last_figure] = mFont_suji_data[temp2];
    }
    cur_num = next;
  }

  for (i = last_figure; i >= 0; i--) {
    str[i] = left_fill_char;
  }

  if (separator) {
    int count = 0;
    for (i = figure - 1; i >= 0; i--) {
      if (mFont_suji_check(str[i])) {
        count++;
        if (count > 3) {
          for (j = 0; j < i; j++) {
            str[j] = str[j + 1];
          }

          str[i] = CHAR_COMMA;
          count = 0;
        }
      }
    }
  }

  if (left_cut) {
    return mMsg_CutLeftSpace(str, figure);
  }

  return figure;
}

extern int mFont_char_save_data_check(u8 c) {
  return c == CHAR_CONTROL_CODE || c == CHAR_MESSAGE_TAG;
}

// TODO: Match
extern u8 mFont_small_to_capital(u8 small) {
  static const u8 tbl[56][2] = {
    { CHAR_a, CHAR_A },
    { CHAR_b, CHAR_B },
    { CHAR_c, CHAR_C },
    { CHAR_d, CHAR_D },
    { CHAR_e, CHAR_E },
    { CHAR_f, CHAR_F },
    { CHAR_g, CHAR_G },
    { CHAR_h, CHAR_H },
    { CHAR_i, CHAR_I },
    { CHAR_j, CHAR_J },
    { CHAR_k, CHAR_K },
    { CHAR_l, CHAR_L },
    { CHAR_m, CHAR_M },
    { CHAR_n, CHAR_N },
    { CHAR_o, CHAR_O },
    { CHAR_p, CHAR_P },
    { CHAR_q, CHAR_Q },
    { CHAR_r, CHAR_R },
    { CHAR_s, CHAR_S },
    { CHAR_t, CHAR_T },
    { CHAR_u, CHAR_U },
    { CHAR_v, CHAR_V },
    { CHAR_w, CHAR_W },
    { CHAR_x, CHAR_X },
    { CHAR_y, CHAR_Y },
    { CHAR_z, CHAR_Z },
    { CHAR_DIARESIS_a, CHAR_DIAERESIS_A },
    { CHAR_GRAVE_a, CHAR_GRAVE_A },
    { CHAR_ACUTE_a, CHAR_ACUTE_A },
    { CHAR_CIRCUMFLEX_a, CHAR_CIRCUMFLEX_A },
    { CHAR_TILDE_a, CHAR_TILDE_A },
    { CHAR_ANGSTROM_a, CHAR_ANGSTROM_A },
    { CHAR_LOWER_CEDILLA, CHAR_CEDILLA },
    { CHAR_GRAVE_e, CHAR_GRAVE_E },
    { CHAR_ACUTE_e, CHAR_ACUTE_E },
    { CHAR_CIRCUMFLEX_e, CHAR_CIRCUMFLEX_E },
    { CHAR_DIARESIS_e, CHAR_DIARESIS_E },
    { CHAR_GRAVE_i, CHAR_GRAVE_I },
    { CHAR_ACUTE_i, CHAR_ACUTE_I },
    { CHAR_CIRCUMFLEX_i, CHAR_CIRCUMFLEX_I },
    { CHAR_DIARESIS_i, CHAR_DIARESIS_I },
    { CHAR_LOWER_ETH, CHAR_ETH },
    { CHAR_TILDE_n, CHAR_TILDE_N },
    { CHAR_GRAVE_o, CHAR_GRAVE_O },
    { CHAR_ACUTE_o, CHAR_ACUTE_O },
    { CHAR_CIRCUMFLEX_o, CHAR_CIRCUMFLEX_O },
    { CHAR_TILDE_o, CHAR_TILDE_O },
    { CHAR_DIARESIS_o, CHAR_DIARESIS_O },
    { CHAR_oe, CHAR_OE },
    { CHAR_GRAVE_u, CHAR_GRAVE_U },
    { CHAR_ACUTE_u, CHAR_ACUTE_U },
    { CHAR_CIRCUMFLEX_u, CHAR_CIRCUMFLEX_U },
    { CHAR_DIARESIS_u, CHAR_DIARESIS_U },
    { CHAR_ACUTE_y, CHAR_ACUTE_Y },
    { CHAR_LOWER_THORN, CHAR_THORN },
    { CHAR_LOWER_ASH, CHAR_ASH }
  };

  int i;
  for (i = 0; i < 56; i++) {
    if (tbl[i][0] == small) {
      return tbl[i][1];
    }
  }

  return small;
}

static int mFont_Get_FontOffset(int c) {
  static u8 offset_data[256] = {
    6, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 5, 6, 6, 6,
    6, 4, 4, 3, 3, 3, 3, 3, 3, 5, 5, 5, 5, 5, 5, 5,
    7, 6, 5, 5, 5, 1, 4, 8, 6, 6, 1, 0, 8, 4, 8, 0,
    4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 8, 1, 4, 3, 4, 4,
    2, 3, 5, 4, 4, 5, 5, 4, 4, 6, 6, 5, 5, 2, 4, 4,
    5, 3, 5, 5, 5, 4, 4, 2, 5, 5, 6, 5, 0, 5, 5, 3,
    5, 5, 5, 5, 5, 6, 7, 5, 4, 7, 7, 6, 7, 3, 5, 5,
    5, 4, 6, 6, 7, 5, 6, 3, 6, 6, 7, 5, 5, 5, 5, 0,
    0, 6, 6, 6, 6, 8, 4, 5, 5, 5, 5, 5, 5, 3, 5, 5,
    0, 5, 5, 5, 5, 5, 5, 6, 6, 5, 5, 4, 5, 6, 6, 6,
    3, 4, 2, 3, 5, 2, 2, 0, 0, 0, 0, 0, 5, 5, 5, 0,
    1, 1, 1, 0, 3, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0,
    4, 4, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 4,
    8, 3, 8, 6, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2
  };

  return offset_data[c];
}

static u8* mFont_Get_FontTex_p() {
  return FONT_nes_tex_font1;
}

extern int mFont_GetCodeWidth(int c, int cut) {
  return mFont_TEX_CHAR_WIDTH - (cut ? mFont_Get_FontOffset(c) : 0);
}

extern int mFont_GetStringWidth(u8* str, int len, int cut) {
  int width = 0;
  int i;

  for (i = 0; i < len; i++) {
    width += mFont_GetCodeWidth(str[i], cut);
  }

  return width;
}

extern int mFont_GetStringWidth2(u8* str, int len, int cut) {
  int width = 0;
  int i;
  int skip_size;

  for (i = 0; i < len; i += skip_size) {
    u8 c = str[i];
    if (c != CHAR_CONTROL_CODE) {
      width += mFont_GetCodeWidth(c, cut);
    }

    skip_size = mFont_CodeSize_get(str + i);
  }

  return width;
}

typedef struct control_code_s {
  s8 size;
  s8 attribute;
} mFont_ControlCodeInfo_t;

static mFont_ControlCodeInfo_t mFont_cont_info_tbl[mFont_CONT_CODE_NUM] = {
  { 2, mFont_CONT_ATTRIBUTE_0 },
  { 2, mFont_CONT_ATTRIBUTE_0 },
  { 2, mFont_CONT_ATTRIBUTE_0 },
  { 3, mFont_CONT_ATTRIBUTE_0 },
  { 2, mFont_CONT_ATTRIBUTE_0 },
  { 5, mFont_CONT_ATTRIBUTE_0 },
  { 2, mFont_CONT_ATTRIBUTE_0 },
  { 2, mFont_CONT_ATTRIBUTE_0 },
  { 5, mFont_CONT_ATTRIBUTE_DEMO },
  { 5, mFont_CONT_ATTRIBUTE_DEMO },
  { 5, mFont_CONT_ATTRIBUTE_DEMO },
  { 5, mFont_CONT_ATTRIBUTE_DEMO },
  { 5, mFont_CONT_ATTRIBUTE_DEMO },
  { 2, mFont_CONT_ATTRIBUTE_0 },
  { 4, mFont_CONT_ATTRIBUTE_0 },
  { 4, mFont_CONT_ATTRIBUTE_0 },
  { 4, mFont_CONT_ATTRIBUTE_0 },
  { 4, mFont_CONT_ATTRIBUTE_0 },
  { 4, mFont_CONT_ATTRIBUTE_0 },
  { 6, mFont_CONT_ATTRIBUTE_0 },
  { 8, mFont_CONT_ATTRIBUTE_0 },
  { 10, mFont_CONT_ATTRIBUTE_0 },
  { 6, mFont_CONT_ATTRIBUTE_0 },
  { 8, mFont_CONT_ATTRIBUTE_0 },
  { 10, mFont_CONT_ATTRIBUTE_0 },
  { 2, mFont_CONT_ATTRIBUTE_0 },
  { 2, mFont_CONT_ATTRIBUTE_STRING },
  { 2, mFont_CONT_ATTRIBUTE_STRING },
  { 2, mFont_CONT_ATTRIBUTE_STRING },
  { 2, mFont_CONT_ATTRIBUTE_STRING },
  { 2, mFont_CONT_ATTRIBUTE_STRING },
  { 2, mFont_CONT_ATTRIBUTE_STRING },
  { 2, mFont_CONT_ATTRIBUTE_STRING },
  { 2, mFont_CONT_ATTRIBUTE_STRING },
  { 2, mFont_CONT_ATTRIBUTE_STRING },
  { 2, mFont_CONT_ATTRIBUTE_STRING },
  { 2, mFont_CONT_ATTRIBUTE_STRING },
  { 2, mFont_CONT_ATTRIBUTE_STRING },
  { 2, mFont_CONT_ATTRIBUTE_STRING },
  { 2, mFont_CONT_ATTRIBUTE_STRING },
  { 2, mFont_CONT_ATTRIBUTE_STRING },
  { 2, mFont_CONT_ATTRIBUTE_STRING },
  { 2, mFont_CONT_ATTRIBUTE_STRING },
  { 2, mFont_CONT_ATTRIBUTE_STRING },
  { 2, mFont_CONT_ATTRIBUTE_STRING },
  { 2, mFont_CONT_ATTRIBUTE_STRING },
  { 2, mFont_CONT_ATTRIBUTE_STRING },
  { 2, mFont_CONT_ATTRIBUTE_STRING },
  { 2, mFont_CONT_ATTRIBUTE_STRING },
  { 2, mFont_CONT_ATTRIBUTE_STRING },
  { 2, mFont_CONT_ATTRIBUTE_STRING },
  { 2, mFont_CONT_ATTRIBUTE_STRING },
  { 2, mFont_CONT_ATTRIBUTE_STRING },
  { 2, mFont_CONT_ATTRIBUTE_STRING },
  { 2, mFont_CONT_ATTRIBUTE_STRING },
  { 2, mFont_CONT_ATTRIBUTE_STRING },
  { 2, mFont_CONT_ATTRIBUTE_STRING },
  { 2, mFont_CONT_ATTRIBUTE_STRING },
  { 2, mFont_CONT_ATTRIBUTE_STRING },
  { 2, mFont_CONT_ATTRIBUTE_STRING },
  { 2, mFont_CONT_ATTRIBUTE_STRING },
  { 2, mFont_CONT_ATTRIBUTE_STRING },
  { 2, mFont_CONT_ATTRIBUTE_STRING },
  { 2, mFont_CONT_ATTRIBUTE_STRING },
  { 2, mFont_CONT_ATTRIBUTE_STRING },
  { 2, mFont_CONT_ATTRIBUTE_3 },
  { 2, mFont_CONT_ATTRIBUTE_3 },
  { 2, mFont_CONT_ATTRIBUTE_3 },
  { 2, mFont_CONT_ATTRIBUTE_3 },
  { 2, mFont_CONT_ATTRIBUTE_3 },
  { 2, mFont_CONT_ATTRIBUTE_3 },
  { 2, mFont_CONT_ATTRIBUTE_3 },
  { 2, mFont_CONT_ATTRIBUTE_3 },
  { 2, mFont_CONT_ATTRIBUTE_3 },
  { 2, mFont_CONT_ATTRIBUTE_3 },
  { 2, mFont_CONT_ATTRIBUTE_3 },
  { 2, mFont_CONT_ATTRIBUTE_3 },
  { 2, mFont_CONT_ATTRIBUTE_3 },
  { 2, mFont_CONT_ATTRIBUTE_3 },
  { 2, mFont_CONT_ATTRIBUTE_3 },
  { 6, mFont_CONT_ATTRIBUTE_CHARACTER },
  { 3, mFont_CONT_ATTRIBUTE_0 },
  { 3, mFont_CONT_ATTRIBUTE_SENTENCE },
  { 3, mFont_CONT_ATTRIBUTE_SENTENCE },
  { 3, mFont_CONT_ATTRIBUTE_CHARACTER },
  { 2, mFont_CONT_ATTRIBUTE_0 },
  { 4, mFont_CONT_ATTRIBUTE_BGM },
  { 4, mFont_CONT_ATTRIBUTE_BGM },
  { 3, mFont_CONT_ATTRIBUTE_0 },
  { 3, mFont_CONT_ATTRIBUTE_SE },
  { 3, mFont_CONT_ATTRIBUTE_SENTENCE },
  { 2, mFont_CONT_ATTRIBUTE_SE },
  { 2, mFont_CONT_ATTRIBUTE_3 },
  { 2, mFont_CONT_ATTRIBUTE_3 },
  { 2, mFont_CONT_ATTRIBUTE_0 },
  { 2, mFont_CONT_ATTRIBUTE_0 },
  { 2, mFont_CONT_ATTRIBUTE_0 },
  { 2, mFont_CONT_ATTRIBUTE_3 },
  { 2, mFont_CONT_ATTRIBUTE_0 },
  { 6, mFont_CONT_ATTRIBUTE_0 },
  { 3, mFont_CONT_ATTRIBUTE_AGB_DUMMY },
  { 3, mFont_CONT_ATTRIBUTE_AGB_DUMMY },
  { 4, mFont_CONT_ATTRIBUTE_AGB_DUMMY },
  { 3, mFont_CONT_ATTRIBUTE_SENTENCE },
  { 2, mFont_CONT_ATTRIBUTE_AGB_DUMMY },
  { 2, mFont_CONT_ATTRIBUTE_AGB_DUMMY },
  { 6, mFont_CONT_ATTRIBUTE_0 },
  { 2, mFont_CONT_ATTRIBUTE_AGB_DUMMY },
  { 2, mFont_CONT_ATTRIBUTE_AGB_DUMMY },
  { 3, mFont_CONT_ATTRIBUTE_AGB_DUMMY },
  { 3, mFont_CONT_ATTRIBUTE_AGB_DUMMY },
  { 3, mFont_CONT_ATTRIBUTE_AGB_DUMMY },
  { 3, mFont_CONT_ATTRIBUTE_AGB_DUMMY },
  { 2, mFont_CONT_ATTRIBUTE_STRING },
  { 2, mFont_CONT_ATTRIBUTE_0 },
  { 2, mFont_CONT_ATTRIBUTE_0 },
  { 2, mFont_CONT_ATTRIBUTE_0 },
  { 2, mFont_CONT_ATTRIBUTE_0 },
  { 2, mFont_CONT_ATTRIBUTE_STRING },
  { 4, mFont_CONT_ATTRIBUTE_0 },
  { 4, mFont_CONT_ATTRIBUTE_0 },
  { 12, mFont_CONT_ATTRIBUTE_0 },
  { 14, mFont_CONT_ATTRIBUTE_0 }
};

extern int mFont_CodeSize_get(u8* str) {
  int size = 1;
  int c0 = *str;
  int c1;

  if (c0 == CHAR_CONTROL_CODE) {
    c1 = *(str + 1);
    if (c1 >= 0 && c1 < mFont_CONT_CODE_NUM) {
      size = mFont_cont_info_tbl[c1].size;
    }
    else {
      size = 2;
    }
  }
  else if (c0 == CHAR_MESSAGE_TAG) {
    size = 2;
  }

  return size;
}

extern int mFont_CodeSize_idx_get(u8* str, int idx) {
  return mFont_CodeSize_get(str + idx);
}

extern int mFont_cont_attr_get(int cont_code) {
  return mFont_cont_info_tbl[cont_code].attribute;
}

extern void mFont_gppSetMode(Gfx** gfx_pp) {
  static Gfx disp[] = {
    gsDPPipeSync(),
    gsSPTexture(0x8000, 0x8000, 0, G_TX_RENDERTILE, G_ON),
    gsSPClearGeometryMode(G_ZBUFFER | G_SHADE | G_CULL_BOTH | G_FOG | G_LIGHTING | G_TEXTURE_GEN | G_TEXTURE_GEN_LINEAR | G_LOD | G_SHADING_SMOOTH),
    gsDPSetTextureLOD(G_TL_TILE),
    gsDPSetTextureConvert(G_TC_FILT),
    gsDPSetTextureLUT(G_TT_NONE),
    gsDPSetTextureFilter(G_TF_BILERP),
    gsDPSetCycleType(G_CYC_1CYCLE),
    gsDPSetTexturePersp(G_TP_PERSP),
    gsDPSetCombineMode(mFont_CC_FONT, mFont_CC_FONT),
    gsDPSetRenderMode(G_RM_XLU_SURF, G_RM_XLU_SURF2),
    gsDPSetPrimColor(0, 0, 255, 255, 255, 255),
    gsSPEndDisplayList()
  };

  gSPDisplayList((*gfx_pp)++, disp);
}

extern void mFont_SetMode(GRAPH* graph, int mode) {
  Gfx* g;

  OPEN_DISP(graph);
  if (mode == 0) {
    g = NOW_POLY_OPA_DISP;
    mFont_gppSetMode(&g);
    SET_POLY_OPA_DISP(g);
  }
  else {
    g = NOW_FONT_DISP;
    mFont_gppSetMode(&g);
    SET_FONT_DISP(g);
  }
  CLOSE_DISP(graph);
}

static Gfx* mFont_gfxSetPrimColor(Gfx* gfx, int r, int g, int b, int a) {
  gDPSetPrimColor(gfx++, 0, 0, r, g, b, a);
  return gfx;
}

static Gfx* mFont_gppSetPrimColor(Gfx** gpp, rgba_t* color) {
  Gfx* g = *gpp;

  gDPSetPrimColor(
    g++,
    0, 0,
    color->r,
    color->g,
    color->b,
    color->a
  );

  *gpp = g;
}

extern void mFont_SetPrimColor(GRAPH* graph, int r, int g, int b, int a, int mode) {
  Gfx* gfx;
  
  OPEN_DISP(graph);

  if (mode == 0) {
    gfx = NOW_POLY_OPA_DISP;
    gfx = mFont_gfxSetPrimColor(gfx, r, g, b, a);
    SET_POLY_OPA_DISP(gfx);
  }
  else {
    gfx = NOW_FONT_DISP;
    gfx = mFont_gfxSetPrimColor(gfx, r, g, b, a);
    SET_FONT_DISP(gfx);
  }

  CLOSE_DISP(graph);
}

static Gfx* mFont_gfxSetCombineMode(Gfx* gfx, int no_font, int sync) {
  if (sync) {
    gDPPipeSync(gfx++);
  }

  if (no_font) {
    gDPSetCombineMode(gfx++, mFont_CC_FONT, mFont_CC_NOFONT);
  }
  else {
    gDPSetCombineMode(gfx++, mFont_CC_FONT, mFont_CC_FONT);
  }

  return gfx;
}

static void mFont_gppSetCombineMode(Gfx** gpp, int no_font, int sync) {
  Gfx* g = *gpp;

  if (sync) {
    gDPPipeSync(g++);
  }

  if (no_font) {
    gDPSetCombineMode(g++, mFont_CC_FONT, mFont_CC_NOFONT);
  }
  else {
    gDPSetCombineMode(g++, mFont_CC_FONT, mFont_CC_FONT);
  }

  *gpp = g;
}

extern void mFont_SetCombineMode(GRAPH* graph, int revert_flag, int mode) {
  Gfx* g;

  OPEN_DISP(graph);
  
  if (mode == 0) {
    g = NOW_POLY_OPA_DISP;
    g = mFont_gfxSetCombineMode(g, revert_flag, TRUE);
    SET_POLY_OPA_DISP(g);
  }
  else {
    g = NOW_FONT_DISP;
    g = mFont_gfxSetCombineMode(g, revert_flag, TRUE);
    SET_FONT_DISP(g);
  }

  CLOSE_DISP(graph);
}

static void mFont_gppLoadTexture(Gfx** gpp, int c, int* uls, int* ult, int* lrs, int* lrt) {
  Gfx* g = *gpp;
  u8* font_tex = mFont_Get_FontTex_p();
  int t = c >> 4;
  int n_ult = t << 4;
  int n_uls = (c - n_ult) * mFont_TEX_CHAR_WIDTH;
  int n_lrs = n_uls + (mFont_TEX_CHAR_WIDTH - 1);
  int n_lrt = n_ult + (mFont_TEX_CHAR_HEIGHT - 1);

  gDPLoadTextureTile_4b_Dolphin(
    g++,
    font_tex,
    G_IM_FMT_I,
    mFont_TEX_CHAR_WIDTH * 16, mFont_TEX_CHAR_HEIGHT * 16
  );


  *gpp = g;
  *uls = n_uls;
  *ult = n_ult;
  *lrs = n_lrs;
  *lrt = n_lrt;
}

static void mFont_SetVertex_dol(Vtx* vtx, int x, int y, int s, int t) {
  vtx->v.ob[0] = x;
  vtx->v.ob[1] = y;
  vtx->v.ob[2] = 0;
  
  vtx->v.flag = 1;
  
  vtx->v.tc[0] = s;
  vtx->v.tc[1] = t;

  vtx->v.cn[0] = 0;
  vtx->v.cn[1] = 0;
  vtx->v.cn[2] = 0;
  vtx->v.cn[3] = 0;
}

// TEMP
#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240

#define HALF_SCR_WIDTH ((f32)(SCREEN_WIDTH / 2))
#define HALF_SCR_HEIGHT ((f32)(SCREEN_HEIGHT / 2))
#define NHALF_SCR_WIDTH ((f32)(-SCREEN_WIDTH / 2))
#define NHALF_SCR_HEIGHT ((f32)(-SCREEN_HEIGHT / 2))

extern f32 mFont_SetVertexRectangle(
  GAME* game,
  f32 x, f32 y,
  int s, int t,
  f32 scale_x, f32 scale_y,
  int mode
) {
  f32 sS = ((f32)s) * scale_x;
  f32 sT = ((f32)t) * scale_y;

  f32 n = x + sS;

  f32 pos_x = x - HALF_SCR_WIDTH;
  f32 pos_y = -y + HALF_SCR_HEIGHT;

  f32 start_x = pos_x;
  f32 start_y = pos_y;
  f32 end_x = start_x + sS;
  f32 end_y = start_y - sT;

  f32 l_ofs = 0.0f;
  f32 r_ofs = 0.0f;
  f32 t_ofs = 0.0f;
  f32 b_ofs = 0.0f;

  if (start_x > HALF_SCR_WIDTH ||
      start_y < NHALF_SCR_HEIGHT ||
      end_x < NHALF_SCR_WIDTH ||
      end_y > HALF_SCR_HEIGHT)
  {
    return n;
  }

  {
    f32 dist_l = start_x - NHALF_SCR_WIDTH;
    f32 dist_t = start_y - HALF_SCR_HEIGHT;
    f32 dist_r = end_x - HALF_SCR_WIDTH;
    f32 dist_b = end_y - NHALF_SCR_HEIGHT;
  
    if (dist_l < 0.0f) {
      start_x = NHALF_SCR_WIDTH;
      l_ofs -= dist_l;
    }
  
    if (dist_t > 0.0f) {
      start_y = HALF_SCR_HEIGHT;
      t_ofs += dist_t;
    }
  
    if (dist_r > 0.0f) {
      end_x = HALF_SCR_WIDTH;
      r_ofs += dist_r;
    }
  
    if (dist_b < 0.0f) {
      end_y = NHALF_SCR_HEIGHT;
      b_ofs -= dist_b;
    }
  }

  {
    f32 inv_scale_x = 1.0f / scale_x;
    f32 inv_scale_y = 1.0f / scale_y;
  
    int vert_left_s = (int)(l_ofs * inv_scale_x) << 6;
    int vert_top_t = (int)(t_ofs * inv_scale_y) << 6;
    int vert_right_s = (int)(((f32)s) - r_ofs * inv_scale_x) << 6;
    int vert_bot_t = (int)(((f32)t) - b_ofs * inv_scale_y) << 6;
  
    int vert_left_x = (int)(start_x * mFont_SCALE_F);
    int vert_top_y = (int)(start_y * mFont_SCALE_F);
    int vert_right_x = (int)(end_x * mFont_SCALE_F);
    int vert_bot_y = (int)(end_y * mFont_SCALE_F);
  
    Vtx* vtx = GRAPH_ALLOC_TYPE(game->graph, Vtx, 4);
    Gfx* gfx = GRAPH_ALLOC_TYPE(game->graph, Gfx, 3);
  
    if (vtx != NULL && gfx != NULL) {
      mFont_SetVertex_dol(vtx++, vert_left_x, vert_top_y, vert_left_s, vert_top_t);
      mFont_SetVertex_dol(vtx++, vert_right_x, vert_top_y, vert_right_s, vert_top_t);
      mFont_SetVertex_dol(vtx++, vert_right_x, vert_bot_y, vert_right_s, vert_bot_t);
      mFont_SetVertex_dol(vtx, vert_left_x, vert_bot_y, vert_left_s, vert_bot_t);
  
      OPEN_DISP(game->graph);
  
      if (mode == 1) {
        gSPDisplayList(NOW_FONT_DISP++, gfx);
      }
      else {
        gSPDisplayList(NOW_POLY_OPA_DISP++, gfx);
      }
  
      CLOSE_DISP(game->graph);
  
      gSPVertex(gfx++, vtx - 3, 4, 0);
      gSPNTrianglesInit_5b(
        gfx++,
        2, // 2 triangles
        0, 2, 1, // 0 -> 2 -> 1
        0, 3, 2, // 0 -> 3 -> 2
        0, 0, 0  // unused
      );
      gSPEndDisplayList(gfx);
    }
  }

  return n;
}

static f32 mFont_SetLineStrings_AndSpace_new(
  GAME* game,
  u8* str,
  int len,
  f32 x, f32 y,
  int r, int g, int b, int a,
  int flag_revert, int flag_cut,
  int unused,
  f32 scale_x, f32 scale_y,
  int mode
) {
  Gfx* g;
  mFontSentence sentence;
  int use_polygon_opaque = mode == 0;
  u32 flag;
  xy_t pos;
  xy_t scale;
  rgba_t color;

  if (fabsf(scale_x) < 0.001f || fabsf(scale_y) < 0.001f) {
    return 0.0f;
  }

  if (len <= 0) {
    return 0.0f;
  }

  OPEN_DISP(game->graph);

  if (use_polygon_opaque) {
    g = NOW_POLY_OPA_DISP;
  }
  else {
    g = NOW_FONT_DISP;
  }

  flag = mFont_SENTENCE_FLAG_USE_POLY;
  if (flag_revert) {
    flag |= mFont_SENTENCE_FLAG_REVERT;
  }

  if (flag_cut) {
    flag |= mFont_SENTENCE_FLAG_CUT;
  }

  pos.x = x;
  pos.y = y;

  scale.x = scale_x;
  scale.y = scale_y;

  color.r = r;
  color.g = g;
  color.b = b;
  color.a = a;

  mFontSentence_set(&sentence, str, len, flag, &pos, mFont_LineType_Top, &scale, &color, 0, 0, 0, 0, 0);
  mFontSentence_gppDraw(&sentence, game, &g);

  if (use_polygon_opaque) {
    SET_POLY_OPA_DISP(g);
  }
  else {
    SET_FONT_DISP(g);
  }

  CLOSE_DISP(game->graph);

  return sentence.width;
}

extern f32 mFont_SetLineStrings_AndSpace(
  GAME* game,
  u8* str,
  int len,
  f32 x, f32 y,
  int r, int g, int b, int a,
  int flag_revert, int flag_cut,
  int unused,
  f32 scale_x, f32 scale_y,
  int mode
) {
  return mFont_SetLineStrings_AndSpace_new(
    game,
    str,
    len,
    x, y,
    r, g, b, a,
    flag_revert, flag_cut,
    unused,
    scale_x, scale_y,
    mode
  );
}

extern f32 mFont_SetLineStrings(
  GAME* game,
  u8* str,
  int len,
  f32 x, f32 y,
  int r, int g, int b, int a,
  int flag_revert, int flag_cut,
  f32 scale_x, f32 scale_y,
  int mode
) {
  return mFont_SetLineStrings_AndSpace(
    game,
    str,
    len,
    x, y,
    r, g, b, a,
    flag_revert, flag_cut,
    0,
    scale_x, scale_y,
    mode
  );
}

extern void mFont_CulcOrthoMatrix(Mtx* m) {
  guOrtho(
    m,
    NHALF_SCR_WIDTH * mFont_SCALE_F, HALF_SCR_WIDTH * mFont_SCALE_F,
    NHALF_SCR_HEIGHT * mFont_SCALE_F, HALF_SCR_HEIGHT * mFont_SCALE_F,
    -800.0f, 800.0f,
    1.0f
  );
}

extern void mFont_SetMatrix(GRAPH* graph, int mode) {
  static Mtx mtx_projection;
  static int first = TRUE;

  if (first) {
    mFont_CulcOrthoMatrix(&mtx_projection);
    first = FALSE;
  }

  Matrix_push();
  Matrix_put(&MtxF_clear);

  OPEN_DISP(graph);

  if (mode == 1) {
    gSPMatrix(NOW_FONT_DISP++, &mtx_projection, G_MTX_PROJECTION | G_MTX_LOAD | G_MTX_NOPUSH);
    gSPMatrix(NOW_FONT_DISP++, &Mtx_clear, G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);
  }
  else {
    gSPMatrix(NOW_POLY_OPA_DISP++, &mtx_projection, G_MTX_PROJECTION | G_MTX_LOAD | G_MTX_NOPUSH);
    gSPMatrix(NOW_POLY_OPA_DISP++, &Mtx_clear, G_MTX_MODELVIEW | G_MTX_LOAD | G_MTX_NOPUSH);
  }
  
  CLOSE_DISP(graph);
}

extern void mFont_UnSetMatrix(GRAPH* graph, int mode) {
  Matrix_pull();
}

extern void mFont_Main_start(GRAPH* graph) {
  mFont_SetMode(graph, 1);
}

/* @unused f32 mFont_Get_NextCharRectX(???) */

/* @unused f32 mFont_Get_NextCharRectY(???) */

static void mFont_gppDrawCharRect(
  GRAPH* graph,
  Gfx** gfx_pp,
  int c,
  u32 ulx, u32 uly,
  u32 lrx, u32 lry,
  int dsdx, int dtdy
) {
  Gfx* g = *gfx_pp;
  int uls;
  int ult;
  int lrs;
  int lrt;

  mFont_gppLoadTexture(&g, c, &uls, &ult, &lrs, &lrt);

  // Convert to signed 10.5 format
  uls <<= 5;
  ult <<= 5;

  g = gfx_gSPTextureRectangle1(g, ulx, uly, lrx, lry, G_TX_RENDERTILE, (int)uls, (int)ult, dsdx, dtdy);
  *gfx_pp = g;
}

// TODO: Non-matching
static void mFont_gppDrawCharPoly(
  GRAPH* graph,
  Gfx** gfx_pp,
  int c,
  xy_t* pos_tl, xy_t* pos_br,
  int s, int t
) {
  Gfx* g = *gfx_pp;
  Vtx* vtx = GRAPH_ALLOC_TYPE(graph, Vtx, 4);
  int uls, ult, lrs, lrt;
  int ulx, uly, lrx, lry;
  xy_t vert_tl, vert_br;
  f32 inv_half_s, inv_half_t;

  mFont_gppLoadTexture(&g, c, &uls, &ult, &lrs, &lrt);
    
  vert_tl.x = pos_tl->x - ((f32)SCREEN_WIDTH * 0.5f);
  vert_tl.y = -(pos_tl->y - ((f32)SCREEN_HEIGHT * 0.5f));
  vert_br.x = pos_br->x - ((f32)SCREEN_WIDTH * 0.5f) - vert_tl.x;
  vert_br.y = -(pos_br->y - ((f32)SCREEN_HEIGHT * 0.5f)) - vert_tl.y;
    
  inv_half_s = 0.5f / (f32)s;
  inv_half_t = 0.5f / (f32)t;
    
  ulx = (vert_tl.x + vert_br.x * inv_half_s) * mFont_SCALE_F;
  uly = (vert_tl.y + vert_br.y * inv_half_t) * mFont_SCALE_F;
  lrx = (vert_tl.x + vert_br.x * (1.0f - inv_half_s)) * mFont_SCALE_F;
  lry = (vert_tl.y + vert_br.y * (1.0f - inv_half_t)) * mFont_SCALE_F;

  lrs = ((uls + s) << 6) - 32;
  lrt = ((ult + t) << 6) - 32;
  uls = (uls << 6) + 32;
  ult = (ult << 6) + 32;

  mFont_SetVertex_dol(vtx + 0, ulx, uly, uls, ult);
  mFont_SetVertex_dol(vtx + 1, ulx, lry, uls, lrt);
  mFont_SetVertex_dol(vtx + 2, lrx, lry, lrs, lrt);
  mFont_SetVertex_dol(vtx + 3, lrx, uly, lrs, ult);

  gSPVertex(g++, vtx, 4, 0);
  gSPNTrianglesInit_5b(
    g++,
    2, // 2 tris
    0, 1, 2, // tri 1
    0, 2, 3, // tri 2
    0, 0, 0  // unused
  );

  *gfx_pp = g;
}

static void mFontChar_gppDraw_sentence_restore(mFontChar* this, mFontSentence* sentence, Gfx** gfx_pp) {
  if (this->color_ctr > 0) {
    this->color_ctr--;
    if (this->color_ctr == 0) {
      rgba_t_move(&this->color, &sentence->color);
      mFont_gppSetPrimColor(gfx_pp, &this->color);
    }
  }

  if ((this->flags & mFont_CHAR_FLAG_SCALE)) {
    this->scale.x = 1.0f;
    this->scale.y = 1.0f;
    this->inv_scale.x = 1.0f;
    this->inv_scale.y = 1.0f;

    this->flags &= ~mFont_CHAR_FLAG_SCALE;
    this->flags |= mFont_CHAR_FLAG_SCALE_RESET;
  }
}

static void mFontChar_total_scale_reset(mFontChar* this, mFontSentence* sentence) {
  if ((this->flags & mFont_CHAR_FLAG_SCALE_RESET)) {
    this->scaled_size.x = this->scale.x * sentence->scale.x * sentence->line_scale;
    this->scaled_size.y = this->scale.y * sentence->scale.y * sentence->line_scale;
    this->inv_scaled_size.x = this->inv_scale.x * sentence->inv_scale.x * sentence->inv_line_scale;
    this->inv_scaled_size.y = this->inv_scale.y * sentence->inv_scale.y * sentence->inv_line_scale;

    this->flags &= ~mFont_CHAR_FLAG_SCALE_RESET;
  }
}

static void mFontChar_ContProc_set_color_char(mFontChar* this, Gfx** gfx_pp) {
  this->color.r = this->char_p[2];
  this->color.g = this->char_p[3];
  this->color.b = this->char_p[4];
  this->color.a = 255;
  
  this->color_ctr = this->char_p[5];

  mFont_gppSetPrimColor(gfx_pp, &this->color);
}

static void mFontChar_ContProc_set_char_scale(mFontChar* this, Gfx** gfx_pp) {
  int scale_val = this->char_p[2];
  f32 scale = (f32)scale_val * (1.0f / 32.0f);
  f32 inv_scale = 1.0f / scale;

  this->scale.x = scale;
  this->scale.y = scale;
  this->inv_scale.x = inv_scale;
  this->inv_scale.y = inv_scale;

  this->flags |= (mFont_CHAR_FLAG_SCALE | mFont_CHAR_FLAG_SCALE_RESET);
}

static mFont_ControlCodeCharFunc mFontChar_cont_proc_get(int control_code) {
  if (control_code == mFont_CONT_CODE_SET_COLOR_CHAR) {
    return mFontChar_ContProc_set_color_char;
  }
  else if (control_code == mFont_CONT_CODE_SET_CHAR_SCALE) {
    return mFontChar_ContProc_set_char_scale;
  }
  else {
    return NULL;
  }
}

static void mFontChar_set(
  mFontChar* this,
  u8* char_p, int len,
  u8 flags,
  xy_t* pos,
  xy_t* scale, xy_t* inv_scale,
  f32 ofs_y,
  rgba_t* color
) {
  bzero(this, sizeof(mFontChar));

  this->char_p = char_p;
  this->len = len;
  this->flags = flags;
  this->position = *pos;
  this->scale = *scale;
  this->inv_scale = *inv_scale;
  this->ofs_y = ofs_y;

  rgba_t_move(&this->color, color);
}

static void mFontChar_gppDrawRect(mFontChar* this, GRAPH* graph, Gfx** gfx_pp) {
  u32 ulx, uly, lrx, lry;
  int dsdx, dtdy;
  f32 t;
  f32 width;

  t = -this->ofs_y;
  uly = (int)((t * this->scaled_size.y - t + this->position.y) * 4.0f);

  t = mFont_TEX_CHAR_HEIGHT_F32 - this->ofs_y;
  lry = (int)((t * this->scaled_size.y - t + this->position.y + mFont_TEX_CHAR_HEIGHT_F32) * 4.0f);

  ulx = (int)(this->position.x * 4.0f);

  width = (f32)mFont_GetCodeWidth(*this->char_p, this->flags & mFont_CHAR_FLAG_CUT) * this->scaled_size.x;

  lrx = (int)((width + this->position.x) * 4.0f);

  dsdx = (1024.0f * this->inv_scaled_size.x); /* 1 << 10 for correct s5.10 format? */
  dtdy = (1024.0f * this->inv_scaled_size.y); /* 1 << 10 for correct s5.10 format? */

  this->width = width;

  mFont_gppDrawCharRect(graph, gfx_pp, *this->char_p, ulx, uly, lrx, lry, dsdx, dtdy);
}

// TODO: match
static void mFontChar_gppDrawPoly(mFontChar* this, GRAPH* graph, Gfx** gfx_pp) {
  xy_t top_left;
  xy_t bottom_right;

  f32 t;
  f32 width;
  int s;

  int c = *this->char_p;

  t = -this->ofs_y;
  top_left.y = (t * this->scaled_size.y) - t + this->position.y;

  t = mFont_TEX_CHAR_HEIGHT_F32 - this->ofs_y;
  bottom_right.y = (t * this->scaled_size.y) - t + this->position.y + mFont_TEX_CHAR_HEIGHT_F32;

  top_left.x = this->position.x;

  s = mFont_GetCodeWidth(c, this->flags & mFont_CHAR_FLAG_CUT);

  width = s * this->scaled_size.x;
  bottom_right.x = width + this->position.x;

  this->width = width;

  mFont_gppDrawCharPoly(graph, gfx_pp, c, &top_left, &bottom_right, s, mFont_TEX_CHAR_HEIGHT);
}

static void mFontChar_gppDraw(mFontChar* this, GRAPH* graph, Gfx** gfx_pp) {
  if (this->flags & mFont_CHAR_FLAG_USE_POLY) {
    mFontChar_gppDrawPoly(this, graph, gfx_pp);
  }
  else {
    mFontChar_gppDrawRect(this, graph, gfx_pp);
  }
}

static f32 mFontSentence_line_offset_calc(int line_type) {
  static f32 offset_value[mFont_LineType_End] =  {
    mFont_TEX_CHAR_HEIGHT_F32 * 0.0f,
    mFont_TEX_CHAR_HEIGHT_F32 * 0.5f,
    mFont_TEX_CHAR_HEIGHT_F32 * 1.0f
  };

  return offset_value[line_type];
}

static void mFontSentence_ContProc_set_line_offset(mFontSentence* this, Gfx** gfx_pp) {
  mFontChar* character = &this->character;
  int t = (u8)(*(this->sentence_p + this->now_idx + 2)) - 128;
  this->offset = t;

  character->position.y = this->position.y + this->offset;
}

static void mFontSentence_ContProc_set_line_type(mFontSentence* this, Gfx** gfx_pp) {
  mFontChar* character = &this->character;
  int type = *(this->sentence_p + this->now_idx + 2);

  this->line_type = type;
  character->ofs_y = mFontSentence_line_offset_calc(this->line_type);
}

static void mFontSentence_ContProc_set_line_scale(mFontSentence* this, Gfx** gfx_pp) {
  mFontChar* character = &this->character;
  int scale = *(this->sentence_p + this->now_idx + 2);
  f32 f_scale = (f32)scale * (1.0f / 32.0f);
  f32 f_inv_scale  = 1.0f / f_scale;

  this->line_scale = f_scale;
  this->inv_line_scale = f_inv_scale;

  character->flags |= mFont_CHAR_FLAG_SCALE_RESET;
}

static void mFontSentence_ContProc_space(mFontSentence* this, Gfx** gfx_pp) {
  mFontChar* character = &this->character;
  int c = *(this->sentence_p + this->now_idx + 2);
  this->width += c * character->scaled_size.x;
}

static mFont_ControlCodeSentenceFunc mFontSentence_cont_proc_get(int control_code) {
  if (control_code == mFont_CONT_CODE_SET_LINE_OFFSET) {
    return mFontSentence_ContProc_set_line_offset;
  }
  else if (control_code == mFont_CONT_CODE_SET_LINE_TYPE) {
    return mFontSentence_ContProc_set_line_type;
  }
  else if (control_code == mFont_CONT_CODE_SET_LINE_SCALE) {
    return mFontSentence_ContProc_set_line_scale;
  }
  else if (control_code == mFont_CONT_CODE_SPACE) {
    return mFontSentence_ContProc_space;
  }
  else {
    return NULL;
  }
}

static void mFontSentence_animal_voice_se(mFontSentence* this) {
  mFontChar* character = &this->character;
  f32 f_scale;
  int scale;

  f_scale = character->scale.x * character->scale.x + character->scale.y * character->scale.y;
  f_scale = sqrtf(f_scale);
  
  scale = this->line_scale * f_scale * (32.0f / SQRT_OF_2_F) + 0.5f;
  scale = scale < 0 ? 0 : (scale > 255 ? 255 : scale);

  // TODO: these definitely deserve to be promoted to defines or enums in the future.
  // Unsure which sfx & case this is referring to.
  if (this->voice_idx == 0x80 && this->se_character_id != 0 && scale == 32 &&
      character->color.r == 50 && character->color.g == 60 && character->color.b == 50 && character->color.a == 255) {
    this->se_character_id = 0;
  }

  sAdo_VoiceSe(this->voice_idx, this->voice2_idx, this->voice3_idx, this->se_character_id, (u8)scale, 0);
}

extern void mFontSentence_set(
  mFontSentence* this,
  u8* sentence_p, int len,
  u32 flags,
  xy_t* pos,
  int line_type,
  xy_t* scale,
  rgba_t* color,
  int se_voice_pos,
  u8 voice_idx, u8 voice2_idx, u8 voice3_idx,
  s16 voice_se_character_idx
) {
  static xy_t unit_vec_xy = { 1.0f, 1.0f };
  u8 character_flags = 0;

  bzero(this, sizeof(mFontSentence));

  this->sentence_p = sentence_p;
  this->len = len;

  this->flags = flags;

  this->position = *pos;
  this->line_type = line_type;
  this->scale = *scale;
  this->inv_scale.x = 1.0f / scale->x;
  this->inv_scale.y = 1.0f / scale->y;
  this->line_scale = 1.0f;
  this->inv_line_scale = 1.0f;

  rgba_t_move(&this->color, color);

  this->se_pos = se_voice_pos;
  this->voice_idx = voice_idx;
  this->voice2_idx = voice2_idx;
  this->voice3_idx = voice3_idx;
  this->se_character_id = voice_se_character_idx;

  if (this->flags & mFont_SENTENCE_FLAG_CUT) {
    character_flags |= mFont_CHAR_FLAG_CUT;
  }

  if (this->flags & mFont_SENTENCE_FLAG_USE_POLY) {
    character_flags |= mFont_CHAR_FLAG_USE_POLY;
  }

  character_flags |= mFont_CHAR_FLAG_SCALE_RESET;

  mFontChar_set(
    &this->character,
    this->sentence_p,
    1,
    character_flags,
    &this->position,
    &unit_vec_xy,
    &unit_vec_xy,
    mFontSentence_line_offset_calc(this->line_type),
    &this->color
  );
}

static void mFontSentence_gppDraw_before(mFontSentence* this, Gfx** gfx_pp) {
  if (!(this->flags & mFont_SENTENCE_FLAG_3)) {
    if (this->flags & mFont_SENTENCE_FLAG_USE_POLY) {
      mFont_gppSetMode(gfx_pp);
    }
    else {
      mFont_gppSetRectMode(gfx_pp);
    }
  }

  if (!(this->flags & mFont_SENTENCE_FLAG_NO_COMBINE)) {
    mFont_gppSetCombineMode(gfx_pp, this->flags & mFont_SENTENCE_FLAG_REVERT, FALSE);
  }

  if (!(this->flags & mFont_SENTENCE_FLAG_NO_COLOR)) {
    mFont_gppSetPrimColor(gfx_pp, &this->color);
  }
}

static void mFontSentence_gppDraw_main(mFontSentence* this, GAME* game, Gfx** gfx_pp) {
  GRAPH* graph = game->graph;
  u8* char_p = this->sentence_p + this->now_idx;
  int c = *char_p;
  int char_size = mFont_CodeSize_get(char_p);

  this->character.char_p = char_p;
  this->character.len = char_size;

  if (c == CHAR_CONTROL_CODE) {
    int next = *(char_p + 1);
    int type = mFont_cont_attr_get(next);

    if (type == mFont_CONT_ATTRIBUTE_SENTENCE) {
      (mFontSentence_cont_proc_get(next))(this, gfx_pp);
    }
    else if (type == mFont_CONT_ATTRIBUTE_CHARACTER) {
      (mFontChar_cont_proc_get(next))(&this->character, gfx_pp);
    }
  }
  else {
    if (!((this->flags & mFont_SENTENCE_SKIP_DRAW_NEW_LINE) && c == CHAR_NEW_LINE)) {
      this->character.position.x = this->position.x + this->width;
      mFontChar_total_scale_reset(&this->character, this);
      mFontChar_gppDraw(&this->character, graph, gfx_pp);

      if ((this->flags & mFont_SENTENCE_FLAG_VOICE_SE) && this->now_idx == this->se_pos) {
        mFontSentence_animal_voice_se(this);
      }

      this->width += this->character.width;
      mFontChar_gppDraw_sentence_restore(&this->character, this, gfx_pp);
    }
  }

  this->now_idx += char_size;
}

static void mFontSentence_gppDraw_after(mFontSentence* this, Gfx** gfx_pp) {

}

extern void mFontSentence_gppDraw(mFontSentence* this, GAME* game, Gfx** gfx_pp) {
  mFontSentence_gppDraw_before(this, gfx_pp);

  while (this->now_idx < this->len) {
    mFontSentence_gppDraw_main(this, game, gfx_pp);
  }

  mFontSentence_gppDraw_after(this, gfx_pp);
}

static u8* mFont_Get_MarkTex_p(int mark_type) {
  static u8* tex_p_array[mFont_MARKTYPE_TOTAL] = {
    FONT_nes_tex_jyouge,
    FONT_nes_tex_sayuu,
    FONT_nes_tex_cursor,
    FONT_nes_tex_next,
    FONT_nes_tex_choice
  };

  if (0 <= mark_type && mark_type < mFont_MARKTYPE_TOTAL && tex_p_array[mark_type] != NULL) {
    return tex_p_array[mark_type];
  }

  return NULL;
}

static int mFont_Get_MarkTex_sizeW(int mark_type) {
  static int size_array[mFont_MARKTYPE_TOTAL] = { 16, 16, 16, 16, 16 };

  if (0 <= mark_type && mark_type < mFont_MARKTYPE_TOTAL && size_array[mark_type] != 0) {
    return size_array[mark_type];
  }

  return 0;
}

static int mFont_Get_MarkTex_sizeH(int mark_type) {
  static int size_array[mFont_MARKTYPE_TOTAL] = { 16, 16, 16, 16, 16 };

  if (0 <= mark_type && mark_type < mFont_MARKTYPE_TOTAL && size_array[mark_type] != 0) {
    return size_array[mark_type];
  }

  return 0;
}

// TODO: match
extern f32 mFont_SetMarkChar(
  GAME* game,
  u8 mark_type,
  f32 x, f32 y,
  int r, int g, int b, int a,
  int revert_flag,
  f32 scale_x, f32 scale_y,
  int mode
) {
  f32 next_x = x;

  if ((int)mark_type < mFont_MARKTYPE_TOTAL) {
    u8* tex = mFont_Get_MarkTex_p(mark_type);
    int w = mFont_Get_MarkTex_sizeW(mark_type);
    int h = mFont_Get_MarkTex_sizeH(mark_type);
    GRAPH* graph = game->graph;
    Gfx* gfx = GRAPH_ALLOC_TYPE(graph, Gfx, 9);

    if (tex != NULL && w != 0 && h != 0 && gfx != NULL) {
      mFont_SetMode(graph, mode);
      mFont_SetPrimColor(graph, r, g, b, a, mode);
      mFont_SetCombineMode(graph, revert_flag, mode);

      OPEN_DISP(graph);

      if (mode == 1) {
        gSPDisplayList(NOW_FONT_DISP++, gfx);
      }
      else {
        gSPDisplayList(NOW_POLY_OPA_DISP++, gfx);
      }

      CLOSE_DISP(graph);

      /* TODO: they seem to have a secondary macro that does the operations directly */
      gDPLoadTextureTile_4b_Dolphin(
        gfx++,
        tex,
        G_IM_FMT_I,
        w, h + mFont_TEX_CHAR_HEIGHT
      );

      gSPEndDisplayList(gfx);
      next_x = mFont_SetVertexRectangle(game, x, y, w, h, scale_x, scale_y, mode);
    }
  }

  return next_x;
}

extern void mFont_gppSetRectMode(Gfx** gfx_pp) {
  static Gfx disp[] = {
    gsDPPipeSync(),
    gsSPTexture(0x8000, 0x8000, 0, G_TX_RENDERTILE, G_ON),
    gsSPClearGeometryMode(G_ZBUFFER | G_SHADE | G_CULL_BOTH | G_FOG | G_LIGHTING | G_TEXTURE_GEN | G_TEXTURE_GEN_LINEAR | G_LOD | G_SHADING_SMOOTH),
    gsDPSetTextureLOD(G_TL_TILE),
    gsDPSetTextureConvert(G_TC_FILT),
    gsDPSetTextureLUT(G_TT_NONE),
    gsDPSetTextureFilter(G_TF_BILERP),
    gsDPSetCycleType(G_CYC_1CYCLE),
    gsDPSetTexturePersp(G_TP_NONE),
    gsDPSetCombineMode(mFont_CC_FONT, mFont_CC_FONT),
    gsDPSetRenderMode(G_RM_AA_DEC_LINE, G_RM_AA_DEC_LINE2),
    gsDPSetPrimColor(0, 0, 255, 255, 255, 255),
    gsSPEndDisplayList(),
  };

  gSPDisplayList((*gfx_pp)++, disp);
}
