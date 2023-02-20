#include "lvgl/lvgl.h"
#include "sstar_fbdev.h"
#include <stdio.h>
#include <unistd.h>
#include "mainWindow.h"
#include "appconfig.h"



#if FANG_PING_1280x1024
	#define VERTICAL_SCREEN_WIDTH 	1024//竖屏
	#define VERTICAL_SCREEN_HEIGHT  1280

	#define HORIZONTAL_SCREEN_WIDTH  1280//横屏
	#define HORIZONTAL_SCREEN_HEIGHT 1024
#else
	#define VERTICAL_SCREEN_WIDTH 480//竖屏
	#define VERTICAL_SCREEN_HEIGHT 800

	#define HORIZONTAL_SCREEN_WIDTH 800//横屏
	#define HORIZONTAL_SCREEN_HEIGHT 480
#endif

#if FANG_PING_1280x1024_SPECIAL_DEMAND
	#define HORIZONTAL_SCREEN_HEIGHT_SPECIAL 720//保证方屏视频播放区域16：9
#endif


/**********************
 *      TYPEDEFS
 **********************/
typedef enum {
    DISP_SMALL,
    DISP_MEDIUM,
    DISP_LARGE,
}disp_size_t;





static lv_style_t style_scr_act;

static lv_obj_t *ptrMainWindow = NULL;
static lv_obj_t *ptrTvSig = NULL;
static lv_obj_t *ptrTvVersion = NULL;
static lv_obj_t *ptrPic = NULL;
static lv_obj_t *ptrQrCode = NULL;
static lv_obj_t *ptrVedio = NULL;

static uint32_t fb_width = 0;
static uint32_t fb_height = 0;



static layoutPositon_t TvSig_Pos;
static layoutPositon_t TvVer_Pos;
static layoutPositon_t Pic_Pos;
static layoutPositon_t QrCode_Pos;
static layoutPositon_t Vedio_Pos;
static lv_style_t qrcode_style;



/*
 *获取对象可见属性
 */
bool ui_get_obj_visible(lv_obj_t * obj)
{
	lv_opa_t pic_opa = 0;
	
	if(NULL == obj){
		printf("[%s]obj deos not exist\n", __FUNCTION__);
		return false;
	}
	pic_opa = lv_obj_get_style_opa(obj, 0);

	if(LV_OPA_TRANSP == pic_opa)	return false;
	else							return true;
}

/*
 *设置对象可见属性
 */
void ui_set_obj_visible(lv_obj_t *obj, bool isVisible)
{
	if(NULL == obj){
		printf("[%s]obj deos not exist\n", __FUNCTION__);
		return;
	}

	if(isVisible){
		lv_obj_set_style_opa(obj, LV_OPA_COVER, 0);
	}else{
		lv_obj_set_style_opa(obj, LV_OPA_TRANSP, 0);
	}	
}


uint16_t ui_get_zoom_factor(lv_obj_t *obj, const char *file)
{
	lv_coord_t obj_width;
	lv_coord_t obj_height;
	lv_img_header_t img_header;

	if(NULL == obj || NULL == file){
		printf("[%s]param is NULL\n", __FUNCTION__);
		return -1;
	}

	lv_obj_update_layout(obj);  /*Be sure the object's size is calculated*/
    obj_width = lv_obj_get_width(obj);
    obj_height = lv_obj_get_height(obj);
	
	//printf("[%s]width: %d, height: %d\n", __FUNCTION__, obj_width, obj_height);

	if(!obj_width || !obj_height){
		printf("[%s]obj size is zero\n",__FUNCTION__);
		return -1;
	}

	if(LV_RES_OK != lv_img_decoder_get_info(file, &img_header)){
		printf("[%s]get img header fail\n", __FUNCTION__);
		return -1;
	}

	if(!img_header.w || !img_header.h){
		printf("[%s]img size error\n", __FUNCTION__);
		return -1;
	}

	uint32_t y_a = obj_height * img_header.w;
	uint32_t x_b = obj_width * img_header.h;

	//printf("[%s]ow: %d, oh: %d, iw: %d, ih: %d\n", __FUNCTION__, obj_width, obj_height, img_header.w, img_header.h);

	if(x_b >= y_a){
		if(img_header.h >= obj_height){
			uint32_t x = obj_height * 256;
			return x / img_header.h;
		}
	}else{
		if(img_header.w > obj_width){
			uint32_t x = obj_width * 256;
			return x / img_header.w;
		}
	}

	printf("[%s]not valid factor\n",__FUNCTION__);
	return -1;
}


//pic widget functions
void ui_create_picWidget(void)
{
	if(NULL == ptrMainWindow){
		printf("create pic object fail\n");
		return;
	}

	ptrPic = lv_img_create(ptrMainWindow);

	lv_obj_set_size(ptrPic, Pic_Pos.mWidth, Pic_Pos.mHeight);
	lv_obj_set_pos(ptrPic, Pic_Pos.mX, Pic_Pos.mY);
}

/*
 *获取图片控件句柄
 */
lv_obj_t *ui_get_picWidget_handle(void)
{
	return ptrPic;
}

void ui_set_picWidget_pos(int x, int y)
{
	if(NULL == ptrPic){
		printf("pic object has not been created\n");
		return;
	}
	
	lv_obj_set_pos(ptrPic, x, y);
}

void ui_set_picWidget_size(int width, int height)
{
	if(NULL == ptrPic){
		printf("pic object has not been created\n");
		return;
	}

	lv_obj_set_size(ptrPic, width, height);
}

void ui_update_picWidget_content(const char *path)
{
	if(NULL == ptrPic){
		printf("pic object has not been created\n");
		return;
	}

	char fileName[256];
	memset(fileName, 0, sizeof(fileName));
	snprintf(fileName, sizeof(fileName), "A:%s", path);
	lv_img_set_src(ptrPic, fileName);
	//uint16_t factor = ui_get_zoom_factor(ptrPic, fileName);
	//printf("[%s]factor: %d\n", __FUNCTION__, factor);
	//if(-1 != factor)	lv_img_set_zoom(ptrPic, factor);	
}

void ui_set_picWidget_visible(bool isVisible)
{
	if(NULL == ptrPic){
		printf("pic object has not been created\n");
		return;
	}

	if(isVisible){
		lv_obj_set_style_opa(ptrPic, LV_OPA_COVER, 0);
	}else{
		lv_obj_set_style_opa(ptrPic, LV_OPA_TRANSP, 0);
	}
}

bool ui_get_picWidget_visible(void)
{
	return ui_get_obj_visible(ptrPic);
}


//tv signal widget functions
void ui_create_TvSigWidget(void)
{
	if(NULL == ptrMainWindow){
		printf("create tv sig object fail\n");
		return;
	}
printf("@@@@@@@@@@@@@@@@@ui_create_TvSigWidget\n");
	ptrTvSig = lv_img_create(ptrMainWindow);

	lv_obj_set_size(ptrTvSig, TvSig_Pos.mWidth, TvSig_Pos.mHeight);
	lv_obj_set_pos(ptrTvSig, TvSig_Pos.mX, TvSig_Pos.mY);
}

void ui_set_TvSigWidget_pos(int x, int y)
{
	if(NULL == ptrTvSig){
		printf("pic object has not been created\n");
		return;
	}

	lv_obj_set_pos(ptrTvSig, x, y);
}

void ui_set_TvSigWidget_size(int width, int height)
{
	if(NULL == ptrTvSig){
		printf("pic object has not been created\n");
		return;
	}
	
	lv_obj_set_size(ptrTvSig, width, height);
}

void ui_update_TvSigWidget_content(const char *path)
{
	if(NULL == ptrTvSig){
		printf("pic object has not been created\n");
		return;
	}

	char fileName[256];
	memset(fileName, 0, sizeof(fileName));
	snprintf(fileName, sizeof(fileName), "A:%s", path);
	lv_img_set_src(ptrTvSig, fileName);	
printf("@@@@@@@@@@@@@@@@@ui_update_TvSigWidget_content\n");
	uint16_t factor = ui_get_zoom_factor(ptrTvSig, fileName);
	//printf("[%s]factor: %d\n", __FUNCTION__, factor);
	if(-1 != factor)	lv_img_set_zoom(ptrTvSig, factor);	
}

void ui_set_TvSigWidget_visible(bool isVisible)
{
	if(NULL == ptrTvSig){
		printf("pic object has not been created\n");
		return;
	}

	if(isVisible){
		lv_obj_set_style_opa(ptrTvSig, LV_OPA_COVER, 0);
	}else{
		lv_obj_set_style_opa(ptrTvSig, LV_OPA_TRANSP, 0);
	}
}


//tv version widget functions
void ui_create_TvVersionWidget(void)
{
	if(NULL == ptrMainWindow){
		printf("create tv version object fail\n");
		return;
	}

	ptrTvVersion = lv_label_create(ptrMainWindow);

	lv_obj_set_size(ptrTvVersion, TvVer_Pos.mWidth, TvVer_Pos.mHeight);
	lv_obj_set_pos(ptrTvVersion, TvVer_Pos.mX, TvVer_Pos.mY);	
}

void ui_set_TvVersionWidget_pos(int x, int y)
{
	if(NULL == ptrTvVersion){
		printf("tv version object has not been created\n");
		return;
	}

	lv_obj_set_pos(ptrTvVersion, y, y);	
}

void ui_set_TvVersionWidget_size(int width, int height)
{
	if(NULL == ptrTvVersion){
		printf("tv version object has not been created\n");
		return;
	}

	lv_obj_set_size(ptrTvVersion, width, height);
}

void ui_set_TvVersionWidget_text(const char *text)
{
	if(NULL == ptrTvVersion){
		printf("tv version object has not been created\n");
		return;
	}

	lv_label_set_text(ptrTvVersion, text);
}

void ui_set_TvVersionWidget_visible(bool isVisible)
{
	if(NULL == ptrTvVersion){
		printf("tv version object has not been created\n");
		return;
	}

	if(isVisible){
		lv_obj_set_style_opa(ptrTvVersion, LV_OPA_COVER, 0);
	}else{
		lv_obj_set_style_opa(ptrTvVersion, LV_OPA_TRANSP, 0);
	}	
}


//qrcode widget functions
void ui_create_QrCodeWidget(void)
{	
	if(NULL == ptrMainWindow){
		printf("qr code object creates fail\n");
		return;
	}

	ptrQrCode = lv_qrcode_create(ptrMainWindow, QrCode_Pos.mWidth, lv_color_hex3(0x00), lv_color_hex3(0xffffff));

	lv_obj_set_pos(ptrQrCode, QrCode_Pos.mX, QrCode_Pos.mY);
	
	lv_style_init(&qrcode_style);

	lv_style_set_bg_opa(&qrcode_style, LV_OPA_COVER);
	lv_style_set_bg_color(&qrcode_style, lv_color_hex3(0xffffff));

	lv_style_set_border_width(&qrcode_style, 3);
	lv_style_set_border_color(&qrcode_style, lv_color_hex3(0xffffff));
	//lv_style_set_outline_width(&qrcode_style, 3);//圆角
	//lv_style_set_outline_color(&qrcode_style, lv_color_hex3(0xffffff));

	lv_obj_add_style(ptrQrCode, &qrcode_style, 0);
}

void ui_update_QrCodeWidget(const char *data)
{
	if(NULL == data){
		printf("ERR: qrcode data is NULL\n");
		return;
	}

	if(NULL == ptrQrCode){
		printf("qr code object has not been created\n");
		return;
	}

	lv_qrcode_update(ptrQrCode, (char *)data, strlen(data));
}

void ui_set_QrCodeWidget_pos(int x, int y)
{
	if(NULL == ptrQrCode){
		printf("qr code object has not been created\n");
		return;
	}

	lv_obj_set_pos(ptrQrCode, x, y);
}

void ui_set_QrCodeWidget_size(int width, int height)
{
	if(NULL == ptrQrCode){
		printf("qr code object has not been created\n");
		return;
	}

	lv_obj_set_size(ptrQrCode, width, height);
}

void ui_set_QrCodeWidget_visible(bool isVisible)
{
	if(NULL == ptrQrCode){
		printf("qr code object has not been created\n");
		return;
	}

	if(isVisible){
		lv_obj_set_style_opa(ptrQrCode, LV_OPA_COVER, 0);
	}else{
		lv_obj_set_style_opa(ptrQrCode, LV_OPA_TRANSP, 0);
	}	
}

/*
 *设置二维码控件内容
 */
void ui_update_QR_Content(lv_obj_t *qr, const char *data)
{
	if(NULL == qr){
		printf("[%s]ERR: qr object does not exist\n", __FUNCTION__);
		return;
	}

	if(NULL == data){
		printf("[%s]ERR:data is NULL\n", __FUNCTION__);
		return;
	}

	lv_qrcode_update(qr, (char *)data, strlen(data));
}

/*
 *创建视频控件
 */
 void ui_create_vedioWidget(void)
{
	if(NULL == ptrMainWindow){
		printf("create vedio object fail\n");
		return;
	}

	ptrVedio = lv_obj_create(ptrMainWindow);

	lv_obj_remove_style_all(ptrVedio);
	lv_obj_set_style_bg_opa(ptrVedio, LV_OPA_COVER, 0);
	#if 0
	lv_style_init(&style_scr_act);
    /* 默认不是透明的，后面按需要切换即可 */
    lv_style_set_bg_opa(&style_scr_act, LV_OPA_COVER);
	lv_obj_add_style(ptrVedio, &style_scr_act, 0);
	#endif
	lv_obj_set_size(ptrVedio, Vedio_Pos.mWidth, Vedio_Pos.mHeight);
	lv_obj_set_pos(ptrVedio, Vedio_Pos.mX, Vedio_Pos.mY);
}

/*
 *设置视频控件位置和大小
 */
void ui_set_vedioWidget_pos(layoutPositon_t pos)
{
	if(NULL == ptrVedio){
		printf("[%s]vedio object is not created\n", __FUNCTION__);
		return;
	}

	Vedio_Pos.mX = pos.mX;
	Vedio_Pos.mY = pos.mY;
	Vedio_Pos.mWidth = pos.mWidth;
	Vedio_Pos.mHeight = pos.mHeight;

	lv_obj_set_size(ptrVedio, Vedio_Pos.mWidth, Vedio_Pos.mHeight);
	lv_obj_set_pos(ptrVedio, Vedio_Pos.mX, Vedio_Pos.mY);
}
/*
 *获取视频控件的句柄
 */
lv_obj_t *ui_get_VedioWidget_handle(void)
{
	return ptrVedio;
}


//honeatar karl.hong add 20210826
static void set_wind_rotate_0_180(void)
{
	/*calculate vedio object pos*/
#if FANG_PING_1280x1024_SPECIAL_DEMAND
	Vedio_Pos.mX = 0;
	Vedio_Pos.mY = 0;
	Vedio_Pos.mWidth = fb_width;
	Vedio_Pos.mHeight = fb_width*9/16;
#else
	Vedio_Pos.mX = 0;
	Vedio_Pos.mY = 0;
	Vedio_Pos.mWidth = fb_width;
	Vedio_Pos.mHeight = fb_height;
#endif

	/*calculate pic object pos*/
#if FANG_PING_1280x1024_SPECIAL_DEMAND
	Pic_Pos.mX = 0;
	Pic_Pos.mY = Vedio_Pos.mHeight;
	Pic_Pos.mWidth = fb_width;
	Pic_Pos.mHeight = fb_height - Vedio_Pos.mHeight;
#else
	Pic_Pos.mX = 0;
	Pic_Pos.mY = 0;
	Pic_Pos.mWidth = fb_width;
	Pic_Pos.mHeight = fb_height;
#endif

	/*calculate tv version object pos*/
	TvVer_Pos.mX = 15;
	TvVer_Pos.mY = 1;
	TvVer_Pos.mWidth = 200;
	TvVer_Pos.mHeight = 26;

	/*calculate qrcode object pos*/
	QrCode_Pos.mX = fb_width - 76;//
	QrCode_Pos.mY = 5;
	QrCode_Pos.mWidth = 60;
	QrCode_Pos.mHeight = 60;

	/*calculate tv signal object pos*/
	TvSig_Pos.mX = 15;
	TvSig_Pos.mY = 26;
	TvSig_Pos.mWidth = 24;
	TvSig_Pos.mHeight = 24;
}

static void set_wind_rotate_90_270(void)
{
	/*calculate vedio object pos*/
	Vedio_Pos.mX = 0;
	Vedio_Pos.mY = 0;
	Vedio_Pos.mWidth = fb_width;
	Vedio_Pos.mHeight = fb_width*9/16;

	/*calculate pic object pos*/
	Pic_Pos.mX = 0;
	Pic_Pos.mY = Vedio_Pos.mHeight;
	Pic_Pos.mWidth = fb_width;
	Pic_Pos.mHeight = fb_height - Vedio_Pos.mHeight;

	/*calculate tv version object pos*/
	TvVer_Pos.mX = 15;
	TvVer_Pos.mY = 1;
	TvVer_Pos.mWidth = 200;
	TvVer_Pos.mHeight = 26;

	/*calculate qrcode object pos*/
	QrCode_Pos.mX = fb_width - 76;
	QrCode_Pos.mY = 5;
	QrCode_Pos.mWidth = 60;
	QrCode_Pos.mHeight = 60;

	/*calculate tv signal object pos*/
	TvSig_Pos.mX = 15;
	TvSig_Pos.mY = 26;
	TvSig_Pos.mWidth = 24;
	TvSig_Pos.mHeight = 24;
}

static void Calc_Object_Position(void)
{
	switch(GetRotateSetting()){
		case 0:
		case 180:
			set_wind_rotate_0_180();
			break;
		case 90:
		case 270:
			set_wind_rotate_90_270();
			break;
		default:break;
	}
}
//honestar karl.hong add 20210826


void ui_create_mainWindow(void)
{	
	printf("@@@@@@@@@@@@@@@@@ui_create_mainWindow\n");
#if 1
	//create main window
	ptrMainWindow = lv_obj_create(NULL);

	//set main window size
	//fbdev_get_sizes(&fb_width, &fb_height);
	#if (SSTAR_GFX_ROTATE == 1) 
    fb_width = 480;
    fb_height = 800;
	#else
    fb_width = 800;
    fb_height = 480;
	#endif
	lv_obj_set_size(ptrMainWindow, (lv_coord_t)fb_width, (lv_coord_t)fb_height);
	lv_obj_set_pos(ptrMainWindow, 0, 0);

	//set main window bg color
	lv_obj_set_style_bg_color(ptrMainWindow, lv_color_hex3(0x00), 0);

	//Calc_Object_Position();
	#if (SSTAR_GFX_ROTATE == 1) 
	set_wind_rotate_90_270();
	#else
	set_wind_rotate_0_180();
	#endif
	//create vedio widget
	ui_create_vedioWidget();

	//create pic widget
	ui_create_picWidget();
	ui_set_picWidget_visible(true);
	#if (SSTAR_GFX_ROTATE == 1) 
	ui_update_picWidget_content("/mnt/lvgl_ui/res/ui/def_ad_img_480x800.png");
	#else
	ui_update_picWidget_content("/mnt/lvgl_ui/res/ui/def_ad_img_800x480.png");
	#endif
	
	#if 0
	lv_obj_t * img = lv_img_create(lv_scr_act());
        /* Assuming a File system is attached to letter 'A'
        * E.g. set LV_USE_FS_STDIO 'A' in lv_conf.h */
        lv_img_set_src(img, "A:/mnt/lvgl_demo/def_ad_img.png");
	#endif
	//create tv version widget
	ui_create_TvVersionWidget();
	ui_set_TvVersionWidget_text("V1.0.0-sstar");

	//create tv signal widget
	ui_create_TvSigWidget();
	ui_update_TvSigWidget_content("/mnt/lvgl_ui/icon/sig/no_sig.png");

	//create qrcode widget
	ui_create_QrCodeWidget();
	ui_set_QrCodeWidget_visible(false);
	ui_update_QrCodeWidget("Hello World!");	

	//load main window to display
	lv_scr_load(ptrMainWindow);

	lv_obj_set_style_bg_opa(lv_scr_act(), LV_OPA_TRANSP, 0);//默认设置透明
#endif
}

