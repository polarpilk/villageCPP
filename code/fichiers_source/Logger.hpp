#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <iostream>
#include <string>
#include <type_traits>

enum class LogCategory {
    TOUT,
    VILLAGEOIS,
    PRODUCTION,
    RESSOURCES,
    ECONOMIE,
    SYSTEME,
    DEBUG
};

inline LogCategory operator|(LogCategory a, LogCategory b)
{
    using T = std::underlying_type_t<LogCategory>;
    return static_cast<LogCategory>(
        static_cast<T>(a) | static_cast<T>(b)
        );
}

inline LogCategory operator&(LogCategory a, LogCategory b)
{
    using T = std::underlying_type_t<LogCategory>;
    return static_cast<LogCategory>(
        static_cast<T>(a) & static_cast<T>(b)
        );
}

class Logger {
private:
    inline static LogCategory currentFilter = LogCategory::TOUT;

public:
    static void setFilter(LogCategory filter) {
        currentFilter = filter;
    }

    static LogCategory getFilter() {
        return currentFilter;
    }

    static void log(LogCategory category, const std::string& message) {
        if (currentFilter == LogCategory::TOUT || currentFilter == category) {
            std::cout << getCategoryPrefix(category) << message << std::endl;
        }
    }

private:
    static std::string getCategoryPrefix(LogCategory category) {
        switch (category) {
        case LogCategory::VILLAGEOIS:  return "[VILLAGEOIS] ";
        case LogCategory::PRODUCTION:  return "[PRODUCTION] ";
        case LogCategory::RESSOURCES:  return "[RESSOURCES] ";
        case LogCategory::ECONOMIE:    return "[ECONOMIE] ";
        case LogCategory::SYSTEME:     return "[SYSTEME] ";
        case LogCategory::DEBUG:       return "[DEBUG] ";
        case LogCategory::TOUT:         return "";
        default:                       return "";
        }
    }
};

#endif
