/*
 * MediaPlayer.h
 *
 *  Created on: 2021年2月1日
 *      Author: Administrator
 */

#ifndef JNI_MODEL_MEDIAPLAYER_H_
#define JNI_MODEL_MEDIAPLAYER_H_
#ifdef __cplusplus
extern "C"{
#endif // __cplusplus

#include "lvgl/lvgl.h"


////////////////////////add it by adien//////////////////////
typedef enum 
{
	NEED_CONTINUE,
	NEED_IGNORE,
}playCallbackType;
typedef int (*user_player_callback)(playCallbackType,char *,int);//函数指针

void SSTAR_setVolume(int vol);
int init_player_thread(user_player_callback function_callback);
void user_start_play_video(char *videoPath,int carousel);
void user_stop_play_video(void);
const char *user_get_play_filePath(void);
////////////////////////add it by adien//////////////////////

class MediaPlayer{
public:
	static MediaPlayer* getInstance();

	void initPlayer(lv_obj_t *mVideoview_videoPtr, lv_obj_t *mTextViewPicPtr);
	static void setPlayerVol(int newVol);
	static void showPlayer(bool isSplitDisplay,user_player_callback function_callback);
	static void quitPlayer();
	static void setMute();

private:
	MediaPlayer();
};



#define MEDIAPLAYER 	MediaPlayer::getInstance()
#ifdef __cplusplus
}
#endif // __cplusplus
#endif /* JNI_MODEL_MEDIAPLAYER_H_ */
