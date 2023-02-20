#ifndef __SSTARDISP__H__
#define __SSTARDISP__H__

#ifdef __cplusplus
extern "C"{
#endif // __cplusplus

#include <mi_disp_datatype.h>

#define MAKE_YUYV_VALUE(y,u,v) ((y) << 24) | ((u) << 16) | ((y) << 8) | (v)
#define YUYV_BLACK MAKE_YUYV_VALUE(0,128,128)


//int sstar_disp_init(MI_DISP_PubAttr_t* pstDispPubAttr);
//int sstar_disp_Deinit(MI_DISP_PubAttr_t *pstDispPubAttr);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif //__SSTARDISP__H__
