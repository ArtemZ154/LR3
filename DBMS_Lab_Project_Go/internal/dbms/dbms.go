package dbms

import (
	"dbms_lab_project/internal/datastructures"
	"fmt"
	"strconv"
	"strings"
)

type DBMS struct {
	arrays      map[string]*datastructures.Array
	singlyLists map[string]*datastructures.SinglyLinkedList
	doublyLists map[string]*datastructures.DoublyLinkedList
	stacks      map[string]*datastructures.Stack
	queues      map[string]*datastructures.Queue
	trees       map[string]*datastructures.FullBinaryTree
	sets        map[string]*datastructures.Set
	lfuCaches   map[string]*datastructures.LFUCache
	htChaining  map[string]*datastructures.HashTableChaining
	htOpenAddr  map[string]*datastructures.HashTableOpenAddr
}

func NewDBMS() *DBMS {
	return &DBMS{
		arrays:      make(map[string]*datastructures.Array),
		singlyLists: make(map[string]*datastructures.SinglyLinkedList),
		doublyLists: make(map[string]*datastructures.DoublyLinkedList),
		stacks:      make(map[string]*datastructures.Stack),
		queues:      make(map[string]*datastructures.Queue),
		trees:       make(map[string]*datastructures.FullBinaryTree),
		sets:        make(map[string]*datastructures.Set),
		lfuCaches:   make(map[string]*datastructures.LFUCache),
		htChaining:  make(map[string]*datastructures.HashTableChaining),
		htOpenAddr:  make(map[string]*datastructures.HashTableOpenAddr),
	}
}

func (db *DBMS) Clear() {
	db.arrays = make(map[string]*datastructures.Array)
	db.singlyLists = make(map[string]*datastructures.SinglyLinkedList)
	db.doublyLists = make(map[string]*datastructures.DoublyLinkedList)
	db.stacks = make(map[string]*datastructures.Stack)
	db.queues = make(map[string]*datastructures.Queue)
	db.trees = make(map[string]*datastructures.FullBinaryTree)
	db.sets = make(map[string]*datastructures.Set)
	db.lfuCaches = make(map[string]*datastructures.LFUCache)
	db.htChaining = make(map[string]*datastructures.HashTableChaining)
	db.htOpenAddr = make(map[string]*datastructures.HashTableOpenAddr)
}

func (db *DBMS) LoadStructure(typeStr, name, data string) {
	switch typeStr {
	case "Array":
		arr := datastructures.NewArray()
		arr.Deserialize(data)
		db.arrays[name] = arr
	case "SinglyLinkedList":
		l := datastructures.NewSinglyLinkedList()
		l.Deserialize(data)
		db.singlyLists[name] = l
	case "DoublyLinkedList":
		l := datastructures.NewDoublyLinkedList()
		l.Deserialize(data)
		db.doublyLists[name] = l
	case "Stack":
		s := datastructures.NewStack()
		s.Deserialize(data)
		db.stacks[name] = s
	case "Queue":
		q := datastructures.NewQueue()
		q.Deserialize(data)
		db.queues[name] = q
	case "FullBinaryTree":
		t := datastructures.NewFullBinaryTree()
		t.Deserialize(data)
		db.trees[name] = t
	case "Set":
		s := datastructures.NewSet(16)
		s.Deserialize(data)
		db.sets[name] = s
	case "LFUCache":
		c := datastructures.NewLFUCache(0) // Capacity will be set in deserialize
		c.Deserialize(data)
		db.lfuCaches[name] = c
	case "HashTableChaining":
		ht := datastructures.NewHashTableChaining(16)
		ht.Deserialize(data)
		db.htChaining[name] = ht
	case "HashTableOpenAddr":
		ht := datastructures.NewHashTableOpenAddr(16)
		ht.Deserialize(data)
		db.htOpenAddr[name] = ht
	}
}

func (db *DBMS) SerializeAll() string {
	var sb strings.Builder
	for name, s := range db.arrays {
		sb.WriteString(fmt.Sprintf("Array %s %s\n", name, s.Serialize()))
	}
	for name, s := range db.singlyLists {
		sb.WriteString(fmt.Sprintf("SinglyLinkedList %s %s\n", name, s.Serialize()))
	}
	for name, s := range db.doublyLists {
		sb.WriteString(fmt.Sprintf("DoublyLinkedList %s %s\n", name, s.Serialize()))
	}
	for name, s := range db.stacks {
		sb.WriteString(fmt.Sprintf("Stack %s %s\n", name, s.Serialize()))
	}
	for name, s := range db.queues {
		sb.WriteString(fmt.Sprintf("Queue %s %s\n", name, s.Serialize()))
	}
	for name, s := range db.trees {
		sb.WriteString(fmt.Sprintf("FullBinaryTree %s %s\n", name, s.Serialize()))
	}
	for name, s := range db.sets {
		sb.WriteString(fmt.Sprintf("Set %s %s\n", name, s.Serialize()))
	}
	for name, s := range db.lfuCaches {
		sb.WriteString(fmt.Sprintf("LFUCache %s %s\n", name, s.Serialize()))
	}
	for name, s := range db.htChaining {
		sb.WriteString(fmt.Sprintf("HashTableChaining %s %s\n", name, s.Serialize()))
	}
	for name, s := range db.htOpenAddr {
		sb.WriteString(fmt.Sprintf("HashTableOpenAddr %s %s\n", name, s.Serialize()))
	}
	return sb.String()
}

func (db *DBMS) Execute(command []string) string {
	if len(command) == 0 {
		return "Error: Empty command."
	}
	cmd := command[0]
	if cmd == "PRINT" {
		return db.SerializeAll()
	}
	if cmd == "SEMPTY" {
		if len(command) < 2 {
			return "Error: SEMPTY requires a name."
		}
		if s, ok := db.stacks[command[1]]; ok {
			if s.Empty() {
				return "-> TRUE"
			}
			return "-> FALSE"
		}
		return "Error: Stack '" + command[1] + "' not found."
	}

	if cmd == "QEMPTY" {
		if len(command) < 2 {
			return "Error: QEMPTY requires a name."
		}
		if q, ok := db.queues[command[1]]; ok {
			if q.Empty() {
				return "-> TRUE"
			}
			return "-> FALSE"
		}
		return "Error: Queue '" + command[1] + "' not found."
	}

	if len(command) < 2 {
		return "Error: Command requires a name."
	}
	name := command[1]

	// Array
	if cmd == "MPUSH" {
		if len(command) < 3 {
			return "Error: MPUSH requires at least one value."
		}
		if _, ok := db.arrays[name]; !ok {
			db.arrays[name] = datastructures.NewArray()
		}
		for i := 2; i < len(command); i++ {
			db.arrays[name].PushBack(command[i])
		}
		return "-> OK"
	}
	if cmd == "MGET" {
		if len(command) < 3 {
			return "Error: MGET requires an index."
		}
		if arr, ok := db.arrays[name]; ok {
			idx, err := strconv.Atoi(command[2])
			if err != nil {
				return "Error: Invalid index."
			}
			val, err := arr.Get(idx)
			if err != nil {
				return "Error: " + err.Error()
			}
			return "-> " + val
		}
		return "Error: Array '" + name + "' not found."
	}
	if cmd == "MDEL" {
		if len(command) < 3 {
			return "Error: MDEL requires an index."
		}
		if arr, ok := db.arrays[name]; ok {
			idx, err := strconv.Atoi(command[2])
			if err != nil {
				return "Error: Invalid index."
			}
			err = arr.Remove(idx)
			if err != nil {
				return "Error: " + err.Error()
			}
			return "-> OK"
		}
		return "Error: Array '" + name + "' not found."
	}
	if cmd == "MINSERT" {
		if len(command) < 4 {
			return "Error: MINSERT requires an index and a value."
		}
		if arr, ok := db.arrays[name]; ok {
			idx, err := strconv.Atoi(command[2])
			if err != nil {
				return "Error: Invalid index."
			}
			err = arr.Insert(idx, command[3])
			if err != nil {
				return "Error: " + err.Error()
			}
			return "-> OK"
		}
		return "Error: Array '" + name + "' not found."
	}
	if cmd == "MSET" {
		if len(command) < 4 {
			return "Error: MSET requires an index and a value."
		}
		if arr, ok := db.arrays[name]; ok {
			idx, err := strconv.Atoi(command[2])
			if err != nil {
				return "Error: Invalid index."
			}
			err = arr.Set(idx, command[3])
			if err != nil {
				return "Error: " + err.Error()
			}
			return "-> OK"
		}
		return "Error: Array '" + name + "' not found."
	}

	// Stack
	if cmd == "SPUSH" {
		if len(command) < 3 {
			return "Error: SPUSH requires a value."
		}
		if _, ok := db.stacks[name]; !ok {
			db.stacks[name] = datastructures.NewStack()
		}
		db.stacks[name].Push(command[2])
		return "-> OK"
	}
	if cmd == "SPOP" {
		if s, ok := db.stacks[name]; ok {
			val, err := s.Pop()
			if err != nil {
				return "Error: " + err.Error()
			}
			return "-> " + val
		}
		return "Error: Stack '" + name + "' not found."
	}

	// Queue
	if cmd == "QPUSH" {
		if len(command) < 3 {
			return "Error: QPUSH requires a value."
		}
		if _, ok := db.queues[name]; !ok {
			db.queues[name] = datastructures.NewQueue()
		}
		db.queues[name].Push(command[2])
		return "-> OK"
	}
	if cmd == "QPOP" {
		if q, ok := db.queues[name]; ok {
			val, err := q.Pop()
			if err != nil {
				return "Error: " + err.Error()
			}
			return "-> " + val
		}
		return "Error: Queue '" + name + "' not found."
	}

	// SinglyLinkedList
	if cmd == "LPUSHFRONT" {
		if len(command) < 3 {
			return "Error: LPUSHFRONT requires a value."
		}
		if _, ok := db.singlyLists[name]; !ok {
			db.singlyLists[name] = datastructures.NewSinglyLinkedList()
		}
		db.singlyLists[name].PushFront(command[2])
		return "-> OK"
	}
	if cmd == "LPUSHBACK" {
		if len(command) < 3 {
			return "Error: LPUSHBACK requires a value."
		}
		if _, ok := db.singlyLists[name]; !ok {
			db.singlyLists[name] = datastructures.NewSinglyLinkedList()
		}
		db.singlyLists[name].PushBack(command[2])
		return "-> OK"
	}
	if cmd == "LPOPFRONT" {
		if l, ok := db.singlyLists[name]; ok {
			val, err := l.PopFront()
			if err != nil {
				return "Error: " + err.Error()
			}
			return "-> " + val
		}
		return "Error: List '" + name + "' not found."
	}
	if cmd == "LPOPBACK" {
		if l, ok := db.singlyLists[name]; ok {
			val, err := l.PopBack()
			if err != nil {
				return "Error: " + err.Error()
			}
			return "-> " + val
		}
		return "Error: List '" + name + "' not found."
	}
	if cmd == "LREMOVE" {
		if len(command) < 3 {
			return "Error: LREMOVE requires a value."
		}
		if l, ok := db.singlyLists[name]; ok {
			l.RemoveValue(command[2])
			return "-> OK"
		}
		return "Error: List '" + name + "' not found."
	}
	if cmd == "LFIND" {
		if len(command) < 3 {
			return "Error: LFIND requires a value."
		}
		if l, ok := db.singlyLists[name]; ok {
			if l.Find(command[2]) {
				return "-> TRUE"
			}
			return "-> FALSE"
		}
		return "Error: List '" + name + "' not found."
	}
	if cmd == "LINSERT_AFTER" {
		if len(command) < 4 {
			return "Error: LINSERT_AFTER requires target and new value."
		}
		if l, ok := db.singlyLists[name]; ok {
			l.InsertAfter(command[2], command[3])
			return "-> OK"
		}
		return "Error: List '" + name + "' not found."
	}
	if cmd == "LINSERT_BEFORE" {
		if len(command) < 4 {
			return "Error: LINSERT_BEFORE requires target and new value."
		}
		if l, ok := db.singlyLists[name]; ok {
			l.InsertBefore(command[2], command[3])
			return "-> OK"
		}
		return "Error: List '" + name + "' not found."
	}
	if cmd == "LREMOVE_AFTER" {
		if len(command) < 3 {
			return "Error: LREMOVE_AFTER requires a value."
		}
		if l, ok := db.singlyLists[name]; ok {
			l.RemoveAfter(command[2])
			return "-> OK"
		}
		return "Error: List '" + name + "' not found."
	}
	if cmd == "LREMOVE_BEFORE" {
		if len(command) < 3 {
			return "Error: LREMOVE_BEFORE requires a value."
		}
		if l, ok := db.singlyLists[name]; ok {
			l.RemoveBefore(command[2])
			return "-> OK"
		}
		return "Error: List '" + name + "' not found."
	}

	// DoublyLinkedList
	if cmd == "DLPUSHFRONT" {
		if len(command) < 3 {
			return "Error: DLPUSHFRONT requires a value."
		}
		if _, ok := db.doublyLists[name]; !ok {
			db.doublyLists[name] = datastructures.NewDoublyLinkedList()
		}
		db.doublyLists[name].PushFront(command[2])
		return "-> OK"
	}
	if cmd == "DLPUSHBACK" {
		if len(command) < 3 {
			return "Error: DLPUSHBACK requires a value."
		}
		if _, ok := db.doublyLists[name]; !ok {
			db.doublyLists[name] = datastructures.NewDoublyLinkedList()
		}
		db.doublyLists[name].PushBack(command[2])
		return "-> OK"
	}
	if cmd == "DLPOPFRONT" {
		if l, ok := db.doublyLists[name]; ok {
			val, err := l.PopFront()
			if err != nil {
				return "Error: " + err.Error()
			}
			return "-> " + val
		}
		return "Error: List '" + name + "' not found."
	}
	if cmd == "DLPOPBACK" {
		if l, ok := db.doublyLists[name]; ok {
			val, err := l.PopBack()
			if err != nil {
				return "Error: " + err.Error()
			}
			return "-> " + val
		}
		return "Error: List '" + name + "' not found."
	}
	if cmd == "DLREMOVE" {
		if len(command) < 3 {
			return "Error: DLREMOVE requires a value."
		}
		if l, ok := db.doublyLists[name]; ok {
			l.RemoveValue(command[2])
			return "-> OK"
		}
		return "Error: List '" + name + "' not found."
	}
	if cmd == "DLFIND" {
		if len(command) < 3 {
			return "Error: DLFIND requires a value."
		}
		if l, ok := db.doublyLists[name]; ok {
			if l.Find(command[2]) {
				return "-> TRUE"
			}
			return "-> FALSE"
		}
		return "Error: List '" + name + "' not found."
	}
	if cmd == "DLINSERT_AFTER" {
		if len(command) < 4 {
			return "Error: DLINSERT_AFTER requires target and new value."
		}
		if l, ok := db.doublyLists[name]; ok {
			l.InsertAfter(command[2], command[3])
			return "-> OK"
		}
		return "Error: List '" + name + "' not found."
	}
	if cmd == "DLINSERT_BEFORE" {
		if len(command) < 4 {
			return "Error: DLINSERT_BEFORE requires target and new value."
		}
		if l, ok := db.doublyLists[name]; ok {
			l.InsertBefore(command[2], command[3])
			return "-> OK"
		}
		return "Error: List '" + name + "' not found."
	}
	if cmd == "DLREMOVE_AFTER" {
		if len(command) < 3 {
			return "Error: DLREMOVE_AFTER requires a value."
		}
		if l, ok := db.doublyLists[name]; ok {
			l.RemoveAfter(command[2])
			return "-> OK"
		}
		return "Error: List '" + name + "' not found."
	}
	if cmd == "DLREMOVE_BEFORE" {
		if len(command) < 3 {
			return "Error: DLREMOVE_BEFORE requires a value."
		}
		if l, ok := db.doublyLists[name]; ok {
			l.RemoveBefore(command[2])
			return "-> OK"
		}
		return "Error: List '" + name + "' not found."
	}

	// HashTableChaining
	if cmd == "HPUT" {
		if len(command) < 4 {
			return "Error: HPUT requires key and value."
		}
		if _, ok := db.htChaining[name]; !ok {
			db.htChaining[name] = datastructures.NewHashTableChaining(16)
		}
		db.htChaining[name].Put(command[2], command[3])
		return "-> OK"
	}
	if cmd == "HGET" {
		if len(command) < 3 {
			return "Error: HGET requires a key."
		}
		if ht, ok := db.htChaining[name]; ok {
			val, err := ht.Get(command[2])
			if err != nil {
				return "Error: " + err.Error()
			}
			return "-> " + val
		}
		return "Error: HashTable '" + name + "' not found."
	}
	if cmd == "HDEL" {
		if len(command) < 3 {
			return "Error: HDEL requires a key."
		}
		if ht, ok := db.htChaining[name]; ok {
			ht.Remove(command[2])
			return "-> OK"
		}
		return "Error: HashTable '" + name + "' not found."
	}

	// HashTableOpenAddr
	if cmd == "OHPUT" {
		if len(command) < 4 {
			return "Error: OHPUT requires key and value."
		}
		if _, ok := db.htOpenAddr[name]; !ok {
			db.htOpenAddr[name] = datastructures.NewHashTableOpenAddr(16)
		}
		db.htOpenAddr[name].Put(command[2], command[3])
		return "-> OK"
	}
	if cmd == "OHGET" {
		if len(command) < 3 {
			return "Error: OHGET requires a key."
		}
		if ht, ok := db.htOpenAddr[name]; ok {
			val, err := ht.Get(command[2])
			if err != nil {
				return "Error: " + err.Error()
			}
			return "-> " + val
		}
		return "Error: HashTable '" + name + "' not found."
	}
	if cmd == "OHDEL" {
		if len(command) < 3 {
			return "Error: OHDEL requires a key."
		}
		if ht, ok := db.htOpenAddr[name]; ok {
			ht.Remove(command[2])
			return "-> OK"
		}
		return "Error: HashTable '" + name + "' not found."
	}

	// FullBinaryTree
	if cmd == "TINSERT" {
		if len(command) < 3 {
			return "Error: TINSERT requires a value."
		}
		if _, ok := db.trees[name]; !ok {
			db.trees[name] = datastructures.NewFullBinaryTree()
		}
		db.trees[name].Insert(command[2])
		return "-> OK"
	}
	if cmd == "TFIND" {
		if len(command) < 3 {
			return "Error: TFIND requires a value."
		}
		if t, ok := db.trees[name]; ok {
			if t.Find(command[2]) {
				return "-> TRUE"
			}
			return "-> FALSE"
		}
		return "Error: Tree '" + name + "' not found."
	}
	if cmd == "TISFULL" {
		if t, ok := db.trees[name]; ok {
			if t.IsFull() {
				return "-> TRUE"
			}
			return "-> FALSE"
		}
		return "Error: Tree '" + name + "' not found."
	}

	// Set
	if cmd == "SADD" {
		if len(command) < 3 {
			return "Error: SADD requires a value."
		}
		if _, ok := db.sets[name]; !ok {
			db.sets[name] = datastructures.NewSet(16)
		}
		db.sets[name].Add(command[2])
		return "-> OK"
	}
	if cmd == "SREM" {
		if len(command) < 3 {
			return "Error: SREM requires a value."
		}
		if s, ok := db.sets[name]; ok {
			s.Remove(command[2])
			return "-> OK"
		}
		return "Error: Set '" + name + "' not found."
	}
	if cmd == "SISMEMBER" {
		if len(command) < 3 {
			return "Error: SISMEMBER requires a value."
		}
		if s, ok := db.sets[name]; ok {
			if s.Contains(command[2]) {
				return "-> TRUE"
			}
			return "-> FALSE"
		}
		return "Error: Set '" + name + "' not found."
	}

	// LFUCache
	if cmd == "CPUT" {
		if len(command) < 4 {
			return "Error: CPUT requires key and value."
		}
		if _, ok := db.lfuCaches[name]; !ok {
			// Default capacity 3 if not exists? Or error?
			// C++ code doesn't show auto-creation with capacity.
			// Assuming user should create it or default 3.
			db.lfuCaches[name] = datastructures.NewLFUCache(3)
		}
		db.lfuCaches[name].Set(command[2], command[3])
		return "-> OK"
	}
	if cmd == "CGET" {
		if len(command) < 3 {
			return "Error: CGET requires a key."
		}
		if c, ok := db.lfuCaches[name]; ok {
			val := c.Get(command[2])
			if val == "" {
				return "Error: Key not found"
			}
			return "-> " + val
		}
		return "Error: Cache '" + name + "' not found."
	}

	// Tasks
	if cmd == "ASTEROIDS" {
		if arr, ok := db.arrays[name]; ok {
			res := solveAsteroids(arr)
			return "-> " + res.Serialize()
		}
		return "Error: Array '" + name + "' not found."
	}
	if cmd == "MINPARTITION" {
		if len(command) < 4 {
			return "Error: MINPARTITION requires input set and two output sets."
		}
		inputName := name
		s1Name := command[2]
		s2Name := command[3]
		if input, ok := db.sets[inputName]; ok {
			if _, ok := db.sets[s1Name]; !ok {
				db.sets[s1Name] = datastructures.NewSet(16)
			}
			if _, ok := db.sets[s2Name]; !ok {
				db.sets[s2Name] = datastructures.NewSet(16)
			}
			diff := solveMinPartition(input, db.sets[s1Name], db.sets[s2Name])
			return "-> " + diff
		}
		return "Error: Set '" + inputName + "' not found."
	}
	if cmd == "FINDSUM" {
		if len(command) < 4 {
			return "Error: FINDSUM requires target and output array."
		}
		target, err := strconv.Atoi(command[2])
		if err != nil {
			return "Error: Invalid target."
		}
		outputName := command[3]
		if input, ok := db.arrays[name]; ok {
			if _, ok := db.arrays[outputName]; !ok {
				db.arrays[outputName] = datastructures.NewArray()
			}
			found := solveFindSum(input, target, db.arrays[outputName])
			if found {
				return "-> TRUE"
			}
			return "-> FALSE"
		}
		return "Error: Array '" + name + "' not found."
	}
	if cmd == "LONGESTSUBSTR" {
		// name is the string itself? No, command is LONGESTSUBSTR <string>
		// Wait, C++ code: solveLongestSubstring(const std::string& s)
		// DBMS.cpp:
		// if (cmd == "LONGESTSUBSTR") { if (command.size() < 2) ... return "-> " + std::to_string(solveLongestSubstring(command[1])); }
		// So name is the string.
		return "-> " + strconv.Itoa(solveLongestSubstring(name))
	}

	return "Error: Unknown command."
}
