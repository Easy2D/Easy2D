#include "easy2d/core/string.h"
#include <cmath>
#include <easy2d/easy2d.h>
#include <easy2d/utils/logger.h>
#include <sstream>

using namespace easy2d;

// ============================================================================
// 碰撞测试节点 - 有实际边界框
// ============================================================================
class CollisionBox : public Node {
public:
  CollisionBox(float width, float height, const Color &color)
      : width_(width), height_(height), color_(color), isColliding_(false) {
    setSpatialIndexed(true);
  }

  void setColliding(bool colliding) { isColliding_ = colliding; }

  Rect getBoundingBox() const override {
    // 返回实际的矩形边界
    Vec2 pos = getPosition();
    return Rect(pos.x - width_ / 2, pos.y - height_ / 2, width_, height_);
  }

  void onDraw(RenderBackend &renderer) override {
    Vec2 pos = getPosition();

    // 绘制填充矩形
    Color fillColor = isColliding_ ? Color(1.0f, 0.2f, 0.2f, 0.8f) : color_;
    renderer.fillRect(
        Rect(pos.x - width_ / 2, pos.y - height_ / 2, width_, height_),
        fillColor);

    // 绘制边框
    Color borderColor = isColliding_ ? Color(1.0f, 0.0f, 0.0f, 1.0f)
                                     : Color(1.0f, 1.0f, 1.0f, 0.5f);
    float borderWidth = isColliding_ ? 3.0f : 2.0f;
    renderer.drawRect(
        Rect(pos.x - width_ / 2, pos.y - height_ / 2, width_, height_),
        borderColor, borderWidth);
  }

private:
  float width_, height_;
  Color color_;
  bool isColliding_;
};

// ============================================================================
// Hello World 场景 - 空间索引演示
// ============================================================================
class HelloWorldScene : public Scene {
public:
  HelloWorldScene() = default;
  ~HelloWorldScene() override = default;

  void onEnter() override {
    E2D_LOG_INFO("HelloWorldScene::onEnter - 空间索引演示");

    auto &app = Application::instance();
    float centerX = app.getConfig().width / 2.0f;
    float centerY = app.getConfig().height / 2.0f;

    // 设置背景色
    setBackgroundColor(Color(0.05f, 0.05f, 0.1f, 1.0f));

    // 创建碰撞框
    createCollisionBoxes(centerX, centerY);

    // 创建旋转的中心方块
    centerBox_ =
        makePtr<CollisionBox>(100.0f, 100.0f, Color(0.2f, 0.6f, 1.0f, 0.8f));
    centerBox_->setPosition(Vec2(centerX, centerY));
    addChild(centerBox_);

    // 加载字体
    loadFonts();

    E2D_LOG_INFO("创建了 {} 个碰撞框", boxes_.size() + 1);
  }

  void onUpdate(float dt) override {
    Scene::onUpdate(dt);

    // 旋转中心方块
    rotationAngle_ += 60.0f * dt;
    if (rotationAngle_ >= 360.0f)
      rotationAngle_ -= 360.0f;

    // 让中心方块沿圆形路径移动
    float radius = 150.0f;
    float rad = rotationAngle_ * 3.14159f / 180.0f;
    Vec2 center = Vec2(400.0f, 300.0f); // 屏幕中心
    centerBox_->setPosition(Vec2(center.x + std::cos(rad) * radius,
                                 center.y + std::sin(rad) * radius));
    centerBox_->setRotation(rotationAngle_);

    performCollisionDetection();
  }

  void onRender(RenderBackend &renderer) override {
    Scene::onRender(renderer);

    // 绘制说明文字（使用真字体）
    drawInstructions(renderer);

    // 绘制碰撞信息
    drawCollisionInfo(renderer);
  }

private:
  /**
   * @brief 加载字体资源
   */
  void loadFonts() {
    auto &resources = Application::instance().resources();

    // 尝试加载系统字体，如果不存在则使用默认路径
    // Windows 系统字体路径
    std::vector<std::string> fontPaths = {
        "C:/Windows/Fonts/simsun.ttc",
        "C:/Windows/Fonts/seguiemj.ttf",
    };
        

    bool fontLoaded = false;
    for (const auto &path : fontPaths) {
      titleFont_ = resources.loadFont(path, 32); // 大字体用于标题
      if (titleFont_) {
        infoFont_ = resources.loadFont(path, 18); // 小字体用于信息
        fontLoaded = true;
        E2D_LOG_INFO("成功加载字体: {}", path);
        break;
      }
    }

    if (!fontLoaded) {
      E2D_LOG_WARN("无法加载字体，将使用备用渲染方式");
    }
  }

  void createCollisionBoxes(float centerX, float centerY) {
    // 创建静态碰撞框
    std::vector<std::pair<Vec2, Color>> positions = {
        {Vec2(centerX - 200, centerY - 150), Color(0.3f, 1.0f, 0.3f, 0.7f)},
        {Vec2(centerX + 200, centerY - 150), Color(1.0f, 0.3f, 0.3f, 0.7f)},
        {Vec2(centerX - 200, centerY + 150), Color(0.3f, 0.3f, 1.0f, 0.7f)},
        {Vec2(centerX + 200, centerY + 150), Color(1.0f, 1.0f, 0.3f, 0.7f)},
        {Vec2(centerX, centerY - 200), Color(1.0f, 0.3f, 1.0f, 0.7f)},
        {Vec2(centerX, centerY + 200), Color(0.3f, 1.0f, 1.0f, 0.7f)},
    };

    for (const auto &[pos, color] : positions) {
      auto box = makePtr<CollisionBox>(80.0f, 80.0f, color);
      box->setPosition(pos);
      addChild(box);
      boxes_.push_back(box);
    }
  }

  void performCollisionDetection() {
    // 清除之前的碰撞状态
    centerBox_->setColliding(false);
    for (auto &box : boxes_) {
      box->setColliding(false);
    }

    // 使用空间索引进行碰撞检测
    auto collisions = queryCollisions();

    collisionCount_ = collisions.size();

    // 标记碰撞的节点
    for (const auto &[nodeA, nodeB] : collisions) {
      if (auto boxA = dynamic_cast<CollisionBox *>(nodeA)) {
        boxA->setColliding(true);
      }
      if (auto boxB = dynamic_cast<CollisionBox *>(nodeB)) {
        boxB->setColliding(true);
      }
    }
  }

  void drawInstructions(RenderBackend &renderer) {

    // 使用真字体绘制标题
    renderer.drawText(*titleFont_, String("你好"), Vec2(200.0f, 30.0f),
                      Color(1.0f, 1.0f, 1.0f, 1.0f));

    // 绘制说明文字
    renderer.drawText(*infoFont_, "Blue box rotates and collides",
                      Vec2(250.0f, 75.0f), Color(0.8f, 0.8f, 0.8f, 1.0f));
    renderer.drawText(*infoFont_, "Red = Collision Detected",
                      Vec2(280.0f, 100.0f), Color(1.0f, 0.5f, 0.5f, 1.0f));
  }

  void drawCollisionInfo(RenderBackend &renderer) {

    // 使用真字体绘制碰撞统计
    std::stringstream ss;
    ss << "Collisions: " << collisionCount_;
    renderer.drawText(*infoFont_, ss.str(), Vec2(20.0f, 540.0f),
                      Color(1.0f, 1.0f, 1.0f, 1.0f));

    // 绘制 FPS 信息
    auto &app = Application::instance();
    ss.str("");
    ss << "FPS: " << app.fps();
    renderer.drawText(*infoFont_, ss.str(), Vec2(20.0f, 565.0f),
                      Color(0.8f, 1.0f, 0.8f, 1.0f));

    renderer.drawText(*infoFont_, "请按ESC退出", Vec2(280.0f, 560.0f),
                      Color(0.8f, 0.8f, 0.8f, 1.0f));
  }

  Ptr<CollisionBox> centerBox_;
  std::vector<Ptr<CollisionBox>> boxes_;
  float rotationAngle_ = 0.0f;
  size_t collisionCount_ = 0;

  // 字体资源
  Ptr<FontAtlas> titleFont_;
  Ptr<FontAtlas> infoFont_;
};

// ============================================================================
// 主函数
// ============================================================================
int main() {
  Logger::init();
  Logger::setLevel(LogLevel::Info);

  E2D_LOG_INFO("========================");
  E2D_LOG_INFO("Easy2D v3.0 Spatial Index Demo");
  E2D_LOG_INFO("========================");

  auto &app = Application::instance();

  AppConfig config;
  config.title = "Easy2D v3.0 - Spatial Index Demo";
  config.width = 800;
  config.height = 600;
  config.vsync = true;
  config.fpsLimit = 0;

  if (!app.init(config)) {
    E2D_LOG_ERROR("Failed to initialize application!");
    return -1;
  }

  app.enterScene(makePtr<HelloWorldScene>());

  E2D_LOG_INFO("Starting main loop...");

  app.run();

  E2D_LOG_INFO("Application finished.");

  Logger::shutdown();

  return 0;
}
