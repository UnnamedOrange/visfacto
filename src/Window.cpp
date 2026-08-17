/**
 * @file Window.cpp
 * @author UnnamedOrange
 * @brief A generic Win32 window.
 * @version 0.1
 * @date 2026-8-17
 *
 * @copyright Copyright (c) UnnamedOrange. Licensed under the MIT License.
 * See the LICENSE file in the repository root for full license text.
 */

#include "Window.hpp"

#include <string>
#include <utility>

#include <Windows.h>

using Self = orange::Window;

int Self::message_loop() {
    MSG msg{};
    while (GetMessageW(&msg, nullptr, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }
    return int(msg.wParam);
}

LRESULT CALLBACK Self::virtual_proc(HWND const hWnd, UINT const uMsg, WPARAM const wParam, LPARAM const lParam) {
    Self* this_pointer = reinterpret_cast<Self*>(GetWindowLongPtrW(hWnd, GWLP_USERDATA));
    if (this_pointer == nullptr && uMsg == WM_NCCREATE) {
        auto const& create_struct = *reinterpret_cast<CREATESTRUCTW const*>(lParam);
        this_pointer = reinterpret_cast<Self*>(create_struct.lpCreateParams);
        SetWindowLongPtrW(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this_pointer));
        this_pointer->_handle = hWnd;
    }

    LRESULT result;
    if (this_pointer != nullptr) {
        result = this_pointer->window_proc(hWnd, uMsg, wParam, lParam);
    } else {
        result = DefWindowProcW(hWnd, uMsg, wParam, lParam);
    }

    if (this_pointer != nullptr && uMsg == WM_NCDESTROY) {
        this_pointer->_handle = nullptr;
        SetWindowLongPtrW(hWnd, GWLP_USERDATA, 0);
    }

    return result;
}

Self::Window(                         //
    HINSTANCE const _instance_handle, //
    std::wstring _class_name          //
    )
    : instance_handle(_instance_handle), //
      class_name(std::move(_class_name)) //
{
    WNDCLASSEXW wc{};
    wc.cbSize = sizeof(wc);
    wc.lpfnWndProc = &Self::virtual_proc;
    wc.hInstance = this->instance_handle;
    wc.lpszClassName = this->class_name.c_str();
    RegisterClassExW(&wc);
}

Self::~Window() {
    if (this->_handle != nullptr) {
        SetWindowLongPtrW(this->_handle, GWLP_USERDATA, 0);
        DestroyWindow(this->_handle);
    }
}

bool Self::create(wchar_t const* const window_name) {
    auto const created_handle = CreateWindowExW( //
        0,                                       //
        this->class_name.c_str(),                //
        window_name,                             //
        WS_OVERLAPPEDWINDOW,                     //
        0, 0, 0, 0,                              //
        nullptr,                                 //
        nullptr,                                 //
        this->instance_handle,                   //
        this                                     //
    );
    if (created_handle == nullptr) {
        return false;
    }
    return true;
}
