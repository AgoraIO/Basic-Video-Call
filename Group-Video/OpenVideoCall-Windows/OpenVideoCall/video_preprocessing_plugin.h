
#ifndef  _VIDEO_PREPROCESSING_PLUGIN_H_
#define _VIDEO_PREPROCESSING_PLUGIN_H_

#include <IAgoraRtcEngine.h>

int load_preprocessing_plugin(agora::rtc::IRtcEngine* engine);
int unload_preprocessing_plugin(agora::rtc::IRtcEngine* engine);

#endif //_VIDEO_PREPROCESSING_PLUGIN_H_
