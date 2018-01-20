//#include "kimgui/module.hpp"
//
//namespace kimgui {
//	const char* getClipboardText(void* user_data);
//	void setClipboardText(void* user_data, const char* text);
//
//	Bool32 isInited = ICIMGUI_FALSE;
//	std::shared_ptr<GLFWwindow> pCurrWindow = nullptr;
//
//	void static moduleCreate()
//	{
//		if (isInited == KGS_TRUE) return;
//
//		//Keyboard mapping. ImGui will use those indices to peek into the io.KeyDown[] array
//		ImGuiIO& io = ImGui::GetIO();
//		io.KeyMap[ImGuiKey_Tab] = GLFW_KEY_TAB;                         // Keyboard mapping. ImGui will use those indices to peek into the io.KeyDown[] array.
//		io.KeyMap[ImGuiKey_LeftArrow] = GLFW_KEY_LEFT;
//		io.KeyMap[ImGuiKey_RightArrow] = GLFW_KEY_RIGHT;
//		io.KeyMap[ImGuiKey_UpArrow] = GLFW_KEY_UP;
//		io.KeyMap[ImGuiKey_DownArrow] = GLFW_KEY_DOWN;
//		io.KeyMap[ImGuiKey_PageUp] = GLFW_KEY_PAGE_UP;
//		io.KeyMap[ImGuiKey_PageDown] = GLFW_KEY_PAGE_DOWN;
//		io.KeyMap[ImGuiKey_Home] = GLFW_KEY_HOME;
//		io.KeyMap[ImGuiKey_End] = GLFW_KEY_END;
//		io.KeyMap[ImGuiKey_Delete] = GLFW_KEY_DELETE;
//		io.KeyMap[ImGuiKey_Backspace] = GLFW_KEY_BACKSPACE;
//		io.KeyMap[ImGuiKey_Enter] = GLFW_KEY_ENTER;
//		io.KeyMap[ImGuiKey_Escape] = GLFW_KEY_ESCAPE;
//		io.KeyMap[ImGuiKey_A] = GLFW_KEY_A;
//		io.KeyMap[ImGuiKey_C] = GLFW_KEY_C;
//		io.KeyMap[ImGuiKey_V] = GLFW_KEY_V;
//		io.KeyMap[ImGuiKey_X] = GLFW_KEY_X;
//		io.KeyMap[ImGuiKey_Y] = GLFW_KEY_Y;
//		io.KeyMap[ImGuiKey_Z] = GLFW_KEY_Z;
//
//		io.RenderDrawListsFn = nullptr;           // Alternatively you can set this to NULL and call ImGui::GetDrawData() after ImGui::Render() to get the same ImDrawData pointer.
//		io.SetClipboardTextFn = setClipboardText;
//		io.GetClipboardTextFn = getClipboardText;
//		io.ClipboardUserData = nullptr;
//
//
//		/*if (install_callbacks)
//		{
//			glfwSetMouseButtonCallback(window, ImGui_ImplGlfwVulkan_MouseButtonCallback);
//			glfwSetScrollCallback(window, ImGui_ImplGlfwVulkan_ScrollCallback);
//			glfwSetKeyCallback(window, ImGui_ImplGlfwVulkan_KeyCallback);
//			glfwSetCharCallback(window, ImGui_ImplGlfwVulkan_CharCallback);
//		}
//
//		ImGui_ImplGlfwVulkan_CreateDeviceObjects();*/
//
//		//Indicate module was initialized.
//		isInited = ICIMGUI_TRUE;
//	}
//
//	void moduleDestory()
//	{
//
//		isInited = ICIMGUI_FALSE;
//	}
//
//	std::shared_ptr<GLFWwindow> getCurrWindow()
//	{
//		return pCurrWindow;
//	}
//
//	void setCurrWindow(std::shared_ptr<GLFWwindow> pWindow)
//	{
//		pCurrWindow = pWindow;
//
//		/*glfwSetMouseButtonCallback(window, ImGui_ImplGlfwVulkan_MouseButtonCallback);
//		glfwSetScrollCallback(window, ImGui_ImplGlfwVulkan_ScrollCallback);
//		glfwSetKeyCallback(window, ImGui_ImplGlfwVulkan_KeyCallback);
//		glfwSetCharCallback(window, ImGui_ImplGlfwVulkan_CharCallback);*/
//	}
//
//	const char* getClipboardText(void* user_data)
//	{
//		if (pCurrWindow == nullptr)
//		{
//			throw std::runtime_error("Current using window don't exist!");
//		}
//		return glfwGetClipboardString(pCurrWindow.get());
//	}
//
//	void setClipboardText(void* user_data, const char* text)
//	{
//		if (pCurrWindow == nullptr)
//		{
//			throw std::runtime_error("Current using window don't exist!");
//		}
//		glfwSetClipboardString(pCurrWindow.get(), text);
//	}
//} //icimgui