#include <easy2d/utils/logger.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/rotating_file_sink.h>

namespace easy2d {

// 静态成员定义
std::shared_ptr<spdlog::logger> Logger::coreLogger_ = nullptr;
bool Logger::initialized_ = false;

void Logger::init() {
    if (initialized_) {
        return;
    }

    // 创建多 sink logger（控制台 + 可选文件）
    std::vector<spdlog::sink_ptr> sinks;

    // 添加彩色控制台输出
    auto consoleSink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    consoleSink->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%^%l%$] %v");
    consoleSink->set_level(spdlog::level::trace);
    sinks.push_back(consoleSink);

    // 创建 logger
    coreLogger_ = std::make_shared<spdlog::logger>("Easy2D", sinks.begin(), sinks.end());
    coreLogger_->set_level(spdlog::level::trace);
    coreLogger_->flush_on(spdlog::level::warn);

    // 注册为默认 logger（可选）
    spdlog::register_logger(coreLogger_);

    initialized_ = true;
    coreLogger_->info("Logger initialized");
}

void Logger::shutdown() {
    if (coreLogger_) {
        coreLogger_->info("Logger shutting down");
        coreLogger_->flush();
    }

    coreLogger_.reset();
    spdlog::drop_all();
    initialized_ = false;
}

void Logger::setConsoleOutput(bool enable) {
    if (!coreLogger_) return;

    // 获取当前 sinks
    auto& sinks = coreLogger_->sinks();
    
    // 查找控制台 sink
    auto it = std::find_if(sinks.begin(), sinks.end(), [](const spdlog::sink_ptr& sink) {
        return dynamic_cast<spdlog::sinks::stdout_color_sink_mt*>(sink.get()) != nullptr;
    });

    if (enable) {
        // 如果不存在控制台 sink，添加一个
        if (it == sinks.end()) {
            auto consoleSink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
            consoleSink->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%^%l%$] %v");
            consoleSink->set_level(spdlog::level::trace);
            coreLogger_->sinks().push_back(consoleSink);
        }
    } else {
        // 移除控制台 sink
        if (it != sinks.end()) {
            sinks.erase(it);
        }
    }
}

void Logger::setFileOutput(const std::string& filename) {
    if (!coreLogger_) {
        init();
    }

    // 查找并移除已有的文件 sink
    auto& sinks = coreLogger_->sinks();
    auto it = std::find_if(sinks.begin(), sinks.end(), [](const spdlog::sink_ptr& sink) {
        return dynamic_cast<spdlog::sinks::basic_file_sink_mt*>(sink.get()) != nullptr ||
               dynamic_cast<spdlog::sinks::rotating_file_sink_mt*>(sink.get()) != nullptr;
    });

    if (it != sinks.end()) {
        sinks.erase(it);
    }

    // 如果 filename 为空，则不添加文件 sink（移除现有文件输出）
    if (filename.empty()) {
        return;
    }

    try {
        // 创建文件 sink（使用 rotating 文件，最多 3 个文件，每个最大 5MB）
        auto fileSink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>(
            filename, 1024 * 1024 * 5, 3, false);
        fileSink->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%l] %v");
        fileSink->set_level(spdlog::level::trace);
        coreLogger_->sinks().push_back(fileSink);
        
        coreLogger_->info("File logging enabled: {}", filename);
    } catch (const spdlog::spdlog_ex& ex) {
        // 文件创建失败，记录到控制台
        coreLogger_->error("Failed to create log file: {}", ex.what());
    }
}

} // namespace easy2d
