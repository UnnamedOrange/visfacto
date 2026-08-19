# visfacto

Your Visual Effects define the de facto state

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](LICENSE)
[![Platform: Windows](https://img.shields.io/badge/platform-Windows-blue.svg)](https://www.microsoft.com/windows)
[![C++23](https://img.shields.io/badge/C%2B%2B-23-00599C.svg)](https://en.cppreference.com/w/cpp/23)

**visfacto** is a small Windows tray application that keeps your visual effect settings exactly as you set them. Remote desktop software, for example, can turn off window animations, shadows, and font smoothing and leave them that way. visfacto quietly puts your settings back.

## Features

- Keeps your visual effect settings exactly the way you set them, automatically undoing changes made by remote desktop software and other apps.
- One-click enable or disable from the system tray.
- Optional start with Windows.
- Single portable exe - no installation required.

## Requirements

- Windows 10 or later

## Download

Ready-to-run builds are published on the [Releases](https://github.com/UnnamedOrange/visfacto/releases) page.

## Build

```powershell
git clone https://github.com/UnnamedOrange/visfacto.git
cd visfacto
cmake --preset msvc-x64
cmake --build --preset msvc-x64-release
```

The exe is written to `build/msvc-x64/Release/visfacto.exe`.

For a debug build, use the `msvc-x64-debug` preset instead:

```powershell
cmake --build --preset msvc-x64-debug
```

## Usage

1. Run `visfacto.exe`.
2. Right-click the visfacto icon in the system tray.
3. Use the tray menu to control the app.

The tray menu has four items:

- **Enable protection** - turn protection on or off.
- **Start with Windows** - add or remove the run-at-logon entry.
- **About** - open the project page on GitHub.
- **Exit** - quit visfacto.

## How it works

When protection is enabled, visfacto reads the current value of each protected setting with `SystemParametersInfo` and saves the values as a baseline. A timer and the `WM_SETTINGCHANGE` message trigger the check. If a value no longer matches the baseline, visfacto writes it back right away.

## License

Copyright (c) UnnamedOrange. Licensed under the MIT License.
See the [LICENSE](./LICENSE) file in the repository root for full license text.
