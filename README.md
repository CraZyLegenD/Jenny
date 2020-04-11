# Jenny

[![CI][CI_B]][CI]  [![Publish][PUB_B]][PUB] [![Download][BT_B]][BT] ![GitHub code size in bytes][CS_B] ![GitHub][LC_B]

[CI_B]: https://github.com/LanderlYoung/Jenny/workflows/Android%20CI/badge.svg
[CI]: https://github.com/LanderlYoung/Jenny/actions?workflow=Android+CI
[PUB_B]: https://github.com/LanderlYoung/Jenny/workflows/Publish/badge.svg
[PUB]: https://github.com/LanderlYoung/Jenny/actions?workflow=Publish
[BT_B]: https://api.bintray.com/packages/landerlyoung/maven/jenny-annotation/images/download.svg
[BT]: https://bintray.com/landerlyoung/maven
[CS_B]: https://img.shields.io/github/languages/code-size/LanderlYoung/Jenny
[LC_B]: https://img.shields.io/github/license/LanderlYoung/Jenny

**JNI glue code generator**

Jenny is a java annotation processor, which helps you generate C/C++ code for JNI calls according to your java native class.

# Part I. Guide

When writing JNI code, people usually come across APIs where java method/field/type signatures are required, some of them like `JNIEnv::RegisterNatives`, `JNIEnv::FindClass`, `JNIEnv::GetMethodID`, etc. It is very hard to hand-craft those signatures correctly and efficiently, so programmers often waste much time writing those boilerplate.

Jenny is now our JNI code nanny, who takes care of all those boilerplate so you can be even more productive.

Jenny comes to rescue in two ways:

## 1. call Java API from Native Code

Let's see a very simple example that we are going to call `Runnable::run`, normally we do this:

```cpp
void callRunnable(JNIEnv* env, jobject runnable) {
    jclass RunnableClass = env->FindClass("java/lang/Runnable");
    jmethodID run = env->GetMethodID(RunnableClass, "run", "()V");
    env->CallVoidMethod(runnable, run);
    env->DeleteLocalRef(RunnableClass);
}
```
the above code is actuallu simplified, there should be error-handling code.

with jenny, we just do:

```cpp
void callRunnable(JNIEnv* env, jobject runnable) {
    java::RunnableProxy runnableProxy(env, runnable);
    runnableProxy.run();
}
```

Not only does our code become cleaner, but also more robust and efficient. The `java::RunnableProxy` handles all the `FindClass`, `GetMethodID`, `CallVoidMethod` calls and also cached the found Runnable class reference, so we don't need to call `FindClass` every time.

The following is an even more complex example to show off how life becomes easier with Jenny.

We gonna implement the following java code in native code
```java
String httpGet(String url) throws IOException {
    URL u = new URL(url);
    URLConnection conn = u.openConnection();
    InputStream input = conn.getInputStream();

    byte[] buffer = new byte[1024];
    int len = input.read(buffer);
    input.close();

    return new String(buffer, 0, len);
}
```

This code makes a URL connection and read at most 1024 bytes, there are actually lots of method calls.

```cpp
jstring httpGet(JNIEnv *env, jstring url) {
    auto url = URLProxy::newInstance(env, url);
    URLConnectionProxy conn(env, url.openConnection());
    InputStreamProxy input(env, conn.getInputStream());

    jbyteArray buffer = env->NewByteArray(1024);
    jint len = input.read(buffer);
    input.close();

    return reinterpret_cast<jstring>(*StringProxy::newInstance(env, buffer, 0, len));
}
```

As you can see, with the help of Jenny, the native code is actually almost identically to the java code.

And without Jenny, it looks like [nightmare](https://gist.github.com/LanderlYoung/1a203f519ba5f91b38c1d81534d63664#file-withoutjenny-cpp).

## 2. Java Native methods

On the other hand, when writing a native method/class in java, we usually use javah (or javac) to generate a corresponding c/c++ header file for the native class.

The generated methods have a very long name and must be exported from your dynamic library (so, dll, dylib). When you want to have clean code and hide symbols from published libraries. JNI also provides a different way to implement native method by register then using `JNIEnv::RegisterNatives`. This is an excellent solution, and it's only (and huge) drawback is we have to write method signatures by our self.

Take a look at the following example:

```java
@NativeClass
class NativeRuntime {
    public static final int RUNTIME_TYPE_MAIN = 1;
    public static final int RUNTIME_TYPE_SUB = 1;
    static native long createInstance();
    native void post(long instance, int type, Runnable job);
}
```

The code is written in `kotlin` but very much like java in that is replace the `native` keyword with `extern`.

The generated header looks like this:

```cpp
namespace NativeRuntime {
static constexpr auto FULL_CLASS_NAME = u8"io/github/landerlyoung/jennysampleapp/NativeRuntime";
static constexpr jint RUNTIME_TYPE_MAIN = 1;
static constexpr jint RUNTIME_TYPE_SUB = 1;

jlong JNICALL createInstance(JNIEnv* env, jclass clazz);
void JNICALL post(JNIEnv* env, jobject thiz, jlong instance, jint type, jobject job);

inline bool registerNativeFunctions(JNIEnv* env) {
   const JNINativeMethod gsNativeMethods[] = {
       { u8"createInstance", u8"()J", createInstance },
       { u8"post", u8"(JILjava/lang/Runnable;)V", post }
   };
   const int gsMethodCount = sizeof(gsNativeMethods) / sizeof(JNINativeMethod);
   jclass clazz = env->FindClass(FULL_CLASS_NAME);
   return !env->RegisterNatives(clazz, gsNativeMethods, gsMethodCount);
} } // endof namespace NativeRuntime
```

The generated header 

1. wraps all methods inside a `namespace` named after the java class
2. the functions have the same name as java methods
3. declare all java constant in the header as a `constexpr`
4. generate a `registerNativeFunctions` handles all the boilerplate

# Part II. How To

Jenny comes with two component
1. the annotation classes
2. the annotation-processor

[![Download][BT_B]][BT] 👈👈👈 click here for latest version.

add dependencies to your Gradle config:

```java
dependencies {
    compileOnly "io.github.landerlyoung:jenny-annotation:$version"
    kapt "io.github.landerlyoung:jenny-compiler:$version"
    // for non-kotlin project use:
    // annotationProcessor "io.github.landerlyoung:jenny-compiler:$version"
}
```

## @NativeProxy
When calling Java API from Native Code, you should annotate that class with `@NativeProxy` annotation. Than jenny-compiler will generate proxy file for taht class, which you can copy to your c++ source directory and use them.

```java
@NativeProxy(namespace = "jni")
class Callback {
    public native void onComplete(String message);
}}
```

Then build your project, Jenny will generate `.h`/`.cpp` files on the default gen source dir, for Android gradle project it's `build/generated/source`.
Or int the log you wold see something start with "Jenny |".