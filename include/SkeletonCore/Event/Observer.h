#pragma once

#include "SignalPolicy.h"

#include <EASTL/algorithm.h>


SKELETON_NAMESPACE_BEGIN(SkeletonCore)

template<typename ThreadPolicy = SignalPolicyST>
class Observer : private ThreadPolicy
{
    template<typename, typename> friend class Signal;

    struct Connection
    {
        Connection() noexcept = default;

        Connection(const DelegateKey& key) : delegate(key), observer() {}
        Connection(const DelegateKey& key, Observer* obs) : delegate(key), observer(obs->GetWeakPtr()) {}

        DelegateKey delegate;
        typename SignalPolicyST::WeakPtr observer;
    };

    // Ordering function
    struct OrderFunc
    {
        inline bool operator()(const DelegateKey& lhs, const DelegateKey& rhs)
        {
            std::size_t x = lhs[0] ^ rhs[0];
            std::size_t y = lhs[1] ^ rhs[1];
            auto k = (x < y) && x < (x ^ y);
            return lhs[k] < rhs[k];
        }

        inline bool operator()(const Connection& lhs, const Connection& rhs) const
        {
            return operator()(lhs.delegate, rhs.delegate);
        }
    };

private:
    void NoLockInsert(const DelegateKey& key, Observer* observer)
    {
        auto begin = std::begin(_connections);
        auto end = std::end(_connections);

        _connections.emplace(std::upper_bound(begin, end, key, OrderFunc()), key, observer);
    }

    void Insert(const DelegateKey& key, Observer* observer)
    {
        [[maybe_unused]]
        auto lock = ThreadPolicy::LockGuard();
        NoLockInsert(key, observer);
    }

    void Remove(const DelegateKey& key) noexcept
    {
        [[maybe_unused]]
        auto lock = ThreadPolicy::LockGuard();

        auto begin = std::begin(_connections);
        auto end = std::end(_connections);

        auto slots = ea::equal_range(begin, end, key, OrderFunc());
        _connections.erase(slots.first, slots.second);
    }

private:
    template<typename Function, typename... Args>
    void ForEach(Args&&... args)
    {
        [[maybe_unused]]
        auto lock = ThreadPolicy::LockGuard();

        for(const auto& slot : ThreadPolicy::CopyOrRef(_connections, lock))
        {
            if(auto observer = ThreadPolicy::Observed(slot.observer))
            {
                Function::Bind(slot.delegate)(args...);
            }
        }
    }

private:
    void NoLockDisconnectAll() noexcept
    {
        // Kill all connections first before clearing local memory
        for(const auto& slot : _connections)
        {
            if(auto observed = ThreadPolicy::Visiting(slot.observer))
            {
                auto ptr = static_cast<Observer*>(ThreadPolicy::Unmask(observed));
                ptr->Remove(slot.delegate);
            }
        }

        _connections.clear();
    }

    void MoveConnectionsFrom(Observer* other) noexcept
    {
        [[maybe_unused]]
        auto lock = ThreadPolicy::ScopedLock(other);

        // Make sure this is disconnected and ready to receive
        NoLockDisconnectAll();

        // Disconnect other from everyone else and connect them to this
        for(const auto& slot : other->_connections)
        {
            if(auto observed = other->Visiting(slot.observer))
            {
                auto ptr = static_cast<Observer*>(ThreadPolicy::unmask(observed));
                ptr->remove(slot.delegate);
                ptr->insert(slot.delegate, this);
                NoLockInsert(slot.delegate, ptr);
            }
            // Connect free functions and function objects
            else
            {
                NoLockInsert(slot.delegate, this);
            }
        }

        other->_connections.clear();
    }

public:
    void DisconnectAll() noexcept
    {
        [[maybe_unused]]
        auto lock = ThreadPolicy::LockGuard();
        NoLockDisconnectAll();
    }

    bool IsEmpty() const noexcept
    {
        [[maybe_unused]]
        auto lock = ThreadPolicy::LockGuard();
        return _connections.empty();
    }

protected:
    ~Observer()
    {
        ThreadPolicy::BeforeDisconnectAll();
        DisconnectAll();
    }

    Observer() noexcept = default;

    // Observer may be movable depending on policy, but should never be copied
    Observer(const Observer&) noexcept = delete;
    Observer& operator= (const Observer&) noexcept = delete;

    // When moving an observer, make sure everyone it's connected to knows about it
    Observer(Observer&& other) noexcept
    {
        MoveConnectionsFrom(std::addressof(other));
    }

    Observer& operator=(Observer&& other) noexcept
    {
        MoveConnectionsFrom(std::addressof(other));
        return *this;
    }

private:
    ea::vector<Connection> _connections;
};

SKELETON_NAMESPACE_END()