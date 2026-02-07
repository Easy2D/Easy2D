#include <easy2d/easy2d.h>
#include <easy2d/utils/logger.h>
#include <windows.h>

using namespace easy2d;

// ============================================================================
// 获取 Windows 系统字体路径
// ============================================================================
std::string getSystemFontPath() {
    char windowsDir[MAX_PATH];
    GetWindowsDirectoryA(windowsDir, MAX_PATH);
    return std::string(windowsDir) + "\\Fonts\\arial.ttf";
}

// ============================================================================
// 字体渲染测试场景
// ============================================================================
class FontTestScene : public Scene {
public:
    void onEnter() override {
        E2D_LOG_INFO("FontTestScene::onEnter - 字体渲染测试");
        
        setBackgroundColor(Color(0.05f, 0.05f, 0.1f, 1.0f));
        
        // 获取系统字体路径
        std::string fontPath = getSystemFontPath();
        E2D_LOG_INFO("Loading font from: {}", fontPath);
        
        // 创建字体图集
        font_ = Application::instance().renderer().createFontAtlas(fontPath, 24, true);
        
        if (!font_) {
            E2D_LOG_ERROR("Failed to load font!");
        } else {
            E2D_LOG_INFO("Font loaded successfully!");
            E2D_LOG_INFO("Font metrics: ascent={}, descent={}, lineGap={}, lineHeight={}",
                         font_->getAscent(), font_->getDescent(), font_->getLineGap(), font_->getLineHeight());

            const char32_t sampleChars[] = {
                U'a', U'b', U'c', U'd', U'e', U'f', U'g', U'h', U'i', U'j', U'k', U'l',
                U'm', U'n', U'o', U'p', U'q', U'r', U's', U't', U'u', U'v', U'w', U'x', U'y', U'z'
            };
            for (char32_t cp : sampleChars) {
                const Glyph* g = font_->getGlyph(cp);
                if (!g) continue;
                E2D_LOG_INFO("Glyph {}: bearingY={}, height={}, bottomAtBaseline={}",
                             static_cast<int>(cp), g->bearingY, g->height, g->bearingY + g->height);
            }
        }
    }

    void onRender(RenderBackend& renderer) override {
        Scene::onRender(renderer);
        
        if (!font_) return;
        
        float y = 50.0f;
        Color white(1.0f, 1.0f, 1.0f, 1.0f);
        Color yellow(1.0f, 1.0f, 0.0f, 1.0f);
        Color cyan(0.0f, 1.0f, 1.0f, 1.0f);
        
        // 测试基本文字
        renderer.drawText(*font_, "PRESS ESC TO EXIT", Vec2(50.0f, y), white);
        y += 40.0f;
        
        // 测试大写文字
        renderer.drawText(*font_, "COLLISION: 0", Vec2(50.0f, y), yellow);
        y += 40.0f;
        
        // 测试混合文字
        renderer.drawText(*font_, "RED COLLISION DETECTED", Vec2(50.0f, y), Color(1.0f, 0.3f, 0.3f, 1.0f));
        y += 40.0f;
        
        // 测试蓝色文字
        renderer.drawText(*font_, "BLUE BOX ROTATES AND COLLIDES", Vec2(50.0f, y), Color(0.3f, 0.6f, 1.0f, 1.0f));
        y += 40.0f;
        
        // 测试标题文字
        renderer.drawText(*font_, "SPATIAL INDEX DEMO", Vec2(50.0f, y), cyan);
        y += 60.0f;
        
        // 测试数字和符号
        renderer.drawText(*font_, "0123456789 - Test 123", Vec2(50.0f, y), white);
        y += 40.0f;
        
        // 测试小写字母
        renderer.drawText(*font_, "abcdefghijklmnopqrstuvwxyz", Vec2(50.0f, y), Color(0.5f, 1.0f, 0.5f, 1.0f));
        y += 40.0f;
        
        // 测试大写字母
        renderer.drawText(*font_, "ABCDEFGHIJKLMNOPQRSTUVWXYZ", Vec2(50.0f, y), Color(1.0f, 0.5f, 1.0f, 1.0f));
    }

private:
    Ptr<FontAtlas> font_;
};

// ============================================================================
// 主函数
// ============================================================================
int main() {
    Logger::init();
    Logger::setLevel(LogLevel::Debug);
    
    E2D_LOG_INFO("========================");
    E2D_LOG_INFO("Easy2D v3.0 Font Test");
    E2D_LOG_INFO("========================");
    
    auto& app = Application::instance();
    
    AppConfig config;
    config.title = "Easy2D v3.0 - Font Test";
    config.width = 800;
    config.height = 600;
    config.vsync = true;
    config.fpsLimit = 60;
    
    if (!app.init(config)) {
        E2D_LOG_ERROR("Failed to initialize application!");
        return -1;
    }
    
    app.enterScene(makePtr<FontTestScene>());
    
    E2D_LOG_INFO("Starting main loop...");
    
    app.run();
    
    E2D_LOG_INFO("Application finished.");
    
    Logger::shutdown();
    
    return 0;
}
