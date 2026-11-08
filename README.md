# StudioCore V0.1 Starter

A small Windows-first C++20 / Qt 6 starter for the StudioCore live-production project.

## Current checkpoint

- Qt 6 desktop application
- `ProductionEngine` owns an abstract `IVideoSource`
- `TestPatternSource` generates 1280x720 frames
- Program preview canvas displays the generated frames

This is intentionally a CPU-backed prototype. It validates the source/engine/UI boundary
before adding the Direct3D 11 renderer and NDI SDK. It is not yet a production video
pipeline and does not yet implement Preview/Program switching.

## Requirements

- Windows 10 or 11
- Visual Studio 2022 with the Desktop development with C++ workload
- CMake 3.21+
- Qt 6.5+ with an MSVC 2022 kit

## Configure and build

Open a Developer PowerShell for VS 2022, then from this directory:

```powershell
cmake -S . -B build -DCMAKE_PREFIX_PATH="C:/Qt/6.x.x/msvc2022_64"
cmake --build build --config Debug
```

Replace the Qt path with the actual location of your Qt MSVC installation.

Run:

```powershell
.\build\Debug\StudioCore.exe
```

If using a single-configuration generator, the executable may instead be under
`build/StudioCore.exe`.

## Next checkpoint

1. Add a separate Preview channel and source selection.
2. Add CUT to copy Preview's selected source to Program.
3. Add a source list and basic status indicators.
4. Replace the prototype image path with a Direct3D 11 texture pipeline.
5. Add NDI input and output adapters.
