#include <jni.h>
#include <string>
#include <stdio.h>
#include <vector>
#include "Renderer.h"

static Renderer *pObject = new Renderer();

extern "C" JNIEXPORT void JNICALL
Java_org_raydelto_md2loader_MyRenderer_surfaceCreated(
        JNIEnv *env,
        jobject /* this */) {
    pObject->onSurfaceCreated();
}

extern "C" JNIEXPORT void JNICALL
Java_org_raydelto_md2loader_MyRenderer_drawFrame(
        JNIEnv *env,
        jobject /* this */) {
    pObject->onDrawFrame();
}

extern "C" JNIEXPORT void JNICALL
Java_org_raydelto_md2loader_MyRenderer_setScreenSize(
        JNIEnv *env,
        jobject /* this */,
        jint width,
        jint height) {
    pObject->setScreenSize(width, height);
}

extern "C" JNIEXPORT void JNICALL
Java_org_raydelto_md2loader_MyRenderer_setRotationAngles(
        JNIEnv *env,
        jobject /* this */,
        jfloat x,
        jfloat y) {
    pObject->setRotationAngles(x, y);
}

extern "C" JNIEXPORT void JNICALL
Java_org_raydelto_md2loader_MyRenderer_setScale(
        JNIEnv *env,
        jobject /* this */,
        jfloat scale) {
    pObject->setScale(scale);
}
