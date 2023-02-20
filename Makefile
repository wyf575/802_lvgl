CC= arm-linux-gnueabihf-gcc
CPP = arm-linux-gnueabihf-g++ 

#配置源文件目录
PROJ_ROOT_PATH  :=./
#源文件后缀名
PROJ_SRC_SUFFIX := %.cpp %.c
PROJ_HEAD_SUFFIX := %.h
# 递归遍历目录下的所有的文件 
rwildcard=$(wildcard $1$2) $(foreach d,$(wildcard $1*),$(call rwildcard,$d/,$2))  
# 获取相应的源文件
PROJ_ALL_FILES := $(foreach src_path,$(PROJ_ROOT_PATH), $(call rwildcard,$(src_path),*.*) )   
PROJ_SRC_FILES := $(filter $(PROJ_SRC_SUFFIX),$(PROJ_ALL_FILES))   
PROJ_HEAD_FILES := $(filter $(PROJ_HEAD_SUFFIX),$(PROJ_ALL_FILES))   

# 获取相应的源文件
LOCAL_SRC_FILES  := $(PROJ_SRC_FILES) 
LOCAL_HEAD_FILES := $(PROJ_HEAD_FILES) 

#源文件目录结构
PROJ_DIR_TREE := $(dir $(foreach src_path, $(LOCAL_SRC_FILES), $(call rwildcard,$(src_path),*/) ) )  
PROJ_DIR_TREE := $(sort $(PROJ_DIR_TREE))  

#player功能启用开关
CONFIG_PLAYER_SWITCH := "enable"
#CONFIG_PLAYER_SWITCH :=

#cloudplay功能启用开关
CONFIG_CLOUD_PLAY_SWITCH := "enable"
#CONFIG_CLOUD_PLAY_SWITCH :=

#myplayer多进程功能启用开关
CONFIG_MULTIPROC_SWITCH := "enable"
#CONFIG_MULTIPROC_SWITCH :=
OBJS_ROOT_DIR=./obj/
OBJS_1 = $(patsubst %.cpp,%.o,$(LOCAL_SRC_FILES))
OBJS_2 = $(patsubst %.c,%.o,$(OBJS_1))
OBJS   = $(addprefix $(OBJS_ROOT_DIR), $(OBJS_2))
OBJS_DIR_TREE=$(addprefix $(OBJS_ROOT_DIR),$(PROJ_DIR_TREE))

CFLAGS=-O3 -mfpu=neon#-Os -pipe  -fno-caller-saves -Wno-unused-result -Wno-format-truncation -Wno-write-strings -mfloat-abi=hard -mfpu=vfp -Wformat -Werror=return-type -Werror=format-security -fstack-protector -D_FORTIFY_SOURCE=1 -Wl,-z,now -Wl,-z,relro -Wl,--warn-common -Wl,--warn-once -Wl,-z,combreloc -Wl,-z,defs
CXXFLAGS=-O3 -mfpu=neon#-Os -pipe  -fno-caller-saves -Wno-unused-result -Wno-format-truncation -Wno-write-strings -mfloat-abi=hard -mfpu=vfp -Wformat -Werror=return-type -Werror=format-security -fstack-protector -D_FORTIFY_SOURCE=1 -Wl,-z,now -Wl,-z,relro -Wl,-z,defs 
CXXFLAGS+= -fpermissive#-fexceptions -fpermissive -Wall -O2

STATIC_LIB = ./src/curl/libcurl.a ./src/openssl/libssl.a ./src/openssl/libcrypto.a ./src/zlib/libz.a

	
INCLUDE_DIR := 	-I. \
				-I ./src \
				-I ./src/include \
				-I ./src/sdkdir/include \
				-I ./src/voicedetect \
				-I ./src/tts \
				-I ./src/rtsp/include	\
				-I ./src/rtsp	\
				-I ./src/airplay/include \
				-I ./src/nz3802 \
				-I ./src/lv_ui \
				-I ./src/main_app \
				-I ./src/main_app/platform 
				
		
LDFLAGS = -pthread -lmi_sys -lmi_disp -lmi_panel -lmi_ao -lmi_gfx -lmi_divp -lmi_vdec -lmi_common -lmi_wlan -lcjson -lmi_ai -lssl -lcrypto -ldl -lrt -lmosquitto
LDFLAGS += 	-L. \
			-L ./src/sdkdir/lib \
			-L./src \
			-L./src/3rdlib \


		

INCLUDE_DIR += 	-I ./src/player \
				-I ./src/player/ffmpeg \
				-I ./src/player/ffmpeg/include \
				-I ./src/player/mediastream \
				-I ./src/player/imagedata	\
				-I ./src/v4l2/inc \
				-I ./src/v4l2/inc/uvc \
				-I ./src/v4l2/inc/video \
				-I ./src/v4l2/src \
				-I ./src/v4l2/src/control \
				-I ./src/v4l2/src/gcc \
				-I ./src/v4l2/src/libv4l \
				-I ./src/v4l2/src/processing \
				-I ./src/hotplugdetect \
				-I ./src/hotplugdetect/common \
				-I ./src/hotplugdetect/usbdetect \
				-I ./src/hotplugdetect/wifidetect \
				-I ./src/hotplugdetect/wirednetworkdetect \
				-I ./src/jsoncpp/include \
				-I ./src/jsoncpp/ \
				-I ./src/model \
				-I ./src/io 


INCLUDE_DIR +=  -I ./lvgl/demos \
				-I ./lvgl \
				-I ./lv_drivers/indev \
 				-I ./lvgl/src \
				-I ./lvgl/src/core \
				-I ./lvgl/src/draw \
				-I ./lvgl/src/draw/sw \
				-I ./lvgl/src/extra \
				-I ./lvgl/src/font \
				-I ./lvgl/src/gpu \
				-I ./lvgl/src/hal \
				-I ./lvgl/src/misc \
				-I ./lvgl/src/widgets \
				-I ./lv_porting_sstar 

ifneq ($(CONFIG_PLAYER_SWITCH),)
CFLAGS+=-DSUPPORT_PLAYER_MODULE
CXXFLAGS+=-DSUPPORT_PLAYER_MODULE
endif

ifneq ($(CONFIG_CLOUD_PLAY_SWITCH),)
CFLAGS+=-DSUPPORT_CLOUD_PLAY_MODULE
CXXFLAGS+=-DSUPPORT_CLOUD_PLAY_MODULE
endif

ifneq ($(CONFIG_MULTIPROC_SWITCH),)
CFLAGS+=-DSUPPORT_PLAYER_PROCESS
CXXFLAGS+=-DSUPPORT_PLAYER_PROCESS
else
LDFLAGS += -lavformat -lavcodec -lavutil -lswscale -lswresample -L./src/player/ffmpeg/lib
endif
CFLAGS+=-DCHIP_i2m
CXXFLAGS+=-DCHIP_i2m
TARGET=lvgl_app_802

ifeq ($(TARGET), $(wildcard $(TARGET)))
	DELETE_TARGET=$(TARGET)
endif

# alias commads if compile on windows
RM =rm
MKDIR=mkdir
ECHO=echo
#alias end

all:   $(TARGET)
	@$(ECHO)
	@$(ECHO) "[armeabi] Install        : $(TARGET)"
	

$(OBJS_ROOT_DIR)%.o: %.cpp $(LOCAL_HEAD_FILES)
	@$(ECHO) "[armeabi] Compile++      : "$< 
	@-$(MKDIR) $(OBJS_DIR_TREE) -p 
	@$(CPP) -c $< -o $@ $(CXXFLAGS) $(INCLUDE_DIR) $(LDFLAGS) 
	
$(OBJS_ROOT_DIR)%.o: %.c $(LOCAL_HEAD_FILES)
	@$(ECHO) "[armeabi] Compile      : "$< 
	@-$(MKDIR) $(OBJS_DIR_TREE) -p 
	@$(CC) -c $< -o $@ $(CFLAGS) $(INCLUDE_DIR) $(LDFLAGS)

$(TARGET):$(OBJS) 
	@$(ECHO) "[armeabi] Build App  : "$@ 
	@$(CPP) $^ -o $@ $(LDFLAGS) $(INCLUDE_DIR) $(STATIC_LIB)
	
prepare: 
	@$(ECHO) "[armeabi] GCC            : "$(CC)
	@$(ECHO) "[armeabi] G++            : "$(CPP)
	@-$(MKDIR) $(OBJS_DIR_TREE) -p 
	@-$(RM) $(TARGET) -rf 
	
clean:
	@$(ECHO) "[armeabi] Clean          : "$(OBJS_ROOT_DIR) 
	@-$(RM)  $(OBJS_ROOT_DIR) -rf 
	@$(ECHO) "[armeabi] Clean          : "$(TARGET)
	@-$(RM)  $(TARGET) -rf 

.PHONY:all clean prepare $(TARGET)

