#pragma once

#include <easy2d/core/types.h>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <memory>
#include <string>

namespace easy2d {

// ============================================================================
// 日志级别枚举
// ============================================================================
enum class LogLevel {
    Trace = spdlog::level::trace,
    Debug = spdlog::level::debug,
    Info = spdlog::level::info,
    Warn = spdlog::level::warn,
    Error = spdlog::level::err,
    Fatal = spdlog::level::critical,
    Off = spdlog::level::off
};

// ============================================================================
// Logger 类 - spdlog 的薄封装
// ============================================================================
class Logger {
public:
    /// 初始化日志系统
    static void init();
    
    /// 关闭日志系统
    static void shutdown();
    
    /// 获取核心 logger
    static std::shared_ptr<spdlog::logger>& getCoreLogger();
    
    /// 设置日志级别
    static void setLevel(LogLevel level);
    
    /// 设置是否输出到控制台
    static void setConsoleOutput(bool enable);
    
    /// 设置是否输出到文件
    static void setFileOutput(const std::string& filename);

private:
    static std::shared_ptr<spdlog::logger> coreLogger_;
    static bool initialized_;
};

// ============================================================================
// 日志宏
// ============================================================================

#ifdef E2D_DEBUG
    #define E2D_LOG_TRACE(...) ::easy2d::Logger::getCoreLogger()->trace(__VA_ARGS__)
    #define E2D_LOG_DEBUG(...) ::easy2d::Logger::getCoreLogger()->debug(__VA_ARGS__)
#else
    #define E2D_LOG_TRACE(...) 
    #define E2D_LOG_DEBUG(...) 
#endif

#define E2D_LOG_INFO(...)  ::easy2d::Logger::getCoreLogger()->info(__VA_ARGS__)
#define E2D_LOG_WARN(...)  ::easy2d::Logger::getCoreLogger()->warn(__VA_ARGS__)
#define E2D_LOG_ERROR(...) ::easy2d::Logger::getCoreLogger()->error(__VA_ARGS__)
#define E2D_LOG_FATAL(...) ::easy2d::Logger::getCoreLogger()->critical(__VA_ARGS__)

// ============================================================================
// 内联实现
// ============================================================================

inline std::shared_ptr<spdlog::logger>& Logger::getCoreLogger() {
    if (!coreLogger_) {
        init();
    }
    return coreLogger_;
}

inline void Logger::setLevel(LogLevel level) {
    if (coreLogger_) {
        coreLogger_->set_level(static_cast<spdlog::level::level_enum>(level));
    }
}

} // namespace easy2d
