# Developer/packaging notes for libabseil

This file documents how this build2 package is put together. It is intended
for maintainers of the package, not its consumers.

## Upstream source and layout

The upstream Abseil source code is tracked as a git submodule in `upstream/`
(see `.gitmodules`). The build2 package files (buildfiles, manifests, etc.)
are kept separate and the upstream component directories under `absl/` are
overlaid into the `libabseil/absl/` package directory using directory
symlinks. For example:

    libabseil/absl/base   -> ../../upstream/absl/base
    libabseil/absl/strings -> ../../upstream/absl/strings
    ...

These directory symlinks are also declared in `.gitattributes` with
`symlink=dir` so the package can be checked out from git directly on Windows.

## Upgrading to a new upstream version

1. Update the `upstream/` submodule to the new release tag and update the
   package version in `libabseil/manifest`.

2. Re-create any symlinks for files/directories that upstream added or removed
   (compare against the upstream component layout).

3. Build and run the tests in the native and MinGW cross configurations
   before releasing.
