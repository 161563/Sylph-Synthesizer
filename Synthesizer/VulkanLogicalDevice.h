#pragma once
#include <vulkan/vulkan.h>

#include "VulkanPhysicalDevice.h"

namespace renderer
{
	namespace vulkan
	{
		namespace utility
		{

			class VulkanLogicalDevice
			{
			public:
				VulkanLogicalDevice(VulkanPhysicalDevice* vulkanPhysicalDevice, bool debug, std::vector<const char*> extensions, VulkanDeviceFeatures_t requestedDeviceFeatures);
				~VulkanLogicalDevice();

				VkDevice GetDevice();

				VulkanPhysicalDevice* getPhysicalDevice();

				VkQueue GetGraphicsQueue();
				VkQueue GetComputeQueue();
				VkQueue GetPresentQueue();

			protected:
				VkDevice device;

				VulkanPhysicalDevice* vulkanPhysicalDevice;

				VkQueue graphicsQueue;
				VkQueue computeQueue;
				VkQueue presentQueue;
			};

		}
	}
}