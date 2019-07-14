#pragma once
#pragma comment(lib, "vulkan-1.lib")
#include <vulkan/vulkan.h>

#include "VulkanInstance.h"
#include "VulkanPhysicalDevice.h"
#include "VulkanLogicalDevice.h"

namespace renderer
{
	namespace vulkan
	{
#ifdef NDEBUG
		const bool enableValidationLayers = false;
#else
		const bool enableValidationLayers = true;
#endif

		public class VulkanRenderer
		{
		public:
			VulkanRenderer(System::Windows::Forms::Control^ target_control);
			~VulkanRenderer();

		protected:
			utility::VulkanInstance* instance;
			utility::VulkanPhysicalDevice* physical_device;
			utility::VulkanLogicalDevice* logical_device;

			VkSurfaceKHR surface;

			VkSwapchainKHR swapchain;
			VkFormat swapchain_format;
			VkExtent2D swapchain_extent;

			uint32_t swapchain_image_count;
			std::vector<VkImage> swapchain_images;
			std::vector<VkImageView> swapchain_image_views;

			VkCommandPool graphics_command_pool;
		};

	}

}