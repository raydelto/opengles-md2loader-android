#include <jni.h>
#include "Renderer.h"
#include <memory>

static std::unique_ptr<Raydelto::MD2Loader::Renderer> pRenderer = std::make_unique<Raydelto::MD2Loader::Renderer>();

extern "C" JNIEXPORT void JNICALL
Java_org_raydelto_md2loader_MyRenderer_surfaceCreated(
    JNIEnv *env,
    jobject /* this */)
{
    pRenderer->OnSurfaceCreated();
}

extern "C" JNIEXPORT void JNICALL
Java_org_raydelto_md2loader_MyRenderer_drawFrame(
    JNIEnv *env,
    jobject /* this */)
{
    pRenderer->OnDrawFrame();
}

extern "C" JNIEXPORT void JNICALL
Java_org_raydelto_md2loader_MyRenderer_setScreenSize(
    JNIEnv *env,
    jobject /* this */,
    jint width,
    jint height)
{
    pRenderer->SetScreenSize(width, height);
}

extern "C" JNIEXPORT void JNICALL
Java_org_raydelto_md2loader_MyRenderer_setRotationAngles(
    JNIEnv *env,
    jobject /* this */,
    jfloat x,
    jfloat y)
{
    pRenderer->SetRotationAngles(x, y);
}

extern "C" JNIEXPORT void JNICALL
Java_org_raydelto_md2loader_MyRenderer_setScale(
    JNIEnv *env,
    jobject /* this */,
    jfloat scale)
{
    pRenderer->SetScale(scale);
}
