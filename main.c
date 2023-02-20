/*
 * test.c
 */

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include "lv_obj.h"
#include "sstar_port.h"
#include "lv_label.h"
#include "lv_disp.h"
#include "lv_style.h"
#include "lv_demos.h"
#include <sys/time.h>

#include "widgets/lv_demo_widgets.h"
#include "music/lv_demo_music.h"
#include "benchmark/lv_demo_benchmark.h"
//#include "ui.h"
#include "./src/appconfig.h"
#include "lvgl/lvgl.h"
#include "./lv_porting_sstar/sstar_fbdev.h"
#include "evdev.h"
#include "./src/lv_ui/mainWindow.h"
#include "./src/main_app/main_app.h"

#define PATH_IMAGE	"/customer/"


void *tick_thread(void * data)
{
    (void)data;

    while(1) {
        usleep(1000);
        lv_tick_inc(1); /*Tell LittelvGL that 5 milliseconds were elapsed*/
    }

    return 0;
}

unsigned int _GetTime0()
{
    struct timespec ts;
    unsigned int ms;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    ms = (ts.tv_sec * 1000) + (ts.tv_nsec / 1000000);
    if(ms == 0)
    {
        ms = 1;
    }
    return ms;
}
#if 0
// img对象设置png图片自适应显示
static void lv_obj_img_png_set_zoom(lv_obj_t * obj_img, const char *src, uint32_t obj_width, uint32_t obj_height)
{
    if (obj_img == NULL || src == NULL || src[0] == '\0')
    {
        printf("[%s:%d] param errror\n", __FUNCTION__, __LINE__);
        return;
    }
 
    if (obj_width == 0 || obj_height == 0)
    {
        printf("[%s:%d] param errror\n", __FUNCTION__, __LINE__);
        return;
    }
 
    uint32_t img_width = 0, img_height = 0, zoom_factor = 0;
    // 获取img对象的信息
    lv_img_header_t header;
    if (lv_img_decoder_get_info(src, &header) != LV_RES_OK)
    {
        printf("[%s:%d] lv_img_decoder_get_info errror\n", __FUNCTION__, __LINE__);
        return;
    }
   
    img_width = header.w;
    img_height = header.h;
 
    printf("[%s:%d] img_width:%u, img_height:%u, obj_width:%u, obj_height:%u\n", __FUNCTION__, __LINE__, img_width, img_height, obj_width, obj_height);
    if (img_width != 0 && img_height != 0)
    {
        uint32_t y_a= obj_height * img_width;   
        uint32_t x_b= obj_width * img_height;
 
        if (x_b >= y_a)
        {
            if (img_height >= obj_height)
            {
                uint32_t x = obj_height * 256;
                zoom_factor = x / img_height;
                lv_img_set_zoom(obj_img, zoom_factor);
            }
        }
        else
        {
            if (img_width > obj_width)
            {
                uint32_t x = obj_width * 256;
                zoom_factor = x / img_width;
                lv_img_set_zoom(obj_img, zoom_factor);
            }
        }
    }
}
#endif
int main(int argc, char *argv[])
{
	char test_path[1024] = { 0 };
    pthread_t pt;
    if (0 != sstar_lv_init()) {
        printf("ERR: sstar_lv_init failed.\n");
        return -1;
    }
    lv_png_init();
    #if 0
    if (0 == strcmp(argv[1], "a")) {
        lv_demo_music();
    } else if (0 == strcmp(argv[1], "b")) {
        lv_demo_widgets();
    } else if (0 == strcmp(argv[1], "c")) {
        lv_demo_benchmark();
    } else if (0 == strcmp(argv[1], "d")) {
		//lv_example_png_1();
		//lv_example_gif_1();
		//lv_example_qrcode_1();
        

        #if 1
        lv_obj_t * img = lv_img_create(lv_scr_act());
        /* Assuming a File system is attached to letter 'A'
        * E.g. set LV_USE_FS_STDIO 'A' in lv_conf.h */
        lv_img_set_src(img, "A:/mnt/lvgl_demo/def_ad_img.png");
        //lv_obj_align(img, LV_ALIGN_TOP_MID, 0, 0);
        //lv_obj_set_size(img, 480, 400);
        //lv_obj_set_pos(img,0,400);
        #endif

        #if 0
        unsigned int obj_width = 480, obj_height = 800; // 设置对象大小100*100
        lv_img_set_src(img, "A:/mnt/lvgl_demo/def_ad_img.png"); // 设置图片
        lv_obj_align(img, LV_ALIGN_BOTTOM_MID, 0, 0);
        lv_obj_img_png_set_zoom(img, "A:/mnt/lvgl_demo/def_ad_img.png", obj_width, obj_height);
        #endif
		const char * data = "Hello world";

		/*Create a 100x100 QR code*/
		lv_obj_t * qr = lv_qrcode_create(lv_scr_act(), 100, lv_color_hex3(0x33f), lv_color_hex3(0xeef));

		/*Set data*/
		lv_qrcode_update(qr, data, strlen(data));
		
        //ui_init();
    }
#endif
    GetPanelSetting();  
    ui_create_mainWindow();

    register_user_thread((void *)app_main_thread, NULL, NULL);



    pthread_create(&pt, NULL, tick_thread, NULL);
    while(1) {
        unsigned int curr = _GetTime0();
        lv_task_handler();
        unsigned int time_diff = _GetTime0() - curr;
        if (time_diff < 10) {
            usleep(( 10 - time_diff ) * 1000);
        }
    }
    pthread_join(pt, NULL);

    sstar_lv_deinit();
    unregister_user_thread();
    return 0;
}
