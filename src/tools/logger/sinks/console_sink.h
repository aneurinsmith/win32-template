/*
    \brief		The header file for the ConsoleSink class.
    \file		console_sink.h
    \author		Aneurin F. Smith
*/

#pragma once
#include "base_sink.h"

/*
    \brief		The strucutre template for a ConsoleSink. Extends the basesink interface.
*/
class ConsoleSink : public basesink {

public:

    /*
        \brief		Constructor method for a ConsoleSink. Initilises a new thread to avoid interfering with the
                    debugged application too much. Will block the calling thread until the created thread is fully
                    initilised.
        \param      _name – The name to be used as the title of the console window.
        \param      _ringbuffer – The maximum number of rows in the logger before old ones get deleted.
    */
    ConsoleSink(wstring _name, int _ringbuffer) {

        name = _name;
        ringbuffer = _ringbuffer;
        min_lvl = (Level)0;

        unique_lock<mutex> lk(cv_m);

        thread = (HANDLE)_beginthread(ThreadStart, 0, (void*)this);

        cv.wait(lk);
    }

    ~ConsoleSink() {
        PostThreadMessageA(GetThreadId(thread), WM_QUIT, 0, 0);
    }

    /*
        \brief		Changes the minimum level of message to be output by the sink.
        \param      lvl – The minimum level the sink will output.
    */
    void set_level(Level lvl) {
        min_lvl = lvl;
    }

    /*
        \brief		Prints the message to the sink, provided the minimum level is met.
        \param      lvl – The level of the message to be checked.
    */
    void print(Level lvl, string msg) {
        if(lvl >= min_lvl) enqueue(msg);
    }

private:

    queue<string> q;

    /*
        \brief      Enqueue a message into the string queue.
        \param      msg – The message to be enqueued.
    */
    void enqueue(string msg) {
        q.push(msg);
    }

    /*
        \brief      Retrieve a message from the front of the queue, and remove it.
        \return     The string at the front of the queue.
    */
    string dequeue() {

        string _msg = q.front();
        q.pop();

        return _msg;
    }

    HWND hwnd;
    HWND console;
    HANDLE thread;

    mutex cv_m;
    condition_variable cv;

    wstring name;
    int ringbuffer;
    Level min_lvl;

    /*
        \brief      The entry point of the thread. Creates a window and processes messages until WM_QUIT is posted to the window.
        \param      _data – A pointer to a ConsoleSink data structure to be operated on.
    */
    static void ThreadStart(void* _data) {
        ConsoleSink* data = (ConsoleSink*)_data;

        WNDCLASSEXW wcex = {};
        if (!GetClassInfoExW(GetModuleHandle(NULL), L"console", &wcex)) {
            wcex.cbSize = sizeof(wcex);
            wcex.lpfnWndProc = WindowProc;
            wcex.cbClsExtra = 0;
            wcex.cbWndExtra = 0;
            wcex.hInstance = GetModuleHandle(NULL);
            wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
            wcex.lpszClassName = L"console";

            RegisterClassExW(&wcex);
        }

        CreateWindowW(L"console", data->name.c_str(), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 1200, 600, NULL, (HMENU)0, GetModuleHandle(NULL), (void*)data);

        RedrawWindow(data->hwnd, NULL, NULL, NULL);
        ShowWindow(data->hwnd, SW_SHOW);

        data->cv.notify_all();

        try {

            MSG msg = {};
            while (true) {
                if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
                    TranslateMessage(&msg);
                    DispatchMessage(&msg);
                    if (msg.message == WM_QUIT) {
                        break;
                    }
                }else if (!data->q.empty()) {
                    SendMessageA(data->console, WM_SETREDRAW, FALSE, NULL);
                    LockWindowUpdate(data->console);
                    string m = data->dequeue();

                    char* c = new char[m.size() + 1];
                    copy(m.begin(), m.end(), c);
                    c[m.size()] = 0;

                    int count = SendMessageA(data->console, LB_GETCOUNT, 0, 0);
                    if (count > data->ringbuffer || count > 32766) {
                        SendMessageA(data->console, LB_DELETESTRING, 0, 0);
                    }

                    int index = SendMessageA(data->console, LB_ADDSTRING, 0, (LPARAM)c);
                    SendMessageA(data->console, LB_SETTOPINDEX, index, NULL);

                    LockWindowUpdate(NULL);
                    if (data->q.empty()) {
                        SendMessageA(data->console, WM_SETREDRAW, TRUE, NULL);
                    }
                } 
            }
        }
        catch (std::runtime_error& ex) {
            MessageBoxA(NULL, ex.what(), "Runtime Exception", MB_OK);
        }

        _endthread();
    }

    /*
        \brief      A subclassed window process, used by a ListBox window which acts as the output for the console window.
        \see        https://learn.microsoft.com/en-us/windows/win32/winmsg/window-procedures
    */
    static LRESULT CALLBACK ConsoleProc(HWND wnd, UINT msg, WPARAM wpm, LPARAM lpm, UINT_PTR _subclass, DWORD_PTR _data) {

        ConsoleSink* data = reinterpret_cast<ConsoleSink*>(::GetWindowLongPtr(wnd, GWLP_USERDATA));

        switch (msg) {
            case WM_ERASEBKGND: {
                return 1;
            }
            case WM_PAINT: {

                PAINTSTRUCT ps;
                HDC hdc = BeginPaint(wnd, &ps);
                HDC memDC = CreateCompatibleDC(hdc);
                HBITMAP memBM = CreateCompatibleBitmap(hdc, ps.rcPaint.right, ps.rcPaint.bottom);
                SelectObject(memDC, memBM);

                HBRUSH brush = CreateSolidBrush(0x0C0C0C);
                FillRect(memDC, &ps.rcPaint, brush);

                if (SendMessage(wnd, LB_GETCOUNT, 0, 0) > 0) {
                    DefSubclassProc(wnd, WM_PAINT, (WPARAM)memDC, (LPARAM)0);
                }

                BitBlt(hdc, 0, 0, ps.rcPaint.right, ps.rcPaint.bottom, memDC, 0, 0, SRCCOPY);
                EndPaint(wnd, &ps);

                DeleteDC(memDC);
                DeleteObject(memBM);
                DeleteObject(brush);

                return 0;
            }
                         
            case WM_KILLFOCUS:
            case WM_VSCROLL:
            case WM_SETCURSOR:
            case WM_SIZE: {
                InvalidateRect(wnd, NULL, true);

                break;
            }

            case WM_MOUSEWHEEL: {

                int delta = GET_WHEEL_DELTA_WPARAM(wpm);
                int sp = GetScrollPos(wnd, SB_VERT);

                if (delta > 0) {
                    sp -= min(sp,3);
                }
                else if (delta < 0) {
                    sp += 3;
                }

                WPARAM m = MAKEWPARAM(SB_THUMBPOSITION, sp);
                SendMessage(wnd, WM_VSCROLL, m, 0);

                InvalidateRect(wnd, NULL, true);

                return 0;
            }
        }
        return DefSubclassProc(wnd, msg, wpm, lpm);
    }

    /*
        \brief      The main window process of the console window.
        \see        https://learn.microsoft.com/en-us/windows/win32/winmsg/window-procedures
    */
    static LRESULT CALLBACK WindowProc(HWND wnd, UINT msg, WPARAM wpm, LPARAM lpm) {

        ConsoleSink* data = reinterpret_cast<ConsoleSink*>(::GetWindowLongPtr(wnd, GWLP_USERDATA));

        switch (msg) {
            case WM_NCCREATE: {

                data = static_cast<ConsoleSink*>((reinterpret_cast<LPCREATESTRUCT>(lpm))->lpCreateParams);
                data->hwnd = wnd;
                ::SetWindowLongPtr(wnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(data));

                break;
            }
            case WM_CREATE: {

                HWND cmd = FindWindow(L"ConsoleWindowClass", NULL);
                
                HICON iconSM = (HICON)SendMessage(cmd, WM_GETICON, ICON_SMALL, 0); //GetClassLong(cmd, GCL_HICONSM);
                SendMessage(wnd, WM_SETICON, (WPARAM)ICON_BIG, (LPARAM)LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(ICO_APP)));
                SendMessage(wnd, WM_SETICON, (WPARAM)ICON_SMALL, (LPARAM)iconSM);

                data->console = CreateWindowExW(LVS_EX_DOUBLEBUFFER, L"LISTBOX", NULL, WS_CHILD | WS_VISIBLE | WS_VSCROLL | LBS_OWNERDRAWVARIABLE, 0, 0, 300, 400, wnd, (HMENU)101, GetModuleHandle(NULL), 0);
                ::SetWindowLongPtr(data->console, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(data));
                SetWindowSubclass(data->console, ConsoleProc, 0, 0);

                HFONT hf;
                hf = CreateFont(18, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET, CLIP_DEFAULT_PRECIS, 0, CLEARTYPE_QUALITY, FF_DONTCARE, L"Consolas");
                SendMessage(data->console, WM_SETFONT, (LPARAM)hf, true);

                SetWindowPos(data->console, wnd, 0, 0, 0, 0, SWP_NOZORDER);
                SetWindowPos(wnd, NULL, 0, 0, 0, 0, SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE);


                BOOL value = TRUE;
                DwmSetWindowAttribute(wnd, DWMWA_USE_IMMERSIVE_DARK_MODE, &value, sizeof(value));
                SetWindowTheme(data->console, L"DarkMode_Explorer", nullptr);
                SetWindowTheme(wnd, L"DarkMode_Explorer", nullptr);

                break;
            }
            case WM_SIZE: {
                SetWindowPos(data->console, 0, 0, 0, LOWORD(lpm), HIWORD(lpm), SWP_NOZORDER);

                break;
            }
            case WM_ERASEBKGND: {
                return 1;
            }
            case WM_DRAWITEM: {
                LPDRAWITEMSTRUCT item = (LPDRAWITEMSTRUCT)lpm;

                switch (item->itemAction) {
                    case ODA_SELECT:
                    case ODA_DRAWENTIRE: {
                        HBRUSH brush = CreateSolidBrush(0xCC0000);
                        SetBkMode(item->hDC, 0x0C0C0C);
                        SetTextColor(item->hDC, 0xCCCCCC);

                        if (item->itemState & ODS_SELECTED) {
                            FillRect(item->hDC, &item->rcItem, brush);
                        }

                        char* str = (char*)item->itemData;
                        DrawTextA(item->hDC, str, strlen(str), &item->rcItem, DT_LEFT | DT_WORDBREAK | DT_CALCRECT);
                        SendMessage(item->hwndItem, LB_SETITEMHEIGHT, item->itemID, item->rcItem.bottom - item->rcItem.top);

                        DrawTextA(item->hDC, str, strlen(str), &item->rcItem, DT_LEFT | DT_WORDBREAK);
                                
                        DeleteObject(brush);
                    }
                    case ODA_FOCUS: {
                        break;
                    }
                }
                return true;
            }
        }

        return DefWindowProc(wnd, msg, wpm, lpm);
    }
};
