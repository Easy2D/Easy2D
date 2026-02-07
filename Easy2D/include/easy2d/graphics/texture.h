#pragma once

#include <easy2d/core/types.h>
#include <easy2d/core/math_types.h>

namespace easy2d {

// ============================================================================
// 纹理接口
// ============================================================================
class Texture {
public:
    virtual ~Texture() = default;

    // 获取尺寸
    virtual int getWidth() const = 0;
    virtual int getHeight() const = 0;
    virtual Size getSize() const = 0;
    
    // 获取通道数
    virtual int getChannels() const = 0;
    
    // 获取原始句柄（用于底层渲染）
    virtual void* getNativeHandle() const = 0;
    
    // 是否有效
    virtual bool isValid() const = 0;
    
    // 设置过滤模式
    virtual void setFilter(bool linear) = 0;
    
    // 设置环绕模式
    virtual void setWrap(bool repeat) = 0;
};

} // namespace easy2d
