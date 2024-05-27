#pragma once

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <functional>
#include <random>
#include <string>
#include <vector>

class Estimator {
    using HashFunc = std::function<uint64_t(const std::string &)>;
    using Slot = std::vector<uint8_t>;

private:
    /// Create a new `Estimator` with the given amount of hashes and columns
    /// (slots).
    Estimator(std::size_t hashes, std::size_t slots) {
        std::mt19937_64                         engine(std::random_device{}());
        std::uniform_int_distribution<uint64_t> distribution;
        for (auto i = 0u; i < hashes; i++) {
            uint64_t a = distribution(engine);
            uint64_t b = distribution(engine);
            hash_functions_.emplace_back(
                [a, b, slots](const auto &key) -> uint64_t {
                    uint64_t hash = 0;
                    for (size_t i = 0; i < key.length(); i++) {
                        hash = (hash * a + key[i]) * b + hash;
                    }
                    // return hash;
                    return hash % slots;
                });
            counters_.emplace_back(slots);
        }
    }

public:
    static auto create(std::size_t items) -> Estimator {
        auto [slots, hashes] = optimal_parameters(items);
        return {hashes, slots};
    }

    auto incr(const std::string &key) -> std::uint8_t {
        std::uint8_t min = UINT8_MAX;
        for (size_t i = 0; i < hash_functions_.size(); i++) {
            size_t hash = hash_functions_[i](key);
            counters_[i][hash]++;
            min = std::min(min, counters_[i][hash]);
        }
        return min;
    }

    // Get the estimated frequency of `key`.
    auto get(const std::string &key) -> std::size_t {
        std::uint8_t min = UINT8_MAX;
        for (size_t i = 0; i < hash_functions_.size(); ++i) {
            size_t hash = hash_functions_[i](key);
            min = std::min(min, counters_[i][hash]);
        }
        return min;
    }

    // right shift all values inside this `Estimator`.
    void age(std::uint8_t shift) {
        for (auto &slot : counters_) {
            for (auto &c : slot) {
                c >>= shift;
            }
        }
    }

    static auto optimal_parameters(size_t items) -> std::pair<size_t, size_t> {
        // derived from https://en.wikipedia.org/wiki/Count%E2%80%93min_sketch
        // width = ceil(e / ε)
        // depth = ceil(ln(1 − δ) / ln(1 / 2))
        double error_range = 1.0 / static_cast<double>(items);
        double failure_probability = 1.0 / static_cast<double>(items);
        auto width = static_cast<size_t>(std::ceil(std::exp(1) / error_range));
        auto depth = static_cast<size_t>(
            std::ceil(std::log(failure_probability) / std::log(0.5)));
        return {std::max(width, static_cast<size_t>(16)),
                std::max(depth, static_cast<size_t>(2))};
    }

private:
    std::vector<HashFunc> hash_functions_;
    std::vector<Slot>     counters_;
};

class TinyLfu {
public:
    // because we use 8-bits counters, window size can be 256 * the cache size
    TinyLfu(std::size_t cache_size)
        : estimator_{Estimator::create(cache_size)}
        // 8x: just a heuristic to balance the memory usage and accuracy
        , window_limit_{cache_size * 8} {}

    auto get(const std::string &key) -> uint8_t {
        return estimator_.get(key);
    }

    auto incr(const std::string &key) -> uint8_t {
        auto window_size = window_counter_++;
        if (window_size == window_limit_ || window_size > window_limit_ * 2) {
            window_counter_ = 0;
            estimator_.age(1);
        }
        return estimator_.incr(key);
    }

private:
    Estimator   estimator_;
    std::size_t window_counter_{0};
    std::size_t window_limit_;
};
