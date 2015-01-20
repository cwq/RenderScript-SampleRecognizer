#include "aexp_samplerecognizer_JNIHelper.h"
#include <algorithm>
#include <time.h>

#include "ScriptC_dtw.h"

/* return current time in milliseconds */
static double now_ms()
{
    struct timespec res;
    clock_gettime(CLOCK_MONOTONIC, &res);
    return 1000.0*res.tv_sec + (double)res.tv_nsec/1e6;
}

int signalCost( int x1, int x2 )
{
    return std::abs( x1-x2 );
}

JNIEXPORT jint JNICALL Java_aexp_samplerecognizer_JNIHelper_nativeDistance
  (JNIEnv *env, jclass jthis, jshortArray jsignal1, jint s1len, jshortArray jsignal2, jint s2len)
{
    short* signal1 = env->GetShortArrayElements(jsignal1, NULL);
    short* signal2 = env->GetShortArrayElements(jsignal2, NULL);
    int* d0 = new int[s1len];
    int* d1 = new int[s1len];
    int s = 0;
    for( int x = 0 ; x < s1len ; ++x ) {
        s += signalCost((int)signal1[x],(int)signal2[0]);
        d0[x] = s;
    }
    s = 0;
    for( int y = 1 ; y < s2len ; ++y ) {
        s += signalCost((int)signal1[0],(int)signal2[y]);
        d1[0] = s;
        for( int x = 1 ; x < s1len ; ++x ) {
            int cs = signalCost( (int)signal1[x],(int)signal2[y]);
            int m = std::min(d0[x-1],d1[x-1] );
            m = std::min( m, d0[x]);
            d1[x] = cs+m;
        }
        for( int x = 0 ; x < s1len ; ++x )
            d0[x] = d1[x];
    }
    int maxc = d1[s1len-1];
    int l = std::max(s1len, s2len);
    maxc /= l;
    env->ReleaseShortArrayElements(jsignal1, signal1, 0);
    env->ReleaseShortArrayElements(jsignal2, signal2, 0);
    return maxc;
}

JNIEXPORT jint JNICALL Java_aexp_samplerecognizer_JNIHelper_nativeDistanceScriptC
  (JNIEnv *env, jclass jthis, jshortArray jsignal1, jint s1len, jshortArray jsignal2, jint s2len, jstring name)
{
    const char* cname = (env)->GetStringUTFChars(name, NULL);
    std::string cacheName(cname);
    (env)->ReleaseStringUTFChars(name, cname);

    short* signal1 = env->GetShortArrayElements(jsignal1, NULL);
    short* signal2 = env->GetShortArrayElements(jsignal2, NULL);

    sp<RS> rsC = new RS();
    rsC->init(cacheName);

    ScriptC_dtw* script = new ScriptC_dtw(rsC);

    script->set_s1len(s1len);
    sp<Allocation> signal1Allocation = Allocation::createSized(
            rsC,
            Element::I16(rsC),
            s1len);
    signal1Allocation->copy1DFrom(signal1);
    script->bind_signal1(signal1Allocation);

    script->set_s2len(s2len);
    sp<Allocation> signal2Allocation = Allocation::createSized(
            rsC,
            Element::I16(rsC),
            s2len);
    signal2Allocation->copy1DFrom(signal2);
    script->bind_signal2(signal2Allocation);

    sp<Allocation> d0Allocation = Allocation::createSized(
            rsC,
            Element::I32(rsC),
            s1len);
    script->bind_d0(d0Allocation);

    sp<Allocation> d1Allocation = Allocation::createSized(
            rsC,
            Element::I32(rsC),
            s1len);
    script->bind_d1(d1Allocation);

    script->invoke_dtw();
    rsC->finish();

    int* result = new int[s1len];
    d1Allocation->copy1DTo(result);
    int maxc = result[s1len-1];
    int l = std::max(s1len, s2len);
    maxc /= l;
    env->ReleaseShortArrayElements(jsignal1, signal1, 0);
    env->ReleaseShortArrayElements(jsignal2, signal2, 0);
    return maxc;
}
