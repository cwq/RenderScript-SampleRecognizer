LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

ifeq ($(TARGET_ARCH_ABI),armeabi-v7a)
    LOCAL_CFLAGS += -DHAVE_NEON=1
    LOCAL_ARM_NEON  := true 
    CFLAGS += -mfpu=neon 
endif

LOCAL_MODULE    := recognizer
LOCAL_SRC_FILES := aexp_samplerecognizer_JNIHelper.cpp \
				dtw.rs
LOCAL_LDLIBS    := -lRScpp_static -llog

LOCAL_CFLAGS    += -ffast-math -O2 -funroll-loops

LOCAL_LDFLAGS += -L$(call host-path, $(TARGET_C_INCLUDES)/../lib/rs)
LOCAL_C_INCLUDES += $(TARGET_C_INCLUDES)/rs/cpp
LOCAL_C_INCLUDES += $(TARGET_C_INCLUDES)/rs
LOCAL_C_INCLUDES += $(TARGET_OBJS)/$(LOCAL_MODULE)

include $(BUILD_SHARED_LIBRARY)

include $(CLEAR_VARS)
LOCAL_MODULE := RSSupport
LOCAL_SRC_FILES := $(SYSROOT_LINK)/usr/lib/rs/lib$(LOCAL_MODULE)$(TARGET_SONAME_EXTENSION)
include $(PREBUILT_SHARED_LIBRARY)
