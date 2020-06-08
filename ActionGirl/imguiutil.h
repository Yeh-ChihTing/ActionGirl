#pragma once
#include <windows.h>
#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"

void imguiinit(HWND hwnd);
void imguiexit();
void imguidraw();