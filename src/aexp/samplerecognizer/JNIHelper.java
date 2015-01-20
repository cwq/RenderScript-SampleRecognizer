package aexp.samplerecognizer;

public class JNIHelper {

    static {
        System.loadLibrary("stlport_shared");
        System.loadLibrary("RSSupport");
        System.loadLibrary("recognizer");
    }
    
    public static native int nativeDistance(short signal1[], int s1len, short signal2[], int s2len);
    
    public static native int nativeDistanceScriptC(short signal1[], int s1len, short signal2[], int s2len, String name);
}
