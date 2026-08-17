/**
 * @file Window.hpp
 * @author UnnamedOrange
 * @brief A generic Win32 window.
 * @version 0.1
 * @date 2026-8-17
 *
 * @copyright Copyright (c) UnnamedOrange. Licensed under the MIT License.
 * See the LICENSE file in the repository root for full license text.
 */

#pragma once

#include <string>

#include <Windows.h>

namespace orange {
    /**
     * @brief A generic Win32 window.
     */
    class Window {
        using Self = Window;

    public:
        static int message_loop();

    private:
        static LRESULT CALLBACK virtual_proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

    private:
        HINSTANCE instance_handle;
        std::wstring const class_name;

    private:
        HWND _handle = nullptr;

    public:
        explicit Window(                //
            HINSTANCE _instance_handle, //
            std::wstring _class_name    //
        );
        Window(Self const&) = delete;
        Self& operator=(Self const&) = delete;
        Window(Self&&) = delete;
        Self& operator=(Self&&) = delete;

        virtual ~Window();

    protected:
        virtual LRESULT window_proc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) = 0;

    public:
        HWND handle() const noexcept {
            return this->_handle;
        }

        bool create(wchar_t const* window_name);
    };
} // namespace orange
