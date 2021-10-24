#include "file_browser.h"

#include <sprocket/Core/window.h>

#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>
#include <Windows.h>

namespace spkt {
namespace {

void set_ofn(OPENFILENAMEA* ofn, window* window, const char* filter)
{
	GLFWwindow* w = static_cast<GLFWwindow*>(window->native_handle());
	ZeroMemory(ofn, sizeof(OPENFILENAME));
	ofn->lStructSize = sizeof(OPENFILENAME);
	ofn->hwndOwner = glfwGetWin32Window(w);
	ofn->lpstrFilter = filter;
	ofn->nFilterIndex = 1;
	ofn->Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;
}

}

std::optional<std::string> open_file(window* window, const char* filter)
{
    OPENFILENAMEA ofn;
	set_ofn(&ofn, window, filter);
	CHAR szFile[260] = {0};
	ofn.lpstrFile = szFile;
	ofn.nMaxFile = sizeof(szFile);

	if (GetOpenFileNameA(&ofn) == TRUE) {
		return std::string{ofn.lpstrFile};
	}
    return std::nullopt;
}

std::optional<std::string> save_file(window* window, const char* filter)
{
    OPENFILENAMEA ofn;
	set_ofn(&ofn, window, filter);
	CHAR szFile[260] = {0};
	ofn.lpstrFile = szFile;
	ofn.nMaxFile = sizeof(szFile);

	if (GetSaveFileNameA(&ofn) == TRUE) {
		return std::string{ofn.lpstrFile};
	}
    return std::nullopt;
}

}