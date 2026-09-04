#pragma once 

#if defined(__ANDROID__)
#include <android/log.h>
#define LOG_TAG "MD2Loader"
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#else
#include <cstdio>
#define LOG_TAG "MD2Loader"
#define LOGE(...) do { fprintf(stderr, "[MD2Loader] " __VA_ARGS__); fprintf(stderr, "\n"); fflush(stderr); } while(0)
#endif
