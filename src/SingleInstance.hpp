/**
 * @file SingleInstance.hpp
 * @author UnnamedOrange
 * @brief Enforce a single instance per session.
 * @version 0.1
 * @date 2026-8-15
 *
 * @copyright Copyright (c) UnnamedOrange. Licensed under the MIT License.
 * See the LICENSE file in the repository root for full license text.
 */

#pragma once

#include <Windows.h>

namespace orange {
    /**
     * @brief Enforce a single instance per session.
     */
    class SingleInstance {
        using Self = SingleInstance;

    private:
        HANDLE mutex_handle;
        bool _is_first;

    public:
        explicit SingleInstance(wchar_t const* name);
        SingleInstance(Self const&) = delete;
        Self& operator=(Self const&) = delete;
        SingleInstance(Self&&) = delete;
        Self& operator=(Self&&) = delete;

        ~SingleInstance();

    public:
        bool is_first() const noexcept {
            return this->_is_first;
        }
    };
} // namespace orange
