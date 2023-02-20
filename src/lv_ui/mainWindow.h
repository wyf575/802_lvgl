#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct{
	int mX;
	int mY;
	int mWidth;
	int mHeight;
}layoutPositon_t;


//common api

void ui_create_mainWindow(void);

/*
 *获取对象可见属性
 */
bool ui_get_obj_visible(lv_obj_t * obj);

/*
 *设置对象可见属性
 */
void ui_set_obj_visible(lv_obj_t *obj, bool isVisible);



//pic widget functions
void ui_create_picWidget(void);
void ui_set_picWidget_pos(int x, int y);
void ui_set_picWidget_size(int width, int height);
void ui_update_picWidget_content(const char *path);
void ui_set_picWidget_visible(bool isVisible);
bool ui_get_picWidget_visible(void);
/*
 *获取图片控件句柄
 */
lv_obj_t *ui_get_picWidget_handle(void);



//tv signal widget functions
void ui_create_TvSigWidget(void);
void ui_set_TvSigWidget_pos(int x, int y);
void ui_set_TvSigWidget_size(int width, int height);
void ui_update_TvSigWidget_content(const char *path);
void ui_set_TvSigWidget_visible(bool isVisible);

//tv version widget functions
void ui_create_TvVersionWidget(void);
void ui_set_TvVersionWidget_pos(int x, int y);
void ui_set_TvVersionWidget_size(int width, int height);
void ui_set_TvVersionWidget_text(const char *text);
void ui_set_TvVersionWidget_visible(bool isVisible);


//qrcode widget functions
void ui_create_QrCodeWidget(void);
void ui_update_QrCodeWidget(const char *data);
void ui_set_QrCodeWidget_pos(int x, int y);
void ui_set_QrCodeWidget_size(int width, int height);
void ui_set_QrCodeWidget_visible(bool isVisible);
/*
 *设置二维码控件内容
 */
void ui_update_QR_Content(lv_obj_t *qr, const char *data);


/*
 *设置视频控件位置和大小
 */
void ui_set_videoWidget_pos(layoutPositon_t pos);
/*
 *获取视频控件的句柄
 */
lv_obj_t *ui_get_VedioWidget_handle(void);



#ifdef __cplusplus
} /*extern "C"*/
#endif



#endif
