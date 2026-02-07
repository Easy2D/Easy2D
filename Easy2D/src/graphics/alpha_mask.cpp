#include <easy2d/graphics/alpha_mask.h>

namespace easy2d {

AlphaMask::AlphaMask(int width, int height)
    : width_(width)
    , height_(height)
    , data_(width * height, 255) {
}

AlphaMask AlphaMask::createFromPixels(const uint8_t* pixels, int width, int height, int channels) {
    AlphaMask mask(width, height);
    
    if (!pixels || width <= 0 || height <= 0) {
        return mask;
    }
    
    // 根据通道数提取Alpha值
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            int pixelIndex = (y * width + x) * channels;
            uint8_t alpha = 255;
            
            if (channels == 4) {
                // RGBA格式，Alpha在第四个通道
                alpha = pixels[pixelIndex + 3];
            } else if (channels == 1) {
                // 灰度图，直接作为Alpha
                alpha = pixels[pixelIndex];
            } else if (channels == 3) {
                // RGB格式，没有Alpha通道，视为不透明
                alpha = 255;
            }
            
            mask.data_[y * width + x] = alpha;
        }
    }
    
    return mask;
}

uint8_t AlphaMask::getAlpha(int x, int y) const {
    if (!isValid(x, y)) {
        return 0;
    }
    return data_[y * width_ + x];
}

bool AlphaMask::isOpaque(int x, int y, uint8_t threshold) const {
    return getAlpha(x, y) >= threshold;
}

bool AlphaMask::isValid(int x, int y) const {
    return x >= 0 && x < width_ && y >= 0 && y < height_;
}

} // namespace easy2d
