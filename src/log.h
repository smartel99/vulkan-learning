#ifndef SRC_INTERNAL_LOG_H
#define SRC_INTERNAL_LOG_H

#include <cctype>
#include <ranges>
#include <source_location>
#include <spdlog/sinks/dist_sink.h>
#include <spdlog/spdlog.h>
#include <unordered_map>
#include <vector>

class Log {
public:
    using Logger = std::shared_ptr<spdlog::logger>;

    static void Init();

    static Logger& GetClientLogger() { return GetLogger(s_clientLoggerName); }

    static Logger& GetLogger(const std::string& name)
    {
        if (!s_loggers.contains(name)) {
            spdlog::debug("Added logger: {}", name);
            s_loggers[name] = std::make_shared<spdlog::logger>(name, s_sinks);
            s_loggers[name]->set_level(s_defaultLevel);
        }
        return s_loggers[name];
    }

    static spdlog::level::level_enum GetLoggerLevel(const std::string& name) { return GetLogger(name)->level(); }

    static void SetLoggerLevel(const std::string& name, spdlog::level::level_enum level)
    {
        GetLogger(name)->set_level(level);
    }

    static const auto& GetLoggers() { return s_loggers; }

    static void AddSink(const spdlog::sink_ptr& sink);

    static void RemoveSink(const spdlog::sink_ptr& sink);

    static spdlog::source_loc FormatSourceLocation(const std::source_location& loc);

    static constexpr const char*               s_clientLoggerName = "APP";
    static constexpr spdlog::level::level_enum s_defaultLevel     = spdlog::level::info;

private:
    static inline std::shared_ptr<spdlog::sinks::dist_sink_mt>                     s_sinks   = nullptr;
    static inline std::unordered_map<std::string, std::shared_ptr<spdlog::logger>> s_loggers = {};

    static spdlog::file_event_handlers s_eventHandlers;
};

#pragma region fmt
template<typename T, size_t N>
struct std::formatter<std::span<T, N>> {
    constexpr auto parse(std::format_parse_context& ctx) -> decltype(ctx.begin()) { return ctx.begin(); }

    template<typename FormatContext>
    auto format(std::span<T, N> data, FormatContext& ctx) const -> decltype(ctx.out())
    {
        format_to(ctx.out(), "[");
        for (size_t i = 0; i < data.size(); i++) {
            bool isLast = i == data.size() - 1;
            if (std::isprint(data[i])) { format_to(ctx.out(), "{}{}", static_cast<char>(data[i]), isLast ? "" : ", "); }
            else {
                format_to(ctx.out(), "\\x{:02X}{}", data[i], isLast ? "]" : ", ");
            }
        }
        return ctx.out();
    }
};

template<std::integral T>
struct std::formatter<std::vector<T>> : formatter<std::span<T>> {
    template<typename FormatContext>
    auto format(const std::vector<T>& data, FormatContext& ctx) const -> decltype(ctx.out())
    {
        return formatter<std::span<T>>::format(std::span {data.begin(), data.end()}, ctx);
    }
};

template<>
struct std::formatter<std::basic_string<uint8_t>> : formatter<std::span<uint8_t>> {
    template<typename FormatContext>
    auto format(const std::basic_string<uint8_t>& data, FormatContext& ctx) const -> decltype(ctx.out())
    {
        return formatter<std::span<uint8_t>>::format(std::span {data.begin(), data.end()}, ctx);
    }
};

template<>
struct std::formatter<std::basic_string_view<uint8_t>> : formatter<std::span<uint8_t>> {
    template<typename FormatContext>
    auto format(std::basic_string_view<uint8_t> data, FormatContext& ctx) const -> decltype(ctx.out())
    {
        return formatter<std::span<uint8_t>>::format(std::span {data.begin(), data.end()}, ctx);
    }
};
#pragma endregion

#define BR_LOG(logger, level, msg, ...)                                                                                \
    ::Log::GetLogger(logger)->log(                                                                                     \
      ::Log::FormatSourceLocation(std::source_location::current()), level, msg __VA_OPT__(, ) __VA_ARGS__)

#define BR_LOG_TRACE(logger, ...)    BR_LOG(logger, spdlog::level::trace, __VA_ARGS__)
#define BR_LOG_DEBUG(logger, ...)    BR_LOG(logger, spdlog::level::debug, __VA_ARGS__)
#define BR_LOG_INFO(logger, ...)     BR_LOG(logger, spdlog::level::info, __VA_ARGS__)
#define BR_LOG_WARN(logger, ...)     BR_LOG(logger, spdlog::level::warn, __VA_ARGS__)
#define BR_LOG_ERROR(logger, ...)    BR_LOG(logger, spdlog::level::err, __VA_ARGS__)
#define BR_LOG_CRITICAL(logger, ...) BR_LOG(logger, spdlog::level::critical, __VA_ARGS__)

// Client Log Macros.
#define BR_APP_TRACE(...)    BR_LOG_TRACE(::Log::s_clientLoggerName, __VA_ARGS__)
#define BR_APP_DEBUG(...)    BR_LOG_DEBUG(::Log::s_clientLoggerName, __VA_ARGS__)
#define BR_APP_INFO(...)     BR_LOG_INFO(::Log::s_clientLoggerName, __VA_ARGS__)
#define BR_APP_WARN(...)     BR_LOG_WARN(::Log::s_clientLoggerName, __VA_ARGS__)
#define BR_APP_ERROR(...)    BR_LOG_ERROR(::Log::s_clientLoggerName, __VA_ARGS__)
#define BR_APP_CRITICAL(...) BR_LOG_CRITICAL(::Log::s_clientLoggerName, __VA_ARGS__)
#endif
