/**
 * @file Protection.cpp
 * @author UnnamedOrange
 * @brief Lock the visual-effect settings against modification.
 * @version 0.1
 * @date 2026-8-18
 *
 * @copyright Copyright (c) UnnamedOrange. Licensed under the MIT License.
 * See the LICENSE file in the repository root for full license text.
 */

#include "Protection.hpp"

#include <cstddef>
#include <optional>

using Self = orange::Protection;

std::optional<bool> Self::read_current(Self::Setting const& setting) {
    switch (setting.kind) {
    case Self::Kind::ANIMATION_INFO: {
        ANIMATIONINFO animation_info{};
        animation_info.cbSize = sizeof(animation_info);
        if (!SystemParametersInfoW(setting.spi_get, sizeof(animation_info), &animation_info, 0)) {
            return std::nullopt;
        }
        return std::optional(animation_info.iMinAnimate != 0);
    }
    default: {
        BOOL value;
        if (!SystemParametersInfoW(setting.spi_get, 0, &value, 0)) {
            return std::nullopt;
        }
        return std::optional(value != 0);
    }
    }
}
void Self::apply(Self::Setting const& setting, bool const reference) {
    switch (setting.kind) {
    case Self::Kind::ANIMATION_INFO: {
        ANIMATIONINFO animation_info{};
        animation_info.cbSize = sizeof(animation_info);
        animation_info.iMinAnimate = reference ? TRUE : FALSE;
        SystemParametersInfoW(setting.spi_set, sizeof(animation_info), &animation_info, 0);
        break;
    }
    case Self::Kind::PV_PARAM: {
        auto value = reference ? TRUE : FALSE;
        SystemParametersInfoW(setting.spi_set, 0, &value, 0);
        break;
    }
    default: {
        SystemParametersInfoW(setting.spi_set, reference ? TRUE : FALSE, nullptr, 0);
        break;
    }
    }
}

void Self::capture_baseline() {
    for (size_t i = 0; i < Self::SETTINGS.size(); i++) {
        if (auto const current = Self::read_current(Self::SETTINGS[i])) {
            this->baselines[i] = *current;
        }
    }
}
void Self::check_and_restore() {
    for (size_t i = 0; i < Self::SETTINGS.size(); i++) {
        auto const current = Self::read_current(Self::SETTINGS[i]);
        if (!current) {
            continue;
        }
        if (*current != this->baselines[i]) {
            Self::apply(Self::SETTINGS[i], this->baselines[i]);
        }
    }
}
