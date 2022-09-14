include_guard()

# Unfortunately, for both EABase and EASTL they include themselves recursively within their tests dir.
# So if one feeds CPM.cmake the Git URL, it will use `--recursive` which results in the build system hanging...
# So as a work around I've used direct reference ZIPs (since we don't need their tests).

# EABase
CPMAddPackage(
    NAME EABase
    URL "https://github.com/electronicarts/EABase/archive/521cb053d9320636f53226ffc616216cf532f0ef.zip"
    VERSION 521cb053d9320636f53226ffc616216cf532f0ef)

# EASTL - General STL for replacing standard C++ library
CPMAddPackage(
    NAME EASTL
    URL "https://github.com/electronicarts/EASTL/archive/5eb9b1ec09faaf5965125a07f3907a3d9cd0e1e7.zip"
    VERSION 5eb9b1ec09faaf5965125a07f3907a3d9cd0e1e7)