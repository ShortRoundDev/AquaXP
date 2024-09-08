#ifndef __SLOTS_H
#define __SLOTS_H

#ifndef __cplusplus

#define BUF(a) b##a
#define TEXREG(a) t##a

#else

#define BUF(a) (a)
#define TEXREG(a) (a)

#endif

#define BUF_VIEW BUF(0)
#define BUF_MODEL BUF(1)
#define BUF_MAT_PROPS BUF(2)
#define BUF_PIXEL BUF(3)
#define BUF_ANIM BUF(4)
#define BUF_CUSTOM BUF(4)


#define TEX_ALBEDO TEXREG(0)
#define TEX_MET_ROUGH TEXREG(1)
#define TEX_SKYBOX TEXREG(2)
#define TEX_RAD_CUBE TEXREG(3)
#define TEX_IRRAD_CUBE TEXREG(4)

#endif