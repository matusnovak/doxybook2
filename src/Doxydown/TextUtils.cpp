#include <locale>
#include <chrono>
#include <regex>
#include <sstream>
#include "TextUtils.hpp"

static const std::regex ANCHOR_REGEX("_[a-z0-9]{34,67}$");

#ifdef __MINGW32__
    static const auto US_LOCALE = std::locale("en_US");
#else
    static const auto US_LOCALE = std::locale("en_US.utf8");
#endif

static std::string replaceAll(std::string str, const std::string& from, const std::string& to) {
    size_t pos = 0;
    while ((pos = str.find(from, pos)) != std::string::npos) {
        str.replace(pos, from.length(), to);
        pos += to.length();
    }
    return str;
}

std::string Doxydown::TextUtils::title(std::string str) {
    if (!str.empty()) str[0] = std::toupper(str[0], US_LOCALE);
    return str;
}

extern std::string Doxydown::TextUtils::toLower(std::string str) {
    for (auto& c : str) {
        c = std::tolower(c, US_LOCALE);
    }
    return str;
}

std::string Doxydown::TextUtils::safeAnchorId(std::string str) {
    return replaceAll(toLower(std::move(str)), "::", "");
}

std::string Doxydown::TextUtils::date(const std::string& format) {
    const auto t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    char mbstr[100];
    std::strftime(mbstr, sizeof(mbstr), format.c_str(), std::localtime(&t));
    return mbstr;
}

std::string Doxydown::TextUtils::stripNamespace(const std::string& str) {
    const auto pos = str.find_last_of("::");
    if (pos != std::string::npos) return str.substr(pos + 1);
    return str;
}

std::string Doxydown::TextUtils::stripAnchor(const std::string& str) {
    std::stringstream ss;
    std::regex_replace(std::ostreambuf_iterator<char>(ss), str.begin(), str.end(), ANCHOR_REGEX, "");
    return ss.str();
}
