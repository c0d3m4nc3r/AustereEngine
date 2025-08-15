#pragma once

#include <memory>

namespace AE
{
    template <typename T>
    using SPtr = std::shared_ptr<T>;

    template <typename T>
    using UPtr = std::unique_ptr<T>;

    template <typename T>
    using WPtr = std::weak_ptr<T>;

    template <typename T, typename... Args>
    SPtr<T> MakeSPtr(Args&&... args)
    {
        return std::make_shared<T>(std::forward<Args>(args)...);
    }

    template <typename T, typename... Args>
    UPtr<T> MakeUPtr(Args&&... args)
    {
        return std::make_unique<T>(std::forward<Args>(args)...);
    }
}
