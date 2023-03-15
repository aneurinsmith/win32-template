
#include "include.h"
#include "core.h"
#include "window/window.h"

int CALLBACK WinMain(_In_ HINSTANCE hInst, _In_opt_ HINSTANCE hPrevInst, _In_ LPSTR cmdLine, _In_ int cmdShow) {

	MSG msg = {};

	Window w;

	try {
		w.init();

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