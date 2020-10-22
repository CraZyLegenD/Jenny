/**
 * File generated by Jenny -- https://github.com/LanderlYoung/Jenny
 *
 * For bug report, please refer to github issue tracker https://github.com/LanderlYoung/Jenny/issues,
 * or contact author landerlyoung@gmail.com.
 */
#include <CallbackProxy.h>
#include <NestedClassProxy.h>
#include "ComputeIntensiveClass.h"

#include "gen/jenny_fusion_proxies.h"

/*
 * Class:     io_github_landerlyoung_jennysampleapp_ComputeIntensiveClass
 * Method:    public int addInNative(int a, int b)
 * Signature: (II)I
 */
jint ComputeIntensiveClass::addInNative(JNIEnv* env, jobject thiz, jint a, jint b) {
    jint c = a + b;
    return c;
}

/*
 * Class:     io_github_landerlyoung_jennysampleapp_ComputeIntensiveClass
 * Method:    public static void computeSomething(byte[] sth)
 * Signature: ([B)V
 */
void ComputeIntensiveClass::computeSomething(JNIEnv *env, jclass clazz, jbyteArray sth) {
    return;
}

/*
 * Class:     io_github_landerlyoung_jennysampleapp_ComputeIntensiveClass
 * Method:    public static java.lang.String greet()
 * Signature: ()Ljava/lang/String;
 */
jstring ComputeIntensiveClass::greet(JNIEnv *env, jclass clazz) {
    return env->NewStringUTF("Hello From Jenny");
}

/*
 * Class:     io_github_landerlyoung_jennysampleapp_ComputeIntensiveClass
 * Method:    public final void testParamParse(int a, java.lang.String b, long[] c, float[][] d, java.lang.Exception e, java.lang.Class<java.lang.String> f, java.util.HashMap<?,?> g)
 * Signature: (ILjava/lang/String;[J[[FLjava/lang/Exception;Ljava/lang/Class;Ljava/util/HashMap;)V
 */
void ComputeIntensiveClass::testParamParse(JNIEnv *env, jobject thiz, jint a, jstring b, jlongArray c, jobjectArray d, jthrowable e, jclass f, jobject g) {
    return;
}

/*
 * Class:     io_github_landerlyoung_jennysampleapp_ComputeIntensiveClass
 * Method:    public static long returnsLong()
 * Signature: ()J
 */
jlong ComputeIntensiveClass::returnsLong(JNIEnv *env, jclass clazz) {
    return 0;
}

/*
 * Class:     io_github_landerlyoung_jennysampleapp_ComputeIntensiveClass
 * Method:    public static boolean returnsBool()
 * Signature: ()Z
 */
jboolean ComputeIntensiveClass::returnsBool(JNIEnv *env, jclass clazz) {
    return JNI_FALSE;
}

/*
 * Class:     io_github_landerlyoung_jennysampleapp_ComputeIntensiveClass
 * Method:    public static java.lang.Object returnsObject()
 * Signature: ()Ljava/lang/Object;
 */
jobject ComputeIntensiveClass::returnsObject(JNIEnv *env, jclass clazz) {
    return nullptr;
}

/*
 * Class:     io.github.landerlyoung.jennysampleapp.ComputeIntensiveClass
 * Method:    public static void testOverload()
 * Signature: ()V
 */
void ComputeIntensiveClass::testOverload__(JNIEnv *env, jclass clazz) {

}

/*
 * Class:     io.github.landerlyoung.jennysampleapp.ComputeIntensiveClass
 * Method:    public static void testOverload(int i)
 * Signature: (I)V
 */
void ComputeIntensiveClass::testOverload__I(JNIEnv *env, jclass clazz, jint i) {

}

/*
 * Class:     io_github_landerlyoung_jennysampleapp_ComputeIntensiveClass
 * Method:    public static java.lang.String httpGet(java.lang.String url)
 * Signature: (Ljava/lang/String;)Ljava/lang/String;
 */
jstring JNICALL ComputeIntensiveClass::httpGet(JNIEnv *env, jclass clazz, jstring _url) {
    using namespace java::okhttp;

    jenny::LocalRef<jstring> url(_url, false);

    OkHttpClientProxy client = OkHttpClientProxy::newInstance();
    BuilderProxy builder = BuilderProxy::newInstance().url(url);
    RequestProxy request = builder.build();
    CallProxy call = client.newCall(request.getThis());
    ResponseProxy response = call.execute();
    ResponseBodyProxy body = response.body();
    return body.string().release();


    /*
     * equivalent java code to {@link ComputeIntensiveClass#httpGet(String)}

        String run(String url) throws IOException {
         OkHttpClient client = new OkHttpClient();
          Request request = new Request.Builder()
              .url(url)
              .build();

          Response response = client.newCall(request).execute();
          return response.body().string();
        }

     */
}

void ComputeIntensiveClass::runJniHelperTest(JNIEnv* env, jclass clazz) {
    jenny::internal::jniHelperUnitTest(env);
}

/*
 * Class:     io_github_landerlyoung_jennysampleapp_ComputeIntensiveClass
 * Method:    public int computeThenCallback(io.github.landerlyoung.jennysampleapp.Callback listener)
 * Signature: (Lio/github/landerlyoung/jennysampleapp/Callback;)I
 */
jint ComputeIntensiveClass::computeThenCallback(JNIEnv* env, jobject thiz, jobject listener) {
    CallbackProxy::onJobStart(env, listener);

    auto name = CallbackProxy::getName(env, listener);


    auto newInstance = CallbackProxy::newInstance(env);
    CallbackProxy::setLock(env, listener, newInstance);
    CallbackProxy::onJobProgress(env, listener, 20);

    auto nestedClass = NestedClassProxy::newInstance(env, listener);
    CallbackProxy::setLock(env, newInstance, nestedClass);
    CallbackProxy::onJobProgress(env, listener, 50);

    CallbackProxy::setAStaticField(env, nullptr);

    CallbackProxy::setCount(env, listener, 100);
    CallbackProxy::setLock(env, listener, listener);
    CallbackProxy::onJobProgress(env, listener, 100);

    jstring str = env->NewStringUTF("Yes, callback from jni");
    CallbackProxy::onJobDone(env, listener, JNI_TRUE, str);

    env->DeleteLocalRef(name);
    env->DeleteLocalRef(str);

    env->DeleteLocalRef(newInstance);
    env->DeleteLocalRef(nestedClass);
    return 0;
}




