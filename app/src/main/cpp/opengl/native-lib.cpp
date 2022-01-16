#include <jni.h>
#include "Renderer.h"
#include <memory>

static std::unique_ptr<Raydelto::MD2Loader::Renderer> pObject = std::make_unique<Raydelto::MD2Loader::Renderer>();

extern "C" JNIEXPORT void JNICALL
Java_org_raydelto_md2loader_MyRenderer_surfaceCreated(
    JNIEnv *env,
    jobject /* this */)
{
    pObject->onSurfaceCreated();
}

extern "C" JNIEXPORT void JNICALL
Java_org_raydelto_md2loader_MyRenderer_drawFrame(
    JNIEnv *env,
    jobject /* this */)
{
    pObject->onDrawFrame();
}

extern "C" JNIEXPORT void JNICALL
Java_org_raydelto_md2loader_MyRenderer_setScreenSize(
    JNIEnv *env,
    jobject /* this */,
    jint width,
    jint height)
{
    pObject->setScreenSize(width, height);
}

extern "C" JNIEXPORT void JNICALL
Java_org_raydelto_md2loader_MyRenderer_setRotationAngles(
    JNIEnv *env,
    jobject /* this */,
    jfloat x,
    jfloat y)
{
    pObject->setRotationAngles(x, y);
}

extern "C" JNIEXPORT void JNICALL
Java_org_raydelto_md2loader_MyRenderer_setScale(
    JNIEnv *env,
    jobject /* this */,
    jfloat scale)
{
    pObject->setScale(scale);
}
