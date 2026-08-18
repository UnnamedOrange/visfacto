/**
 * @file Protection.hpp
 * @author UnnamedOrange
 * @brief Lock the visual-effect settings against modification.
 * @version 0.1
 * @date 2026-8-18
 *
 * @copyright Copyright (c) UnnamedOrange. Licensed under the MIT License.
 * See the LICENSE file in the repository root for full license text.
 */

#pragma once

#include <array>
#include <optional>

#include <Windows.h>

namespace orange {
    /**
     * @brief Lock the visual-effect settings against modification.
     */
    class Protection {
        using Self = Protection;

    private:
        enum class Kind {
            BOOL,           // The value goes through uiParam.
            PV_PARAM,       // The value goes through pvParam (BOOL*).
            ANIMATION_INFO, // The value goes through ANIMATIONINFO.
        };

        struct Setting {
            Self::Kind kind;
            UINT spi_get;
            UINT spi_set;
        };

        static constexpr std::array<Self::Setting, 16> SETTINGS = {
            Self::Setting{Self::Kind::ANIMATION_INFO, SPI_GETANIMATION, SPI_SETANIMATION},
            Self::Setting{Self::Kind::BOOL, SPI_GETDRAGFULLWINDOWS, SPI_SETDRAGFULLWINDOWS},
            Self::Setting{Self::Kind::PV_PARAM, SPI_GETCLIENTAREAANIMATION, SPI_SETCLIENTAREAANIMATION},
            Self::Setting{Self::Kind::PV_PARAM, SPI_GETCOMBOBOXANIMATION, SPI_SETCOMBOBOXANIMATION},
            Self::Setting{Self::Kind::PV_PARAM, SPI_GETCURSORSHADOW, SPI_SETCURSORSHADOW},
            Self::Setting{Self::Kind::PV_PARAM, SPI_GETDROPSHADOW, SPI_SETDROPSHADOW},
            Self::Setting{Self::Kind::PV_PARAM, SPI_GETFONTSMOOTHING, SPI_SETFONTSMOOTHING},
            Self::Setting{Self::Kind::PV_PARAM, SPI_GETGRADIENTCAPTIONS, SPI_SETGRADIENTCAPTIONS},
            Self::Setting{Self::Kind::PV_PARAM, SPI_GETHOTTRACKING, SPI_SETHOTTRACKING},
            Self::Setting{Self::Kind::PV_PARAM, SPI_GETLISTBOXSMOOTHSCROLLING, SPI_SETLISTBOXSMOOTHSCROLLING},
            Self::Setting{Self::Kind::PV_PARAM, SPI_GETMENUANIMATION, SPI_SETMENUANIMATION},
            Self::Setting{Self::Kind::PV_PARAM, SPI_GETMENUFADE, SPI_SETMENUFADE},
            Self::Setting{Self::Kind::PV_PARAM, SPI_GETSELECTIONFADE, SPI_SETSELECTIONFADE},
            Self::Setting{Self::Kind::PV_PARAM, SPI_GETTOOLTIPANIMATION, SPI_SETTOOLTIPANIMATION},
            Self::Setting{Self::Kind::PV_PARAM, SPI_GETTOOLTIPFADE, SPI_SETTOOLTIPFADE},
            Self::Setting{Self::Kind::PV_PARAM, SPI_GETUIEFFECTS, SPI_SETUIEFFECTS},
        };

    private:
        static std::optional<bool> read_current(Self::Setting const& setting);
        static void apply(Self::Setting const& setting, bool reference);

    private:
        std::array<bool, Self::SETTINGS.size()> baselines{};

    public:
        Protection() = default;
        Protection(Self const&) = delete;
        Self& operator=(Self const&) = delete;
        Protection(Self&&) = delete;
        Self& operator=(Self&&) = delete;

    public:
        void capture_baseline();
        void check_and_restore();
    };
} // namespace orange
