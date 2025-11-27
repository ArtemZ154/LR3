#include "DBMS.h"
#include <stdexcept>
#include <sstream>
#include <vector>
#include <string>
#include <numeric> // для std::accumulate
#include <map>     // для prefix-sum (Задание 4)
#include <algorithm> // для std::max
#include <memory>  // для std::unique_ptr (Задание 5)
#include "DataStructures/HashTableOpenAddr.h" // Для solveLongestSubstring

// --- Анонимное пространство имен для helper-функций ---
namespace {

    std::string compute_hash(const std::string& key) {
        unsigned long hash = 5381;
        for (char c : key) {
            hash = ((hash << 5) + hash) + c; // hash * 33 + c
        }
        return std::to_string(hash);
    }

Array<std::string> solveAsteroids(const Array<std::string>& input) {
    Stack<int> stack;
    std::vector<int> asteroids;
    for (size_t i = 0; i < input.size(); ++i) {
        try {
            asteroids.push_back(std::stoi(input.get(i)));
        } catch (...) { /* пропустить не-числа */ }
    }

    for (int ast : asteroids) {
        int current_ast = ast;
        bool destroyed = false;
        while (!stack.empty() && current_ast < 0 && stack.peek() > 0) {
            int top = stack.peek();
            if (top > -current_ast) {
                destroyed = true;
                break;
            } else if (top < -current_ast) {
                stack.pop();
            } else { // top == -current_ast
                stack.pop();
                destroyed = true;
                break;
            }
        }
        if (!destroyed) {
            stack.push(current_ast);
        }
    }

    Array<std::string> result;
    Stack<int> reverseStack;
    while(!stack.empty()) {
        reverseStack.push(stack.pop());
    }
    while(!reverseStack.empty()) {
        result.push_back(std::to_string(reverseStack.pop()));
    }
    return result;
}

std::string solveMinPartition(const Set<std::string>& input, Set<std::string>& s1, Set<std::string>& s2) {
    std::vector<int> nums;
    std::vector<std::string> num_strs;
    int totalSum = 0;

    size_t count = input.size();
    std::string* elements = input.getElements();
    for (size_t i = 0; i < count; ++i) {
        const std::string& s = elements[i];
        try {
            int num = std::stoi(s);
            nums.push_back(num);
            num_strs.push_back(s);
            totalSum += num;
        } catch (...) { }
    }
    delete[] elements;

    int n = nums.size();
    int targetSum = totalSum / 2;

    std::vector<bool> dp(targetSum + 1, false);
    std::vector<int> parent(targetSum + 1, -1);
    dp[0] = true;

    for (int i = 0; i < n; ++i) {
        for (int j = targetSum; j >= nums[i]; --j) {
            if (dp[j - nums[i]] && !dp[j]) {
                dp[j] = true;
                parent[j] = i;
            }
        }
    }

    int s1Sum = 0;
    for (int j = targetSum; j >= 0; --j) {
        if (dp[j]) {
            s1Sum = j;
            break;
        }
    }

    std::vector<bool> s1_indices(n, false);
    int currSum = s1Sum;
    while (currSum > 0 && parent[currSum] != -1) {
        int index = parent[currSum];
        if (!s1_indices[index]) {
            s1_indices[index] = true;
            s1.add(num_strs[index]);
            currSum -= nums[index];
        }
    }

    for (int i = 0; i < n; ++i) {
        if (!s1_indices[i]) {
            s2.add(num_strs[i]);
        }
    }

    int s2Sum = totalSum - s1Sum;
    int diff = std::abs(s1Sum - s2Sum);

    return std::to_string(diff);
}

bool solveFindSum(const Array<std::string>& input, int target, Array<std::string>& output) {
    std::vector<int> nums;
    std::vector<std::string> num_strs;
    for (size_t i = 0; i < input.size(); ++i) {
        try {
            std::string s = input.get(i);
            nums.push_back(std::stoi(s));
            num_strs.push_back(s);
        } catch (...) { /* ignore */ }
    }

    std::map<long long, std::vector<int>> prefixSumMap;
    prefixSumMap[0].push_back(-1);
    long long currentSum = 0;
    bool found = false;

    for (int i = 0; i < nums.size(); ++i) {
        currentSum += nums[i];
        long long diff = currentSum - target;
        if (prefixSumMap.count(diff)) {
            for (int start_index : prefixSumMap[diff]) {
                found = true;
                std::stringstream ss;
                ss << "{";
                for (int j = start_index + 1; j <= i; ++j) {
                    ss << num_strs[j] << (j == i ? "" : ", ");
                }
                ss << "}";
                output.push_back(ss.str());
            }
        }
        prefixSumMap[currentSum].push_back(i);
    }
    return found;
}

namespace bst_task {
    struct Node {
        int data;
        std::unique_ptr<Node> left;
        std::unique_ptr<Node> right;
        Node(int val) : data(val) {}
    };

    int insert(std::unique_ptr<Node>& node, int value, int depth) {
        if (!node) {
            node = std::make_unique<Node>(value);
            return depth;
        }
        if (value < node->data) {
            return insert(node->left, value, depth + 1);
        }
        if (value > node->data) {
            return insert(node->right, value, depth + 1);
        }
        return -1;
    }
}

int solveLongestSubstring(const std::string& s) {
    if (s.empty()) return 0;
    HashTableOpenAddr char_map;
    int left = 0;
    int maxLength = 0;
    for (int right = 0; right < s.length(); ++right) {
        std::string current_char_str(1, s[right]);
        std::string found_index_str;
        if (char_map.get(current_char_str, found_index_str)) {
            int last_pos = std::stoi(found_index_str);
            if (last_pos >= left) {
                left = last_pos + 1;
            }
        }
        char_map.put(current_char_str, std::to_string(right));
        maxLength = std::max(maxLength, right - left + 1);
    }
    return maxLength;
}

}

std::string DBMS::execute(const std::vector<std::string>& command) {
    if (command.empty()) return "Error: Empty command.";
    const std::string& cmd = command[0];

    try {
        if (cmd == "PRINT") return printAll();
        
        if (cmd == "SEMPTY") {
            if (command.size() < 2) return "Error: SEMPTY requires a name.";
            return stacks.at(command[1]).empty() ? "-> TRUE" : "-> FALSE";
        }
        if (cmd == "QEMPTY") {
            if (command.size() < 2) return "Error: QEMPTY requires a name.";
            return queues.at(command[1]).empty() ? "-> TRUE" : "-> FALSE";
        }

        // --- Array ---
        if (cmd == "MPUSH") {
            if (command.size() < 3) return "Error: MPUSH requires at least one value.";
            for (size_t i = 2; i < command.size(); ++i) arrays[command[1]].push_back(command[i]);
            return "-> OK";
        }
        if (cmd == "MGET") { if (command.size() < 3) return "Error: MGET requires an index."; return "-> " + arrays.at(command[1]).get(std::stoul(command[2])); }
        if (cmd == "MDEL") { if (command.size() < 3) return "Error: MDEL requires an index."; arrays.at(command[1]).remove(std::stoul(command[2])); return "-> OK"; }
        if (cmd == "MINSERT") { if (command.size() < 4) return "Error: MINSERT requires an index and a value."; arrays[command[1]].insert(std::stoul(command[2]), command[3]); return "-> OK"; }
        if (cmd == "MSET") { if (command.size() < 4) return "Error: MSET requires an index and a value."; arrays[command[1]].set(std::stoul(command[2]), command[3]); return "-> OK"; }

        // --- Stack ---
        if (cmd == "SPUSH") { if (command.size() < 3) return "Error: SPUSH requires a value."; stacks[command[1]].push(command[2]); return "-> OK"; }
        if (cmd == "SPOP") { if (command.size() < 2) return "Error: SPOP requires a name."; return "-> " + stacks.at(command[1]).pop(); }

        // --- Queue ---
        if (cmd == "QPUSH") { if (command.size() < 3) return "Error: QPUSH requires a value."; queues[command[1]].push(command[2]); return "-> OK"; }
        if (cmd == "QPOP") { if (command.size() < 2) return "Error: QPOP requires a name."; return "-> " + queues.at(command[1]).pop(); }

        // --- Singly Linked List ---
        if (cmd == "FPUSHB") { if (command.size() < 3) return "Error: FPUSHB requires a value."; singly_lists[command[1]].push_back(command[2]); return "-> OK"; }
        if (cmd == "FPUSHF") { if (command.size() < 3) return "Error: FPUSHF requires a value."; singly_lists[command[1]].push_front(command[2]); return "-> OK"; }
        if (cmd == "FPOPF") { if (command.size() < 2) return "Error: FPOPF requires a name."; return "-> " + singly_lists.at(command[1]).pop_front(); }
        if (cmd == "FDELV") { if (command.size() < 3) return "Error: FDELV requires a value."; singly_lists.at(command[1]).remove_value(command[2]); return "-> OK"; }
        if (cmd == "FFIND") { if (command.size() < 3) return "Error: FFIND requires a value."; return singly_lists.at(command[1]).find(command[2]) ? "-> TRUE" : "-> FALSE"; }
        if (cmd == "FINSA") { if (command.size() < 4) return "Error: FINSA requires a target and a new value."; singly_lists[command[1]].insert_after(command[2], command[3]); return "-> OK"; }
        if (cmd == "FINSB") { if (command.size() < 4) return "Error: FINSB requires a target and a new value."; singly_lists[command[1]].insert_before(command[2], command[3]); return "-> OK"; }
        if (cmd == "FREMA") { if (command.size() < 3) return "Error: FREMA requires a target value."; singly_lists[command[1]].remove_after(command[2]); return "-> OK"; }
        if (cmd == "FREMB") { if (command.size() < 3) return "Error: FREMB requires a target value."; singly_lists[command[1]].remove_before(command[2]); return "-> OK"; }

        // --- Doubly Linked List ---
        if (cmd == "LPUSHB") { if (command.size() < 3) return "Error: LPUSHB requires a value."; doubly_lists[command[1]].push_back(command[2]); return "-> OK"; }
        if (cmd == "LPUSHF") { if (command.size() < 3) return "Error: LPUSHF requires a value."; doubly_lists[command[1]].push_front(command[2]); return "-> OK"; }
        if (cmd == "LPOPB") { if (command.size() < 2) return "Error: LPOPB requires a name."; return "-> " + doubly_lists.at(command[1]).pop_back(); }
        if (cmd == "LPOPF") { if (command.size() < 2) return "Error: LPOPF requires a name."; return "-> " + doubly_lists.at(command[1]).pop_front(); }
        if (cmd == "LREMA") { if (command.size() < 3) return "Error: LREMA requires a target value."; doubly_lists.at(command[1]).remove_after(command[2]); return "-> OK"; }
        if (cmd == "LREMB") { if (command.size() < 3) return "Error: LREMB requires a target value."; doubly_lists.at(command[1]).remove_before(command[2]); return "-> OK"; }
        if (cmd == "LINSA") { if (command.size() < 4) return "Error: LINSA requires a target and a new value."; doubly_lists[command[1]].insert_after(command[2], command[3]); return "-> OK"; }
        if (cmd == "LINSB") { if (command.size() < 4) return "Error: LINSB requires a target and a new value."; doubly_lists[command[1]].insert_before(command[2], command[3]); return "-> OK"; }
        if (cmd == "LDELV") { if (command.size() < 3) return "Error: LDELV requires a value."; doubly_lists.at(command[1]).remove_value(command[2]); return "-> OK"; }
        if (cmd == "LFIND") { if (command.size() < 3) return "Error: LFIND requires a value."; return doubly_lists.at(command[1]).find(command[2]) ? "-> TRUE" : "-> FALSE"; }

        // --- Tree ---
        if (cmd == "TINSERT") { if (command.size() < 3) return "Error: TINSERT requires a value."; trees[command[1]].insert(command[2]); return "-> OK"; }
        if (cmd == "TGET") { if (command.size() < 3) return "Error: TGET requires a value to find."; return trees.at(command[1]).find(command[2]) ? "-> TRUE" : "-> FALSE"; }
        if (cmd == "TISFULL") { if (command.size() < 2) return "Error: TISFULL requires a name."; return trees.at(command[1]).isFull() ? "-> TRUE" : "-> FALSE"; }

        // --- Hash Table Chaining ---
        if (cmd == "CH_PUT") {
            if (command.size() < 4) return "Error: CH_PUT requires <name> <key> <value>";
            ht_chaining[command[1]].put(command[2], command[3]);
            return "-> OK";
        }
        if (cmd == "CH_GET") {
            if (command.size() < 3) return "Error: CH_GET requires <name> <key>";
            return "-> " + ht_chaining.at(command[1]).get(command[2]);
        }
        if (cmd == "CH_DEL") {
            if (command.size() < 3) return "Error: CH_DEL requires <name> <key>";
            ht_chaining.at(command[1]).remove(command[2]);
            return "-> OK";
        }

        // --- Hash Table Open Addressing ---
        if (cmd == "OA_PUT") {
            if (command.size() < 4) return "Error: OA_PUT requires <name> <key> <value>";
            ht_open_addr[command[1]].put(command[2], command[3]);
            return "-> OK";
        }
        if (cmd == "OA_GET") {
            if (command.size() < 3) return "Error: OA_GET requires <name> <key>";
            return "-> " + ht_open_addr.at(command[1]).get(command[2]);
        }
        if (cmd == "OA_DEL") {
            if (command.size() < 3) return "Error: OA_DEL requires <name> <key>";
            ht_open_addr.at(command[1]).remove(command[2]);
            return "-> OK";
        }

        // --- Set ---
        if (cmd == "SETADD") {
            if (command.size() < 3) return "Error: SETADD requires at least one value.";
            for (size_t i = 2; i < command.size(); ++i) sets[command[1]].add(command[i]);
            return "-> OK";
        }
        if (cmd == "SETDEL") { if (command.size() < 3) return "Error: SETDEL requires a value."; sets[command[1]].remove(command[2]); return "-> OK"; }
        if (cmd == "SET_AT") { if (command.size() < 3) return "Error: SET_AT requires a value."; return sets.at(command[1]).contains(command[2]) ? "-> TRUE" : "-> FALSE"; }

        // --- Special Tasks ---
        if (cmd == "ASTEROID_COLLIDE") {
            if (command.size() < 3) return "Error: ASTEROID_COLLIDE requires <input_array> <output_array>";
            arrays[command[2]] = solveAsteroids(arrays.at(command[1]));
            return "-> OK";
        }
        if (cmd == "PARTITION_MIN_DIFF") {
            if (command.size() < 4) return "Error: PARTITION_MIN_DIFF requires <input_set> <out_set1> <out_set2>";
            std::string diff = solveMinPartition(sets.at(command[1]), sets[command[2]], sets[command[3]]);
            return "-> Difference: " + diff;
        }
        if (cmd == "FIND_SUM_SUBARRAY") {
            if (command.size() < 4) return "Error: FIND_SUM_SUBARRAY requires <input_array> <target_sum> <output_array>";
            int target;
            try { target = std::stoi(command[2]); } catch (...) { return "Error: Invalid target sum."; }
            if (solveFindSum(arrays.at(command[1]), target, arrays[command[3]])) return "-> OK";
            else return "-> Error: Subarray not found.";
        }
        if (cmd == "BST_ADD_DEPTHS") {
            if (command.size() < 3) return "Error: BST_ADD_DEPTHS requires <input_array> <output_array>";
            std::unique_ptr<bst_task::Node> bst_root = nullptr;
            arrays[command[2]].clear();
            const auto& input_array = arrays.at(command[1]);
            for (size_t i = 0; i < input_array.size(); ++i) {
                try {
                    int val = std::stoi(input_array.get(i));
                    int depth = bst_task::insert(bst_root, val, 1);
                    if (depth != -1) arrays[command[2]].push_back(std::to_string(depth));
                } catch (...) { /* ignore */ }
            }
            return "-> OK";
        }
        if (cmd == "LONGEST_SUBSTRING") {
            if (command.size() < 2) return "Error: LONGEST_SUBSTRING requires a string.";
            return "-> " + std::to_string(solveLongestSubstring(command[1]));
        }
        if (cmd == "LFU_INIT") {
            if (command.size() < 3) return "Error: LFU_INIT requires <name> <capacity>";
            try { lfu_caches[command[1]] = LFUCache(std::stoi(command[2])); } catch (...) { return "Error: Invalid capacity."; }
            return "-> OK";
        }
        if (cmd == "LFU_SET") {
            if (command.size() < 4) return "Error: LFU_SET requires <name> <key> <value>";
            lfu_caches.at(command[1]).set(command[2], command[3]);
            return "-> OK";
        }
        if (cmd == "LFU_GET") {
            if (command.size() < 3) return "Error: LFU_GET requires <name> <key>";
            return "-> " + lfu_caches.at(command[1]).get(command[2]);
        }

    } catch (const std::out_of_range& e) {
        return "Error: Not found (or out of range).";
    } catch (const std::exception& e) {
        return "Error: " + std::string(e.what());
    }

    return "Error: Unknown command '" + cmd + "'";
}

void DBMS::clear() {
    arrays.clear();
    singly_lists.clear();
    doubly_lists.clear();
    stacks.clear();
    queues.clear();
    trees.clear();
    sets.clear();
    lfu_caches.clear();
    ht_chaining.clear();
    ht_open_addr.clear();
}

void DBMS::loadStructure(const std::string& type, const std::string& name, const std::string& data) {
    if (type == "Array") arrays[name].deserialize(data);
    else if (type == "SinglyLinkedList") singly_lists[name].deserialize(data);
    else if (type == "DoublyLinkedList") doubly_lists[name].deserialize(data);
    else if (type == "Stack") stacks[name].deserialize(data);
    else if (type == "Queue") queues[name].deserialize(data);
    else if (type == "Tree") trees[name].deserialize(data);
    else if (type == "Set") sets[name].deserialize(data);
    else if (type == "LFUCache") lfu_caches[name].deserialize(data);
    else if (type == "HT_Chain") ht_chaining[name].deserialize(data);
    else if (type == "HT_Open") ht_open_addr[name].deserialize(data);
}

std::string DBMS::serializeAll() const {
    std::stringstream ss;
    for (const auto& p : arrays) ss << "Array " << p.first << " " << p.second.serialize() << "\n";
    for (const auto& p : singly_lists) ss << "SinglyLinkedList " << p.first << " " << p.second.serialize() << "\n";
    for (const auto& p : doubly_lists) ss << "DoublyLinkedList " << p.first << " " << p.second.serialize() << "\n";
    for (const auto& p : stacks) ss << "Stack " << p.first << " " << p.second.serialize() << "\n";
    for (const auto& p : queues) ss << "Queue " << p.first << " " << p.second.serialize() << "\n";
    for (const auto& p : trees) ss << "Tree " << p.first << " " << p.second.serialize() << "\n";
    for (const auto& p : sets) ss << "Set " << p.first << " " << p.second.serialize() << "\n";
    for (const auto& p : lfu_caches) ss << "LFUCache " << p.first << " " << p.second.serialize() << "\n";
    for (const auto& p : ht_chaining) ss << "HT_Chain " << p.first << " " << p.second.serialize() << "\n";
    for (const auto& p : ht_open_addr) ss << "HT_Open " << p.first << " " << p.second.serialize() << "\n";
    return ss.str();
}

std::string DBMS::printAll() const { return serializeAll(); }