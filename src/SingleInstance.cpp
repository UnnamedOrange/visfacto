/**
 * @file SingleInstance.cpp
 * @author UnnamedOrange
 * @brief Enforce a single instance per session.
 * @version 0.1
 * @date 2026-8-15
 *
 * @copyright Copyright (c) UnnamedOrange. Licensed under the MIT License.
 * See the LICENSE file in the repository root for full license text.
 */

#include "SingleInstance.hpp"

#include <Windows.h>

using Self = orange::SingleInstance;

Self::SingleInstance(wchar_t const* const name) {
    this->mutex_handle = CreateMutexW(nullptr, TRUE, name);
    this->_is_first =                    //
        this->mutex_handle != nullptr && //
        GetLastError() != ERROR_ALREADY_EXISTS;
}

Self::~SingleInstance() {
    if (this->mutex_handle) {
        CloseHandle(this->mutex_handle);
    }
}
