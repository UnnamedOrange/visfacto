/**
 * @file TrayWindow.cpp
 * @author UnnamedOrange
 * @brief The hidden tray window.
 * @version 0.1
 * @date 2026-8-18
 *
 * @copyright Copyright (c) UnnamedOrange. Licensed under the MIT License.
 * See the LICENSE file in the repository root for full license text.
 */

#include "TrayWindow.hpp"

#include <array>
#include <cassert>
#include <cwchar>
#include <memory>
#include <type_traits>

#include <Windows.h>
#include <windowsx.h>

#include "resource.h"

using Self = orange::TrayWindow;

#define HANDLE_WM_TRAYICON(hWnd, wParam, lParam, fn) ((fn)((hWnd), (UINT)(wParam), (UINT)(LOWORD(lParam))), 0L)

Self::TrayWindow(HINSTANCE const _instance_handle)      //
    : Super(_instance_handle, Self::WINDOW_CLASS_NAME), //
      instance_handle(_instance_handle)                 //
{}

LRESULT Self::window_proc(HWND const hWnd, UINT const uMsg, WPARAM const wParam, LPARAM const lParam) {
    if (this->taskbar_created_message != 0 && //
        uMsg == this->taskbar_created_message //
    ) {
        this->add_tray_icon();
        return 0;
    }
    switch (uMsg) {
        HANDLE_MSG(hWnd, WM_TRAYICON, [this](HWND const, UINT const, UINT const mouse_message) { //
            this->on_tray_icon(mouse_message);
        });
        HANDLE_MSG(hWnd, WM_DESTROY, [this](HWND const) { //
            this->on_destroy();
        });
    }
    return DefWindowProcW(hWnd, uMsg, wParam, lParam);
}

bool Self::create_window_and_tray_icon() {
    if (!Super::create(Self::WINDOW_NAME)) {
        return false;
    }

    this->taskbar_created_message = RegisterWindowMessageW(L"TaskbarCreated");
    this->add_tray_icon();

    return true;
}

void Self::on_tray_icon(UINT const mouse_message) {
    switch (mouse_message) {
    case WM_RBUTTONUP: {
        auto const cmd = this->track_popup_menu();
        this->on_tray_menu_item(cmd);
        break;
    }
    default: {
        break;
    }
    }
}
void Self::on_destroy() {
    this->remove_tray_icon();
    PostQuitMessage(0);
}

void Self::add_tray_icon() {
    NOTIFYICONDATAW nid{};
    nid.cbSize = sizeof(nid);
    nid.hWnd = this->Super::handle();
    nid.uID = Self::ID_TRAYICON;
    nid.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP;
    nid.uCallbackMessage = Self::WM_TRAYICON;
    nid.hIcon = LoadIconW(this->instance_handle, MAKEINTRESOURCEW(IDI_ICON));
#pragma warning(suppress : 4996)
    wcsncpy(nid.szTip, Self::WINDOW_NAME, std::size(nid.szTip));
    Shell_NotifyIconW(NIM_ADD, &nid);
}
void Self::remove_tray_icon() {
    NOTIFYICONDATAW nid{};
    nid.cbSize = sizeof(nid);
    nid.hWnd = this->Super::handle();
    nid.uID = Self::ID_TRAYICON;
    Shell_NotifyIconW(NIM_DELETE, &nid);
}

UINT Self::track_popup_menu() {
    // Load the menu resource.
    auto const menu_handle = std::unique_ptr<  //
        std::remove_pointer_t<HMENU>,          //
        decltype([](HMENU const menu_handle) { //
            DestroyMenu(menu_handle);
        })>{
        LoadMenuW(nullptr, MAKEINTRESOURCEW(IDR_MENU_TRAY)),
    };
    assert(menu_handle);
    auto const sub_menu_handle = GetSubMenu(menu_handle.get(), 0);
    assert(sub_menu_handle != nullptr);

    // Check the items with the states.
    CheckMenuItem(                            //
        sub_menu_handle,                      //
        ID_TRAY_AUTOSTART,                    //
        MF_BYCOMMAND |                        //
            (this->autostart.get_is_enabled() //
                 ? MF_CHECKED                 //
                 : MF_UNCHECKED)              //
    );

    // Popup menu and track.
    POINT pt;
    GetCursorPos(&pt);
    SetForegroundWindow(this->Super::handle());
    auto const ret = TrackPopupMenu(     //
        sub_menu_handle,                 //
        TPM_RETURNCMD | TPM_RIGHTBUTTON, //
        pt.x, pt.y,                      //
        0,                               //
        this->Super::handle(),           //
        nullptr                          //
    );
    PostMessageW(this->Super::handle(), WM_NULL, 0, 0);

    return ret;
}
void Self::on_tray_menu_item(UINT const cmd) {
    switch (cmd) {
    case ID_TRAY_EXIT: {
        DestroyWindow(this->Super::handle());
        break;
    }
    case ID_TRAY_ABOUT: {
        ShellExecuteW(                                    //
            nullptr,                                      //
            L"open",                                      //
            L"https://github.com/UnnamedOrange/visfacto", //
            nullptr,                                      //
            nullptr,                                      //
            SW_SHOWNORMAL                                 //
        );
        break;
    }
    case ID_TRAY_AUTOSTART: {
        this->autostart.set_is_enabled(!this->autostart.get_is_enabled());
        break;
    }
    default: {
        break;
    }
    }
}
