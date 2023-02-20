#include <sys/time.h>
#include "widgetUI.h"
#include "../main_app.h"
#include "appconfig.h"
#include "../player_task/configPlayer.h"
#include "../net_listener.h"
#include "../debug_printf/debug_printf.h"
#include "net/NetManager.h"
#include "net/DeviceSigManager.h"
#include "lvgl/lvgl.h"





#define CHECK_TRUE(x) {if(false==x){printf("please init UI first!");return -1;}}


extern const char *get_popularize_str(void);
extern const char *UnGetDeviceSN(void);
static bool pageStarted = false;
volatile bool isPlaying = false;
static bool isVedioVisible = false;

bool get_playerSelect_status(void)
{
	//return pageStarted;
	return true;
}

//////////////////////////////////////////////////视频播放器部分////////////////////////////////////////////////////
/*
*视频播放器初始化
*/
static bool init_player_created = false;
int init_player_thread(user_player_callback function_callback)
{
	CHECK_TRUE(get_playerSelect_status());
	if (false == init_player_created)
	{
		//MEDIAMANAGER->initPlayList();
		MEDIAPLAYER->initPlayer(ui_get_VedioWidget_handle(), ui_get_picWidget_handle());
		MEDIAPLAYER->showPlayer(false,function_callback);
		printf("init_player_thread[视频线程启动完成]\n");
		init_player_created = true;
	}
	return 0;
}

/*
*视频播放器开始播放
*/
int start_video_play(char *videoPath,carouselType carousel)
{
	if (NULL == videoPath)
	{
		return -1;
	}
	CHECK_TRUE(init_player_created);
	if(true == get_video_isPlaying())
	{
		user_stop_play_video();
	}
	set_video_widget_visible(true);//add by wyf fasle
	user_start_play_video(videoPath,carousel);
	isPlaying = true;
	return 0;
}

/*
*视频播放器停止播放
*/
int stop_video_play(void)
{
	CHECK_TRUE(init_player_created);
	if(true == get_video_isPlaying())
	{
		user_stop_play_video();
	}
	isPlaying = false;
	set_video_widget_visible(false);
	return 0;
}

/*
*设置视频播放器控件显示或隐藏
*/
int set_video_widget_visible(bool visible)
{  
	CHECK_TRUE(get_playerSelect_status());
	printf("set_video_widget_visible[设置视频显示/隐藏][%d]\n",visible);
	isVedioVisible = visible;
	ui_set_obj_visible(ui_get_VedioWidget_handle(), !isVedioVisible);
	return 0;
}

/*
*设置视频播放器音量
*/
int set_video_vol_level(int level)
{
	CHECK_TRUE(get_playerSelect_status());
	printf("set_video_vol_level[设置视频音量等级][%d]\n",level);
	MEDIAPLAYER->setPlayerVol(level);
	return 0;
}

/*
*获取视频播放器是否在运行
*/
bool get_video_isPlaying(void)
{
	return isPlaying;
}

/*
*获取flyting层视频控件是否显示中
*/
bool GetVideoWidgetState(void)
{
	lv_obj_t *mVedioPtr = ui_get_VedioWidget_handle();
	return !ui_get_obj_visible(mVedioPtr); 
}



/*
*设置视频控件是否显示
*/
void setVideoWidgetVisible(bool enabled)   
{
    set_video_widget_visible(enabled);
}

/*
*设置图片控件是否显示
*/
void setPictureWidgetVisible(bool enabled)   
{
    ui_set_picWidget_visible(enabled);
}

/*
*获取图片控件是否显示
*/
bool GetPicisVisible(void)
{
	return ui_get_picWidget_visible();
}


/*
*设置图片控件加载图片地址
*/
void setPictureWidgetLoad(char *picPath)   
{
	ui_update_picWidget_content(picPath);
}


void setQRcodeWidgetVisible(bool enabled)   
{
    ui_set_QrCodeWidget_visible(enabled);
}


void setQRcodeWidgetLoad(char *pStr)   
{
    ui_update_QrCodeWidget(pStr);
}
//没用到？
void setQRcodeWidgetSize(int qrSizeInt)   
{
    printf("set_QRcode_widget_size[设置二维码大小][%d]\n",qrSizeInt);
	ui_set_QrCodeWidget_size(qrSizeInt, qrSizeInt);
}

void setSignalWidgetLoad(networkType type,int level)   
{
	/*ethan add 20211209*/
	static int old_level = 0;
	static networkType old_type = NETWORKTYPE_2G;
	/*ethan add end 20211209*/
	printf("set_signal_widget[%d][%d]",type,level);
	char bgPicPath[100] = {0};
	switch (type)
	{
		case NETWORKTYPE_ETHERNET:
			snprintf(bgPicPath,100,"%s", ICON_ROOT_PATH"sig/stat_sys_eth_connected.png");
		break;
		case NETWORKTYPE_4G:
		case NETWORKTYPE_WIFI:
			switch (level)
			{
			case 0:
				snprintf(bgPicPath,100,"%s", ICON_ROOT_PATH"sig/no_sig.png");
				break;
			case 1:
			case 2:
			case 3:
			case 4:
				if (NETWORKTYPE_4G == type)
				{
					snprintf(bgPicPath,100, ICON_ROOT_PATH"sig/sig_4g_%d.png",level);
				}else
				{
					snprintf(bgPicPath,100, ICON_ROOT_PATH"sig/sig_wifi_%d.png",level);
				}
				break;
			default:
				break;
			}
		break;
		case NETWORKTYPE_NONE:
	default:
		snprintf(bgPicPath,100,"%s", ICON_ROOT_PATH"sig/no_sig.png");
		break;
	}
	
	if((old_type != type) || (old_level != level))
	{
		printf("setBackgroundPic[%s]\n",bgPicPath);
		ui_update_TvSigWidget_content(bgPicPath);
	}
}

void setVersionWidgetVisible(bool enabled)   
{
    ui_set_TvVersionWidget_visible(enabled);
}
//void setVersionWidgetLoad(char *version,char *sn)//2021.10.19 ethan delete
void setVersionWidgetLoad(char *version,char *sn)//2021.10.19 ethan add
{
	char showstr[50] = {0};
	snprintf(showstr,50,"%s-%s",version,sn);
	printf("set_version_widget................\n");
	ui_set_TvVersionWidget_text(showstr);
}

/*
2021.9.28修改在联网与否的情况下都显示二维码
*/
void updateQRcodeWidget(void)
{
    configParam param =  getConfigParams(); 
    //if (NET_STATUS_OK == get_app_network_status() && NET_STATUS_OK ==get_network_status())
    if(NET_STATUS_OK == get_app_network_status())
    {
        if (1 == param.QRCodeDisplay)
        {
            //显示自定义二维码
            ui_update_QrCodeWidget(param.qr_link);
        }else if(2 == param.QRCodeDisplay)
        {
            //显示客户定制二维码+SN
            const char *netSN = UnGetDeviceSN();
            if (NULL != netSN)
            {
                char tmp_str[262] = {0};
                snprintf(tmp_str,262,"%s%s",param.qr_link,netSN);
                ui_update_QrCodeWidget(tmp_str);
            }
        }else{
            //显示推广二维码
            const char *tmp_str = get_popularize_str();
            if (strlen(tmp_str)>6)
            {
                ui_update_QrCodeWidget(tmp_str);
            }
        }
        setQRcodeWidgetSize(param.qr_size);
        ui_set_QrCodeWidget_visible(true);
    }
    //}else
    //{
    //    set_QRcode_widget_visible(false);
    //}
}

void updateOtherConfigs(void)
{
    struct timeval tv1;
    struct timeval tv2;
    gettimeofday(&tv1, NULL);

    configParam param =  getConfigParams();

    gettimeofday(&tv2, NULL);
    printf("@@@@@@@@@@@@@@@@getConfigParams:%ld;\n", (tv2.tv_sec-tv1.tv_sec)*1000000+tv2.tv_usec-tv1.tv_usec);
    SSTAR_setVolume(param.vol);
    debuglog_yellow("app_network[%d]qr_link[%s]id[%d]qr_size[%d]vol[%d]QRCodeDisplay[%d] \n",get_app_network_status(),param.qr_link,param.id,param.qr_size,param.vol,param.QRCodeDisplay);
}


/*
*获取lvgl控件位置
*/
layoutPositon_t ui_get_obj_layout_position(lv_obj_t *obj)
{
	layoutPositon_t pos;
	memset((char *)&pos, 0, sizeof(pos));
	if(NULL == obj){
		return pos;
	}
	
	lv_obj_update_layout(obj);  /*Be sure the object's size is calculated*/

	pos.mX = lv_obj_get_x(obj);
	pos.mY = lv_obj_get_y(obj);
	pos.mWidth = lv_obj_get_width(obj);
	pos.mHeight = lv_obj_get_height(obj);

	return pos;
}


/*
 *设置lvgl控制位置
 */
int ui_set_obj_layout_position(lv_obj_t *obj, layoutPositon_t pos)
{
	if(NULL == obj){
		printf("[%s]obj does not exist\n", __FUNCTION__);
		return -1;
	}

	lv_obj_set_pos(obj, (lv_coord_t)pos.mX, (lv_coord_t)pos.mY);
	lv_obj_set_size(obj, (lv_coord_t)pos.mWidth, (lv_coord_t)pos.mHeight);

	return 0;
}

/*
 *设置lvgl控件背景图片
 */
int ui_set_obj_background_pic(lv_obj_t *obj, const char *picPath)
{
	if(NULL == obj){
		printf("[%s]obj does not exist\n", __FUNCTION__);
		return -1;
	}

	lv_obj_set_style_bg_img_src(obj, (char *)picPath, 0);

	return 0;
}

