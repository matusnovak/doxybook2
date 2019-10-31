#ifdef _WIN32
#define NOMINMAX
#include <Windows.h>
#else
#include <sys/stat.h>
#endif

#include <locale>
#include <chrono>
#include <regex>
#include <sstream>
#include <Doxydown/Utils.hpp>
#include "ExceptionUtils.hpp"

static const std::regex ANCHOR_REGEX("_[a-z0-9]{34,67}$");

static std::string replaceAll(std::string str, const std::string& from, const std::string& to) {
    size_t pos = 0;
    while ((pos = str.find(from, pos)) != std::string::npos) {
        str.replace(pos, from.length(), to);
        pos += to.length();
    }
    return str;
}

std::string Doxydown::Utils::title(std::string str) {
    if (!str.empty()) str[0] = ::toupper(str[0]);
    return str;
}

extern std::string Doxydown::Utils::toLower(std::string str) {
    for (auto& c : str) {
        c = ::tolower(c);
    }
    return str;
}

std::string Doxydown::Utils::safeAnchorId(std::string str) {
    return replaceAll(toLower(std::move(str)), "::", "");
}

std::string Doxydown::Utils::date(const std::string& format) {
    const auto t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    char mbstr[100];
    std::strftime(mbstr, sizeof(mbstr), format.c_str(), std::localtime(&t));
    return mbstr;
}

std::string Doxydown::Utils::stripNamespace(const std::string& str) {
    const auto pos = str.find_last_of("::");
    if (pos != std::string::npos) return str.substr(pos + 1);
    return str;
}

std::string Doxydown::Utils::stripAnchor(const std::string& str) {
    std::stringstream ss;
    std::regex_replace(std::ostreambuf_iterator<char>(ss), str.begin(), str.end(), ANCHOR_REGEX, "");
    return ss.str();
}

void Doxydown::Utils::createDirectory(const std::string& path) {
#ifdef _WIN32
    if (!CreateDirectoryA(path.c_str(), NULL) && ERROR_ALREADY_EXISTS != GetLastError()) {
        throw EXCEPTION("Failed to create directory {} error {}", path, int(GetLastError()));
    }
#else
    const auto err = mkdir(path.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    if (err != 0 && err != EEXIST) {
        throw EXCEPTION("Failed to create directory {} error {}", path, err);
    }
#endif
}
