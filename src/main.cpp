/**
 * @file main.cpp
 * @author UnnamedOrange
 * @brief The main entry.
 * @version 0.1
 * @date 2026-8-13
 *
 * @copyright Copyright (c) UnnamedOrange. Licensed under the MIT License.
 * See the LICENSE file in the repository root for full license text.
 */

#include <Windows.h>

#include "SingleInstance.hpp"

using orange::SingleInstance;

namespace {
    constexpr wchar_t MUTEX_NAME[] = L"Local\\visfacto";
}

int WINAPI wWinMain(HINSTANCE const hInstance, HINSTANCE const, PWSTR const pCmdLine, int const nCmdShow) {
    UNREFERENCED_PARAMETER(hInstance);
    UNREFERENCED_PARAMETER(pCmdLine);
    UNREFERENCED_PARAMETER(nCmdShow);

    auto single_instance = SingleInstance(MUTEX_NAME);
    if (!single_instance.is_first()) {
        return 0;
    }

    return 0;
}
