package dbms

import (
	"testing"
)

func TestDBMS_Execute_MoreErrors_2(t *testing.T) {
	db := NewDBMS()

	// Empty command
	if db.Execute([]string{}) != "Error: Empty command." {
		t.Error("Empty command failed")
	}

	// PRINT
	db.Execute([]string{"SPUSH", "s", "v"})
	if db.Execute([]string{"PRINT"}) == "" {
		t.Error("PRINT returned empty string")
	}

	// SEMPTY
	if db.Execute([]string{"SEMPTY"}) != "Error: SEMPTY requires a name." {
		t.Error("SEMPTY args failed")
	}
	if db.Execute([]string{"SEMPTY", "missing"}) != "Error: Stack 'missing' not found." {
		t.Error("SEMPTY not found failed")
	}

	// QEMPTY
	if db.Execute([]string{"QEMPTY"}) != "Error: QEMPTY requires a name." {
		t.Error("QEMPTY args failed")
	}
	if db.Execute([]string{"QEMPTY", "missing"}) != "Error: Queue 'missing' not found." {
		t.Error("QEMPTY not found failed")
	}

	// Generic "Command requires a name"
	if db.Execute([]string{"MPUSH"}) != "Error: Command requires a name." {
		t.Error("Generic name check failed")
	}

	// MPUSH
	if db.Execute([]string{"MPUSH", "arr"}) != "Error: MPUSH requires at least one value." {
		t.Error("MPUSH args failed")
	}

	// MGET
	if db.Execute([]string{"MGET", "arr"}) != "Error: MGET requires an index." {
		t.Error("MGET args failed")
	}
	if db.Execute([]string{"MGET", "arr", "0"}) != "Error: Array 'arr' not found." {
		t.Error("MGET not found failed")
	}
	db.Execute([]string{"MPUSH", "arr", "val"})
	if db.Execute([]string{"MGET", "arr", "invalid"}) != "Error: Invalid index." {
		t.Error("MGET invalid index failed")
	}

	// MDEL
	if db.Execute([]string{"MDEL", "arr"}) != "Error: MDEL requires an index." {
		t.Error("MDEL args failed")
	}
	if db.Execute([]string{"MDEL", "missing", "0"}) != "Error: Array 'missing' not found." {
		t.Error("MDEL not found failed")
	}
	if db.Execute([]string{"MDEL", "arr", "invalid"}) != "Error: Invalid index." {
		t.Error("MDEL invalid index failed")
	}

	// MINSERT
	if db.Execute([]string{"MINSERT", "arr", "0"}) != "Error: MINSERT requires an index and a value." {
		t.Error("MINSERT args failed")
	}
	if db.Execute([]string{"MINSERT", "missing", "0", "v"}) != "Error: Array 'missing' not found." {
		t.Error("MINSERT not found failed")
	}
	if db.Execute([]string{"MINSERT", "arr", "invalid", "v"}) != "Error: Invalid index." {
		t.Error("MINSERT invalid index failed")
	}

	// MSET
	if db.Execute([]string{"MSET", "arr", "0"}) != "Error: MSET requires an index and a value." {
		t.Error("MSET args failed")
	}
	if db.Execute([]string{"MSET", "missing", "0", "v"}) != "Error: Array 'missing' not found." {
		t.Error("MSET not found failed")
	}
	if db.Execute([]string{"MSET", "arr", "invalid", "v"}) != "Error: Invalid index." {
		t.Error("MSET invalid index failed")
	}

	// SPUSH
	if db.Execute([]string{"SPUSH", "s"}) != "Error: SPUSH requires a value." {
		t.Error("SPUSH args failed")
	}

	// SPOP
	if db.Execute([]string{"SPOP", "missing"}) != "Error: Stack 'missing' not found." {
		t.Error("SPOP not found failed")
	}

	// QPUSH
	if db.Execute([]string{"QPUSH", "q"}) != "Error: QPUSH requires a value." {
		t.Error("QPUSH args failed")
	}

	// QPOP
	if db.Execute([]string{"QPOP", "missing"}) != "Error: Queue 'missing' not found." {
		t.Error("QPOP not found failed")
	}

	// LPUSHFRONT
	if db.Execute([]string{"LPUSHFRONT", "l"}) != "Error: LPUSHFRONT requires a value." {
		t.Error("LPUSHFRONT args failed")
	}

	// LPUSHBACK
	if db.Execute([]string{"LPUSHBACK", "l"}) != "Error: LPUSHBACK requires a value." {
		t.Error("LPUSHBACK args failed")
	}

	// LPOPFRONT
	if db.Execute([]string{"LPOPFRONT", "missing"}) != "Error: List 'missing' not found." {
		t.Error("LPOPFRONT not found failed")
	}

	// LPOPBACK
	if db.Execute([]string{"LPOPBACK", "missing"}) != "Error: List 'missing' not found." {
		t.Error("LPOPBACK not found failed")
	}

	// LREMOVE
	if db.Execute([]string{"LREMOVE", "l"}) != "Error: LREMOVE requires a value." {
		t.Error("LREMOVE args failed")
	}
	if db.Execute([]string{"LREMOVE", "missing", "v"}) != "Error: List 'missing' not found." {
		t.Error("LREMOVE not found failed")
	}

	// LFIND
	if db.Execute([]string{"LFIND", "l"}) != "Error: LFIND requires a value." {
		t.Error("LFIND args failed")
	}
	if db.Execute([]string{"LFIND", "missing", "v"}) != "Error: List 'missing' not found." {
		t.Error("LFIND not found failed")
	}

	// LINSERT_AFTER
	if db.Execute([]string{"LINSERT_AFTER", "l", "t"}) != "Error: LINSERT_AFTER requires target and new value." {
		t.Error("LINSERT_AFTER args failed")
	}
	if db.Execute([]string{"LINSERT_AFTER", "missing", "t", "n"}) != "Error: List 'missing' not found." {
		t.Error("LINSERT_AFTER not found failed")
	}

	// LINSERT_BEFORE
	if db.Execute([]string{"LINSERT_BEFORE", "l", "t"}) != "Error: LINSERT_BEFORE requires target and new value." {
		t.Error("LINSERT_BEFORE args failed")
	}
	if db.Execute([]string{"LINSERT_BEFORE", "missing", "t", "n"}) != "Error: List 'missing' not found." {
		t.Error("LINSERT_BEFORE not found failed")
	}

	// LREMOVE_AFTER
	if db.Execute([]string{"LREMOVE_AFTER", "l"}) != "Error: LREMOVE_AFTER requires a value." {
		t.Error("LREMOVE_AFTER args failed")
	}
	if db.Execute([]string{"LREMOVE_AFTER", "missing", "v"}) != "Error: List 'missing' not found." {
		t.Error("LREMOVE_AFTER not found failed")
	}

	// HashTableChaining
	if db.Execute([]string{"HPUT", "h"}) != "Error: HPUT requires key and value." {
		t.Error("HPUT args failed")
	}
	if db.Execute([]string{"HGET", "h"}) != "Error: HGET requires a key." {
		t.Error("HGET args failed")
	}
	if db.Execute([]string{"HGET", "missing", "k"}) != "Error: HashTable 'missing' not found." {
		t.Error("HGET not found failed")
	}
	if db.Execute([]string{"HDEL", "h"}) != "Error: HDEL requires a key." {
		t.Error("HDEL args failed")
	}
	if db.Execute([]string{"HDEL", "missing", "k"}) != "Error: HashTable 'missing' not found." {
		t.Error("HDEL not found failed")
	}

	// HashTableOpenAddr
	if db.Execute([]string{"OHPUT", "h"}) != "Error: OHPUT requires key and value." {
		t.Error("OHPUT args failed")
	}
	if db.Execute([]string{"OHGET", "h"}) != "Error: OHGET requires a key." {
		t.Error("OHGET args failed")
	}
	if db.Execute([]string{"OHGET", "missing", "k"}) != "Error: HashTable 'missing' not found." {
		t.Error("OHGET not found failed")
	}
	if db.Execute([]string{"OHDEL", "h"}) != "Error: OHDEL requires a key." {
		t.Error("OHDEL args failed")
	}
	if db.Execute([]string{"OHDEL", "missing", "k"}) != "Error: HashTable 'missing' not found." {
		t.Error("OHDEL not found failed")
	}

	// FullBinaryTree
	if db.Execute([]string{"TINSERT", "t"}) != "Error: TINSERT requires a value." {
		t.Error("TINSERT args failed")
	}
	if db.Execute([]string{"TFIND", "t"}) != "Error: TFIND requires a value." {
		t.Error("TFIND args failed")
	}
	if db.Execute([]string{"TFIND", "missing", "v"}) != "Error: Tree 'missing' not found." {
		t.Error("TFIND not found failed")
	}
	if db.Execute([]string{"TISFULL", "missing"}) != "Error: Tree 'missing' not found." {
		t.Error("TISFULL not found failed")
	}
}

func TestDBMS_Execute_AllSuccess(t *testing.T) {
	db := NewDBMS()

	// Array
	if db.Execute([]string{"MPUSH", "arr", "1"}) != "-> OK" {
		t.Error("MPUSH failed")
	}
	if db.Execute([]string{"MGET", "arr", "0"}) != "-> 1" {
		t.Error("MGET failed")
	}
	if db.Execute([]string{"MSET", "arr", "0", "2"}) != "-> OK" {
		t.Error("MSET failed")
	}
	if db.Execute([]string{"MINSERT", "arr", "0", "1"}) != "-> OK" {
		t.Error("MINSERT failed")
	}
	if db.Execute([]string{"MDEL", "arr", "0"}) != "-> OK" {
		t.Error("MDEL failed")
	}

	// Stack
	if db.Execute([]string{"SPUSH", "s", "1"}) != "-> OK" {
		t.Error("SPUSH failed")
	}
	if db.Execute([]string{"SPOP", "s"}) != "-> 1" {
		t.Error("SPOP failed")
	}

	// Queue
	if db.Execute([]string{"QPUSH", "q", "1"}) != "-> OK" {
		t.Error("QPUSH failed")
	}
	if db.Execute([]string{"QPOP", "q"}) != "-> 1" {
		t.Error("QPOP failed")
	}

	// SinglyLinkedList
	if db.Execute([]string{"LPUSHFRONT", "l", "1"}) != "-> OK" {
		t.Error("LPUSHFRONT failed")
	}
	if db.Execute([]string{"LPUSHBACK", "l", "2"}) != "-> OK" {
		t.Error("LPUSHBACK failed")
	}
	if db.Execute([]string{"LPOPFRONT", "l"}) != "-> 1" {
		t.Error("LPOPFRONT failed")
	}
	if db.Execute([]string{"LPOPBACK", "l"}) != "-> 2" {
		t.Error("LPOPBACK failed")
	}
	db.Execute([]string{"LPUSHFRONT", "l", "1"})
	if db.Execute([]string{"LFIND", "l", "1"}) != "-> TRUE" {
		t.Error("LFIND failed")
	}
	if db.Execute([]string{"LINSERT_AFTER", "l", "1", "2"}) != "-> OK" {
		t.Error("LINSERT_AFTER failed")
	}
	if db.Execute([]string{"LINSERT_BEFORE", "l", "2", "1.5"}) != "-> OK" {
		t.Error("LINSERT_BEFORE failed")
	}
	if db.Execute([]string{"LREMOVE_AFTER", "l", "1"}) != "-> OK" {
		t.Error("LREMOVE_AFTER failed")
	}
	if db.Execute([]string{"LREMOVE", "l", "1"}) != "-> OK" {
		t.Error("LREMOVE failed")
	}

	// DoublyLinkedList
	if db.Execute([]string{"DLPUSHFRONT", "dl", "1"}) != "-> OK" {
		t.Error("DLPUSHFRONT failed")
	}
	if db.Execute([]string{"DLPUSHBACK", "dl", "2"}) != "-> OK" {
		t.Error("DLPUSHBACK failed")
	}
	if db.Execute([]string{"DLPOPFRONT", "dl"}) != "-> 1" {
		t.Error("DLPOPFRONT failed")
	}
	if db.Execute([]string{"DLPOPBACK", "dl"}) != "-> 2" {
		t.Error("DLPOPBACK failed")
	}
	db.Execute([]string{"DLPUSHFRONT", "dl", "1"})
	if db.Execute([]string{"DLFIND", "dl", "1"}) != "-> TRUE" {
		t.Error("DLFIND failed")
	}
	if db.Execute([]string{"DLINSERT_AFTER", "dl", "1", "2"}) != "-> OK" {
		t.Error("DLINSERT_AFTER failed")
	}
	if db.Execute([]string{"DLINSERT_BEFORE", "dl", "2", "1.5"}) != "-> OK" {
		t.Error("DLINSERT_BEFORE failed")
	}
	if db.Execute([]string{"DLREMOVE_AFTER", "dl", "1"}) != "-> OK" {
		t.Error("DLREMOVE_AFTER failed")
	}
	if db.Execute([]string{"DLREMOVE_BEFORE", "dl", "2"}) != "-> OK" {
		t.Error("DLREMOVE_BEFORE failed")
	}
	if db.Execute([]string{"DLREMOVE", "dl", "1"}) != "-> OK" {
		t.Error("DLREMOVE failed")
	}

	// HashTableChaining
	if db.Execute([]string{"HPUT", "h", "k", "v"}) != "-> OK" {
		t.Error("HPUT failed")
	}
	if db.Execute([]string{"HGET", "h", "k"}) != "-> v" {
		t.Error("HGET failed")
	}
	if db.Execute([]string{"HDEL", "h", "k"}) != "-> OK" {
		t.Error("HDEL failed")
	}

	// HashTableOpenAddr
	if db.Execute([]string{"OHPUT", "oh", "k", "v"}) != "-> OK" {
		t.Error("OHPUT failed")
	}
	if db.Execute([]string{"OHGET", "oh", "k"}) != "-> v" {
		t.Error("OHGET failed")
	}
	if db.Execute([]string{"OHDEL", "oh", "k"}) != "-> OK" {
		t.Error("OHDEL failed")
	}

	// FullBinaryTree
	if db.Execute([]string{"TINSERT", "t", "1"}) != "-> OK" {
		t.Error("TINSERT failed")
	}
	if db.Execute([]string{"TFIND", "t", "1"}) != "-> TRUE" {
		t.Error("TFIND failed")
	}
	if db.Execute([]string{"TISFULL", "t"}) != "-> TRUE" {
		t.Error("TISFULL failed")
	}

	// Set
	if db.Execute([]string{"SADD", "set", "1"}) != "-> OK" {
		t.Error("SADD failed")
	}
	if db.Execute([]string{"SISMEMBER", "set", "1"}) != "-> TRUE" {
		t.Error("SISMEMBER failed")
	}
	if db.Execute([]string{"SREM", "set", "1"}) != "-> OK" {
		t.Error("SREM failed")
	}

	// LFUCache
	if db.Execute([]string{"CPUT", "c", "k", "v"}) != "-> OK" {
		t.Error("CPUT failed")
	}
	if db.Execute([]string{"CGET", "c", "k"}) != "-> v" {
		t.Error("CGET failed")
	}
}
