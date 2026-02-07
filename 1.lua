-- ==============================================
-- Easy2D v3.0 构建脚本
-- ==============================================

set_project("Easy2D")
set_version("3.0.0")
set_languages("c++17")
set_encodings("utf-8")
add_rules("mode.debug", "mode.release")

-- 核心路径
local SRC_DIR       = "src"
local INC_DIR       = "include"


-- ==============================================
-- 1. Easy2D 静态库
-- ==============================================
target("easy2d")
    set_kind("static")

    -- 库文件名（Debug 加 d 后缀）
    if is_plat("mingw") then
        set_basename(is_mode("debug") and "easy2dd" or "easy2d")
    else
        set_basename(is_mode("debug") and "libeasy2dd" or "libeasy2d")
    end

    -- 引擎源文件
    add_files(path.join(SRC_DIR, "**.cpp"))

    -- GLEW 源文件
    add_files(path.join(INC_DIR, "glew/src/glew.c"))

    -- GLFW 通用源文件
    add_files(path.join(INC_DIR, "glfw/src/common/*.c"))

    -- 公开头文件目录
    add_includedirs(INC_DIR, {public = true})

    -- 第三方头文件目录
    add_includedirs(path.join(INC_DIR, "glew/include"), {public = true})
    add_includedirs(path.join(INC_DIR, "glfw/include"), {public = true})
    add_includedirs(INC_DIR, {public = true})  -- glm, stb, spdlog, etc.
    add_includedirs(path.join(INC_DIR, "simpleini"), {public = true})

    -- 全平台宏定义
    add_defines("GLEW_STATIC")

    -- ==============================================
    -- Windows 平台
    -- ==============================================
    if is_plat("windows") or is_plat("mingw") then
        -- GLFW Windows 源文件
        add_files(path.join(INC_DIR, "glfw/src/windows/*.c"))
        add_includedirs(path.join(INC_DIR, "glfw/src/common"), {public = false})
        add_includedirs(path.join(INC_DIR, "glfw/src/windows"), {public = false})
        add_defines("_GLFW_WIN32")
        add_defines("WIN32_LEAN_AND_MEAN", "NOMINMAX")

        -- 系统库（已移除 Direct2D/DirectWrite 相关库）
        add_syslinks("user32", "gdi32", "shell32", "winmm", "imm32",
                     "version", "ole32", "opengl32", {public = true})
    end

    if is_plat("linux") then
        -- GLFW Linux 源文件（X11 后端）
        add_files(path.join(INC_DIR, "glfw/src/linux/linux_x11/*.c"))
        add_files(path.join(INC_DIR, "glfw/src/linux/*.c"))
        add_includedirs(path.join(INC_DIR, "glfw/src/common"), {public = false})
        add_includedirs(path.join(INC_DIR, "glfw/src/linux/linux_x11"), {public = false})
        add_includedirs(path.join(INC_DIR, "glfw/src/linux"), {public = false})
        add_defines("_GLFW_X11")
        add_syslinks("GL", "X11", "pthread", "dl", {public = true})
    end

    if is_plat("macosx") then
        -- GLFW macOS 源文件
        add_files(path.join(INC_DIR, "glfw/src/macos/*.c"))
        add_files(path.join(INC_DIR, "glfw/src/macos/*.m"))
        add_includedirs(path.join(INC_DIR, "glfw/src/common"), {public = false})
        add_includedirs(path.join(INC_DIR, "glfw/src/macos"), {public = false})
        add_defines("_GLFW_COCOA")
        add_frameworks("Cocoa", "IOKit", "CoreFoundation", "CoreVideo", "OpenGL", {public = true})
    end

    -- ==============================================
    -- 编译器配置
    -- ==============================================
    if is_plat("windows") then
        if get_config("toolchain") == "msvc" or get_config("toolchain") == "clang-cl" then
            add_cxxflags("/EHsc", "/Zc:__cplusplus", {force = true})
            add_cxxflags("/wd4996", {force = true})
            if is_mode("debug") then
                set_runtimes("MDd")
                add_defines("E2D_DEBUG", "_DEBUG", {public = true})
                add_cxxflags("/Od", "/Zi", {force = true})
            else
                set_runtimes("MD")
                add_defines("NDEBUG", {public = true})
                add_cxxflags("/O2", "/Ob2", {force = true})
            end
        end
    end

    if is_plat("mingw") then
        add_cxxflags("-Wall", "-Wextra", "-Wpedantic", {force = true})
        add_cxxflags("-Wno-unused-parameter", "-Wno-missing-field-initializers", {force = true})
        if is_mode("debug") then
            add_defines("E2D_DEBUG", "_DEBUG", {public = true})
            add_cxxflags("-O0", "-g", "-ggdb", {force = true})
        else
            add_defines("NDEBUG", {public = true})
            add_cxxflags("-O2", {force = true})
        end
    end

    if is_plat("linux") or is_plat("macosx") then
        add_cxxflags("-Wall", "-Wextra", {force = true})
        add_cxxflags("-Wno-unused-parameter", {force = true})
        if is_mode("debug") then
            add_defines("E2D_DEBUG", "_DEBUG", {public = true})
            add_cxxflags("-O0", "-g", {force = true})
        else
            add_defines("NDEBUG", {public = true})
            add_cxxflags("-O2", {force = true})
        end
    end
target_end()


-- ==============================================
-- 1. Hello World 示例
-- ==============================================
target("hello_world")
    set_kind("binary")
    add_files("examples/hello_world/**.cpp")
    add_deps("easy2d")
    set_targetdir("$(builddir)/bin")
target_end()

-- ==============================================
-- 2. 字体测试示例
-- ==============================================
target("font_test")
    set_kind("binary")
    add_files("examples/font_test/**.cpp")
    add_deps("easy2d")
    set_targetdir("$(builddir)/bin")
target_end()


-- ==============================================
-- 3. 推箱子游戏示例
-- ==============================================
target("push_box")
    set_kind("binary")
    add_files("examples/push_box/src/**.cpp")
    add_deps("easy2d")
    set_targetdir("$(builddir)/bin")
    -- 复制资源文件到输出目录
    after_build(function (target)
        os.cp("examples/push_box/src/assets", path.join(target:targetdir(), "/"))
    end)
target_end()
