#pragma once 

#include <android/log.h>

#define LOG_TAG "MD2Loader"
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
