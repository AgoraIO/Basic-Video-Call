LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_SRC_FILES := \
	video_preprocessing_plugin_jni.cpp \

LOCAL_LDLIBS := -ldl -llog

LOCAL_MODULE := apm-plugin-video-preprocessing

include $(BUILD_SHARED_LIBRARY)
