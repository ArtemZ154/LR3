#pragma once
#include <string>
#include <vector>
#include <map>
#include "DataStructures/Array.h"
#include "DataStructures/SinglyLinkedList.h"
#include "DataStructures/DoublyLinkedList.h"
#include "DataStructures/Stack.h"
#include "DataStructures/Queue.h"
#include "DataStructures/FullBinaryTree.h"
#include "DataStructures/Set.h"
#include "DataStructures/LFUCache.h"
#include "DataStructures/HashTableChaining.h"
#include "DataStructures/HashTableOpenAddr.h"

class DBMS {
public:
    std::string execute(const std::vector<std::string>& command);
    void clear();
    void loadStructure(const std::string& type, const std::string& name, const std::string& data);
    std::string serializeAll() const;
private:
    std::map<std::string, Array<std::string>> arrays;
    std::map<std::string, SinglyLinkedList<std::string>> singly_lists;
    std::map<std::string, DoublyLinkedList<std::string>> doubly_lists;
    std::map<std::string, Stack<std::string>> stacks;
    std::map<std::string, Queue<std::string>> queues;
    std::map<std::string, FullBinaryTree<std::string>> trees;
    std::map<std::string, Set<std::string>> sets;
    std::map<std::string, LFUCache> lfu_caches;
    std::map<std::string, HashTableChaining> ht_chaining;
    std::map<std::string, HashTableOpenAddr> ht_open_addr;

    std::string printAll() const;
};