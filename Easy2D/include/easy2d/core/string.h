#pragma once

#include <easy2d/core/types.h>
#include <string>
#include <vector>
#include <codecvt>
#include <locale>
#include <cstring>

namespace easy2d {

// ============================================================================
// String 类 - 跨平台字符串，内部统一使用 UTF-8 存储
// ============================================================================
class String {
public:
    // ------------------------------------------------------------------------
    // 构造函数
    // ------------------------------------------------------------------------
    String() = default;
    
    String(const char* utf8) : data_(utf8 ? utf8 : "") {}
    String(const std::string& utf8) : data_(utf8) {}
    explicit String(const wchar_t* wide);
    explicit String(const std::wstring& wide);
    explicit String(const char16_t* utf16);
    explicit String(const std::u16string& utf16);
    explicit String(const char32_t* utf32);
    explicit String(const std::u32string& utf32);

    // ------------------------------------------------------------------------
    // 静态工厂方法
    // ------------------------------------------------------------------------
    static String fromUtf8(const char* utf8);
    static String fromUtf8(const std::string& utf8);
    static String fromWide(const wchar_t* wide);
    static String fromWide(const std::wstring& wide);
    static String fromUtf16(const char16_t* utf16);
    static String fromUtf16(const std::u16string& utf16);
    static String fromUtf32(const char32_t* utf32);
    static String fromUtf32(const std::u32string& utf32);
    
    /// 从 GBK/GB2312 编码构造（Windows 中文系统常用）
    static String fromGBK(const char* gbk);
    static String fromGBK(const std::string& gbk);

    // ------------------------------------------------------------------------
    // 编码转换
    // ------------------------------------------------------------------------
    const std::string& toUtf8() const { return data_; }
    std::string toUtf8String() const { return data_; }
    
    std::wstring toWide() const;
    std::u16string toUtf16() const;
    std::u32string toUtf32() const;
    
    /// 转换为 GBK/GB2312 编码（Windows 中文系统常用）
    std::string toGBK() const;

    // ------------------------------------------------------------------------
    // 基础操作
    // ------------------------------------------------------------------------
    /// 返回 Unicode 字符数（不是字节数）
    size_t length() const;
    
    /// 返回 UTF-8 字节数
    size_t byteSize() const { return data_.size(); }
    
    bool empty() const { return data_.empty(); }
    
    const char* c_str() const { return data_.c_str(); }
    const std::string& str() const { return data_; }
    std::string& str() { return data_; }

    // ------------------------------------------------------------------------
    // 字符串操作
    // ------------------------------------------------------------------------
    void clear() { data_.clear(); }
    
    String& append(const String& other);
    String& append(const char* utf8);
    
    String substring(size_t start, size_t len = npos) const;
    
    /// 查找子串，返回 Unicode 字符索引，未找到返回 npos
    size_t find(const String& substr, size_t start = 0) const;
    
    /// 是否以指定字符串开头
    bool startsWith(const String& prefix) const;
    
    /// 是否以指定字符串结尾
    bool endsWith(const String& suffix) const;
    
    /// 去除首尾空白字符
    String trim() const;
    
    /// 分割字符串
    std::vector<String> split(const String& delimiter) const;
    
    /// 替换所有匹配子串
    String replaceAll(const String& from, const String& to) const;

    // ------------------------------------------------------------------------
    // 运算符重载
    // ------------------------------------------------------------------------
    String operator+(const String& other) const;
    String& operator+=(const String& other);
    
    bool operator==(const String& other) const { return data_ == other.data_; }
    bool operator!=(const String& other) const { return data_ != other.data_; }
    bool operator<(const String& other) const { return data_ < other.data_; }
    bool operator>(const String& other) const { return data_ > other.data_; }
    
    char operator[](size_t index) const { return data_[index]; }

    // ------------------------------------------------------------------------
    // 迭代器支持
    // ------------------------------------------------------------------------
    auto begin() { return data_.begin(); }
    auto end() { return data_.end(); }
    auto begin() const { return data_.begin(); }
    auto end() const { return data_.end(); }
    auto cbegin() const { return data_.cbegin(); }
    auto cend() const { return data_.cend(); }

    // ------------------------------------------------------------------------
    // 静态常量
    // ------------------------------------------------------------------------
    static constexpr size_t npos = static_cast<size_t>(-1);

    // ------------------------------------------------------------------------
    // 格式化字符串（类似 sprintf）
    // ------------------------------------------------------------------------
    template<typename... Args>
    static String format(const char* fmt, Args&&... args);

private:
    std::string data_; // 内部使用 UTF-8 存储
    
    // UTF-8 辅助函数
    static size_t utf8Length(const std::string& str);
    static std::string utf8Substring(const std::string& str, size_t start, size_t len);
    static size_t utf8CharIndexToByteIndex(const std::string& str, size_t charIndex);
};

// ============================================================================
// 内联实现
// ============================================================================

inline String::String(const wchar_t* wide) {
    if (wide) {
        std::wstring wstr(wide);
        *this = fromWide(wstr);
    }
}

inline String::String(const std::wstring& wide) {
    *this = fromWide(wide);
}

inline String::String(const char16_t* utf16) {
    if (utf16) {
        std::u16string u16str(utf16);
        *this = fromUtf16(u16str);
    }
}

inline String::String(const std::u16string& utf16) {
    *this = fromUtf16(utf16);
}

inline String::String(const char32_t* utf32) {
    if (utf32) {
        std::u32string u32str(utf32);
        *this = fromUtf32(u32str);
    }
}

inline String::String(const std::u32string& utf32) {
    *this = fromUtf32(utf32);
}

// 静态工厂方法
inline String String::fromUtf8(const char* utf8) {
    return String(utf8 ? utf8 : "");
}

inline String String::fromUtf8(const std::string& utf8) {
    return String(utf8);
}

// 编码转换实现
inline std::wstring String::toWide() const {
    if (data_.empty()) return std::wstring();
    
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    try {
        return converter.from_bytes(data_);
    } catch (...) {
        return std::wstring();
    }
}

inline String String::fromWide(const std::wstring& wide) {
    if (wide.empty()) return String();
    
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    try {
        return String(converter.to_bytes(wide));
    } catch (...) {
        return String();
    }
}

inline String String::fromWide(const wchar_t* wide) {
    return wide ? fromWide(std::wstring(wide)) : String();
}

inline std::u16string String::toUtf16() const {
    if (data_.empty()) return std::u16string();
    
    std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> converter;
    try {
        return converter.from_bytes(data_);
    } catch (...) {
        return std::u16string();
    }
}

inline String String::fromUtf16(const std::u16string& utf16) {
    if (utf16.empty()) return String();
    
    std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> converter;
    try {
        return String(converter.to_bytes(utf16));
    } catch (...) {
        return String();
    }
}

inline String String::fromUtf16(const char16_t* utf16) {
    return utf16 ? fromUtf16(std::u16string(utf16)) : String();
}

inline std::u32string String::toUtf32() const {
    std::u32string result;
    result.reserve(length());
    
    const char* ptr = data_.c_str();
    const char* end = ptr + data_.size();
    
    while (ptr < end) {
        char32_t ch = 0;
        unsigned char byte = static_cast<unsigned char>(*ptr);
        
        if ((byte & 0x80) == 0) {
            // 1-byte sequence
            ch = byte;
            ptr += 1;
        } else if ((byte & 0xE0) == 0xC0) {
            // 2-byte sequence
            ch = (byte & 0x1F) << 6;
            ch |= (static_cast<unsigned char>(ptr[1]) & 0x3F);
            ptr += 2;
        } else if ((byte & 0xF0) == 0xE0) {
            // 3-byte sequence
            ch = (byte & 0x0F) << 12;
            ch |= (static_cast<unsigned char>(ptr[1]) & 0x3F) << 6;
            ch |= (static_cast<unsigned char>(ptr[2]) & 0x3F);
            ptr += 3;
        } else if ((byte & 0xF8) == 0xF0) {
            // 4-byte sequence
            ch = (byte & 0x07) << 18;
            ch |= (static_cast<unsigned char>(ptr[1]) & 0x3F) << 12;
            ch |= (static_cast<unsigned char>(ptr[2]) & 0x3F) << 6;
            ch |= (static_cast<unsigned char>(ptr[3]) & 0x3F);
            ptr += 4;
        } else {
            // Invalid UTF-8, skip
            ptr += 1;
            continue;
        }
        
        result.push_back(ch);
    }
    
    return result;
}

inline String String::fromUtf32(const std::u32string& utf32) {
    std::string result;
    
    for (char32_t ch : utf32) {
        if (ch <= 0x7F) {
            // 1-byte
            result.push_back(static_cast<char>(ch));
        } else if (ch <= 0x7FF) {
            // 2-byte
            result.push_back(static_cast<char>(0xC0 | ((ch >> 6) & 0x1F)));
            result.push_back(static_cast<char>(0x80 | (ch & 0x3F)));
        } else if (ch <= 0xFFFF) {
            // 3-byte
            result.push_back(static_cast<char>(0xE0 | ((ch >> 12) & 0x0F)));
            result.push_back(static_cast<char>(0x80 | ((ch >> 6) & 0x3F)));
            result.push_back(static_cast<char>(0x80 | (ch & 0x3F)));
        } else if (ch <= 0x10FFFF) {
            // 4-byte
            result.push_back(static_cast<char>(0xF0 | ((ch >> 18) & 0x07)));
            result.push_back(static_cast<char>(0x80 | ((ch >> 12) & 0x3F)));
            result.push_back(static_cast<char>(0x80 | ((ch >> 6) & 0x3F)));
            result.push_back(static_cast<char>(0x80 | (ch & 0x3F)));
        }
    }
    
    return String(result);
}

inline String String::fromUtf32(const char32_t* utf32) {
    return utf32 ? fromUtf32(std::u32string(utf32)) : String();
}

// UTF-8 辅助函数
inline size_t String::utf8Length(const std::string& str) {
    size_t len = 0;
    for (unsigned char c : str) {
        if ((c & 0xC0) != 0x80) {
            ++len;
        }
    }
    return len;
}

inline size_t String::length() const {
    return utf8Length(data_);
}

inline size_t String::utf8CharIndexToByteIndex(const std::string& str, size_t charIndex) {
    size_t charCount = 0;
    size_t byteIndex = 0;
    
    while (byteIndex < str.size() && charCount < charIndex) {
        unsigned char c = static_cast<unsigned char>(str[byteIndex]);
        if ((c & 0xC0) != 0x80) {
            ++charCount;
        }
        ++byteIndex;
    }
    
    return byteIndex;
}

inline std::string String::utf8Substring(const std::string& str, size_t start, size_t len) {
    size_t startByte = utf8CharIndexToByteIndex(str, start);
    size_t endByte = (len == npos) ? str.size() : utf8CharIndexToByteIndex(str, start + len);
    
    return str.substr(startByte, endByte - startByte);
}

inline String String::substring(size_t start, size_t len) const {
    return String(utf8Substring(data_, start, len));
}

// 字符串操作
inline String& String::append(const String& other) {
    data_.append(other.data_);
    return *this;
}

inline String& String::append(const char* utf8) {
    if (utf8) data_.append(utf8);
    return *this;
}

inline size_t String::find(const String& substr, size_t start) const {
    if (substr.empty() || start >= length()) return npos;
    
    size_t startByte = utf8CharIndexToByteIndex(data_, start);
    size_t pos = data_.find(substr.data_, startByte);
    
    if (pos == std::string::npos) return npos;
    
    // 转换字节索引到字符索引
    return utf8Length(data_.substr(0, pos));
}

inline bool String::startsWith(const String& prefix) const {
    if (prefix.data_.size() > data_.size()) return false;
    return data_.compare(0, prefix.data_.size(), prefix.data_) == 0;
}

inline bool String::endsWith(const String& suffix) const {
    if (suffix.data_.size() > data_.size()) return false;
    return data_.compare(data_.size() - suffix.data_.size(), suffix.data_.size(), suffix.data_) == 0;
}

inline String String::trim() const {
    size_t start = 0;
    while (start < data_.size() && std::isspace(static_cast<unsigned char>(data_[start]))) {
        ++start;
    }
    
    size_t end = data_.size();
    while (end > start && std::isspace(static_cast<unsigned char>(data_[end - 1]))) {
        --end;
    }
    
    return String(data_.substr(start, end - start));
}

inline std::vector<String> String::split(const String& delimiter) const {
    std::vector<String> result;
    
    if (delimiter.empty()) {
        result.push_back(*this);
        return result;
    }
    
    size_t start = 0;
    size_t end = data_.find(delimiter.data_, start);
    
    while (end != std::string::npos) {
        result.push_back(String(data_.substr(start, end - start)));
        start = end + delimiter.data_.size();
        end = data_.find(delimiter.data_, start);
    }
    
    result.push_back(String(data_.substr(start)));
    
    return result;
}

inline String String::replaceAll(const String& from, const String& to) const {
    if (from.empty()) return *this;
    
    std::string result = data_;
    size_t pos = 0;
    
    while ((pos = result.find(from.data_, pos)) != std::string::npos) {
        result.replace(pos, from.data_.size(), to.data_);
        pos += to.data_.size();
    }
    
    return String(result);
}

// 运算符重载
inline String String::operator+(const String& other) const {
    String result(*this);
    result.append(other);
    return result;
}

inline String& String::operator+=(const String& other) {
    return append(other);
}

// 格式化字符串
#include <cstdio>

template<typename... Args>
String String::format(const char* fmt, Args&&... args) {
    int size = std::snprintf(nullptr, 0, fmt, std::forward<Args>(args)...);
    if (size <= 0) return String();
    
    std::string result(size, '\0');
    std::snprintf(&result[0], size + 1, fmt, std::forward<Args>(args)...);
    
    return String(result);
}

// 全局运算符
inline String operator+(const char* lhs, const String& rhs) {
    return String(lhs) + rhs;
}

} // namespace easy2d
