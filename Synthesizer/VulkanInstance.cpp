#include "VulkanInstance.h"

#include <iostream>
#include <vector>
#include <sstream>

#include <Windows.h>

#include "Utility.h"


using namespace System::Windows::Forms;

namespace renderer
{
	namespace vulkan
	{
		namespace utility
		{

			static VkResult CreateDebugReportCallbackEXT(VkInstance instance, const VkDebugReportCallbackCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugReportCallbackEXT* pCallback) {
				auto func = (PFN_vkCreateDebugReportCallbackEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugReportCallbackEXT");
				if (func != nullptr) {
					return func(instance, pCreateInfo, pAllocator, pCallback);
				}
				else {
					return VK_ERROR_EXTENSION_NOT_PRESENT;
				}
			}
			static void DestroyDebugReportCallbackEXT(VkInstance instance, VkDebugReportCallbackEXT callback, const VkAllocationCallbacks* pAllocator) {
				auto func = (PFN_vkDestroyDebugReportCallbackEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugReportCallbackEXT");
				if (func != nullptr) {
					func(instance, callback, pAllocator);
				}
			}

			static VKAPI_ATTR VkBool32 VKAPI_CALL defaultDebugCallback(
				VkDebugReportFlagsEXT flags,
				VkDebugReportObjectTypeEXT objType,
				uint64_t obj,
				size_t location,
				int32_t code,
				const char* layerPrefix,
				const char* msg,
				void* userData) {
				std::wstringstream stream;
				switch (flags) {
				case VK_DEBUG_REPORT_INFORMATION_BIT_EXT:
					stream << L"[INFO] ";
					break;
				case VK_DEBUG_REPORT_WARNING_BIT_EXT:
					stream << L"[WARNING] ";
					break;
				case VK_DEBUG_REPORT_PERFORMANCE_WARNING_BIT_EXT:
					stream << L"[PERFORMANCE] ";
					break;
				case VK_DEBUG_REPORT_ERROR_BIT_EXT:
					stream << L"[ERROR] ";
					break;
				case VK_DEBUG_REPORT_DEBUG_BIT_EXT:
					stream << L"[DEBUG] ";
					break;
				}
				stream << L"[LAYER:" << ToWString(layerPrefix) << L"] " <<
					L"[CODE:" << std::to_wstring(code) << L"] " <<
					L"[OBJ:" << std::to_wstring(obj) << L"] [LOCATION:" <<
					std::to_wstring(location) <<
					L"] " << ToWString(msg) << std::endl;

				OutputDebugString(stream.str().c_str());

				if (flags == VK_DEBUG_REPORT_ERROR_BIT_EXT)
				{
					System::String^ out = gcnew System::String(stream.str().c_str());
					MessageBox::Show(out, L"Sylph Synthesizer - Graphics Error");
				}

				return VK_FALSE;
			}

			VulkanInstance::VulkanInstance(bool debugEnabled,
				DebugLevel debugLevel,
				VulkanAppData engineData,
				VulkanAppData appData,
				PFN_vkDebugReportCallbackEXT debugCallback)
			{
				VkApplicationInfo appInfo = {};
				appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
				appInfo.pEngineName = engineData.appName;
				appInfo.engineVersion = VK_MAKE_VERSION(engineData.versionMajor, engineData.versionMinor, engineData.versionPatch);
				appInfo.pApplicationName = appData.appName;
				appInfo.applicationVersion = VK_MAKE_VERSION(appData.versionMajor, appData.versionMinor, appData.versionPatch);
				appInfo.apiVersion = VK_API_VERSION_1_0;

				VkInstanceCreateInfo createInfo = {};
				createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
				createInfo.pApplicationInfo = &appInfo;

				std::vector<const char*> extensions;
				if (debugEnabled) {
					extensions.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
					extensions.push_back(VK_KHR_SURFACE_EXTENSION_NAME);
					extensions.push_back("VK_KHR_win32_surface");
				}

				createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
				createInfo.ppEnabledExtensionNames = extensions.data();

				std::vector<const char*> layers;
				if (debugEnabled) {
					layers.push_back("VK_LAYER_LUNARG_standard_validation");

					//layers.push_back("VK_LAYER_LUNARG_monitor");
				}

				createInfo.enabledLayerCount = static_cast<uint32_t>(layers.size());
				createInfo.ppEnabledLayerNames = layers.data();

				VkResult res = vkCreateInstance(&createInfo, nullptr, &instance);
				if (res != VK_SUCCESS) {
					std::string s = "[ERROR] [CODE:" + std::to_string(res) + "] Creating vulkan instance failed";
					OutputDebugString(ToWString(s).c_str());
					System::String^ error = gcnew System::String(s.c_str());
					MessageBox::Show(error, L"Sylph Synthesizer - Graphics Error");
					return;
				}

				this->debugEnabled = (debugLevel == DebugLevel::LEVEL_NONE) ? false : debugEnabled;

				if (this->debugEnabled) {
					VkDebugReportCallbackCreateInfoEXT debugCreateInfo = {};
					debugCreateInfo.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT;
					debugCreateInfo.flags = static_cast<uint32_t>(debugLevel);
					debugCreateInfo.pfnCallback = (debugCallback == nullptr) ? defaultDebugCallback : debugCallback;

					VkResult res = CreateDebugReportCallbackEXT(instance, &debugCreateInfo, nullptr, &debugReportCallback);
				}
			}

			VulkanInstance::~VulkanInstance()
			{
				if (debugEnabled)
					DestroyDebugReportCallbackEXT(instance, debugReportCallback, nullptr);
				vkDestroyInstance(instance, nullptr);
			}

			VkInstance VulkanInstance::GetInstance() noexcept
			{
				return instance;
			}

		}
	}
}