#include <Doxybook/Log.hpp>
#include <Doxybook/Utils.hpp>
#include <chrono>
#include <locale>
#include <regex>
#include <sstream>

std::string Doxybook2::replaceAll(std::string str, const std::string& from, const std::string& to) {
    size_t pos = 0;
    while ((pos = str.find(from, pos)) != std::string::npos) {
        str.replace(pos, from.length(), to);
        pos += to.length();
    }
    return str;
}

void Doxybook2::backtrace(const std::exception& e, const size_t level) {
    Log::e("{} {}", std::string(level * 2, ' '), e.what());
    try {
        std::rethrow_if_nested(e);
    } catch (const std::exception& ex) {
        backtrace(ex, level + 1);
    } catch (...) {
    }
}

extern std::string Doxybook2::toLower(std::string str) {
    for (auto& c : str) {
        c = ::tolower(c);
    }
    return str;
}

std::string Doxybook2::safeAnchorId(std::string str) {
    return replaceAll(replaceAll(toLower(std::move(str)), "::", ""), " ", "-");
}

std::string Doxybook2::date(const std::string& format) {
    const auto t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    char mbstr[100];
    std::strftime(mbstr, sizeof(mbstr), format.c_str(), std::localtime(&t));
    return mbstr;
}

std::string Doxybook2::stripNamespace(const std::string& str) {
    auto inside = 0;
    size_t count = 0;
    size_t offset = std::string::npos;
    for (const auto& c : str) {
        switch (c) {
        case '(':
        case '[':
        case '<': {
            inside++;
            break;
        }
        case ')':
        case ']':
        case '>': {
            inside--;
            break;
        }
        case ':': {
            if (inside == 0) {
                offset = count + 1;
            }
        }
        default: {
            break;
        }
        }
        count++;
    }

    if (offset != std::string::npos) {
        return str.substr(offset);
    } else {
        return str;
    }
}

std::string Doxybook2::stripAnchor(const std::string& str) {
    static const std::regex ANCHOR_REGEX("_[a-z0-9]{34,67}$");

    std::stringstream ss;
    std::regex_replace(std::ostreambuf_iterator<char>(ss), str.begin(), str.end(), ANCHOR_REGEX, "");
    return ss.str();
}
