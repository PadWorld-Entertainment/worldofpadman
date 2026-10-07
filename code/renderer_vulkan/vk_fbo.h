#ifndef VK_FBO_H_
#define VK_FBO_H_

#include "vkimpl.h"
#include "../qcommon/q_shared.h"

#define MAX_VK_FBOS 16

typedef struct vk_fbo_s {
	char name[MAX_QPATH];
	int width;
	int height;

	VkImage colorImage;
	VkDeviceMemory colorMemory;
	VkImageView colorView;
	VkFormat colorFormat;

	VkImage depthImage;
	VkDeviceMemory depthMemory;
	VkImageView depthView;
	VkFormat depthFormat;

	VkRenderPass renderPass;
	VkFramebuffer framebuffer;
} vk_fbo_t;

// Create an off-screen render target with color and optional depth
vk_fbo_t *VK_FBO_Create(const char *name, int width, int height, VkFormat colorFormat, qboolean withDepth);

// Destroy an FBO and free all resources
void VK_FBO_Destroy(vk_fbo_t *fbo);

// Initialize the FBO system (creates the main render FBO)
void VK_FBO_Init(void);

// Shutdown the FBO system
void VK_FBO_Shutdown(void);

// Blit one FBO's color attachment to another (or to swapchain if dst is NULL)
void VK_FBO_Blit(VkCommandBuffer cmdBuf, vk_fbo_t *src, VkImage dstImage, VkExtent2D dstExtent);

// Get the main scene render FBO
vk_fbo_t *VK_FBO_GetMain(void);

#endif
