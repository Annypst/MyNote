# 1

### Keywords

Standard Template Library，Decoupling，Containers，Algorithms，Iterators

| **容器名称 (C++)**             | **对应 Java 集合** | **物理内存结构**               | **特点与最合适场景**                             |
| ------------------------------ | ------------------ | ------------------------------ | ------------------------------------------------ |
| **`std::vector<T>`**           | `ArrayList<T>`     | 连续的物理内存（一字排开）     | 尾部插入极快，支持下标随机访问，但中间插入极慢。 |
| **`std::list<T>`**             | `LinkedList<T>`    | 散落的物理节点，靠双向指针连接 | 任意位置插入删除极快，但不支持下标随机访问。     |
| **`std::unordered_map<K, V>`** | `HashMap<K, V>`    | 哈希表（数组 + 链表/红黑树）   | 通过 Key 查找 Value 极快，接近 $O(1)$ 时间。     |

### Hints

既然我们知道 `vector` 频繁自动扩容会导致：

1. 堆区频繁申请和释放，**拖慢运行速度**（性能损耗）。
2. 导致所有指向内部元素的指针、引用、迭代器**瞬间失效（变成悬空指针）**。

那么在 C++ 中，有什么高级技巧可以完美规避这个问题？

答案是：**`reserve(预估容量)` (预留空间)**。

```cpp
std::vector<int> v;
v.reserve(1000); // 💡 物理动作：提前让堆管理器一次性切出 1000 个 int 的连续大空间！

// 此时 capacity 直接就是 1000。
// 随后你进行 1000 次 push_back，它会在这个温暖的安全屋里直接写入，
// 绝对不会发生一次“搬家/扩容”动作，首地址稳如泰山，指针永不失效！
```

### Example Programs

1.vector capacity

```cpp
#include <iostream>
#include <vector>

int main() {
    std::cout << "--- 测试 vector 扩容的地址突变 ---" << std::endl;

    std::vector<int> v;

    // 观察随着元素插入，size、capacity 以及首元素物理地址的变化
    for (int i = 0; i < 10; ++i) {
        v.push_back(i);
        
        // 打印当前的 size, capacity 以及第一个元素的内存地址
        std::cout << "放入元素 " << i 
                  << " | size = " << v.size() 
                  << " | capacity = " << v.capacity();
        
        if (!v.empty()) {
            std::cout << " | 首元素物理地址 = " << &v[0] << std::endl;
        } else {
            std::cout << std::endl;
        }
    }

    return 0;
}
```

2.vector

```cpp
#include <iostream>
#include <vector> // 💡 必须引入头文件

int main() {
    // === 1. 初始化 ===
    std::vector<int> v; // 创建一个装 int 的空动态数组

    // === 2. 尾部插入 (Java: list.add(x)) ===
    v.push_back(10);
    v.push_back(20);
    v.push_back(30);

    // === 3. 访问元素 ===
    std::cout << "第一个元素: " << v[0] << std::endl; // 支持像数组一样用 [] 访问
    std::cout << "越界安全访问: " << v.at(1) << std::endl; // at() 会检查越界，越界时会抛出异常

    // === 4. 遍历元素 ===
    // 方式 A：现代 C++ 范围 for 循环（最常用，类似 Java 的 for-each）
    // 用 const int& 是为了防止拷贝，提高性能
    for (const int& val : v) {
        std::cout << val << " ";
    }
    std::cout << std::endl;

    // 方式 B：使用【迭代器（Iterator）】遍历（底层核心）
    // v.begin() 指向首元素，v.end() 指向尾元素后面那个哨兵位置
    // it 伪装成指针，用 *it 拿到里面的值
    for (std::vector<int>::iterator it = v.begin(); it != v.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;

    // === 5. 删除元素 ===
    v.pop_back(); // 弹出尾部最后一个元素 (此时剩下 10, 20)

    return 0;
}
```

3.list

```cpp
#include <iostream>
#include <list> // 💡 引入头文件

int main() {
    std::list<int> l;

    // === 1. 插入（两端都可以极速插入） ===
    l.push_back(20);  // 尾部插入
    l.push_front(10); // 头部插入：[10, 20]

    // === 2. 链表不支持 [index] 随机访问！ ===
    // ❌ std::cout << l[0]; // 编译报错！因为链表无法通过数学计算直接定位地址

    // === 3. 遍历（只能通过迭代器一步步走过去） ===
    for (const int& val : l) {
        std::cout << val << " ";
    }
    std::cout << std::endl;

    return 0;
}
```

4.unordered_map

```cpp
#include <iostream>
#include <string>
#include <unordered_map> // 💡 引入头文件

int main() {
    // 声明一个 Key 是 string，Value 是 int 的哈希表
    std::unordered_map<std::string, int> scores;

    // === 1. 插入/更新键值对 ===
    scores["Zhou"] = 100;
    scores["Wu"] = 98;

    // === 2. 查找元素 (极其高频，注意避坑) ===
    std::string target = "Zhou";
    
    // 💡 避坑写法：不要直接用 scores[target] 来判断是否存在！
    // 因为如果 key 不存在，[] 运算符会默默在表里新建一个默认值的键值对！
    // 正确写法：使用 find()
    auto it = scores.find(target); 
    if (it != scores.end()) {
        // it->first 是 Key，it->second 是 Value
        std::cout << "找到了！" << it->first << " 的分数是: " << it->second << std::endl;
    } else {
        std::cout << "未找到该用户！" << std::endl;
    }

    // === 3. 遍历哈希表 ===
    // 用 const auto& 让编译器自动推导键值对类型，非常清爽
    for (const auto& pair : scores) {
        std::cout << "Key: " << pair.first << ", Value: " << pair.second << std::endl;
    }

    return 0;
}
```

