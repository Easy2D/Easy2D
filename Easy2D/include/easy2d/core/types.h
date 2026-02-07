#pragma once

#include <cstdint>
#include <functional>
#include <memory>

namespace easy2d {

// ---------------------------------------------------------------------------
// 智能指针别名
// ---------------------------------------------------------------------------
template<typename T>
using Ptr = std::shared_ptr<T>;

template<typename T>
using UniquePtr = std::unique_ptr<T>;

template<typename T>
using WeakPtr = std::weak_ptr<T>;

/// 创建 shared_ptr 的便捷函数
template<typename T, typename... Args>
inline Ptr<T> makePtr(Args&&... args) {
    return std::make_shared<T>(std::forward<Args>(args)...);
}

/// 创建 unique_ptr 的便捷函数
template<typename T, typename... Args>
inline UniquePtr<T> makeUnique(Args&&... args) {
    return std::make_unique<T>(std::forward<Args>(args)...);
}

// ---------------------------------------------------------------------------
// 函数别名
// ---------------------------------------------------------------------------
template<typename Sig>
using Function = std::function<Sig>;

// ---------------------------------------------------------------------------
// 基础类型别名
// ---------------------------------------------------------------------------
using int8   = std::int8_t;
using int16  = std::int16_t;
using int32  = std::int32_t;
using int64  = std::int64_t;
using uint8  = std::uint8_t;
using uint16 = std::uint16_t;
using uint32 = std::uint32_t;
using uint64 = std::uint64_t;

} // namespace easy2d
