#include "VulkanRenderer.h"

#include <Windows.h>

#include <vulkan/vulkan_win32.h>

#include <string>

using namespace System::Windows::Forms;

namespace renderer
{
	namespace vulkan
	{

		VulkanRenderer::VulkanRenderer(System::Windows::Forms::Control^ target_control)
		{
			instance = new utility::VulkanInstance(enableValidationLayers, utility::DebugLevel::LEVEL_INFORMATION);

			VkWin32SurfaceCreateInfoKHR surface_info = {};
			surface_info.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
			HWND hwnd = (HWND)target_control->Handle.ToPointer();
			surface_info.hwnd = hwnd;
			surface_info.hinstance = GetModuleHandle(NULL);

			if (vkCreateWin32SurfaceKHR(instance->GetInstance(), &surface_info, nullptr, &surface) != VK_SUCCESS)
			{
				MessageBox::Show(L"[ERROR] Failed to create surface", L"Sylph Synthesizer - Graphics Error");
			}

			utility::VulkanDeviceFeatures_t required_features = {};
			required_features.depthBiasClamp = 1.f;
			required_features.depthBounds = 1.f;
			required_features.depthClamp = 1.f;
			required_features.fillModeNonSolid = 1.f;
			required_features.samplerAnisotropy = 1.f;
			required_features.wideLines = 1.f;
			required_features.largePoints = 1.f;
			required_features.variableMultisampleRate = 1.f;
			required_features.geometryShader = 1.f;


			utility::VulkanDeviceQueueRequestedFamilies_t requestedQueueFamilies = {};
			requestedQueueFamilies.graphics = true;
			requestedQueueFamilies.compute = true;
			requestedQueueFamilies.present = true;

			physical_device = utility::VulkanPhysicalDevice::GetBestPhysicalDevice(
				instance, surface, { VK_KHR_SWAPCHAIN_EXTENSION_NAME }, required_features,
				{}, requestedQueueFamilies, true);

			if (physical_device == nullptr)
			{
				MessageBox::Show(L"[ERROR] Physical device selection failed", L"Sylph Synthesizer - Graphics Error");
			}

			logical_device = new utility::VulkanLogicalDevice(physical_device,
				enableValidationLayers,
				{ VK_KHR_SWAPCHAIN_EXTENSION_NAME },
				required_features);

			utility::VulkanSwapChainSupportDetails_t support_details = physical_device->GetSwapChainSupportDetails();

			VkSurfaceFormatKHR surface_format = physical_device->ChooseSwapSurfaceFormat(support_details.formats);
			VkPresentModeKHR present_mode = physical_device->ChooseSwapPresentMode(support_details.presentModes);
			VkExtent2D extent = physical_device->chooseSwapExtent(support_details.capabilities, target_control->Width, target_control->Height);

			uint32_t image_count = support_details.capabilities.minImageCount + 1;
			if (support_details.capabilities.maxImageCount > 0 && image_count > support_details.capabilities.maxImageCount)
			{
				image_count = support_details.capabilities.maxImageCount;
			}

			VkSwapchainCreateInfoKHR swapchain_create_info = {};
			swapchain_create_info.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
			swapchain_create_info.surface = surface;
			swapchain_create_info.minImageCount = image_count;
			swapchain_create_info.imageFormat = surface_format.format;
			swapchain_create_info.imageColorSpace = surface_format.colorSpace;
			swapchain_create_info.imageExtent = extent;
			swapchain_create_info.imageArrayLayers = 1;
			swapchain_create_info.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

			utility::VulkanDeviceQueueFamilies_t queue_families = physical_device->GetQueueFamilies();
			uint32_t queueFamilyIndices[] = { (uint32_t)queue_families.graphics, (uint32_t)queue_families.present };

			if (queue_families.graphics != queue_families.present) {
				swapchain_create_info.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
				swapchain_create_info.queueFamilyIndexCount = 2;
				swapchain_create_info.pQueueFamilyIndices = queueFamilyIndices;
			}
			else {
				swapchain_create_info.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
				swapchain_create_info.queueFamilyIndexCount = 0;
				swapchain_create_info.pQueueFamilyIndices = nullptr;
			}

			swapchain_create_info.preTransform = support_details.capabilities.currentTransform;
			swapchain_create_info.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
			swapchain_create_info.presentMode = present_mode;
			swapchain_create_info.clipped = VK_TRUE;
			swapchain_create_info.oldSwapchain = VK_NULL_HANDLE;

			VkResult res = vkCreateSwapchainKHR(logical_device->GetDevice(), &swapchain_create_info, nullptr, &swapchain);

			if (res != VK_SUCCESS)
			{
				MessageBox::Show(L"[ERROR] Swapchain creation failed", L"Sylph Synthesizer - Graphics Error");
			}

			this->swapchain_format = surface_format.format;
			this->swapchain_extent = extent;

			swapchain_image_count = 0;
			vkGetSwapchainImagesKHR(logical_device->GetDevice(), swapchain, &swapchain_image_count, nullptr);
			swapchain_images.resize(swapchain_image_count);
			vkGetSwapchainImagesKHR(logical_device->GetDevice(), swapchain, &swapchain_image_count, swapchain_images.data());

			swapchain_image_views.resize(swapchain_image_count);
			for (int i = 0; i < swapchain_image_count; ++i)
			{
				VkImageViewCreateInfo info = {};
				info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
				info.image = swapchain_images[i];
				info.viewType = VK_IMAGE_VIEW_TYPE_2D;
				info.format = swapchain_format;
				info.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
				info.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
				info.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
				info.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
				info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
				info.subresourceRange.baseMipLevel = 0;
				info.subresourceRange.levelCount = 1;
				info.subresourceRange.baseArrayLayer = 0;
				info.subresourceRange.layerCount = 1;

				VkResult res = vkCreateImageView(logical_device->GetDevice(), &info, nullptr, &swapchain_image_views[i]);

				if (res != VK_SUCCESS)
				{
					MessageBox::Show(L"[ERROR] Creating swapchain image views failed", L"Sylph Synthesizer - Graphics Error");
				}
			}

			VkCommandPoolCreateInfo graphics_command_pool_create_info = {};
			graphics_command_pool_create_info.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
			graphics_command_pool_create_info.queueFamilyIndex = physical_device->GetQueueFamilies().graphics;
			graphics_command_pool_create_info.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

			VkResult res = vkCreateCommandPool(logical_device->GetDevice(), &graphics_command_pool_create_info, nullptr, &graphics_command_pool);
			if (res != VK_SUCCESS) {
				std::string s = "[ERROR] [CODE:" + std::to_string(res) + "] Graphics command pool creation failed";
				System::String^ out = gcnew System::String(s.c_str());
				MessageBox::Show(out, L"Sylph Synthesizer - Graphics Error");
			}
		}

		VulkanRenderer::~VulkanRenderer()
		{
			vkDestroyCommandPool(logical_device->GetDevice(), graphics_command_pool, nullptr);
			for (auto& image_view : swapchain_image_views)
			{
				vkDestroyImageView(logical_device->GetDevice(), image_view, nullptr);
			}
			vkDestroySwapchainKHR(logical_device->GetDevice(), swapchain, nullptr);
			delete logical_device;
			delete physical_device;
			vkDestroySurfaceKHR(instance->GetInstance(), surface, nullptr);
			delete instance;
		}

	}
}