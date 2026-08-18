/**
 * @file Autostart.hpp
 * @author UnnamedOrange
 * @brief Manage the run-at-logon registry entry.
 * @version 0.1
 * @date 2026-8-18
 *
 * @copyright Copyright (c) UnnamedOrange. Licensed under the MIT License.
 * See the LICENSE file in the repository root for full license text.
 */

#pragma once

#include <string>

namespace orange {
    /**
     * @brief Manage the run-at-logon registry entry.
     */
    class Autostart {
        using Self = Autostart;

    private:
        std::wstring const value_name;

    public:
        explicit Autostart(std::wstring _value_name);
        Autostart(Self const&) = delete;
        Self& operator=(Self const&) = delete;
        Autostart(Self&&) = delete;
        Self& operator=(Self&&) = delete;

    public:
        bool get_is_enabled() const;
        void set_is_enabled(bool value);
    };
} // namespace orange
