#ifndef WIDGET_UI
#define WIDGET_UI
#ifdef __cplusplus
extern "C"{
#endif // __cplusplus


#include <stdbool.h>
#include "appconfig.h"
#include "model/MediaPlayer.h"
#include "mainWindow.h"
#include "../device_info.h"
/*
本项目适用的理想的UI控制功能：
1.视频控件的显示和隐藏
2.图片控件的显示和隐藏
3.图片控件加载图片内容
4.二维码控件的显示和隐藏
5.加载二维码内容
6.设置二维码大小
7.设置信号图标内容
8.版本号控件的显示和隐藏
9.版本号控件加载显示内容
*/

/**
 * @brief 设置视频控件是否显示
 * @attention 只有显示情况下视频播放才可见
 * @attention 请对接具体平台及应用
 * @param enabled true:显示 false:隐藏
 */
void setVideoWidgetVisible(bool enabled);

/**
 * @brief 设置图片控件是否显示
 * @attention 请对接具体平台及应用
 * @param enabled true:显示 false:隐藏
 */
void setPictureWidgetVisible(bool enabled);


/**
 * @brief 获取图片控件是否显示
 * @attention 请对接具体平台及应用
 * @param 
 */
bool GetPicisVisible(void);


/**
 * @brief 设置图片控件加载的图片路径
 * @attention 请对接具体平台及应用
 * @param picPath 
 */
void setPictureWidgetLoad(char *picPath);

/**
 * @brief 设置二维码控件是否显示
 * @attention 请对接具体平台及应用
 * @param enabled true:显示 false:隐藏
 */
void setQRcodeWidgetVisible(bool enabled);

/**
 * @brief 设置二维码控件加载二维码内容
 * @attention 请对接具体平台及应用
 * @param pStr 加载的二维码文本
 */
void setQRcodeWidgetLoad(char *pStr);

/**
 * @brief 设置二维码
 * 
 * @param qrSizeInt 
 */
void setQRcodeWidgetSize(int qrSizeInt);

/**
 * @brief Set the Signal Widget Load object
 * 
 * @param type 
 * @param level 
 */
void setSignalWidgetLoad(networkType type,int level);

/**
 * @brief Set the Version Widget Visible object
 * 
 * @param enabled true:显示 false:隐藏
 */
void setVersionWidgetVisible(bool enabled);

/**
 * @brief Set the Version Widget Load object
 * 
 * @param version 
 */
void setVersionWidgetLoad(char *version,char *sn);


void updateQRcodeWidget(void);
void updateOtherConfigs(void);

/*
*视频播放器初始化
*/
int init_player_thread(user_player_callback function_callback);

/*
*视频播放器开始播放
*/
int start_video_play(char *videoPath,carouselType carousel);

/*
*视频播放器停止播放
*/
int stop_video_play(void);

/*
*设置视频播放器控件显示或隐藏
*/
int set_video_widget_visible(bool visible);

/*
*设置视频播放器音量
*/
int set_video_vol_level(int level);

/*
*获取视频播放器是否在运行
*/
bool get_video_isPlaying(void);

/*
*获取flyting层视频控件是否显示中
*/
bool GetVideoWidgetState(void);



/*
*获取lvgl控件位置
*/
layoutPositon_t ui_get_obj_layout_position(lv_obj_t *obj);

/*
 *设置lvgl控制位置
 */
int ui_set_obj_layout_position(lv_obj_t *obj, layoutPositon_t pos);

/*
 *设置lvgl控件背景图片
 */
int ui_set_obj_background_pic(lv_obj_t *obj, const char *picPath);


#ifdef __cplusplus
}
#endif // __cplusplus

#endif // !WIDGET_UI


