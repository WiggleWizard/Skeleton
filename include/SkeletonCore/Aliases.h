#pragma once

#define NAMESPACE_ALIAS(og, n) \
	namespace og {};           \
	namespace n = og;

#define SKELETON_NAMESPACE_ALIAS(ext) NAMESPACE_ALIAS(Skeleton##ext, Sk##ext)

// Allows accessing SkeletonXXX namespaces as SkXXX
NAMESPACE_ALIAS(Skeleton, Sk)
SKELETON_NAMESPACE_ALIAS(Core);
SKELETON_NAMESPACE_ALIAS(OS);
SKELETON_NAMESPACE_ALIAS(Perf);
SKELETON_NAMESPACE_ALIAS(ImGui);

// eastl can be really annoying to type, so we can just make it shorter here...
NAMESPACE_ALIAS(eastl, ea);