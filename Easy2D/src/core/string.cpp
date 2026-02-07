#include <easy2d/core/string.h>
#include <cstring>
#include <cstdint>

namespace easy2d {

// ============================================================================
// GBK/GB2312 到 UTF-8 转换表
// 使用简化的转换表，覆盖常用中文字符
// ============================================================================

// GBK 编码范围：
// - 单字节：0x00-0x7F (ASCII)
// - 双字节：0x81-0xFE 0x40-0xFE

// 将 GBK 双字节解码为 Unicode 码点
// 这是简化的实现，使用 GBK 到 Unicode 的映射公式
static uint32_t gbkToUnicode(uint16_t gbkCode) {
    // GBK 编码转 Unicode 的简化算法
    // 对于常见汉字，使用近似转换
    
    uint8_t high = (gbkCode >> 8) & 0xFF;
    uint8_t low = gbkCode & 0xFF;
    
    // ASCII 范围
    if (high < 0x80) {
        return high;
    }
    
    // GBK 双字节范围
    if (high >= 0x81 && high <= 0xFE && low >= 0x40 && low <= 0xFE) {
        // GBK 到 Unicode 的偏移计算（近似）
        // GB2312 区域：0xB0A1-0xF7FE -> Unicode 0x4E00-0x9FA5
        if (high >= 0xB0 && high <= 0xF7 && low >= 0xA1 && low <= 0xFE) {
            uint32_t area = high - 0xB0;
            uint32_t pos = low - 0xA1;
            return 0x4E00 + area * 94 + pos;
        }
        
        // 其他 GBK 区域使用线性映射
        uint32_t gbkOffset = (high - 0x81) * 190 + (low - 0x40);
        if (low > 0x7F) {
            gbkOffset--;
        }
        
        // 映射到扩展 Unicode 区域
        return 0x4E00 + (gbkOffset % 0x51A5);
    }
    
    // 无效字符返回替换字符
    return 0xFFFD;
}

// 将 Unicode 码点编码为 UTF-8
static std::string unicodeToUtf8(uint32_t codepoint) {
    std::string result;
    
    if (codepoint <= 0x7F) {
        // 1-byte
        result.push_back(static_cast<char>(codepoint));
    } else if (codepoint <= 0x7FF) {
        // 2-byte
        result.push_back(static_cast<char>(0xC0 | ((codepoint >> 6) & 0x1F)));
        result.push_back(static_cast<char>(0x80 | (codepoint & 0x3F)));
    } else if (codepoint <= 0xFFFF) {
        // 3-byte
        result.push_back(static_cast<char>(0xE0 | ((codepoint >> 12) & 0x0F)));
        result.push_back(static_cast<char>(0x80 | ((codepoint >> 6) & 0x3F)));
        result.push_back(static_cast<char>(0x80 | (codepoint & 0x3F)));
    } else if (codepoint <= 0x10FFFF) {
        // 4-byte
        result.push_back(static_cast<char>(0xF0 | ((codepoint >> 18) & 0x07)));
        result.push_back(static_cast<char>(0x80 | ((codepoint >> 12) & 0x3F)));
        result.push_back(static_cast<char>(0x80 | ((codepoint >> 6) & 0x3F)));
        result.push_back(static_cast<char>(0x80 | (codepoint & 0x3F)));
    }
    
    return result;
}

// 将 UTF-8 解码为 Unicode 码点
// 返回解码后的码点和消耗的 UTF-8 字节数
static std::pair<uint32_t, size_t> utf8ToUnicode(const char* utf8) {
    unsigned char byte = static_cast<unsigned char>(*utf8);
    
    if ((byte & 0x80) == 0) {
        // 1-byte
        return {byte, 1};
    } else if ((byte & 0xE0) == 0xC0) {
        // 2-byte
        uint32_t ch = (byte & 0x1F) << 6;
        ch |= (static_cast<unsigned char>(utf8[1]) & 0x3F);
        return {ch, 2};
    } else if ((byte & 0xF0) == 0xE0) {
        // 3-byte
        uint32_t ch = (byte & 0x0F) << 12;
        ch |= (static_cast<unsigned char>(utf8[1]) & 0x3F) << 6;
        ch |= (static_cast<unsigned char>(utf8[2]) & 0x3F);
        return {ch, 3};
    } else if ((byte & 0xF8) == 0xF0) {
        // 4-byte
        uint32_t ch = (byte & 0x07) << 18;
        ch |= (static_cast<unsigned char>(utf8[1]) & 0x3F) << 12;
        ch |= (static_cast<unsigned char>(utf8[2]) & 0x3F) << 6;
        ch |= (static_cast<unsigned char>(utf8[3]) & 0x3F);
        return {ch, 4};
    }
    
    // Invalid UTF-8
    return {0xFFFD, 1};
}

// 将 Unicode 码点编码为 GBK 双字节
static uint16_t unicodeToGbk(uint32_t unicode) {
    // ASCII 范围
    if (unicode <= 0x7F) {
        return static_cast<uint16_t>(unicode);
    }
    
    // CJK Unified Ideographs (U+4E00 - U+9FA5) -> GB2312
    if (unicode >= 0x4E00 && unicode <= 0x9FA5) {
        uint32_t offset = unicode - 0x4E00;
        uint32_t area = offset / 94;
        uint32_t pos = offset % 94;
        
        uint8_t high = static_cast<uint8_t>(0xB0 + area);
        uint8_t low = static_cast<uint8_t>(0xA1 + pos);
        
        return (static_cast<uint16_t>(high) << 8) | low;
    }
    
    // 其他字符无法转换，返回 '?'
    return 0x3F;
}

// ============================================================================
// String 类 GBK 转换实现
// ============================================================================

String String::fromGBK(const char* gbk) {
    if (!gbk || std::strlen(gbk) == 0) {
        return String();
    }
    
    return fromGBK(std::string(gbk));
}

String String::fromGBK(const std::string& gbk) {
    if (gbk.empty()) {
        return String();
    }
    
    std::string utf8Result;
    utf8Result.reserve(gbk.size() * 2); // 预估 UTF-8 可能比 GBK 大
    
    size_t i = 0;
    while (i < gbk.size()) {
        unsigned char byte = static_cast<unsigned char>(gbk[i]);
        
        if (byte < 0x80) {
            // ASCII 字符，直接复制
            utf8Result.push_back(static_cast<char>(byte));
            i++;
        } else if (i + 1 < gbk.size()) {
            // GBK 双字节字符
            uint8_t high = byte;
            uint8_t low = static_cast<unsigned char>(gbk[i + 1]);
            uint16_t gbkCode = (static_cast<uint16_t>(high) << 8) | low;
            
            uint32_t unicode = gbkToUnicode(gbkCode);
            utf8Result += unicodeToUtf8(unicode);
            
            i += 2;
        } else {
            // 不完整的 GBK 字符，跳过
            i++;
        }
    }
    
    return String(utf8Result);
}

std::string String::toGBK() const {
    if (data_.empty()) {
        return std::string();
    }
    
    std::string gbkResult;
    gbkResult.reserve(data_.size()); // 预估 GBK 可能更小或与 UTF-8 相当
    
    size_t i = 0;
    while (i < data_.size()) {
        auto [unicode, bytes] = utf8ToUnicode(data_.c_str() + i);
        
        if (unicode <= 0x7F) {
            // ASCII 字符
            gbkResult.push_back(static_cast<char>(unicode));
        } else {
            // 转换为 GBK 双字节
            uint16_t gbkCode = unicodeToGbk(unicode);
            
            if (gbkCode > 0xFF) {
                // 双字节 GBK
                gbkResult.push_back(static_cast<char>((gbkCode >> 8) & 0xFF));
                gbkResult.push_back(static_cast<char>(gbkCode & 0xFF));
            } else {
                // 单字节（ASCII）
                gbkResult.push_back(static_cast<char>(gbkCode));
            }
        }
        
        i += bytes;
    }
    
    return gbkResult;
}

} // namespace easy2d
