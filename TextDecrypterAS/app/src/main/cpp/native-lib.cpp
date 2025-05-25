#include <jni.h>
#include <string>
#include <android/log.h>

#include "../../../../../TextEncrypterCPP/TextEncrypter.h"

CTextEncrypter Textencrypter;

extern "C"
JNIEXPORT void JNICALL
Java_com_momosoft_imageviewer_MainActivity_setByteData(JNIEnv *env, jobject thiz, jbyteArray data, jint size)
{
    jbyte* bytes = env->GetByteArrayElements(data, nullptr);
    std::uint8_t* ByteData = reinterpret_cast<std::uint8_t*>(bytes);
    Textencrypter.SetByteData(ByteData, static_cast<jsize>(size));
    env->ReleaseByteArrayElements(data, bytes, JNI_ABORT); // or 0 if you modified it
}

extern "C"
JNIEXPORT jstring JNICALL
Java_com_momosoft_imageviewer_MainActivity_getMessage(JNIEnv *env, jobject thiz)
{
    std::u16string text = Textencrypter.ReadTextFromImage();;

    if (text.empty() || text.data() == nullptr) {
        return env->NewStringUTF("");
    }

     return env->NewString(reinterpret_cast<const jchar*>(text.data()), static_cast<jsize>(text.length()));
}
