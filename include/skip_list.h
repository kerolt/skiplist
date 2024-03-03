#ifndef SKIP_LIST_H_
#define SKIP_LIST_H_

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <mutex>
#include "node.h"

template <typename K, typename V>
class SkipList {
    using NodeType = Node<K, V>;

public:
    SkipList(int max_level, std::string dump_file = "./dumpfile")
        : max_level_(max_level)
        , cur_level_(0)
        , ele_counts_(0)
        , dump_file_(dump_file) {
        header_ = std::make_shared<NodeType>(K(), V(), max_level);
    }

    ~SkipList() {
        if (file_reader_.is_open()) {
            file_reader_.close();
        }
        if (file_writer_.is_open()) {
            file_writer_.close();
        }
    }

    int GetRandomLevel() const {
        int level = 1;
        while (std::rand() % 2) {
            level++;
        }
        return (level > max_level_) ? max_level_ : level;
    }

    int InsertElement(K key, V val) {
        std::unique_lock<std::mutex> lock(mutex_);
        auto cur = header_;

        // update[i]是在查找过程中找到的第i层中最后一个小于新节点键值的后一个节点
        auto update = NodeVec<K, V>(max_level_ + 1);

        for (int i = cur_level_; i >= 0; i--) {
            while (cur->forward_[i] != nullptr &&
                   cur->forward_[i]->GetKey() < key) {
                cur = cur->forward_[i];
            }
            // 第i层需要更新的节点
            update[i] = cur;
        }

        cur = cur->forward_[0];
        // 存在
        if (cur != nullptr && cur->GetKey() == key) {
            std::cout << "Key: " << key << " is exists" << std::endl;
            lock.unlock();
            return 1;
        }

        // 不存在
        if (cur == nullptr || cur->GetKey() != key) {
            int rand_level = GetRandomLevel();

            // 若rand_level > cur_level_，则level_+1，并更新update数组
            if (rand_level > cur_level_) {
                for (int i = cur_level_ + 1; i < rand_level + 1; i++) {
                    update[i] = header_;
                }
                cur_level_ = rand_level;
            }

            auto insert_node = CreateNode(key, val, rand_level);
            for (int i = 0; i < rand_level; i++) {
                insert_node->forward_[i] = update[i]->forward_[i];
                update[i]->forward_[i] = insert_node;
            }
            std::cout << "Insert successful! Key: " << key << ", Value: " << val
                      << std::endl;
            ele_counts_++;
        }
        lock.unlock();
        return 0;
    }

    std::shared_ptr<NodeType> CreateNode(K key, V val, int level) {
        auto node = std::make_shared<Node<K, V>>(key, val, level);
        return node;
    }

    void PrintList() const {
        std::cout << "====== Skip List ======= \n";
        for (int i = 0; i <= cur_level_; i++) {
            auto node = header_->forward_[i];
            std::cout << "[Level " << i << "]: ";
            while (node != nullptr) {
                std::cout << node->GetKey() << ":" << node->GetVal() << " ";
                node = node->forward_[i];
            }
            std::cout << std::endl;
        }
    }

    void DeleteElement(K key) {
        std::unique_lock<std::mutex> lock(mutex_);
        auto cur = header_;
        // 需要删除的节点
        auto update = NodeVec<K, V>(max_level_ + 1);

        for (int i = cur_level_; i >= 0; i--) {
            while (cur->forward_[i] != nullptr &&
                   cur->forward_[i]->GetKey() < key) {
                cur = cur->forward_[i];
            }
            update[i] = cur;
        }

        cur = cur->forward_[0];
        if (cur != nullptr && cur->GetKey() == key) {
            for (int i = 0; i <= cur_level_; i++) {
                if (update[i]->forward_[i] != cur) {
                    break;
                }
                update[i]->forward_[i] = cur->forward_[i];
            }
            while (cur_level_ > 0 && header_->forward_[cur_level_] == 0) {
                cur_level_--;
            }

            std::cout << "Delete key " << key << "successfully" << std::endl;
            ele_counts_--;
        }
    }

    bool SearchElement(K key) const {
        std::cout << "Search element-----------\n";
        auto cur = header_;

        for (int i = cur_level_; i >= 0; i--) {
            while (cur->forward_[i] != nullptr &&
                   cur->forward_[i]->GetKey() < key) {
                cur = cur->forward_[i];
            }
        }

        cur = cur->forward_[0];
        if (cur && cur->GetKey() == key) {
            std::cout << "Found key: " << key << ", value: " << cur->GetVal()
                      << std::endl;
            return true;
        }
        std::cout << "Not found key: " << key << std::endl;
        return false;
    }

    void DumpFile() {
        std::cout << "Dump file----------\n";
        file_writer_.open(dump_file_);
        auto node = header_->forward_[0];
        while (node != nullptr) {
            file_writer_ << node->GetKey() << ":" << node->GetVal() << "\n";
            std::cout << node->GetKey() << ":" << node->GetVal() << "\n";
            node = node->forward_[0];
        }
        file_writer_.flush();
        file_writer_.close();
    }

    void LoadFile() {
        std::cout << "Load file----------\n";
        file_reader_.open(dump_file_);
        std::string str, key, val;
        while (std::getline(file_reader_, str)) {
            GetKeyValFromStr(str, key, val);
            if (key.empty() || val.empty()) {
                continue;
            }
            InsertElement(key, val);
            std::cout << "key: " << key << ", value: " << val << std::endl;
        }
        file_reader_.close();
    }

    int Size() const {
        return ele_counts_;
    }

private:
    void GetKeyValFromStr(const std::string& str, std::string& key,
                          std::string& val) const {
        if (!IsValidStr(str)) {
            return;
        }
        auto divide = str.find(kDelimiter);
        key = str.substr(0, divide);
        val = str.substr(divide + 1, str.length());
    }

    bool IsValidStr(const std::string& str) const {
        return !(str.empty() || str.find(kDelimiter) == std::string::npos);
    }

private:
    const std::string kDelimiter = ":";

    int max_level_;
    int cur_level_;
    int ele_counts_;
    std::string dump_file_;
    std::shared_ptr<NodeType> header_;
    std::ofstream file_writer_;
    std::ifstream file_reader_;
    std::mutex mutex_;
};

#endif /* SKIP_LIST_H_ */
