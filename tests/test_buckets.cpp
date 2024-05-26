#include "buckets.hpp"
#include "debug.hpp"

auto main() -> int {
    // 创建一个新的 Buckets 实例
    Buckets<int> buckets;

    // 添加一些数据到桶中
    buckets.insert("apples", 10);
    buckets.insert("oranges", 5);
    buckets.insert("bananas", 8);

    // 测试获取桶中的数据
    LOG_INFO("apples: {}", buckets.get("apples").value_or(0));
    LOG_INFO("oranges {}", buckets.get("oranges").value_or(0));
    LOG_INFO("bananas {}", buckets.get("bananas").value_or(0));
    LOG_INFO("grapes {}", buckets.get("grapes").value_or(0));

    // 测试删除数据
    LOG_INFO("Removing oranges {}", buckets.remove("oranges").value_or(0));
    LOG_INFO("oranges {}", buckets.get("oranges").value_or(0));

    // 测试更新数据
    buckets.insert("apples", 5);
    LOG_INFO("apples {}", buckets.get("apples").value_or(0));

    LOG_INFO("All tests passed!");
}
