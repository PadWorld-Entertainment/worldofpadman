#ifndef VK_SHADERS_H_
#define VK_SHADERS_H_

#include "vulkan/vulkan_core.h"

#define GLS_SRCBLEND_ZERO 0x00000001
#define GLS_SRCBLEND_ONE 0x00000002
#define GLS_SRCBLEND_DST_COLOR 0x00000003
#define GLS_SRCBLEND_ONE_MINUS_DST_COLOR 0x00000004
#define GLS_SRCBLEND_SRC_ALPHA 0x00000005
#define GLS_SRCBLEND_ONE_MINUS_SRC_ALPHA 0x00000006
#define GLS_SRCBLEND_DST_ALPHA 0x00000007
#define GLS_SRCBLEND_ONE_MINUS_DST_ALPHA 0x00000008
#define GLS_SRCBLEND_ALPHA_SATURATE 0x00000009
#define GLS_SRCBLEND_BITS 0x0000000f

#define GLS_DSTBLEND_ZERO 0x00000010
#define GLS_DSTBLEND_ONE 0x00000020
#define GLS_DSTBLEND_SRC_COLOR 0x00000030
#define GLS_DSTBLEND_ONE_MINUS_SRC_COLOR 0x00000040
#define GLS_DSTBLEND_SRC_ALPHA 0x00000050
#define GLS_DSTBLEND_ONE_MINUS_SRC_ALPHA 0x00000060
#define GLS_DSTBLEND_DST_ALPHA 0x00000070
#define GLS_DSTBLEND_ONE_MINUS_DST_ALPHA 0x00000080
#define GLS_DSTBLEND_BITS 0x000000f0

#define GLS_DEPTHMASK_TRUE 0x00000100

#define GLS_POLYMODE_LINE 0x00001000

#define GLS_DEPTHTEST_DISABLE 0x00010000
#define GLS_DEPTHFUNC_EQUAL 0x00020000

#define GLS_ATEST_GT_0 0x10000000
#define GLS_ATEST_LT_80 0x20000000
#define GLS_ATEST_GE_80 0x40000000
#define GLS_ATEST_BITS 0x70000000

#define GLS_DEFAULT GLS_DEPTHMASK_TRUE

enum Vk_Shader_Type { ST_SINGLE_TEXTURE, ST_MULTI_TEXURE_MUL, ST_MULTI_TEXURE_ADD };

void vk_loadShaderModules(void);
void vk_specifyShaderModule(const enum Vk_Shader_Type shader_type, const VkBool32 isClippingPlane, VkShaderModule *vs,
							VkShaderModule *fs);
void vk_destroyShaderModules(void);

#endif
