/*
	\brief		The entry point of the application.
	\file		entry.cpp
	\author		Aneurin F. Smith
*/

#include "include.h"
#include "win32/window/window.h"
int _winCount = 0;
int _sysHeight = 0;
int _sysFrame = 0;

/*
	\brief		The user-provided entry point for a graphical Windows-based application.
	\param		hInst – A handle to the current instance of the application.
	\param		hPrevInst – A handle to the previous instance of the application. This parameter is always NULL.
	\param		cmdLine – The command line for the application, excluding the program name.
	\param		cmdShow – Controls how the window is to be shown.
	\see		https://github.com/AneurinSmith/win32-template/wiki/entry#winmain
	\see		https://learn.microsoft.com/en-us/windows/win32/api/winbase/nf-winbase-winmain
*/
int CALLBACK WinMain(_In_ HINSTANCE hInst, _In_opt_ HINSTANCE hPrevInst, _In_ LPSTR cmdLine, _In_ INT cmdShow) {

	LOG::set_level(INFO);

	try {
		Window w;
		w.init();

		MSG msg = {};
		while (GetMessage(&msg, NULL, 0, 0)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	catch (std::runtime_error& ex) {
		MessageBoxA(NULL, ex.what(), "Runtime Exception", MB_OK);
	}

	return 0;
}
