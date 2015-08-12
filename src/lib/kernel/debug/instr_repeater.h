#define REP0 
#define REP1(X) X
#define REP2(X) REP1(X) REP1(X)
#define REP3(X) REP2(X) REP1(X)
#define REP4(X) REP3(X) REP1(X)
#define REP5(X) REP4(X) REP1(X)
#define REP6(X) REP5(X) REP1(X)
#define REP7(X) REP6(X) REP1(X)
#define REP8(X) REP7(X) REP1(X)
#define REP9(X) REP8(X) REP1(X)

#define REP00(X)
#define REP10(X)  REP9(X)  REP1(X)
#define REP20(X) REP10(X) REP10(X)
#define REP30(X) REP20(X) REP10(X)
#define REP40(X) REP30(X) REP10(X)
#define REP50(X) REP40(X) REP10(X)
#define REP60(X) REP50(X) REP10(X)
#define REP70(X) REP60(X) REP10(X)
#define REP80(X) REP70(X) REP10(X)
#define REP90(X) REP80(X) REP10(X)

#define REP000(X)
#define REP100(X)  REP90(X)  REP10(X)
#define REP200(X) REP100(X) REP100(X)
#define REP300(X) REP200(X) REP100(X)
#define REP400(X) REP300(X) REP100(X)
#define REP500(X) REP400(X) REP100(X)
#define REP600(X) REP500(X) REP100(X)
#define REP700(X) REP600(X) REP100(X)
#define REP800(X) REP700(X) REP100(X)
#define REP900(X) REP800(X) REP100(X)

#define REP(HUNDREDS,TENS,ONES,X) \
  REP##HUNDREDS##00(X) \
  REP##TENS##0(X) \
  REP##ONES(X)
