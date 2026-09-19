# Prebuilt Sparkle dependency

Playground consumes Sparkle as an installed CMake package. A normal Playground configure must not clone, add, or build Sparkle source code.

CI downloads the pinned package through `tools/ci/install-sparkle-package.ps1`, verifies its SHA-256, extracts it under `build/dependencies/sparkle`, and exposes that prefix through `CMAKE_PREFIX_PATH`. The script contains the version, full Sparkle commit, release tag, archive name, and checksum in one place.

To update Sparkle:

1. let Sparkle's `Prebuilt Windows package` workflow publish the required immutable revision;
2. copy the revision and SHA-256 from that revision's GitHub Release;
3. update the pin block in `tools/ci/install-sparkle-package.ps1`;
4. run both Playground CI jobs and confirm the configure log contains no Sparkle vcpkg build.

For local development, install or extract a compatible Sparkle package and pass its prefix normally:

```powershell
cmake -S . -B build -G Ninja `
  -DCMAKE_CXX_COMPILER=clang-cl `
  -DCMAKE_BUILD_TYPE=Release `
  -DCMAKE_PREFIX_PATH=C:/path/to/sparkle/install `
  -DCMAKE_TOOLCHAIN_FILE=$env:VCPKG_INSTALLATION_ROOT/scripts/buildsystems/vcpkg.cmake
```

The prefix must contain `lib/cmake/sparkle/sparkleConfig.cmake`. Tests additionally require the published `Sparkle::TestLibrary` component. Sparkle must be rebuilt only when adopting a new Sparkle revision, changing its public headers/library, or changing the packaged platform, compiler ABI, CRT/linkage, configuration, or included components. Playground-only changes do not rebuild Sparkle.
