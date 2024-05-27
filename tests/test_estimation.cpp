#include "debug.hpp"
#include "estimation.hpp"

auto test_optimal_parameters() {
    auto [slots, hashes] = Estimator::optimal_parameters(1'000'000);
    LOG_INFO("slots = {}, hashes = {}", slots, hashes);
    ASSERT_EQ(slots, 2718282);
    ASSERT_EQ(hashes, 20);
}

auto main() -> int {
    test_optimal_parameters();
    // // auto estimator = Estimator::create(1000);
    // auto estimator = Estimator::create(10);
    // estimator.incr("key1");
    // estimator.incr("key1");
    // estimator.incr("key1");
    // estimator.incr("key2");
    // LOG_INFO("Frequency of key1: {}", estimator.get("key1"));
    // LOG_INFO("Frequency of key2: {}", estimator.get("key2"));
    // estimator.age(1);
}
