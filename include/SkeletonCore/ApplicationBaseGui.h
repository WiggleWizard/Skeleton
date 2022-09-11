#include "ApplicationBase.h"


SKELETON_NAMESPACE_BEGIN(SkeletonCore)

class ApplicationBaseGui : public ApplicationBase
{
public:
	virtual void HideConsole();
	virtual void ShowConsole();
};

SKELETON_NAMESPACE_END()