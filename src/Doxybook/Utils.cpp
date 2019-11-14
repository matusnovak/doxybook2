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
#include <dirent.h>
#include <Doxybook/Utils.hpp>
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

std::string Doxybook2::Utils::title(std::string str) {
    if (!str.empty()) str[0] = ::toupper(str[0]);
    return str;
}

extern std::string Doxybook2::Utils::toLower(std::string str) {
    for (auto& c : str) {
        c = ::tolower(c);
    }
    return str;
}

std::string Doxybook2::Utils::safeAnchorId(std::string str) {
    return replaceAll(toLower(std::move(str)), "::", "");
}

std::string Doxybook2::Utils::date(const std::string& format) {
    const auto t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    char mbstr[100];
    std::strftime(mbstr, sizeof(mbstr), format.c_str(), std::localtime(&t));
    return mbstr;
}

std::string Doxybook2::Utils::stripNamespace(const std::string& str) {
    const auto pos = str.find_last_of("::");
    if (pos != std::string::npos) return str.substr(pos + 1);
    return str;
}

std::string Doxybook2::Utils::stripAnchor(const std::string& str) {
    std::stringstream ss;
    std::regex_replace(std::ostreambuf_iterator<char>(ss), str.begin(), str.end(), ANCHOR_REGEX, "");
    return ss.str();
}

std::string Doxybook2::Utils::escape(std::string str) {
    auto contains = false;
    for (const auto& c : str) {
        switch(c) {
            case '>':
            case '<':
            case '_':
            case '*': {
                contains = true;
                break;
            }
            default: {
                break;
            }
        }
    }

    if (!contains) return str;
    
    std::string ret;
    ret.resize(str.size() * 2);
    auto* dst = &ret[0];
    for (const auto& c : str) {
        switch(c) {
            case '_':
            case '*': {
                *dst++ = '\\';
            }
            default: {
                *dst++ = c;
            }
        }
    }

    return replaceAll(replaceAll(ret, "<", "&lt;"), ">", "&gt;");
}

std::vector<std::string> Doxybook2::Utils::split(const std::string& str, const std::string& delim) {
    std::vector<std::string> tokens;
    size_t last = 0;
    auto pos = str.find(delim);
    while (pos != std::string::npos) {
        tokens.push_back(str.substr(last, pos));
        pos += delim.size();
        last = pos;
        pos = str.find(delim, pos);
    }
    if (last < str.size()) {
        tokens.push_back(str.substr(last));
    }
    return tokens;
}

void Doxybook2::Utils::createDirectory(const std::string& path) {
    auto* dir = opendir(path.c_str());
    if (dir) {
        closedir(dir);
        return;
    }

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
