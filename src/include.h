
#pragma once
#include <Windows.h>
#include <windowsx.h>
#include <CommCtrl.h>
#include <array>
#include <stack>
#include <sstream>
#include <wrl.h>
#include <cstdlib>
#include <shlwapi.h>
#include <ShlObj.h>
#include <direct.h>
#include <compare>

#include <dwmapi.h>
#include <uxtheme.h>
#include <vssym32.h>

#include <cstdio>
#include <stdlib.h>
#include <stdexcept>

#include <dxgi1_3.h>
#include <d3d11_2.h>
#include <d2d1_2.h>
#include <d2d1_1.h>
#include <d2d1.h>
#include <d2d1_2helper.h>
#include <dcomp.h>
#include <dwrite.h>
#include <wincodec.h>
#include <unordered_map>

#pragma comment(lib, "user32.lib")
#pragma comment (lib, "comctl32.lib")
#pragma comment (lib, "dwmapi.lib")
#pragma comment (lib, "uxtheme.lib")
#pragma comment (lib, "wsock32.lib")
#pragma comment(lib, "dxgi")
#pragma comment(lib, "d3d11")
#pragma comment(lib, "d2d1")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "dcomp")
#pragma comment(lib,"shlwapi.lib")

#define APP_NAME L"Template"
#define COL_BODY 0x313031
#define COL_HEAD 0x313031

#define IDC_INVALIDATE 32771


#include "resource.h"

using namespace Microsoft::WRL;
using namespace std;
