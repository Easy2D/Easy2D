#pragma once

#include <easy2d/core/types.h>
#include <easy2d/core/math_types.h>
#include <vector>

namespace easy2d {

// ============================================================================
// Alpha 遮罩 - 存储图片的非透明区域信息
// ============================================================================
class AlphaMask {
public:
    AlphaMask() = default;
    AlphaMask(int width, int height);
    
    /// 从像素数据创建遮罩
    static AlphaMask createFromPixels(const uint8_t* pixels, int width, int height, int channels);
    
    /// 获取指定位置的透明度（0-255）
    uint8_t getAlpha(int x, int y) const;
    
    /// 检查指定位置是否不透明
    bool isOpaque(int x, int y, uint8_t threshold = 128) const;
    
    /// 检查指定位置是否在遮罩范围内
    bool isValid(int x, int y) const;
    
    /// 获取遮罩尺寸
    int getWidth() const { return width_; }
    int getHeight() const { return height_; }
    Size getSize() const { return Size(static_cast<float>(width_), static_cast<float>(height_)); }
    
    /// 获取原始数据
    const std::vector<uint8_t>& getData() const { return data_; }
    
    /// 检查遮罩是否有效
    bool isValid() const { return !data_.empty() && width_ > 0 && height_ > 0; }

private:
    int width_ = 0;
    int height_ = 0;
    std::vector<uint8_t> data_;  // Alpha值数组
};

} // namespace easy2d
