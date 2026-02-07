#pragma once

#include <easy2d/core/types.h>
#include <easy2d/graphics/texture.h>
#include <easy2d/graphics/alpha_mask.h>
#include <easy2d/graphics/font.h>
#include <easy2d/audio/sound.h>
#include <string>
#include <vector>
#include <unordered_map>
#include <mutex>

namespace easy2d {

// ============================================================================
// 资源管理器 - 统一管理纹理、字体、音效等资源
// ============================================================================
class ResourceManager {
public:
    // ------------------------------------------------------------------------
    // 单例访问
    // ------------------------------------------------------------------------
    static ResourceManager& getInstance();

    // ------------------------------------------------------------------------
    // 搜索路径管理
    // ------------------------------------------------------------------------
    
    /// 添加资源搜索路径
    void addSearchPath(const std::string& path);
    
    /// 移除资源搜索路径
    void removeSearchPath(const std::string& path);
    
    /// 清空所有搜索路径
    void clearSearchPaths();
    
    /// 获取搜索路径列表
    const std::vector<std::string>& getSearchPaths() const { return searchPaths_; }
    
    /// 查找资源文件完整路径
    std::string findResourcePath(const std::string& filename) const;

    // ------------------------------------------------------------------------
    // 纹理资源
    // ------------------------------------------------------------------------
    
    /// 加载纹理（带缓存）
    Ptr<Texture> loadTexture(const std::string& filepath);
    
    /// 加载纹理并生成Alpha遮罩（用于不规则形状图片）
    Ptr<Texture> loadTextureWithAlphaMask(const std::string& filepath);
    
    /// 通过key获取已缓存的纹理
    Ptr<Texture> getTexture(const std::string& key) const;
    
    /// 检查纹理是否已缓存
    bool hasTexture(const std::string& key) const;
    
    /// 卸载指定纹理
    void unloadTexture(const std::string& key);
    
    // ------------------------------------------------------------------------
    // Alpha遮罩资源
    // ------------------------------------------------------------------------
    
    /// 获取纹理的Alpha遮罩（如果已生成）
    const AlphaMask* getAlphaMask(const std::string& textureKey) const;
    
    /// 为已加载的纹理生成Alpha遮罩
    bool generateAlphaMask(const std::string& textureKey);
    
    /// 检查纹理是否有Alpha遮罩
    bool hasAlphaMask(const std::string& textureKey) const;

    // ------------------------------------------------------------------------
    // 字体图集资源
    // ------------------------------------------------------------------------
    
    /// 加载字体图集（带缓存）
    Ptr<FontAtlas> loadFont(const std::string& filepath, int fontSize, bool useSDF = false);
    
    /// 通过key获取已缓存的字体图集
    Ptr<FontAtlas> getFont(const std::string& key) const;
    
    /// 检查字体是否已缓存
    bool hasFont(const std::string& key) const;
    
    /// 卸载指定字体
    void unloadFont(const std::string& key);

    // ------------------------------------------------------------------------
    // 音效资源
    // ------------------------------------------------------------------------
    
    /// 加载音效（带缓存）
    Ptr<Sound> loadSound(const std::string& filepath);
    Ptr<Sound> loadSound(const std::string& name, const std::string& filepath);
    
    /// 通过key获取已缓存的音效
    Ptr<Sound> getSound(const std::string& key) const;
    
    /// 检查音效是否已缓存
    bool hasSound(const std::string& key) const;
    
    /// 卸载指定音效
    void unloadSound(const std::string& key);

    // ------------------------------------------------------------------------
    // 缓存清理
    // ------------------------------------------------------------------------
    
    /// 清理所有失效的弱引用（自动清理已释放的资源）
    void purgeUnused();
    
    /// 清理指定类型的所有缓存
    void clearTextureCache();
    void clearFontCache();
    void clearSoundCache();
    
    /// 清理所有资源缓存
    void clearAllCaches();
    
    /// 获取各类资源的缓存数量
    size_t getTextureCacheSize() const;
    size_t getFontCacheSize() const;
    size_t getSoundCacheSize() const;

    ResourceManager();
    ~ResourceManager();
    ResourceManager(const ResourceManager&) = delete;
    ResourceManager& operator=(const ResourceManager&) = delete;

    // 生成字体缓存key
    std::string makeFontKey(const std::string& filepath, int fontSize, bool useSDF) const;
    
    // 互斥锁保护缓存
    mutable std::mutex textureMutex_;
    mutable std::mutex fontMutex_;
    mutable std::mutex soundMutex_;
    
    // 搜索路径
    std::vector<std::string> searchPaths_;
    
    // 资源缓存 - 使用弱指针实现自动清理
    std::unordered_map<std::string, WeakPtr<Texture>> textureCache_;
    std::unordered_map<std::string, WeakPtr<FontAtlas>> fontCache_;
    std::unordered_map<std::string, WeakPtr<Sound>> soundCache_;
};

} // namespace easy2d
