#include <jni.h>
#include <string>


extern "C"
JNIEXPORT jstring JNICALL
Java_com_momosoft_imageviewer_MainActivity_stringFromJNI(JNIEnv *env, jobject thiz) {
    return env->NewStringUTF("Hello from C++");

}