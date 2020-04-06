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

*JNI: the glue code generator*

Jenny is a java annotation processor, which helps you generate C/C++ code for JNI calls according to your java native class.

# Part I. Guide

When writing JNI code, peope usually come across APIs where java method/field/type signatures are requiered, some of them like `JNIEnv::RegisterNatives`, `JNIEnv::FindClass`, `JNIEnv::GetMethodID`, etc. It is very hard to  hand-craft those signatures correctly and efficiently, so programers often waste much time writing those bioler plate.

Jenny is now our JNI code nanny, who takes care of all those bioler plate so you can be even more productive.

Jenny comes to rescue in two ways:

## 1. call Java API from Native Code

Lets see a very simple example that we are going to call `Runnable::run`, normally we do this:

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

Not only does our code become cleaner, but also more robust and efficient, the `java::RunnableProxy` handles all the `FindClass`, `GetMethodID`, `CallVoidMethod` calls and also cached the found Runnable class reference, so we don't need to call `FindClass` every time.

The following is a even more complex example to show off how life become easier with Jenny.

We gonna implemente the following java code in native code
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

This code make a url connection and read at most 1024 bytes, there are actually lots of method calls.

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

On the other hand, when writting a native method/class in java, we usually use javah (or javac) to generate a corresponding c/c++ header file for the native class.

The generated methods have a very long name and must be exported from you dynamic library (so, dll, dylib). When you want to have clean code, and hide symbols from published libraries. JNI also provides a different way to implmente native method by register then using `JNIEnv::RegisterNatives`. This is an excellent solution, and it's only (and huge) drawback is we have to write method signatures by our self.

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
#pragma once
#include <jni.h>
namespace NativeRuntime {
static constexpr auto FULL_CLASS_NAME = u8"io/github/landerlyoung/jennysampleapp/NativeRuntime";
static constexpr jint RUNTIME_TYPE_MAIN = 1;
static constexpr jint RUNTIME_TYPE_SUB = 1;

jlong JNICALL createInstance(JNIEnv* env, jclass clazz);
void JNICALL post(JNIEnv* env, jobject thiz, jlong instance, jint type, jobject job);

inline bool registerNativeFunctions(JNIEnv* env) {
#define jenny_u8cast(u8) const_cast<char *>(reinterpret_cast<const char *>(u8))
   const JNINativeMethod gsNativeMethods[] = {
       {
           /* method name      */ jenny_u8cast(u8"createInstance"),
           /* method signature */ jenny_u8cast(u8"()J"),
           /* function pointer */ reinterpret_cast<void *>(createInstance)
       },
       {
           /* method name      */ jenny_u8cast(u8"post"),
           /* method signature */ jenny_u8cast(u8"(JILjava/lang/Runnable;)V"),
           /* function pointer */ reinterpret_cast<void *>(post)
       }
   };

   const int gsMethodCount = sizeof(gsNativeMethods) / sizeof(JNINativeMethod);

   bool success = false;
   jclass clazz = env->FindClass(jenny_u8cast(FULL_CLASS_NAME));
   if (clazz != nullptr) {
       success = !env->RegisterNatives(clazz, gsNativeMethods, gsMethodCount);
       env->DeleteLocalRef(clazz);
   }
   return success;
#undef jenny_u8cast
}

} // endof namespace NativeRuntime
```

The generated header 

1. wraps all methods inside a `namespace` named after the java class
2. the functions have the same name as java methods
3. declare all java constant in the header as a `constexpr`
4. generate a `registerNativeFunctions` handles all the bioler plate

# Part II. How To

Jenny comes with two parts
1. the annotation classes
2. the annotatin-processor

[![Download][BT_B]][BT] 👈 click here for latest version.

add dependencies to your gradle config:

```java
dependencies {
    compileOnly "io.github.landerlyoung:jenny-annotation:$version"
    kapt "io.github.landerlyoung:jenny-compiler:$version"
    // for non-kotlin project use:
    // annotationProcessor "io.github.landerlyoung:jenny-compiler:$version"
}
```


### Source structure
source code contains two Annotation classes: `NativeClass` and `NativeSource`, and a Annotation processor.

### How to use it

#### First

Add `@NativeClass()`annotation to you native class in order to help Annotation Processor spot you class, and then generate corresponding cpp source.

You can also add `@NativeSource("Cpp code")` to native method, then the generator will fill you cpp function with given code.

sample:

```java
package com.young.test;

// annotate native class
@NativeClass
public class NativeTest {

    //you can fill simple cpp function with some code
    @NativeSource(
    "jint c = a + b;\n" +
    "return c;")
    public native int add(int a, int b);

    public native void cpp_magic(String s, byte[] data);
}
```


#### Second

Process you java code with Java Annotation Processor.

The Processor class is `io.github.landerlyoung.jenny.JennyAnnotationProcessor`. You can pass the processor to your javac command with switch `-processor`, like `javac -classpath Jenny.jar -d out/ -processor "io.github.landerlyoung.jenny.JennyAnnotationProcessor"  com/young/test/NativeTest.java`.

See test for more details.


If you are using IDEs like IntelliJ IDEA or Eclipse, google it to see how to add annotation processors.

### 2.See it's power

By default, Jenny will generate .h file and .cpp file for each class, and each class has it's own `JNI_OnLoad` and `JNI_OnUnload`. In `JNI_OnLoad`, a function named register_<java class name> function will be called to register native functions. So, if you want integrate them into one dynamic library(.dll in windows, .so in linux/unix, .dylib in OSX), just eliminate those two functions in you cpp, and keep one pair of them in one dynamic library, and remember to gerister your native methods.

here is a sample code generated form java class above:

header file com_young_test_NativeTest.h
```cpp
/* 
 * JNI Header file generated by annotation JNI helper
 * written by landerlyoung@gmail.com
 */

/* C/C++ header file for class com.young.test.NativeTest */
#ifndef _Included_com_young_test_NativeTest
#define _Included_com_young_test_NativeTest

#include <jni.h>

/*
 * Class:     com_young_test_NativeTest
 * Method:    com.young.test.NativeTest::add
 * Signature: (II)I
 */
jint add(JNIEnv *env, jobject thiz, jint a, jint b);

/*
 * Class:     com_young_test_NativeTest
 * Method:    com.young.test.NativeTest::cpp_magic
 * Signature: (Ljava/lang/String;Ljava/lang/RuntimeException;)V
 */
void cpp_magic(JNIEnv *env, jobject thiz, jstring s, jthrowable re);

/*
 * registe Native functions
 */
void register_com_young_test_NativeTest(JNIEnv *env);

#ifdef __cplusplus
extern "C" {
#endif
JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void *reserved);
JNIEXPORT void JNICALL JNI_OnUnload(JavaVM *vm, void *reserved);
#ifdef __cplusplus
}
#endif

#endif
```

cpp file com_young_test_NativeTest.cpp

```cpp
#include "com_young_test_NativeTest.h"

//java class name: com.young.test.NativeTest
#define FULL_CLASS_NAME "com/young/test/NativeTest"
#define constants(cons) com_young_test_NativeTest_ ## cons

/*
 * Class:     com_young_test_NativeTest
 * Method:    com.young.test.NativeTest::add
 * Signature: (II)I
 */
jint add(JNIEnv *env, jobject thiz, jint a, jint b) {
    jint c = a + b;
return c;
}

/*
 * Class:     com_young_test_NativeTest
 * Method:    com.young.test.NativeTest::cpp_magic
 * Signature: (Ljava/lang/String;Ljava/lang/RuntimeException;)V
 */
void cpp_magic(JNIEnv *env, jobject thiz, jstring s, jthrowable re) {
}

static const JNINativeMethod gsNativeMethods[] = {
    {
        const_cast<char *>("add"),
        const_cast<char *>("(II)I"),
        reinterpret_cast<void *>(add)
    }, {
        const_cast<char *>("cpp_magic"),
        const_cast<char *>("(Ljava/lang/String;Ljava/lang/RuntimeException;)V"),
        reinterpret_cast<void *>(cpp_magic)
    }
};
static const int gsMethodCount =
    sizeof(gsNativeMethods) / sizeof(JNINativeMethod); //2

/*
 * registe Native functions
 */
void register_com_young_test_NativeTest(JNIEnv *env) {
    jclass clazz = env->FindClass(FULL_CLASS_NAME);
    env->RegisterNatives(clazz, gsNativeMethods,gsMethodCount);
}

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void *reserved) {
    JNIEnv* env;
    if (vm->GetEnv(reinterpret_cast<void**>(&env),
                JNI_VERSION_1_6) != JNI_OK) {
        return -1;
    }
    register_com_young_test_NativeTest(env);
    return JNI_VERSION_1_6;
}

JNIEXPORT void JNICALL JNI_OnUnload(JavaVM *vm, void *reserved) {
    
}
```

#### have fun with Jenny .^_^.

