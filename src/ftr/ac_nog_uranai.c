static void fNU_ct(FTR_ACTOR* ftr_actor, u8* data);
static void fNU_mv(FTR_ACTOR* ftr_actor, ACTOR* my_room_actor, GAME* game, u8* data);
static void fNU_dw(FTR_ACTOR* ftr_actor, ACTOR* my_room_actor, GAME* game, u8* data);
static void fNU_dt(FTR_ACTOR* ftr_actor, u8* data);

static aFTR_vtable_c fNU_func = {
	&fNU_ct,
	&fNU_mv,
	&fNU_dw,
	&fNU_dt,
	NULL,
};

aFTR_PROFILE iam_nog_uranai = {
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	15.7f,
	0.01f,
	aFTR_SHAPE_TYPEA,
	mCoBG_FTR_TYPEA,
	0,
	0,
	0,
	0,
	&fNU_func,
};