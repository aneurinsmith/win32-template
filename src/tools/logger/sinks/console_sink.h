
#pragma once
#include "base_sink.h"
#include "defwin.h"

class ConsoleSink : public basesink, public defwin {

public:

    ConsoleSink(string _name, int _ringbuffer) {
        name = _name;
        ringbuffer = _ringbuffer;
        min_lvl = (Level)0;
        menuName = 0;

        style = WS_OVERLAPPEDWINDOW;

        c.open(CSIDL_APPDATA, "Template\\config.ini", ios::in);
        string config_name = "console." + name;
        pos = {
            c.isSet(config_name, "x") ? c.getVal(config_name, "x") : 40,
            c.isSet(config_name, "y") ? c.getVal(config_name, "y") : 40
        };
        size = {
            c.isSet(config_name, "cx") ? c.getVal(config_name, "cx") : 600,
            c.isSet(config_name, "cy") ? c.getVal(config_name, "cy") : 400
        };
        fs = c.isSet(config_name, "fs") ? c.getVal(config_name, "fs") : 0;
        c.close();
    }

    ~ConsoleSink() {
        SendMessageA(hwnd, WM_DESTROY, 0, 0);

        PostThreadMessageA(GetThreadId(thread), WM_QUIT, 0, 0);
        unique_lock<mutex> lk(m);
        cv.wait(lk);
    }

    void set_level(Level lvl) {
        min_lvl = lvl;
    }

    void print(Level lvl, string msg) {
        if (lvl >= min_lvl) enqueue(msg);
    }

    BOOL init() {

        unique_lock<mutex> lk(m);
        thread = (HANDLE)_beginthread(ThreadStart, 0, (void*)this);
        cv.wait(lk);

        return true;
    }

protected:

    HANDLE  thread;
    HWND    console;
    int     ringbuffer;

    static void ThreadStart(void* _data) {
        ConsoleSink* data = (ConsoleSink*)_data;

        data->defwin::init();
        data->maximize(data->fs);
        data->display();

        data->cv.notify_all();

        MSG msg = {};
        while (true) {

            try {
                if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
                    
                    TranslateMessage(&msg);
                    DispatchMessage(&msg);

                    if (msg.message == WM_QUIT) {
                        data->cv.notify_all();
                        break;
                    }

                }
                else if (!data->q.empty()) {
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
            catch (...) {
                continue;
            }
        }
    }

	LRESULT HandleMessage(HWND wnd, UINT msg, WPARAM wpm, LPARAM lpm) {

        switch (msg) {
            case WM_CREATE: {
                HWND cmd = FindWindow(L"ConsoleWindowClass", NULL);

                HICON iconSM = (HICON)SendMessage(cmd, WM_GETICON, ICON_SMALL, 0);
                SendMessage(wnd, WM_SETICON, (WPARAM)ICON_BIG, (LPARAM)LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(ICO_APP)));
                SendMessage(wnd, WM_SETICON, (WPARAM)ICON_SMALL, (LPARAM)iconSM);

                console = CreateWindowExW(LVS_EX_DOUBLEBUFFER, L"LISTBOX", NULL, WS_VISIBLE | WS_CHILD | WS_VSCROLL | LBS_OWNERDRAWVARIABLE, 0, 0, 300, 400, wnd, (HMENU)101, GetModuleHandle(NULL), (void*)this);
                SetWindowSubclass(console, ConsoleProc, 0, 0);

                HFONT hf;
                hf = CreateFont(18, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, ANSI_CHARSET, CLIP_DEFAULT_PRECIS, 0, CLEARTYPE_QUALITY, FF_DONTCARE, L"Consolas");
                SendMessage(console, WM_SETFONT, (LPARAM)hf, true);

                SetWindowPos(console, wnd, 0, 0, 0, 0, SWP_NOZORDER);
                SetWindowPos(wnd, NULL, 0, 0, 0, 0, SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE);


                BOOL value = TRUE;
                DwmSetWindowAttribute(wnd, DWMWA_USE_IMMERSIVE_DARK_MODE, &value, sizeof(value));
                SetWindowTheme(console, L"DarkMode_Explorer", nullptr);
                SetWindowTheme(wnd, L"DarkMode_Explorer", nullptr);

            }
            case WM_SIZE: {
                SetWindowPos(console, 0, 0, 0, LOWORD(lpm), HIWORD(lpm), SWP_NOZORDER);

                break;
            }
            case WM_ERASEBKGND: {

                HDC hdc = (HDC)wpm;
                RECT rc;
                GetClientRect(wnd, &rc);
                HBRUSH brush = CreateSolidBrush(0x0C0C0C);
                FillRect(hdc, &rc, brush);

                DeleteObject(brush);

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
            case WM_CLOSE:
            case WM_DESTROY:
            {
                WINDOWPLACEMENT wp = {};
                GetWindowPlacement(wnd, &wp);

                c.open(CSIDL_APPDATA, "Template\\config.ini", ios::in | ios::out);
                string config_name = "console." + name;
                c.setVal(config_name, "x", wp.rcNormalPosition.left);
                c.setVal(config_name, "y", wp.rcNormalPosition.top);
                c.setVal(config_name, "cx", wp.rcNormalPosition.right - wp.rcNormalPosition.left);
                c.setVal(config_name, "cy", wp.rcNormalPosition.bottom - wp.rcNormalPosition.top);
                c.setVal(config_name, "fs", wp.flags == 2 ? 1 : 0);
                c.close();

                break;
            }
        }

		return DefWindowProc(wnd,msg,wpm,lpm);
	}

    static LRESULT CALLBACK ConsoleProc(HWND wnd, UINT msg, WPARAM wpm, LPARAM lpm, UINT_PTR _subclass, DWORD_PTR _data) {

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
                    sp -= min(sp, 3);
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

private:
    queue<string> q;

    void enqueue(string msg) {
        q.push(msg);
    }

    string dequeue() {

        string _msg = q.front();
        q.pop();

        return _msg;
    }

    mutex m;
    condition_variable cv;

};
