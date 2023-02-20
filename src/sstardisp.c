#include "string.h"
#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"

#include "mi_sys.h"
#include "sstardisp.h"

#include "mi_panel_datatype.h"
#include "mi_panel.h"
#include "mi_disp_datatype.h"
#include "mi_disp.h"
#include "appconfig.h"


#if defined(__cplusplus)||defined(c_plusplus)
extern "C"{
#endif
#if 0
int sstar_disp_init(MI_DISP_PubAttr_t *pstDispPubAttr)
{
    MI_PANEL_LinkType_e eLinkType;
    MI_DISP_InputPortAttr_t stInputPortAttr;
    MI_DISP_VideoLayerAttr_t stLayerAttr;

    memset(&stInputPortAttr, 0, sizeof(stInputPortAttr));

    MI_SYS_Init();
	
    stInputPortAttr.u16SrcWidth = GetPanelWidth();;
    stInputPortAttr.u16SrcHeight = GetPanelHeigth();
    stInputPortAttr.stDispWin.u16X = 0;
    stInputPortAttr.stDispWin.u16Y = 0;
    stInputPortAttr.stDispWin.u16Width = GetPanelWidth();
    stInputPortAttr.stDispWin.u16Height = GetPanelHeigth();

    MI_DISP_BindVideoLayer(0, 0);
	
	memset(&stLayerAttr, 0, sizeof(stLayerAttr));
    stLayerAttr.stVidLayerSize.u16Width  = GetPanelWidth();
    stLayerAttr.stVidLayerSize.u16Height = GetPanelHeigth();
    stLayerAttr.ePixFormat = E_MI_SYS_PIXEL_FRAME_YUV_MST_420;
    stLayerAttr.stVidLayerDispWin.u16X      = 0;
    stLayerAttr.stVidLayerDispWin.u16Y      = 0;
    stLayerAttr.stVidLayerDispWin.u16Width  = GetPanelWidth();
    stLayerAttr.stVidLayerDispWin.u16Height = GetPanelHeigth();
    MI_DISP_SetVideoLayerAttr(0, &stLayerAttr);
    MI_DISP_EnableVideoLayer(0);

    MI_DISP_SetInputPortAttr(0, 0, &stInputPortAttr);
    MI_DISP_EnableInputPort(0, 0);
    MI_DISP_SetInputPortSyncMode(0, 0, E_MI_DISP_SYNC_MODE_FREE_RUN);	

    return 0;
}
int sstar_disp_Deinit(MI_DISP_PubAttr_t *pstDispPubAttr)
{

    MI_DISP_DisableInputPort(0, 0);
    MI_DISP_DisableVideoLayer(0);
    MI_DISP_UnBindVideoLayer(0, 0);
    //MI_DISP_Disable(0);

    MI_SYS_Exit();
    printf("sstar_disp_Deinit...\n");

    return 0;
}
#endif
#if defined(__cplusplus)||defined(c_plusplus)
}
#endif

