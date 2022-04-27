#include "vulkan/vulkan.h"
#include "glfw3.h"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE

#include "glm.hpp"

#include <cstring>
#include <iostream>
#include <vector>
const std::vector<const char *> validation_layers = {"VK_LAYER_KHRONOS_validation"};
#ifdef NDEBUG
	const bool enableValidationLayers = false;
#else
	const bool enableValidationLayers = true;
#endif


bool is_device_okay(VkPhysicalDevice device) {
	return true;
}

bool checkValidationLayerSupport() {
	uint32_t layerCount;
	vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
	std::vector<VkLayerProperties> availableLayers(layerCount);
	vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

	for(const char *l_name : validation_layers) {
		bool lay_found = false;
		for(const auto& lay_prop : availableLayers) {
			std::cout << lay_prop.layerName << "\n";
			if(strcmp(lay_prop.layerName, l_name) == 0) {
				lay_found = true;
				break;
			}
		}

		if(!lay_found) {
			return false;
		}
	}

	return true;
}

class VulkanComponents {
	public: 
		void create_instance() 
		{
			unsigned int glfw_ext_count = 0;
			const char **glfw_ext = glfwGetRequiredInstanceExtensions(&glfw_ext_count);

			VkApplicationInfo app_info{};
			app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
			app_info.pApplicationName = "Hello Triangle";
			app_info.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
			app_info.pEngineName = "No Engine";
			app_info.engineVersion = VK_MAKE_VERSION(1, 0, 0);
			app_info.apiVersion = VK_API_VERSION_1_0;

			VkInstanceCreateInfo create_info{};
			create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
			create_info.pApplicationInfo = &app_info;
			create_info.enabledExtensionCount = glfw_ext_count;
			create_info.ppEnabledExtensionNames = glfw_ext;
			create_info.enabledLayerCount = 0;

			if(enableValidationLayers) {
				create_info.enabledLayerCount = static_cast<uint32_t>(validation_layers.size());
				create_info.ppEnabledLayerNames = validation_layers.data();
			}

			vkCreateInstance(&create_info, 0, &instance);
		}

		void pick_device()
		{
			unsigned int dev_count = 0;
			vkEnumeratePhysicalDevices(instance, &dev_count, 0);
			if(dev_count == 0) {
				throw std::runtime_error("No card!!");
			}

			std::vector<VkPhysicalDevice> devices(dev_count);
			vkEnumeratePhysicalDevices(instance, &dev_count, devices.data());

			for(const auto &device : devices) {
				if(is_device_okay(device)) {
					phys = device;
					break;
				}
			}

			if(phys == VK_NULL_HANDLE) {
				throw std::runtime_error("no card!!");
			}
		}

		void cleanup()
		{
			vkDestroyInstance(instance, 0);
		}

	private:
		VkInstance instance;
		VkPhysicalDevice phys = VK_NULL_HANDLE;
};



int main()
{
	VulkanComponents components;
	unsigned int ext_count = 0;
	VkResult res;

	glfwInit();
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

	GLFWwindow *wind = glfwCreateWindow(800, 600, "VLKN", 0, 0);
	vkEnumerateInstanceExtensionProperties(0, &ext_count, 0);

	if(enableValidationLayers && !checkValidationLayerSupport()) {
		throw std::runtime_error("Couldn't find validation layers!");
	}

	components.create_instance();

	while(!glfwWindowShouldClose(wind)) {
		glfwPollEvents();
	}

	components.cleanup();

	glfwDestroyWindow(wind);
	glfwTerminate();

	return 0;
}
