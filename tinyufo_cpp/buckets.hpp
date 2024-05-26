#pragma once

#include <optional>
#include <string>
#include <unordered_map>

// the data and its metadata
// pub struct Bucket<T> {
//     uses: Uses,
//     queue: Location,
//     weight: Weight,
//     data: T,
// }

template <typename T>
class Buckets {
public:
    void insert(const std::string &key, T value) {
        data[key] = value;
    }

    auto get(const std::string &key) const -> std::optional<T> {
        auto it = data.find(key);
        if (it != data.end()) {
            return it->second;
        } else {
            return std::nullopt;
        }
    }

    auto remove(const std::string &key) -> std::optional<T> {
        auto it = data.find(key);
        if (it != data.end()) {
            T value = it->second;
            data.erase(it);
            return value;
        } else {
            return std::nullopt;
        }
    }

private:
    std::unordered_map<std::string, T> data;
};
