#include "vk_fbo.h"
#include "vk_instance.h"
#include "vk_image.h"
#include "vk_cmd.h"
#include "tr_config.h"
#include "ref_import.h"

static vk_fbo_t s_fbos[MAX_VK_FBOS];
static int s_numFBOs = 0;
static vk_fbo_t *s_mainFBO = NULL;

static VkImage vk_createFBOImage(int width, int height, VkFormat format, VkImageUsageFlags usage,
								 VkDeviceMemory *outMemory) {
	VkImage image;
	VkImageCreateInfo imgInfo = {0};
	VkMemoryRequirements memReqs;
	VkMemoryAllocateInfo allocInfo = {0};

	imgInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
	imgInfo.imageType = VK_IMAGE_TYPE_2D;
	imgInfo.format = format;
	imgInfo.extent.width = width;
	imgInfo.extent.height = height;
	imgInfo.extent.depth = 1;
	imgInfo.mipLevels = 1;
	imgInfo.arrayLayers = 1;
	imgInfo.samples = VK_SAMPLE_COUNT_1_BIT;
	imgInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
	imgInfo.usage = usage;
	imgInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
	imgInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;

	VK_CHECK(qvkCreateImage(vk.device, &imgInfo, NULL, &image));

	qvkGetImageMemoryRequirements(vk.device, image, &memReqs);

	allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	allocInfo.allocationSize = memReqs.size;
	allocInfo.memoryTypeIndex = find_memory_type(memReqs.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

	VK_CHECK(qvkAllocateMemory(vk.device, &allocInfo, NULL, outMemory));
	VK_CHECK(qvkBindImageMemory(vk.device, image, *outMemory, 0));

	return image;
}

static VkImageView vk_createFBOImageView(VkImage image, VkFormat format, VkImageAspectFlags aspect) {
	VkImageView view;
	VkImageViewCreateInfo viewInfo = {0};

	viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	viewInfo.image = image;
	viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
	viewInfo.format = format;
	viewInfo.subresourceRange.aspectMask = aspect;
	viewInfo.subresourceRange.baseMipLevel = 0;
	viewInfo.subresourceRange.levelCount = 1;
	viewInfo.subresourceRange.baseArrayLayer = 0;
	viewInfo.subresourceRange.layerCount = 1;

	VK_CHECK(qvkCreateImageView(vk.device, &viewInfo, NULL, &view));
	return view;
}

static VkRenderPass vk_createFBORenderPass(VkFormat colorFormat, VkFormat depthFormat, qboolean withDepth) {
	VkRenderPass renderPass;
	VkAttachmentDescription attachments[2];
	VkAttachmentReference colorRef;
	VkAttachmentReference depthRef;
	VkSubpassDescription subpass = {0};
	VkSubpassDependency deps[2];
	VkRenderPassCreateInfo rpInfo = {0};

	// Color attachment
	attachments[0].flags = 0;
	attachments[0].format = colorFormat;
	attachments[0].samples = VK_SAMPLE_COUNT_1_BIT;
	attachments[0].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	attachments[0].storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	attachments[0].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	attachments[0].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	attachments[0].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	attachments[0].finalLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

	colorRef.attachment = 0;
	colorRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	// Depth attachment
	if (withDepth) {
		attachments[1].flags = 0;
		attachments[1].format = depthFormat;
		attachments[1].samples = VK_SAMPLE_COUNT_1_BIT;
		attachments[1].loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		attachments[1].storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		attachments[1].stencilLoadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		attachments[1].stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		attachments[1].initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		attachments[1].finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

		depthRef.attachment = 1;
		depthRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
	}

	subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
	subpass.colorAttachmentCount = 1;
	subpass.pColorAttachments = &colorRef;
	subpass.pDepthStencilAttachment = withDepth ? &depthRef : NULL;

	// Dependencies
	deps[0].srcSubpass = VK_SUBPASS_EXTERNAL;
	deps[0].dstSubpass = 0;
	deps[0].srcStageMask = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
	deps[0].dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
	deps[0].srcAccessMask = VK_ACCESS_SHADER_READ_BIT;
	deps[0].dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
	deps[0].dependencyFlags = 0;

	deps[1].srcSubpass = 0;
	deps[1].dstSubpass = VK_SUBPASS_EXTERNAL;
	deps[1].srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	deps[1].dstStageMask = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
	deps[1].srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
	deps[1].dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
	deps[1].dependencyFlags = 0;

	rpInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	rpInfo.attachmentCount = withDepth ? 2 : 1;
	rpInfo.pAttachments = attachments;
	rpInfo.subpassCount = 1;
	rpInfo.pSubpasses = &subpass;
	rpInfo.dependencyCount = 2;
	rpInfo.pDependencies = deps;

	VK_CHECK(qvkCreateRenderPass(vk.device, &rpInfo, NULL, &renderPass));
	return renderPass;
}

vk_fbo_t *VK_FBO_Create(const char *name, int width, int height, VkFormat colorFormat, qboolean withDepth) {
	vk_fbo_t *fbo;
	VkFramebufferCreateInfo fbInfo = {0};
	VkImageView fbAttachments[2];

	if (s_numFBOs >= MAX_VK_FBOS) {
		ri.Error(ERR_DROP, "VK_FBO_Create: MAX_VK_FBOS hit");
	}

	fbo = &s_fbos[s_numFBOs++];
	memset(fbo, 0, sizeof(*fbo));
	Q_strncpyz(fbo->name, name, sizeof(fbo->name));
	fbo->width = width;
	fbo->height = height;
	fbo->colorFormat = colorFormat;
	fbo->depthFormat = vk.fmt_DepthStencil;

	// Color image
	fbo->colorImage = vk_createFBOImage(width, height, colorFormat,
		VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT,
		&fbo->colorMemory);
	fbo->colorView = vk_createFBOImageView(fbo->colorImage, colorFormat, VK_IMAGE_ASPECT_COLOR_BIT);

	// Depth image
	if (withDepth) {
		fbo->depthImage = vk_createFBOImage(width, height, fbo->depthFormat,
			VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT,
			&fbo->depthMemory);
		fbo->depthView = vk_createFBOImageView(fbo->depthImage, fbo->depthFormat,
			VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT);
	}

	// Render pass
	fbo->renderPass = vk_createFBORenderPass(colorFormat, fbo->depthFormat, withDepth);

	// Framebuffer
	fbAttachments[0] = fbo->colorView;
	fbAttachments[1] = fbo->depthView;

	fbInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
	fbInfo.renderPass = fbo->renderPass;
	fbInfo.attachmentCount = withDepth ? 2 : 1;
	fbInfo.pAttachments = fbAttachments;
	fbInfo.width = width;
	fbInfo.height = height;
	fbInfo.layers = 1;

	VK_CHECK(qvkCreateFramebuffer(vk.device, &fbInfo, NULL, &fbo->framebuffer));

	ri.Printf(PRINT_DEVELOPER, "VK_FBO_Create: '%s' %dx%d\n", name, width, height);
	return fbo;
}

void VK_FBO_Destroy(vk_fbo_t *fbo) {
	if (!fbo)
		return;

	if (fbo->framebuffer != VK_NULL_HANDLE)
		qvkDestroyFramebuffer(vk.device, fbo->framebuffer, NULL);
	if (fbo->renderPass != VK_NULL_HANDLE)
		qvkDestroyRenderPass(vk.device, fbo->renderPass, NULL);
	if (fbo->colorView != VK_NULL_HANDLE)
		qvkDestroyImageView(vk.device, fbo->colorView, NULL);
	if (fbo->colorImage != VK_NULL_HANDLE)
		qvkDestroyImage(vk.device, fbo->colorImage, NULL);
	if (fbo->colorMemory != VK_NULL_HANDLE)
		qvkFreeMemory(vk.device, fbo->colorMemory, NULL);
	if (fbo->depthView != VK_NULL_HANDLE)
		qvkDestroyImageView(vk.device, fbo->depthView, NULL);
	if (fbo->depthImage != VK_NULL_HANDLE)
		qvkDestroyImage(vk.device, fbo->depthImage, NULL);
	if (fbo->depthMemory != VK_NULL_HANDLE)
		qvkFreeMemory(vk.device, fbo->depthMemory, NULL);

	memset(fbo, 0, sizeof(*fbo));
}

void VK_FBO_Blit(VkCommandBuffer cmdBuf, vk_fbo_t *src, VkImage dstImage, VkExtent2D dstExtent) {
	VkImageMemoryBarrier barrier = {0};
	VkImageBlit region = {0};

	// Transition src color from SHADER_READ_ONLY to TRANSFER_SRC
	barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
	barrier.srcAccessMask = VK_ACCESS_SHADER_READ_BIT;
	barrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
	barrier.oldLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
	barrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
	barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.image = src->colorImage;
	barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	barrier.subresourceRange.levelCount = 1;
	barrier.subresourceRange.layerCount = 1;
	qvkCmdPipelineBarrier(cmdBuf, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT,
						  0, 0, NULL, 0, NULL, 1, &barrier);

	// Transition dst from UNDEFINED to TRANSFER_DST
	barrier.srcAccessMask = 0;
	barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
	barrier.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	barrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
	barrier.image = dstImage;
	qvkCmdPipelineBarrier(cmdBuf, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT,
						  0, 0, NULL, 0, NULL, 1, &barrier);

	// Blit
	region.srcOffsets[1].x = src->width;
	region.srcOffsets[1].y = src->height;
	region.srcOffsets[1].z = 1;
	region.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	region.srcSubresource.layerCount = 1;
	region.dstOffsets[1].x = dstExtent.width;
	region.dstOffsets[1].y = dstExtent.height;
	region.dstOffsets[1].z = 1;
	region.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	region.dstSubresource.layerCount = 1;

	qvkCmdBlitImage(cmdBuf, src->colorImage, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
					dstImage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region, VK_FILTER_LINEAR);

	// Transition dst to PRESENT_SRC
	barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
	barrier.dstAccessMask = VK_ACCESS_MEMORY_READ_BIT;
	barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
	barrier.newLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
	barrier.image = dstImage;
	qvkCmdPipelineBarrier(cmdBuf, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT,
						  0, 0, NULL, 0, NULL, 1, &barrier);

	// Transition src back to SHADER_READ_ONLY for next frame
	barrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
	barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
	barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
	barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
	barrier.image = src->colorImage;
	qvkCmdPipelineBarrier(cmdBuf, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
						  0, 0, NULL, 0, NULL, 1, &barrier);
}

void VK_FBO_Init(void) {
	int width, height;
	R_GetWinResolution(&width, &height);

	s_mainFBO = VK_FBO_Create("main", width, height, vk.surface_format.format, qtrue);
	ri.Printf(PRINT_ALL, "VK_FBO_Init: main render FBO %dx%d\n", width, height);
}

void VK_FBO_Shutdown(void) {
	int i;
	for (i = 0; i < s_numFBOs; i++) {
		VK_FBO_Destroy(&s_fbos[i]);
	}
	s_numFBOs = 0;
	s_mainFBO = NULL;
}

vk_fbo_t *VK_FBO_GetMain(void) {
	return s_mainFBO;
}
