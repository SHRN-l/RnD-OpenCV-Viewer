#include <string>
#include <jni.h>
// Add this line to test
#include <opencv2/core.hpp>

extern "C" JNIEXPORT jstring JNICALL
Java_com_sharan_rnd_1opencv_1viewer_MainActivity_stringFromJNI(
        JNIEnv* env,
        jobject /* this */) {

    // Add this line to test
    cv::Mat testMat;

    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}