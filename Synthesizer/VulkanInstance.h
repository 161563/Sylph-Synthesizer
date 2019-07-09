#pragma once
#include <vulkan/vulkan.h>

namespace renderer
{
	namespace vulkan
	{
		namespace utility
		{
			public class VulkanAppData {
			public:
				char* appName = "";
				int versionMajor = 0;
				int versionMinor = 0;
				int versionPatch = 1;
			};

			enum DebugLevel {
				LEVEL_NONE = 0,
				LEVEL_ERROR = VK_DEBUG_REPORT_ERROR_BIT_EXT,
				LEVEL_WARNINGS = VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT,
				LEVEL_PERFORMANCE_WARNING = VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT | VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT,
				LEVEL_INFORMATION = VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT |
				VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT | VK_DEBUG_REPORT_INFORMATION_BIT_EXT,
				LEVEL_DEBUG = VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT |
				VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT | VK_DEBUG_REPORT_INFORMATION_BIT_EXT |
				VK_DEBUG_REPORT_DEBUG_BIT_EXT
			};

			public class VulkanInstance
			{
			public:
				VulkanInstance(bool debugEnabled, 
					DebugLevel debugLevel, 
					VulkanAppData engineData = {}, 
					VulkanAppData appData = {}, 
					PFN_vkDebugReportCallbackEXT debugCallback = nullptr);
				~VulkanInstance();

				VkInstance GetInstance() noexcept;

			private:
				VkInstance instance;
				VkDebugReportCallbackEXT debugReportCallback;
				bool debugEnabled;
			};

		}
	}
}