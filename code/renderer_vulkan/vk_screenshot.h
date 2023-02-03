#ifndef VK_SCREENSHOT_H_
#define VK_SCREENSHOT_H_

#include "../qcommon/q_shared.h"
#include "vulkan/vulkan_core.h"

void R_ScreenShot_f(void);

typedef struct {
	int commandId;
	int width;
	int height;
	unsigned char *captureBuffer;
	unsigned char *encodeBuffer;
	VkBool32 motionJpeg;
} videoFrameCommand_t;

void RB_TakeVideoFrameCmd(const videoFrameCommand_t *cmd);
const void *RB_TakeScreenshotCmd(const void *data);

#endif
