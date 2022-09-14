#pragma once

#include "SkeletonDefs.h"

#include <EASTL/vector.h>
#include <EASTL/array.h>


SKELETON_NAMESPACE_BEGIN(SkeletonCore)

using DelegateKey = ea::array<std::uintptr_t, 2>;

// Single Threaded policy
class SignalPolicyST
{
public:
    // Explicit copy/ref
    template<typename T, typename L>
    inline T const& CopyOrRef(T const& param, L&&) const
    {
        return param;
    }

    constexpr auto LockGuard() const
    {
        return false;
    }

    constexpr auto ScopedLock(SignalPolicyST*) const
    {
        return false;
    }

protected:
    // Compiler generated impl
    SignalPolicyST() noexcept  = default;
    ~SignalPolicyST() noexcept = default;

    SignalPolicyST(const SignalPolicyST&) noexcept = default;
    SignalPolicyST& operator=(const SignalPolicyST&) noexcept = default;

    SignalPolicyST(SignalPolicyST&&) noexcept = default;
    SignalPolicyST& operator=(SignalPolicyST&&) noexcept = default;

    using WeakPtr = SignalPolicyST*;

    constexpr auto GetWeakPtr()
    {
        return this;
    }

    constexpr auto Observed(WeakPtr) const
    {
        return true;
    }

    constexpr auto Visiting(WeakPtr observer) const
    {
        return (observer == this ? nullptr : observer);
    }

    constexpr auto Unmask(WeakPtr observer) const
    {
        return observer;
    }

    constexpr void BeforeDisconnectAll() const
    {

    }
};

SKELETON_NAMESPACE_END()