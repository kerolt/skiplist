#ifndef NODE_H_
#define NODE_H_

#include <cstddef>
#include <vector>
#include <memory>

template <typename K, typename V>
class Node {
public:
    Node() = default;
    ~Node() = default;

    Node(const K key, const V value, int level)
        : key_(key)
        , value_(value)
        , level_(level)
        , forward_(level, nullptr) {}

    K GetKey() const {
        return key_;
    }

    V GetVal() const {
        return value_;
    }

    void SetVal(V val) {
        value_ = val;
    }

public:
    // forward_数组的作用就是：保存该结点在第i层中的前向指针（前一个结点的指针）
    std::vector<std::shared_ptr<Node<K, V>>> forward_;
    int level_;

private:
    K key_;
    V value_;
};

template <typename K, typename V>
using NodeVec = std::vector<std::shared_ptr<Node<K, V>>>;

#endif /* NODE_H_ */
