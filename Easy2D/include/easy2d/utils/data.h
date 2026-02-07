#pragma once

#include <easy2d/core/types.h>
#include <string>

namespace easy2d {

// ============================================================================
// DataStore 类 - INI 文件数据持久化
// ============================================================================
class DataStore {
public:
    DataStore();
    ~DataStore();
    
    /// 加载 INI 文件
    bool load(const std::string& filename);
    
    /// 保存到 INI 文件
    bool save(const std::string& filename);
    
    /// 获取字符串值
    std::string getString(const std::string& section, const std::string& key, const std::string& defaultValue = "");
    
    /// 获取整数值
    int getInt(const std::string& section, const std::string& key, int defaultValue = 0);
    
    /// 获取浮点数值
    float getFloat(const std::string& section, const std::string& key, float defaultValue = 0.0f);
    
    /// 获取布尔值
    bool getBool(const std::string& section, const std::string& key, bool defaultValue = false);
    
    /// 设置字符串值
    void setString(const std::string& section, const std::string& key, const std::string& value);
    
    /// 设置整数值
    void setInt(const std::string& section, const std::string& key, int value);
    
    /// 设置浮点数值
    void setFloat(const std::string& section, const std::string& key, float value);
    
    /// 设置布尔值
    void setBool(const std::string& section, const std::string& key, bool value);
    
    /// 删除键
    void removeKey(const std::string& section, const std::string& key);
    
    /// 删除整个 section
    void removeSection(const std::string& section);
    
    /// 检查键是否存在
    bool hasKey(const std::string& section, const std::string& key);
    
    /// 检查 section 是否存在
    bool hasSection(const std::string& section);
    
    /// 清除所有数据
    void clear();
    
    /// 获取当前文件名
    const std::string& getFilename() const { return filename_; }

private:
    class Impl;
    UniquePtr<Impl> impl_;
    std::string filename_;
};

} // namespace easy2d
