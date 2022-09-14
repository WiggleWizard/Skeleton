#pragma once

#include "SignalPolicy.h"
#include "Observer.h"
#include "SignalFunc.h"


SKELETON_NAMESPACE_BEGIN(SkeletonCore)

// Specialization
template<typename ST, typename ThreadPolicy = SignalPolicyST>
class Signal;

// Main Signal class
template<typename ST, typename ThreadPolicy, typename... Args>
class Signal<ST(Args...), ThreadPolicy> final : public Observer<ThreadPolicy>
{
    using observer = Observer<ThreadPolicy>;
    using function = SignalFunc<ST(Args...)>;

    template<typename T>
    void insertInternal(DelegateKey const& key, typename T::Observer* instance)
    {
        observer::Insert(key, instance);
        instance->Insert(key, this);
    }
    template<typename T>
    void removeInternal(DelegateKey const& key, typename T::Observer* instance)
    {
        observer::Remove(key);
        instance->Remove(key);
    }

    template<typename T>
    void insertInternal(DelegateKey const& key, ...)
    {
        observer::Insert(key, this);
    }
    template<typename T>
    void removeInternal(DelegateKey const& key, ...)
    {
        observer::Remove(key);
    }

public:
    Signal() noexcept = default;
    ~Signal() noexcept = default;

    Signal(Signal const&) noexcept = delete;
    Signal& operator= (Signal const&) noexcept = delete;

    Signal(Signal&&) noexcept = default;
    Signal& operator=(Signal&&) noexcept = default;

public:
    template<typename L>
    void Connect(L* instance)
    {
        observer::Insert(function::template Bind(instance), this);
    }
    template<typename L>
    void Connect(L& instance)
    {
        Connect(std::addressof(instance));
    }

    template <ST(*FuncOffset)(Args...)>
    void Connect()
    {
        observer::Insert(function::template Bind<FuncOffset>(), this);
    }

    template<typename T, ST(T::*FuncOffset)(Args...)>
    void Connect(T* instance)
    {
        insertInternal<T>(function::template Bind<FuncOffset>(instance), instance);
    }
    template<typename T, ST(T::*FuncOffset)(Args...) const>
    void Connect(T* instance)
    {
        insertInternal<T>(function::template Bind<FuncOffset>(instance), instance);
    }

    template<typename T, ST(T::*FuncOffset)(Args...)>
    void Connect(T& instance)
    {
        Connect<FuncOffset, T>(std::addressof(instance));
    }
    template<typename T, ST(T::*FuncOffset)(Args...) const>
    void Connect(T& instance)
    {
        Connect<FuncOffset, T>(std::addressof(instance));
    }

    template<auto FuncOffset, typename T>
    void Connect(T* instance)
    {
        insertInternal<T>(function::template Bind<FuncOffset>(instance), instance);
    }
    template<auto FuncOffset, typename T>
    void Connect(T& instance)
    {
        //Connect<FuncOffset, T>(std::addressof(instance));
    }

public:
    template <typename... FuncArgs>
    void Fire(FuncArgs&&... args)
    {
        observer::template ForEach<function>(std::forward<FuncArgs>(args)...);
    }

    template <typename Accumulate, typename... FuncArgs>
    void FireAccumulate(Accumulate&& accumulate, FuncArgs&&... args)
    {
        observer::template ForEachAccumulate<function, Accumulate>(std::forward<Accumulate>(accumulate), std::forward<FuncArgs>(args)...);
    }
};

SKELETON_NAMESPACE_END()