#include "vk_screenshot.h"
#include "tr_globals.h"
#include "vk_cmd.h"
#include "tr_cvar.h"
#include "vk_image.h"
#include "vk_instance.h"

#include "tr_imageprocess.h"
#include "tr_config.h"
#include "ref_import.h"
#include "render_export.h"

#include "../renderercommon/tr_shared.h"

/*
==============================================================================

						SCREEN SHOTS

NOTE TTimo
some thoughts about the screenshots system:
screenshots get written in fs_homepath + fs_gamedir
vanilla q3 .. baseq3/screenshots/ *.tga
team arena .. missionpack/screenshots/ *.tga

two commands: "screenshot" and "screenshotJPEG"
we use statics to store a count and start writing the first screenshot/screenshot????.tga (.jpg) available
(with FS_FileExists / FS_FOpenFileWrite calls)
FIXME: the statics don't get a reinit between fs_game changes


Images created with tiling equal to VK_IMAGE_TILING_LINEAR have further restrictions on their
limits and capabilities compared to images created with tiling equal to VK_IMAGE_TILING_OPTIMAL.
Creation of images with tiling VK_IMAGE_TILING_LINEAR may not be supported unless other parameters
meetall of the constraints:
* imageType is VK_IMAGE_TYPE_2D
* format is not a depth/stencil format
* mipLevels is 1
* arrayLayers is 1
* samples is VK_SAMPLE_COUNT_1_BIT
* usage only includes VK_IMAGE_USAGE_TRANSFER_SRC_BIT and/or VK_IMAGE_USAGE_TRANSFER_DST_BIT
Implementations may support additional limits and capabilities beyond those listed above.

==============================================================================

*/

static void RB_FlipFrameBuffer(unsigned char *pBuf, const uint32_t w, const uint32_t h) {
	const uint32_t a_row = w * 4;
	const uint32_t nLines = h / 2;

	unsigned char *pTmp = (unsigned char *)ri.Hunk_AllocateTempMemory(a_row);
	unsigned char *pSrc = pBuf;
	unsigned char *pDst = pBuf + w * (h - 1) * 4;

	uint32_t j = 0;
	for (j = 0; j < nLines; j++) {
		memcpy(pTmp, pSrc, a_row);
		memcpy(pSrc, pDst, a_row);
		memcpy(pDst, pTmp, a_row);

		pSrc += a_row;
		pDst -= a_row;
	}

	ri.Hunk_FreeTempMemory(pTmp);
}

// Just reading the pixels for the GPU MEM, don't care about swizzling
static byte *RB_ReadPixels(uint32_t width, uint32_t height) {
	const uint32_t sizeFB = width * height * 4;
	byte *pBuf = (byte *)ri.Hunk_AllocateTempMemory(sizeFB);
	ri.Printf(PRINT_DEVELOPER, "read %dx%d pixels from GPU\n", width, height);
	VkBuffer buffer;
	VkDeviceMemory memory;
	VkBufferImageCopy image_copy;
	VkImageMemoryBarrier image_barrier;
	VkCommandBuffer cmdBuf;
	VkCommandBufferAllocateInfo alloc_info;
	VkCommandBufferBeginInfo begin_info;
	VkSubmitInfo submit_info;
	unsigned char *data;

	qvkDeviceWaitIdle(vk.device);

	// Create image in host visible memory to serve as a destination for framebuffer pixels.
	{
		VkBufferCreateInfo buffer_create_info;
		VkMemoryRequirements memory_requirements;
		VkMemoryAllocateInfo memory_allocate_info;

		memset(&buffer_create_info, 0, sizeof(buffer_create_info));
		buffer_create_info.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		buffer_create_info.size = sizeFB;
		buffer_create_info.usage = VK_BUFFER_USAGE_TRANSFER_DST_BIT;
		VK_CHECK(qvkCreateBuffer(vk.device, &buffer_create_info, NULL, &buffer));

		qvkGetBufferMemoryRequirements(vk.device, buffer, &memory_requirements);

		memset(&memory_allocate_info, 0, sizeof(memory_allocate_info));
		memory_allocate_info.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		memory_allocate_info.allocationSize = memory_requirements.size;
		//
		memory_allocate_info.memoryTypeIndex =
			find_memory_type(memory_requirements.memoryTypeBits,
							 VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

		VK_CHECK(qvkAllocateMemory(vk.device, &memory_allocate_info, NULL, &memory));
		VK_CHECK(qvkBindBufferMemory(vk.device, buffer, memory, 0));
	}

	{
		image_copy.bufferOffset = 0;
		image_copy.bufferRowLength = width;
		image_copy.bufferImageHeight = height;

		image_copy.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		image_copy.imageSubresource.layerCount = 1;
		image_copy.imageSubresource.mipLevel = 0;
		image_copy.imageSubresource.baseArrayLayer = 0;
		image_copy.imageOffset.x = 0;
		image_copy.imageOffset.y = 0;
		image_copy.imageOffset.z = 0;
		image_copy.imageExtent.width = width;
		image_copy.imageExtent.height = height;
		image_copy.imageExtent.depth = 1;
	}

	// Memory barriers are used to explicitly control access to buffer and image subresource ranges.
	// Memory barriers are used to transfer ownership between queue families, change image layouts,
	// and define availability and visibility operations. They explicitly define the access types
	// and buffer and image subresource ranges that are included in the access scopes of a memory
	// dependency that is created by a synchronization command that includes them.
	//
	// Image memory barriers only apply to memory accesses involving a specific image subresource
	// range. That is, a memory dependency formed from an image memory barrier is scoped to access
	// via the specified image subresource range. Image memory barriers can also be used to define
	// image layout transitions or a queue family ownership transfer for the specified image
	// subresource range.

	{
		image_barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
		image_barrier.pNext = NULL;
		image_barrier.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT;
		image_barrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
		image_barrier.oldLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
		image_barrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
		image_barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		image_barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
		image_barrier.image = vk.swapchain_images_array[vk.idx_swapchain_image];
		image_barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		image_barrier.subresourceRange.baseMipLevel = 0;
		image_barrier.subresourceRange.levelCount = 1;
		image_barrier.subresourceRange.baseArrayLayer = 0;
		image_barrier.subresourceRange.layerCount = 1;
	}

	// read pixel with command buffer
	alloc_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	alloc_info.pNext = NULL;
	alloc_info.commandPool = vk.command_pool;
	alloc_info.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	alloc_info.commandBufferCount = 1;
	VK_CHECK(qvkAllocateCommandBuffers(vk.device, &alloc_info, &cmdBuf));

	begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	begin_info.pNext = NULL;
	begin_info.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
	begin_info.pInheritanceInfo = NULL;
	VK_CHECK(qvkBeginCommandBuffer(cmdBuf, &begin_info));

	qvkCmdPipelineBarrier(cmdBuf, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0, 0,
						  NULL, 0, NULL, 1, &image_barrier);
	qvkCmdCopyImageToBuffer(cmdBuf, vk.swapchain_images_array[vk.idx_swapchain_image],
							VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, buffer, 1, &image_copy);
	VK_CHECK(qvkEndCommandBuffer(cmdBuf));

	submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submit_info.pNext = NULL;
	submit_info.waitSemaphoreCount = 0;
	submit_info.pWaitSemaphores = NULL;
	submit_info.pWaitDstStageMask = NULL;
	submit_info.commandBufferCount = 1;
	submit_info.pCommandBuffers = &cmdBuf;
	submit_info.signalSemaphoreCount = 0;
	submit_info.pSignalSemaphores = NULL;
	VK_CHECK(qvkQueueSubmit(vk.queue, 1, &submit_info, VK_NULL_HANDLE));

	VK_CHECK(qvkQueueWaitIdle(vk.queue));

	qvkFreeCommandBuffers(vk.device, vk.command_pool, 1, &cmdBuf);

	// Memory objects created with the memory property VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT
	// are considered mappable. Memory objects must be mappable in order to be successfully
	// mapped on the host.
	//
	// To retrieve a host virtual address pointer to a region of a mappable memory object
	VK_CHECK(qvkMapMemory(vk.device, memory, 0, VK_WHOLE_SIZE, 0, (void **)&data));
	memcpy(pBuf, data, sizeFB);
	qvkUnmapMemory(vk.device, memory);
	qvkFreeMemory(vk.device, memory, NULL);
	qvkDestroyBuffer(vk.device, buffer, NULL);

	RB_FlipFrameBuffer(pBuf, width, height);

	// Remove alpha channel and rbg <-> bgr
	{
		unsigned char *pSrc = pBuf;
		unsigned char *pDst = pBuf;

		uint32_t i;
		for (i = 0; i < width * height; i++) {
			pSrc[0] = pDst[2];
			pSrc[1] = pDst[1];
			pSrc[2] = pDst[0];
			pSrc += 3;
			pDst += 4;
		}
	}
	return pBuf;
}

extern size_t RE_SaveJPGToBuffer(byte *buffer, size_t bufSize, int quality, int image_width, int image_height,
								 byte *image_buffer, int padding);

extern void RE_SaveJPG(const char *filename, int quality, int image_width, int image_height,
					   unsigned char *image_buffer, int padding);
extern void RE_SavePNG(const char *filename, int width, int height, byte *data, int padding);
extern void RE_SaveTGA(const char *filename, int image_width, int image_height, byte *image_buffer, int padding);

static void RB_TakeScreenshotJPEG(int width, int height, const char *fileName) {
	byte *pImg = RB_ReadPixels(width, height);
	RE_SaveJPG(fileName, r_screenshotJpegQuality->integer, width, height, pImg, 0);
	ri.Hunk_FreeTempMemory(pImg);
}

static void RB_TakeScreenshotPNG(int width, int height, const char *fileName) {
	byte *pImg = RB_ReadPixels(width, height);
	RE_SavePNG(fileName, width, height, pImg, 0);
	ri.Hunk_FreeTempMemory(pImg);
}

static void RB_TakeScreenshotTGA(int width, int height, const char *fileName) {
	byte *pImg = RB_ReadPixels(width, height);
	RE_SaveTGA(fileName, width, height, pImg, 0);
	ri.Hunk_FreeTempMemory(pImg);
}

const void *RB_TakeScreenshotCmd(const void *data) {
	const screenshotCommand_t *cmd;

	cmd = (const screenshotCommand_t *)data;

	if (cmd->type == ST_TGA)
		RB_TakeScreenshotTGA(cmd->width, cmd->height, cmd->fileName);
	else if (cmd->type == ST_JPEG)
		RB_TakeScreenshotJPEG(cmd->width, cmd->height, cmd->fileName);
	else if (cmd->type == ST_PNG)
		RB_TakeScreenshotPNG(cmd->width, cmd->height, cmd->fileName);

	return (const void *)(cmd + 1);
}

static void R_TakeScreenshot(int x, int y, int width, int height, char *name, screenshotType_e type) {
	static char fileName[MAX_OSPATH] = {0}; // bad things if two screenshots per frame?

	screenshotCommand_t *cmd = (screenshotCommand_t *)R_GetCommandBuffer(sizeof(*cmd));
	if (!cmd) {
		return;
	}
	cmd->commandId = RC_SCREENSHOT;

	cmd->x = x;
	cmd->y = y;
	cmd->width = width;
	cmd->height = height;

	// Q_strncpyz( fileName, name, sizeof(fileName) );

	strncpy(fileName, name, sizeof(fileName));

	cmd->fileName = fileName;
	cmd->type = type;
}

/*
====================
R_LevelShot

levelshots are specialized 128*128 thumbnails for the
menu system, sampled down from full screen distorted images
====================
*/
static void R_LevelShot(screenshotType_e type, const char *ext) {
	char fileName[MAX_OSPATH];
	byte *source;
	int width, height;
	int arg;

	// Allow custom resample width/height
	arg = atoi(ri.Cmd_Argv(2));
	if (arg > 0)
		width = height = arg;
	else
		width = height = 128;

	Com_sprintf(fileName, sizeof(fileName), "levelshots/%s%s", tr.world->baseName, ext);

	source = RB_ReadPixels(width, height);

	if (type == ST_TGA)
		RE_SaveTGA(fileName, width, height, source, 0);
	else if (type == ST_JPEG)
		RE_SaveJPG(fileName, r_screenshotJpegQuality->integer, width, height, source, 0);
	else if (type == ST_PNG)
		RE_SavePNG(fileName, width, height, source, 0);

	ri.Hunk_FreeTempMemory(source);

	ri.Printf(PRINT_ALL, "Wrote %s\n", fileName);
}

static void R_Screenshot(screenshotType_e type) {
	char checkname[MAX_OSPATH];
	static int lastNumber = -1;
	qboolean silent;

	int width;
	int height;

	static const char *exts[] = {".tga", ".jpg", ".png"};

	if (!strcmp(ri.Cmd_Argv(1), "levelshot")) {
		R_LevelShot(type, exts[type]);
		return;
	}

	if (!strcmp(ri.Cmd_Argv(1), "silent")) {
		silent = qtrue;
	} else {
		silent = qfalse;
	}

	if (ri.Cmd_Argc() == 2 && !silent) {
		// explicit filename
		snprintf(checkname, MAX_OSPATH, "screenshots/%s%s", ri.Cmd_Argv(1), exts[type]);
	} else {
		// scan for a free filename

		// if we have saved a previous screenshot, don't scan again,
		// because recording demo avis can involve thousands of shots
		if (lastNumber == -1) {
			lastNumber = 0;
		}
		// scan for a free number
		for (; lastNumber <= 9999; lastNumber++) {
			R_ScreenshotFilename(lastNumber, checkname, sizeof(checkname), exts[type]);
			if (!ri.FS_FileExists(checkname)) {
				break; // file doesn't exist
			}
		}

		if (lastNumber >= 9999) {
			ri.Printf(PRINT_ALL, "ScreenShot: Couldn't create a file\n");
			return;
		}

		lastNumber++;
	}

	R_GetWinResolution(&width, &height);
	R_TakeScreenshot(0, 0, width, height, checkname, type);

	if (!silent) {
		ri.Printf(PRINT_ALL, "Wrote %s\n", checkname);
	}
}

/*
==================
R_ScreenShot_f

screenshot
screenshot [silent]
screenshot [levelshot]
screenshot [filename]

Doesn't print the pacifier message if there is a second arg
==================
*/
void R_ScreenShotTGA_f(void) {
	R_Screenshot(ST_TGA);
}

void R_ScreenShotJPEG_f(void) {
	R_Screenshot(ST_JPEG);
}

void R_ScreenShotPNG_f(void) {
	R_Screenshot(ST_PNG);
}

void RB_TakeVideoFrameCmd(const videoFrameCommand_t *cmd) {
	size_t memcount, linelen;
	int padwidth, avipadwidth, padlen, avipadlen;
	unsigned char *pImg;

	linelen = cmd->width * 3;

	// Alignment stuff for glReadPixels
	padwidth = PAD(linelen, 4);
	padlen = padwidth - linelen;
	// AVI line padding
	avipadwidth = PAD(linelen, 4);
	avipadlen = avipadwidth - linelen;

	pImg = RB_ReadPixels(cmd->width, cmd->height);

	memcount = padwidth * cmd->height;

	if (cmd->motionJpeg) {
		const uint32_t cnPixels = cmd->width * cmd->height;
		unsigned char *pSrc = pImg;
		const unsigned char *pDst = pImg;
		uint32_t i;
		for (i = 0; i < cnPixels; i++) {
			pSrc[0] = pDst[2];
			pSrc[1] = pDst[1];
			pSrc[2] = pDst[0];
			pSrc += 3;
			pDst += 4;
		}

		memcount = RE_SaveJPGToBuffer(cmd->encodeBuffer, linelen * cmd->height, r_aviMotionJpegQuality->integer,
									  cmd->width, cmd->height, pImg, padlen);

		ri.CL_WriteAVIVideoFrame(cmd->encodeBuffer, memcount);
	} else {
		byte *lineend, *memend;
		byte *srcptr, *destptr;

		srcptr = pImg;
		destptr = cmd->encodeBuffer;
		memend = srcptr + memcount;

		// swap R and B and remove line paddings
		while (srcptr < memend) {
			lineend = srcptr + linelen;
			while (srcptr < lineend) {
				*destptr++ = srcptr[2];
				*destptr++ = srcptr[1];
				*destptr++ = srcptr[0];
				srcptr += 3;
			}

			Com_Memset(destptr, '\0', avipadlen);
			destptr += avipadlen;

			srcptr += padlen;
		}
		ri.CL_WriteAVIVideoFrame(cmd->encodeBuffer, avipadwidth * cmd->height);
	}

	ri.Hunk_FreeTempMemory(pImg);
}

void RE_TakeVideoFrame(int width, int height, unsigned char *captureBuffer, unsigned char *encodeBuffer,
					   qboolean motionJpeg) {
	videoFrameCommand_t *cmd;
	if (!tr.registered) {
		return;
	}

	cmd = R_GetCommandBuffer(sizeof(*cmd));
	if (!cmd) {
		return;
	}

	cmd->commandId = RC_VIDEOFRAME;

	cmd->width = width;
	cmd->height = height;
	cmd->captureBuffer = captureBuffer;
	cmd->encodeBuffer = encodeBuffer;
	cmd->motionJpeg = motionJpeg;
}
