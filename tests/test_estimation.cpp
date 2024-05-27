#include "debug.hpp"
#include "estimation.hpp"

auto test_optimal_parameters() {
    auto [slots, hashes] = Estimator::optimal_parameters(1'000'000);
    LOG_INFO("slots = {}, hashes = {}", slots, hashes);
    ASSERT_EQ(slots, 2718282);
    ASSERT_EQ(hashes, 20);
}

auto test_estimator() {
    auto estimator = Estimator::create(1000);
    estimator.incr("key1");
    estimator.incr("key1");
    estimator.incr("key1");
    estimator.incr("key2");
    LOG_INFO("Frequency of key1: {}", estimator.get("key1"));
    LOG_INFO("Frequency of key2: {}", estimator.get("key2"));
    estimator.age(1);
    LOG_INFO("Frequency of key1 after age(): {}", estimator.get("key1"));
    LOG_INFO("Frequency of key2 after age(): {}", estimator.get("key2"));
}

auto test_tiny_lfu() {
    auto tiny = TinyLfu(1);
    ASSERT_EQ(tiny.get("x"), 0);
    ASSERT_EQ(tiny.incr("x"), 1);
    ASSERT_EQ(tiny.incr("x"), 2);
    ASSERT_EQ(tiny.get("x"), 2);
    LOG_INFO("Frequency of x: {}", tiny.get("x"));
    LOG_INFO("Frequency of y: {}", tiny.get("y"));

    // Might have hash collisions for the others, need to
    // get() before can assert on the incr() value.
    auto two = tiny.get("y");
    ASSERT_EQ(tiny.incr("y"), two + 1);
    ASSERT_EQ(tiny.incr("y"), two + 2);
    ASSERT_EQ(tiny.get("y"), two + 2);
    LOG_INFO("Frequency of x: {}", tiny.get("x"));
    LOG_INFO("Frequency of y: {}", tiny.get("y"));
}

auto main() -> int {
    test_optimal_parameters();
    test_estimator();
    test_tiny_lfu();
}
