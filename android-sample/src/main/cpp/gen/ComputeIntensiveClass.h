/**
 * File generated by Jenny -- https://github.com/LanderlYoung/Jenny
 *
 * DO NOT EDIT THIS FILE.
 *
 * For bug report, please refer to github issue tracker https://github.com/LanderlYoung/Jenny/issues,
 * or contact author landerlyoung@gmail.com.
 */

/* C++ header file for class io/github/landerlyoung/jennysampleapp/ComputeIntensiveClass */
#pragma once

#include <jni.h>

namespace ComputeIntensiveClass {

// DO NOT modify
static constexpr auto FULL_CLASS_NAME = "io/github/landerlyoung/jennysampleapp/ComputeIntensiveClass";

static constexpr auto LOG_TAG = "ComputeIntensiveClass";
static constexpr auto KEY_WHERE_ARE_YOUT_FROM = "where_are_you_from";
static constexpr jint IDEL = -1;
static constexpr jint BUSY = 1;

/*
 * Class:     io_github_landerlyoung_jennysampleapp_ComputeIntensiveClass
 * Method:    public int addInNative(int a, int b)
 * Signature: (II)I
 */
jint JNICALL addInNative(JNIEnv *env, jobject thiz, jint a, jint b);

/*
 * Class:     io_github_landerlyoung_jennysampleapp_ComputeIntensiveClass
 * Method:    public static void computeSomething(byte[] sth)
 * Signature: ([B)V
 */
void JNICALL computeSomething(JNIEnv *env, jclass clazz, jbyteArray sth);

/*
 * Class:     io_github_landerlyoung_jennysampleapp_ComputeIntensiveClass
 * Method:    public static java.lang.String greet()
 * Signature: ()Ljava/lang/String;
 */
jstring JNICALL greet(JNIEnv *env, jclass clazz);

/*
 * Class:     io_github_landerlyoung_jennysampleapp_ComputeIntensiveClass
 * Method:    public final void testParamParse(int a, java.lang.String b, long[] c, float[][] d, java.lang.Exception e, java.lang.Class<java.lang.String> f, java.util.HashMap<?,?> g)
 * Signature: (ILjava/lang/String;[J[[FLjava/lang/Exception;Ljava/lang/Class;Ljava/util/HashMap;)V
 */
void JNICALL testParamParse(JNIEnv *env, jobject thiz, jint a, jstring b, jlongArray c, jobjectArray d, jthrowable e, jclass f, jobject g);

/*
 * Class:     io_github_landerlyoung_jennysampleapp_ComputeIntensiveClass
 * Method:    public static long returnsLong()
 * Signature: ()J
 */
jlong JNICALL returnsLong(JNIEnv *env, jclass clazz);

/*
 * Class:     io_github_landerlyoung_jennysampleapp_ComputeIntensiveClass
 * Method:    public static boolean returnsBool()
 * Signature: ()Z
 */
jboolean JNICALL returnsBool(JNIEnv *env, jclass clazz);

/*
 * Class:     io_github_landerlyoung_jennysampleapp_ComputeIntensiveClass
 * Method:    public static java.lang.Object returnsObject()
 * Signature: ()Ljava/lang/Object;
 */
jobject JNICALL returnsObject(JNIEnv *env, jclass clazz);

/*
 * Class:     io_github_landerlyoung_jennysampleapp_ComputeIntensiveClass
 * Method:    public int computeThenCallback(io.github.landerlyoung.jennysampleapp.Callback listener)
 * Signature: (Lio/github/landerlyoung/jennysampleapp/Callback;)I
 */
jint JNICALL computeThenCallback(JNIEnv *env, jobject thiz, jobject listener);

/**
* register Native functions
* @returns success or not
*/
inline bool registerNativeFunctions(JNIEnv *env) {
   const JNINativeMethod gsNativeMethods[] = {
       {
           /* method name      */ const_cast<char *>("addInNative"),
           /* method signature */ const_cast<char *>("(II)I"),
           /* function pointer */ reinterpret_cast<void *>(addInNative)
       },
       {
           /* method name      */ const_cast<char *>("computeSomething"),
           /* method signature */ const_cast<char *>("([B)V"),
           /* function pointer */ reinterpret_cast<void *>(computeSomething)
       },
       {
           /* method name      */ const_cast<char *>("greet"),
           /* method signature */ const_cast<char *>("()Ljava/lang/String;"),
           /* function pointer */ reinterpret_cast<void *>(greet)
       },
       {
           /* method name      */ const_cast<char *>("testParamParse"),
           /* method signature */ const_cast<char *>("(ILjava/lang/String;[J[[FLjava/lang/Exception;Ljava/lang/Class;Ljava/util/HashMap;)V"),
           /* function pointer */ reinterpret_cast<void *>(testParamParse)
       },
       {
           /* method name      */ const_cast<char *>("returnsLong"),
           /* method signature */ const_cast<char *>("()J"),
           /* function pointer */ reinterpret_cast<void *>(returnsLong)
       },
       {
           /* method name      */ const_cast<char *>("returnsBool"),
           /* method signature */ const_cast<char *>("()Z"),
           /* function pointer */ reinterpret_cast<void *>(returnsBool)
       },
       {
           /* method name      */ const_cast<char *>("returnsObject"),
           /* method signature */ const_cast<char *>("()Ljava/lang/Object;"),
           /* function pointer */ reinterpret_cast<void *>(returnsObject)
       },
       {
           /* method name      */ const_cast<char *>("computeThenCallback"),
           /* method signature */ const_cast<char *>("(Lio/github/landerlyoung/jennysampleapp/Callback;)I"),
           /* function pointer */ reinterpret_cast<void *>(computeThenCallback)
       }
   };
   const int gsMethodCount =
       sizeof(gsNativeMethods) / sizeof(JNINativeMethod);

   bool success = false;
   jclass clazz = env->FindClass(FULL_CLASS_NAME);
   if (clazz != nullptr) {
       success = 0 == env->RegisterNatives(clazz, gsNativeMethods, gsMethodCount);
       env->DeleteLocalRef(clazz);
   }
   return success;
}

} //endof namespace ComputeIntensiveClass
