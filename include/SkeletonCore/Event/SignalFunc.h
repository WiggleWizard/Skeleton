#pragma once

#include "SkeletonDefs.h"

#include <EASTL/array.h>


SKELETON_NAMESPACE_BEGIN(SkeletonCore)

using DelegateKey = ea::array<std::uintptr_t, 2>;

template<typename RT> class SignalFunc;
template<typename RT, typename... Args>
class SignalFunc<RT(Args...)> final
{
    template<typename> friend class Observer;

    using Thunk = RT(*)(void*, Args&&...);

    static inline SignalFunc Bind(DelegateKey const& delegateKey)
    {
        return
        {
            reinterpret_cast<void*>(delegateKey[0]),
            reinterpret_cast<Thunk>(delegateKey[1])
        };
    }

public:

    // Factory methods
    template<auto FunctionPtr>
    static inline SignalFunc Bind()
    {
        return
        {
            nullptr, [](void* /*NULL*/, Args&&... args)
            {
                return (*FunctionPtr)(std::forward<Args>(args)...);
            }
        };
    }

    template<auto Ptr, typename T>
    static inline SignalFunc Bind(T* pointer)
    {
        return
        {
            pointer, [](void* thisPtr, Args&&... args)
            {
                return (static_cast<T*>(thisPtr)->*Ptr)(std::forward<Args>(args)...);
            }
        };
    }

    template<typename L>
    static inline SignalFunc Bind(L* pointer)
    {
        return
        {
            pointer, [](void* thisPtr, Args&&... args)
            {
                return static_cast<L*>(thisPtr)->operator()(std::forward<Args>(args)...);
            }
        };
    }

    // Execution operator
    template<typename... Uref>
    inline RT operator()(Uref&&... args) const
    {
        return (*functionPointer)(instancePointer, static_cast<Args&&>(args)...);
    }

    inline operator DelegateKey() const
    {
        return
        {
            reinterpret_cast<std::uintptr_t>(instancePointer),
            reinterpret_cast<std::uintptr_t>(functionPointer)
        };
    }

    void* const instancePointer;
    const Thunk functionPointer;
};

SKELETON_NAMESPACE_END()