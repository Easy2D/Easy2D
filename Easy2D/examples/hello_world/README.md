# Easy2D v3.0 Hello World 示例

这是一个验证 Easy2D v3.0 引擎基本功能的示例程序。

## 运行截图

程序显示一个包含以下元素的动画场景：
- **中心旋转方块** - 蓝色主方块，带对角线（90°/秒旋转）
- **环绕卫星** - 4个彩色小方块围绕中心旋转（红、绿、蓝、黄）
- **脉冲圆圈** - 绿色呼吸效果的光环
- **背景网格** - 半透明的装饰性格子
- **HELLO 字样** - 用几何线条拼出的文字

## 功能展示

### ✅ 基础架构
- **Application 单例模式** - Meyer's 单例初始化应用
- **场景系统** - 自定义场景类 `HelloWorldScene`
- **渲染后端** - OpenGL 3.3 渲染器

### ✅ 渲染功能
- **形状渲染** - 矩形、线条、圆圈
- **颜色系统** - 使用 `Color` 类设置各种颜色
- **透明度** - 支持半透明渲染
- **网格渲染** - 背景装饰网格

### ✅ 场景图系统
- **节点层级** - Node 节点的父子关系
- **变换系统** - 位置、旋转（旋转方块动画）
- **自定义渲染** - 在 `onRender` 中直接调用渲染器

### ✅ 动画系统
- **旋转动画** - 中心方块和环绕卫星的旋转
- **脉冲效果** - 正弦波控制的呼吸效果
- **时间控制** - 使用 `dt` 实现平滑动画

### ✅ 主循环
- **更新阶段** - `onUpdate` 每帧调用
- **渲染阶段** - `onRender` 绘制所有内容
- **双缓冲** - GLFW 窗口交换缓冲

## 运行示例

### Windows
```bash
# 使用 xmake 构建
xmake build hello_world

# 运行
xmake run hello_world
# 或直接运行
.\build\mingw\x86_64\bin\hello_world.exe
```

## 控制说明

- **ESC** - 退出程序
- 点击窗口右上角 X 按钮退出

## 代码结构

```cpp
// 1. 创建场景类
class HelloWorldScene : public Scene {
    void onEnter() override;      // 场景进入时初始化
    void onUpdate(float dt) override;  // 每帧更新
    void onRender(RenderBackend&) override;   // 自定义渲染
};

// 2. 主函数
int main() {
    auto& app = Application::instance();
    app.init(config);
    app.enterScene(makePtr<HelloWorldScene>());
    app.run();
    return 0;
}
```

## 渲染内容详解

### 中心旋转方块
```cpp
void drawRotatedBox(RenderBackend& renderer, const Vec2& center, 
                    float size, float angle, const Color& color) {
    // 计算旋转后的四个顶点
    // 绘制填充矩形 + 边框 + 对角线
}
```

### 环绕卫星
```cpp
float orbitRadius = 150.0f;
float angle = orbitAngle_ + (i * 90.0f);  // 90度间隔
Vec2 pos(centerX + cos(rad) * orbitRadius,
         centerY + sin(rad) * orbitRadius);
```

### 脉冲效果
```cpp
pulseScale_ = 1.0f + 0.2f * sin(pulseTime_ * 3.0f);
// 使用正弦波控制圆圈大小
```

### HELLO 文字
```cpp
// 用 drawLine 绘制字母形状
// H: 两条竖线 + 一条横线
// E: 一条竖线 + 三条横线
// L: 一条竖线 + 底部横线
// O: 一个圆圈
```

## 验证的功能清单

- [x] 窗口创建（GLFW）
- [x] OpenGL 3.3 上下文初始化
- [x] 渲染后端创建
- [x] 场景管理器
- [x] 场景生命周期（onEnter/onExit/onUpdate/onRender）
- [x] 节点系统（Node）
- [x] 场景图层级
- [x] 变换系统
- [x] 形状渲染（fillRect, drawRect, drawLine, drawCircle, fillCircle）
- [x] 颜色系统
- [x] 透明度混合
- [x] 动画更新（基于时间的旋转）
- [x] 事件队列
- [x] 日志系统
- [x] 主循环（update/render）

## 性能信息

- **目标帧率**: 60 FPS（启用 VSync）
- **渲染对象**: 
  - 1 个旋转方块
  - 4 个环绕卫星
  - 3 层脉冲圆圈
  - 背景网格（17x13 条线）
  - HELLO 文字（多条线段）

## 下一步

基于这个示例，你可以：
1. 添加精灵（Sprite）显示图片
2. 加载字体文件，使用 Text 节点显示文字
3. 添加音频播放
4. 实现鼠标交互
5. 添加场景切换

## 项目信息

- **引擎版本**: Easy2D v3.0
- **渲染后端**: OpenGL 3.3 Core Profile
- **构建系统**: xmake
- **支持平台**: Windows, Linux, macOS

---

**注意**: 这个示例不依赖字体文件，完全使用几何图形渲染，适合验证引擎核心功能。
