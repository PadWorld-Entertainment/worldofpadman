#ifndef VK_POSTPROCESS_H_
#define VK_POSTPROCESS_H_

#include "vkimpl.h"
#include "../qcommon/q_shared.h"

void VK_PostProcess_Init(void);
void VK_PostProcess_Shutdown(void);

// Renders post-processing effects (greyscale etc.) from the main FBO to the swapchain.
// Returns qtrue if post-processing was applied, qfalse if skipped (no effects active).
qboolean VK_PostProcess_Render(VkCommandBuffer cmdBuf, uint32_t swapchainIdx);

#endif
