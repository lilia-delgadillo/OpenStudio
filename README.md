# OpenStudio 1

Open source broadcast switching software based in C++ 20 and Qt 6.5

## Current Features

- DirectX 11 Rendering Pipeline
- NDI 5 source discovery and rendering
- PRGM display

## Requirements

- Windows 10 or 11
- Visual Studio 2022 with the Desktop development with C++ workload
- CMake 3.21+
- Qt 6.5+ with an MSVC 2022 kit
- NDI 5+ SDK (For Development) or NDI Tools 5+ (For use)

## Configure and build

Open a Developer PowerShell for VS 2022, then from this directory:

```powershell
cmake -B build-msvc
cmake --build build-msvc --config debug
"C:\Qt\6.12.0\msvc2022_64\bin\windeployqt.exe" --debug ".\build-msvc\Debug\OpenStudio.exe"
```

Replace the Qt path with the actual location of your Qt MSVC installation.

Run:

```powershell
.\build\Debug\StudioCore.exe
```

If using a single-configuration generator, the executable may instead be under
`build/StudioCore.exe`.

## Credits

Lilia D. (lilia-delgadillo) - Dev / Founder

### Brands

NDI® is a registered trademark of Vizrt NDI AB - [NDI](https://ndi.video) 
