#include <jni.h>
#include <string>
#include "../../../../../../Source/SmashBros/AndroidBridge.h"

extern "C" JNIEXPORT jstring JNICALL
Java_com_brokenphysics_issb_MainActivity_stringFromJNI(
        JNIEnv* env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}

extern "C" JNIEXPORT void JNICALL
Java_com_brokenphysics_issb_MainActivity_nativeSetTouchControlsEnabled(
        JNIEnv* env,
        jclass /* this */,
        jboolean enabled) {
    SmashBros::AndroidBridge::setTouchControlsEnabled((bool)enabled);
}