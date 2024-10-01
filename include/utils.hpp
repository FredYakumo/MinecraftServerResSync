#ifndef UTILS_H
#define UTILS_H

#include <cstring>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <openssl/sha.h>
#include <sstream>
#include <sys/types.h>

namespace utils {
    inline std::string read_to_end(std::ifstream &fs) {
        std::stringstream ss{};
        while (ss << fs.rdbuf()) {
        }
        return ss.str();
    }

    inline std::string calculate_str_sha1(const char *origin) {
        unsigned char hash[SHA_DIGEST_LENGTH];

        SHA1(reinterpret_cast<const unsigned char *>(origin), strlen(origin), hash);
        std::stringstream ss;
        for (const unsigned char i : hash) {
            ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<uint32_t>(i);
        }
        return ss.str();
    }

    class SplitString {
    public:
        SplitString(std::string_view str, char delimiter) : m_str(str), m_delimiter(delimiter), m_start(0) {}

        class Iterator {
        public:
            Iterator(std::string_view str, char delimiter, size_t start) : m_str(str), m_delimiter(delimiter), m_start(start) { find_next(); }

            std::string_view operator*() const {
                return m_str.substr(m_start, (m_end == std::string_view::npos) ? std::string_view::npos : m_end - m_start);
            }

            Iterator &operator++() {
                m_start = m_end;
                if (m_start != std::string_view::npos) {
                    ++m_start;
                    find_next();
                }
                return *this;
            }

            bool operator!=(const Iterator &other) const {
                return m_start != other.m_start;
            }

        private:
            void find_next() {
                m_end = m_str.find(m_delimiter, m_start);
            }


            std::string_view m_str;
            char m_delimiter;
            size_t m_start;
            size_t m_end{};
        };

        [[nodiscard]] Iterator begin() const { return {m_str, m_delimiter, m_start}; }
        [[nodiscard]] Iterator end() const { return {m_str, m_delimiter, std::string_view::npos}; }

    private:
        std::string_view m_str;
        char m_delimiter;
        size_t m_start;
    };
} // namespace utils

#endif