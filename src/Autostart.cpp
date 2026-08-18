/**
 * @file Autostart.cpp
 * @author UnnamedOrange
 * @brief Manage the run-at-logon registry entry.
 * @version 0.1
 * @date 2026-8-18
 *
 * @copyright Copyright (c) UnnamedOrange. Licensed under the MIT License.
 * See the LICENSE file in the repository root for full license text.
 */

#include "Autostart.hpp"

#include <string>
#include <string_view>
#include <utility>

#include <Windows.h>

using Self = orange::Autostart;

namespace {
    constexpr wchar_t RUN_KEY[] = L"Software\\Microsoft\\Windows\\CurrentVersion\\Run";

    std::wstring get_module_file_name() {
        DWORD size = MAX_PATH;
        auto ret = std::wstring(size, L'\0');
        DWORD len;
        while ((len = GetModuleFileNameW(nullptr, ret.data(), size)) == size) {
            size *= 2;
            ret.resize(size, L'\0');
        }
        ret.resize(len);
        return ret;
    }

    std::wstring get_quoted_module_file_name() {
        auto path = get_module_file_name();
        if (path.empty()) {
            return {};
        }
        std::wstring command;
        command.reserve(path.length() + 2);
        command.push_back(L'"');
        command += std::move(path);
        command.push_back(L'"');
        return command;
    }
} // namespace

Self::Autostart(std::wstring _value_name) //
    : value_name(std::move(_value_name))  //
{}

bool Self::get_is_enabled() const {
    auto const command = get_quoted_module_file_name();
    if (command.empty()) {
        return false;
    }

    auto value = std::wstring(command.length() + 1, L'\0');
    DWORD value_size = DWORD(value.size() * sizeof(wchar_t));
    auto const result = RegGetValueW( //
        HKEY_CURRENT_USER,            //
        RUN_KEY,                      //
        this->value_name.c_str(),     //
        RRF_RT_REG_SZ,                //
        nullptr,                      //
        value.data(),                 //
        &value_size                   //
    );
    if (result != ERROR_SUCCESS) {
        return false;
    }
    auto const value_view = std::wstring_view(value.c_str());
    auto const command_view = std::wstring_view(command);
    return value_view == command_view;
}
void Self::set_is_enabled(bool const value) {
    if (value) {
        auto command = get_quoted_module_file_name();
        if (command.empty()) {
            return;
        }
        RegSetKeyValueW(                                    //
            HKEY_CURRENT_USER,                              //
            RUN_KEY,                                        //
            this->value_name.c_str(),                       //
            REG_SZ,                                         //
            command.c_str(),                                //
            DWORD((command.length() + 1) * sizeof(wchar_t)) //
        );
    } else {
        RegDeleteKeyValueW(          //
            HKEY_CURRENT_USER,       //
            RUN_KEY,                 //
            this->value_name.c_str() //
        );
    }
}
