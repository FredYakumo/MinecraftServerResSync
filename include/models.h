#ifndef MANAGE_RES_H
#define MANAGE_RES_H

#include <algorithm>
#include <boost/beast/http/error.hpp>
#include <boost/beast/http/message.hpp>
#include <boost/beast/http/string_body.hpp>
#include <boost/beast/http/verb.hpp>
#include <cstdint>
#include <memory>
#include <mutex>
#include <optional>
#include <shared_mutex>
#include <string>
#include <string_view>
#include <unordered_map>
#include <utility>
#include <vector>

namespace models {
    template <typename T> class MutData {
        T &data;
        std::unique_lock<std::shared_mutex> lock;

    public:
        MutData(T &data, std::shared_mutex &mtx) : data(data), lock(mtx) {}

        // ~MutData() { lock.unlock(); }

        explicit operator T &() { return data; }

        T &get() { return data; }
    };

    template <typename T> class ShareMutexData {
        T data;
        mutable std::shared_mutex mtx;

    public:
        explicit ShareMutexData(T data) noexcept : data(std::move(data)) {}

        template <typename U> explicit ShareMutexData(U &&data) noexcept : data(std::forward<U>(data)) {}

        void set(T d) {
            std::unique_lock lock(mtx);
            data = std::move(d);
        }

        const T &get_const() const {
            std::shared_lock lock(mtx);
            return data;
        }

        MutData<T> get_mut() { return MutData(data, mtx); }

        T &unsafeGet() noexcept { return data; }
    };

    struct ClassPathList {
        std::vector<std::string> path_list;
    };

    class ManageClassPathMap {
    public:
        std::unordered_map<std::string, ClassPathList> managed_class_map;
    };

    class FileHashMap {
        std::unordered_map<std::string, const std::string> m_file_hash_map;

    public:
        FileHashMap() = default;

        explicit FileHashMap(std::unordered_map<std::string, const std::string> hash_map) : m_file_hash_map(std::move(hash_map)) {}

        // FileHashMap(const std::unordered_map<std::string, const std::string>
        // &hash_map): m_file_hash_map(hash_map) {} FileHashMap(const
        // std::unordered_map<std::string, const std::string> &&hash_map):
        // m_file_hash_map(hash_map) {}

        [[nodiscard]] inline const std::unordered_map<std::string, const std::string> &file_hash_map() const { return m_file_hash_map; }

        FileHashMap &insert_file_hash(std::string_view file_name, std::string_view hash) {
            m_file_hash_map.emplace(file_name, hash);
            return *this;
        }
    };

    class ClassFileResource {
        std::unordered_map<std::string, FileHashMap> m_class_file_hash_map;

    public:
        ClassFileResource() = default;

        explicit ClassFileResource(std::unordered_map<std::string, FileHashMap> class_file_hash_map) : m_class_file_hash_map(std::move(class_file_hash_map)) {}

        // ClassFileResource(const std::unordered_map<std::string, FileHashMap>
        // class_file_hash_map): m_class_file_hash_map(class_file_hash_map) {}
        [[nodiscard]] const std::unordered_map<std::string, FileHashMap> &class_file_hash_map() const { return m_class_file_hash_map; }

        ClassFileResource &insert_file_hash_map(std::string_view class_name, const FileHashMap &file_hash_map) {
            m_class_file_hash_map.insert({std::string{class_name}, file_hash_map});
            return *this;
        }
    };

    class ServerData {
        std::shared_ptr<ShareMutexData<ClassFileResource>> m_class_file_resources{nullptr};
        uint64_t m_listen_port{};
        uint16_t m_thread_count{4};
        std::string m_host;

    public:
        ServerData() = default;

        explicit ServerData(std::shared_ptr<ShareMutexData<ClassFileResource>> class_file_resource, const std::string_view host, const uint64_t listen_port, const uint16_t thread_count = 4)
            : m_class_file_resources(std::move(class_file_resource)), m_listen_port(listen_port), m_thread_count(thread_count), m_host(host) {}

        explicit ServerData(ClassFileResource class_file_resource, const std::string_view host, const uint64_t listen_port, const uint16_t thread_count = 4)
            : m_class_file_resources(std::make_shared<ShareMutexData<ClassFileResource>>(std::move(class_file_resource))), m_listen_port(listen_port), m_thread_count(thread_count), m_host(host) {}

        ServerData &set_class_file_resources(const ClassFileResource &class_file_resource) {
            m_class_file_resources = std::make_shared<ShareMutexData<ClassFileResource>>(class_file_resource);
            return *this;
        }
        [[nodiscard]] const ClassFileResource *class_file_resource() const { return !m_class_file_resources ? nullptr : &m_class_file_resources->get_const(); }
        [[nodiscard]] uint64_t listen_port() const { return m_listen_port; }
        [[nodiscard]] uint16_t thread_count() const { return m_thread_count; }
        [[nodiscard]] std::string_view host() const { return m_host; }
    };

    struct Api {
        boost::beast::http::verb method;
        std::function<void(const boost::beast::http::request<boost::beast::http::string_body> &, boost::beast::http::response<boost::beast::http::string_body> &,  const std::unordered_map<std::string_view, std::string_view> &query_param_map)> api_function;
    };

} // namespace models

#endif
