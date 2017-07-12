LOCAL_PATH:= $(call my-dir)
include $(CLEAR_VARS)

LOCAL_SRC_FILES:=\
	main.c
	#i2c_ioctl_test.c

LOCAL_CFLAGS := -O2 -g


LOCAL_MODULE_PATH := $(TARGET_OUT_OPTIONAL_EXECUTABLES)

LOCAL_MODULE_TAGS := optional

LOCAL_MODULE := i2c_ioctl_test

include $(BUILD_EXECUTABLE)
