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

## Patches to upstream source

We patch upstream source code only when there is no other way, following the
manual approach of the packaging guide (see "How do I patch upstream source
code?"). Next to each patched file `foo.cc` there is `foo.cc.orig`, a symlink
to the unmodified upstream file, and `foo.cc.patch`, the difference between
the two.

Since the `absl/` components are directory symlinks, patching a file requires
turning the directories leading to it into real directories that contain
symlinks to the remaining upstream entries. The directory symlinks inside
such a tree are declared in `.gitattributes`.

### `absl/time/internal/cctz/src/zone_info_source.cc`

With Clang targeting MSVC, cctz defines `zone_info_source_factory` with
`__attribute__((weak))`, for which Clang emits a `.weak.*` helper symbol. The
build2-generated DLL `.def` file exports this symbol and `link.exe` rejects
it with LNK1242. The patch makes Clang use the same `/alternatename` approach
as MSVC.

## Upgrading to a new upstream version

1. Update the `upstream/` submodule to the new release tag and update the
   package version in `libabseil/manifest`.

2. Re-create any symlinks for files/directories that upstream added or removed
   (compare against the upstream component layout).

3. For each `*.patch`, make a fresh copy of the new upstream file and
   re-apply the patch, regenerating it if it no longer applies cleanly:

       $ cp -H foo.cc.orig foo.cc
       $ patch <foo.cc.patch
       $ diff -u foo.cc.orig foo.cc >foo.cc.patch

   Drop the patch (and restore the directory symlink) once upstream has
   addressed the issue.

4. Build and run the tests in the native and MinGW cross configurations
   before releasing.
