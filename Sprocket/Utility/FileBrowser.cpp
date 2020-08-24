#include "FileBrowser.h"

#include <GLFW/glfw3.h>

#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>
#include <Windows.h>

namespace Sprocket {
namespace {

void SetOFN(OPENFILENAMEA* ofn, Window* window, const char* filter)
{
	GLFWwindow* w = static_cast<GLFWwindow*>(window->NativeWindow());
	ZeroMemory(ofn, sizeof(OPENFILENAME));
	ofn->lStructSize = sizeof(OPENFILENAME);
	ofn->hwndOwner = glfwGetWin32Window(w);
	ofn->lpstrFilter = filter;
	ofn->nFilterIndex = 1;
	ofn->Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;
}

}

std::string OpenFile(Window* window, const char* filter)
{
    OPENFILENAMEA ofn;
	SetOFN(&ofn, window, filter);
	CHAR szFile[260] = {0};
	ofn.lpstrFile = szFile;
	ofn.nMaxFile = sizeof(szFile);

	if (GetOpenFileNameA(&ofn) == TRUE) {
		return ofn.lpstrFile;
	}
    return std::string();
}

std::string SaveFile(Window* window, const char* filter)
{
    OPENFILENAMEA ofn;
	SetOFN(&ofn, window, filter);
	CHAR szFile[260] = {0};
	ofn.lpstrFile = szFile;
	ofn.nMaxFile = sizeof(szFile);

	if (GetSaveFileNameA(&ofn) == TRUE) {
		return ofn.lpstrFile;
	}
    return std::string();
}

}