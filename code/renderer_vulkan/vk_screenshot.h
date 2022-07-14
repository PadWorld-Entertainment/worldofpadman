#ifndef VK_SCREENSHOT_H_
#define VK_SCREENSHOT_H_

#include "../qcommon/q_shared.h"
#include "vulkan/vulkan_core.h"

void R_ScreenShotJPEG_f(void);
void R_ScreenShotPNG_f(void);
void R_ScreenShotTGA_f(void);

typedef enum {
	ST_TGA,
	ST_JPEG,
	ST_PNG
} screenshotType_e;

typedef struct {
	int commandId;
	int x;
	int y;
	int width;
	int height;
	char *fileName;

	screenshotType_e type;
} screenshotCommand_t;

typedef struct {
	int commandId;
	int width;
	int height;
	unsigned char *captureBuffer;
	unsigned char *encodeBuffer;
	VkBool32 motionJpeg;
} videoFrameCommand_t;

void RB_TakeVideoFrameCmd(const videoFrameCommand_t *const cmd);
const void *RB_TakeScreenshotCmd(const void *data);

#endif
