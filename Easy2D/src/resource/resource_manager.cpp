#include <easy2d/resource/resource_manager.h>
#include <easy2d/graphics/opengl/gl_texture.h>
#include <easy2d/graphics/opengl/gl_font_atlas.h>
#include <easy2d/audio/audio_engine.h>
#include <easy2d/utils/logger.h>
#include <algorithm>
#include <filesystem>
#include <cstring>

namespace easy2d {

ResourceManager::ResourceManager() = default;
ResourceManager::~ResourceManager() = default;

ResourceManager& ResourceManager::getInstance() {
    static ResourceManager instance;
    return instance;
}

// ============================================================================
// 搜索路径管理
// ============================================================================

void ResourceManager::addSearchPath(const std::string& path) {
    std::lock_guard<std::mutex> lock(textureMutex_);
    
    // 避免重复添加
    auto it = std::find(searchPaths_.begin(), searchPaths_.end(), path);
    if (it == searchPaths_.end()) {
        searchPaths_.push_back(path);
        E2D_LOG_DEBUG("ResourceManager: added search path: {}", path);
    }
}

void ResourceManager::removeSearchPath(const std::string& path) {
    std::lock_guard<std::mutex> lock(textureMutex_);
    
    auto it = std::find(searchPaths_.begin(), searchPaths_.end(), path);
    if (it != searchPaths_.end()) {
        searchPaths_.erase(it);
        E2D_LOG_DEBUG("ResourceManager: removed search path: {}", path);
    }
}

void ResourceManager::clearSearchPaths() {
    std::lock_guard<std::mutex> lock(textureMutex_);
    searchPaths_.clear();
    E2D_LOG_DEBUG("ResourceManager: cleared all search paths");
}

std::string ResourceManager::findResourcePath(const std::string& filename) const {
    // 首先检查是否是绝对路径或相对当前目录存在
    if (std::filesystem::exists(filename)) {
        return filename;
    }
    
    // 在搜索路径中查找
    std::lock_guard<std::mutex> lock(textureMutex_);
    for (const auto& path : searchPaths_) {
        std::filesystem::path fullPath = std::filesystem::path(path) / filename;
        if (std::filesystem::exists(fullPath)) {
            return fullPath.string();
        }
    }
    
    return "";
}

// ============================================================================
// 纹理资源
// ============================================================================

Ptr<Texture> ResourceManager::loadTexture(const std::string& filepath) {
    std::lock_guard<std::mutex> lock(textureMutex_);
    
    // 检查缓存
    auto it = textureCache_.find(filepath);
    if (it != textureCache_.end()) {
        if (auto texture = it->second.lock()) {
            E2D_LOG_TRACE("ResourceManager: texture cache hit: {}", filepath);
            return texture;
        }
        // 弱引用已失效，移除
        textureCache_.erase(it);
    }
    
    // 查找完整路径
    std::string fullPath = findResourcePath(filepath);
    if (fullPath.empty()) {
        E2D_LOG_ERROR("ResourceManager: texture file not found: {}", filepath);
        return nullptr;
    }
    
    // 创建新纹理
    try {
        auto texture = makePtr<GLTexture>(fullPath);
        if (!texture->isValid()) {
            E2D_LOG_ERROR("ResourceManager: failed to load texture: {}", filepath);
            return nullptr;
        }
        
        // 存入缓存
        textureCache_[filepath] = texture;
        E2D_LOG_DEBUG("ResourceManager: loaded texture: {}", filepath);
        return texture;
    } catch (...) {
        E2D_LOG_ERROR("ResourceManager: exception loading texture: {}", filepath);
        return nullptr;
    }
}

Ptr<Texture> ResourceManager::loadTextureWithAlphaMask(const std::string& filepath) {
    // 先加载纹理
    auto texture = loadTexture(filepath);
    if (!texture) {
        return nullptr;
    }
    
    // 生成Alpha遮罩
    generateAlphaMask(filepath);
    
    return texture;
}

const AlphaMask* ResourceManager::getAlphaMask(const std::string& textureKey) const {
    std::lock_guard<std::mutex> lock(textureMutex_);
    
    auto it = textureCache_.find(textureKey);
    if (it != textureCache_.end()) {
        if (auto texture = it->second.lock()) {
            GLTexture* glTexture = static_cast<GLTexture*>(texture.get());
            return glTexture->getAlphaMask();
        }
    }
    return nullptr;
}

bool ResourceManager::generateAlphaMask(const std::string& textureKey) {
    std::lock_guard<std::mutex> lock(textureMutex_);
    
    auto it = textureCache_.find(textureKey);
    if (it != textureCache_.end()) {
        if (auto texture = it->second.lock()) {
            GLTexture* glTexture = static_cast<GLTexture*>(texture.get());
            if (!glTexture->hasAlphaMask()) {
                glTexture->generateAlphaMask();
            }
            return glTexture->hasAlphaMask();
        }
    }
    return false;
}

bool ResourceManager::hasAlphaMask(const std::string& textureKey) const {
    std::lock_guard<std::mutex> lock(textureMutex_);
    
    auto it = textureCache_.find(textureKey);
    if (it != textureCache_.end()) {
        if (auto texture = it->second.lock()) {
            GLTexture* glTexture = static_cast<GLTexture*>(texture.get());
            return glTexture->hasAlphaMask();
        }
    }
    return false;
}

Ptr<Texture> ResourceManager::getTexture(const std::string& key) const {
    std::lock_guard<std::mutex> lock(textureMutex_);
    
    auto it = textureCache_.find(key);
    if (it != textureCache_.end()) {
        return it->second.lock();
    }
    return nullptr;
}

bool ResourceManager::hasTexture(const std::string& key) const {
    std::lock_guard<std::mutex> lock(textureMutex_);
    
    auto it = textureCache_.find(key);
    if (it != textureCache_.end()) {
        return !it->second.expired();
    }
    return false;
}

void ResourceManager::unloadTexture(const std::string& key) {
    std::lock_guard<std::mutex> lock(textureMutex_);
    textureCache_.erase(key);
    E2D_LOG_DEBUG("ResourceManager: unloaded texture: {}", key);
}

// ============================================================================
// 字体图集资源
// ============================================================================

std::string ResourceManager::makeFontKey(const std::string& filepath, int fontSize, bool useSDF) const {
    return filepath + "#" + std::to_string(fontSize) + (useSDF ? "#sdf" : "");
}

Ptr<FontAtlas> ResourceManager::loadFont(const std::string& filepath, int fontSize, bool useSDF) {
    std::lock_guard<std::mutex> lock(fontMutex_);
    
    std::string key = makeFontKey(filepath, fontSize, useSDF);
    
    // 检查缓存
    auto it = fontCache_.find(key);
    if (it != fontCache_.end()) {
        if (auto font = it->second.lock()) {
            E2D_LOG_TRACE("ResourceManager: font cache hit: {}", key);
            return font;
        }
        // 弱引用已失效，移除
        fontCache_.erase(it);
    }
    
    // 查找完整路径
    std::string fullPath = findResourcePath(filepath);
    if (fullPath.empty()) {
        E2D_LOG_ERROR("ResourceManager: font file not found: {}", filepath);
        return nullptr;
    }
    
    // 创建新字体图集
    try {
        auto font = makePtr<GLFontAtlas>(fullPath, fontSize, useSDF);
        if (!font->getTexture() || !font->getTexture()->isValid()) {
            E2D_LOG_ERROR("ResourceManager: failed to load font: {}", filepath);
            return nullptr;
        }
        
        // 存入缓存
        fontCache_[key] = font;
        E2D_LOG_DEBUG("ResourceManager: loaded font: {} (size={}, sdf={})", filepath, fontSize, useSDF);
        return font;
    } catch (...) {
        E2D_LOG_ERROR("ResourceManager: exception loading font: {}", filepath);
        return nullptr;
    }
}

Ptr<FontAtlas> ResourceManager::getFont(const std::string& key) const {
    std::lock_guard<std::mutex> lock(fontMutex_);
    
    auto it = fontCache_.find(key);
    if (it != fontCache_.end()) {
        return it->second.lock();
    }
    return nullptr;
}

bool ResourceManager::hasFont(const std::string& key) const {
    std::lock_guard<std::mutex> lock(fontMutex_);
    
    auto it = fontCache_.find(key);
    if (it != fontCache_.end()) {
        return !it->second.expired();
    }
    return false;
}

void ResourceManager::unloadFont(const std::string& key) {
    std::lock_guard<std::mutex> lock(fontMutex_);
    fontCache_.erase(key);
    E2D_LOG_DEBUG("ResourceManager: unloaded font: {}", key);
}

// ============================================================================
// 音效资源
// ============================================================================

Ptr<Sound> ResourceManager::loadSound(const std::string& filepath) {
    return loadSound(filepath, filepath);
}

Ptr<Sound> ResourceManager::loadSound(const std::string& name, const std::string& filepath) {
    std::lock_guard<std::mutex> lock(soundMutex_);
    
    // 检查缓存
    auto it = soundCache_.find(name);
    if (it != soundCache_.end()) {
        if (auto sound = it->second.lock()) {
            E2D_LOG_TRACE("ResourceManager: sound cache hit: {}", name);
            return sound;
        }
        // 弱引用已失效，移除
        soundCache_.erase(it);
    }
    
    // 查找完整路径
    std::string fullPath = findResourcePath(filepath);
    if (fullPath.empty()) {
        E2D_LOG_ERROR("ResourceManager: sound file not found: {}", filepath);
        return nullptr;
    }
    
    // 使用 AudioEngine 加载音效
    auto sound = AudioEngine::getInstance().loadSound(name, fullPath);
    if (!sound) {
        E2D_LOG_ERROR("ResourceManager: failed to load sound: {}", filepath);
        return nullptr;
    }
    
    // 存入缓存
    soundCache_[name] = sound;
    E2D_LOG_DEBUG("ResourceManager: loaded sound: {}", filepath);
    return sound;
}

Ptr<Sound> ResourceManager::getSound(const std::string& key) const {
    std::lock_guard<std::mutex> lock(soundMutex_);
    
    auto it = soundCache_.find(key);
    if (it != soundCache_.end()) {
        return it->second.lock();
    }
    return nullptr;
}

bool ResourceManager::hasSound(const std::string& key) const {
    std::lock_guard<std::mutex> lock(soundMutex_);
    
    auto it = soundCache_.find(key);
    if (it != soundCache_.end()) {
        return !it->second.expired();
    }
    return false;
}

void ResourceManager::unloadSound(const std::string& key) {
    std::lock_guard<std::mutex> lock(soundMutex_);
    
    // 从 AudioEngine 也卸载
    AudioEngine::getInstance().unloadSound(key);
    soundCache_.erase(key);
    E2D_LOG_DEBUG("ResourceManager: unloaded sound: {}", key);
}

// ============================================================================
// 缓存清理
// ============================================================================

void ResourceManager::purgeUnused() {
    // 清理纹理缓存
    {
        std::lock_guard<std::mutex> lock(textureMutex_);
        for (auto it = textureCache_.begin(); it != textureCache_.end();) {
            if (it->second.expired()) {
                E2D_LOG_TRACE("ResourceManager: purging unused texture: {}", it->first);
                it = textureCache_.erase(it);
            } else {
                ++it;
            }
        }
    }
    
    // 清理字体缓存
    {
        std::lock_guard<std::mutex> lock(fontMutex_);
        for (auto it = fontCache_.begin(); it != fontCache_.end();) {
            if (it->second.expired()) {
                E2D_LOG_TRACE("ResourceManager: purging unused font: {}", it->first);
                it = fontCache_.erase(it);
            } else {
                ++it;
            }
        }
    }
    
    // 清理音效缓存
    {
        std::lock_guard<std::mutex> lock(soundMutex_);
        for (auto it = soundCache_.begin(); it != soundCache_.end();) {
            if (it->second.expired()) {
                E2D_LOG_TRACE("ResourceManager: purging unused sound: {}", it->first);
                it = soundCache_.erase(it);
            } else {
                ++it;
            }
        }
    }
}

void ResourceManager::clearTextureCache() {
    std::lock_guard<std::mutex> lock(textureMutex_);
    size_t count = textureCache_.size();
    textureCache_.clear();
    E2D_LOG_INFO("ResourceManager: cleared {} textures from cache", count);
}

void ResourceManager::clearFontCache() {
    std::lock_guard<std::mutex> lock(fontMutex_);
    size_t count = fontCache_.size();
    fontCache_.clear();
    E2D_LOG_INFO("ResourceManager: cleared {} fonts from cache", count);
}

void ResourceManager::clearSoundCache() {
    std::lock_guard<std::mutex> lock(soundMutex_);
    size_t count = soundCache_.size();
    
    // 同时清理 AudioEngine 中的缓存
    for (const auto& pair : soundCache_) {
        AudioEngine::getInstance().unloadSound(pair.first);
    }
    soundCache_.clear();
    E2D_LOG_INFO("ResourceManager: cleared {} sounds from cache", count);
}

void ResourceManager::clearAllCaches() {
    clearTextureCache();
    clearFontCache();
    clearSoundCache();
    E2D_LOG_INFO("ResourceManager: all caches cleared");
}

size_t ResourceManager::getTextureCacheSize() const {
    std::lock_guard<std::mutex> lock(textureMutex_);
    return textureCache_.size();
}

size_t ResourceManager::getFontCacheSize() const {
    std::lock_guard<std::mutex> lock(fontMutex_);
    return fontCache_.size();
}

size_t ResourceManager::getSoundCacheSize() const {
    std::lock_guard<std::mutex> lock(soundMutex_);
    return soundCache_.size();
}

} // namespace easy2d
