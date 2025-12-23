package dbms

import (
	"os"
	"testing"
)

func TestTasks_Coverage(t *testing.T) {
	db := NewDBMS()

	// Asteroids coverage
	// 1. Non-int input
	db.Execute([]string{"MPUSH", "ast_bad", "10", "invalid", "-5"})
	res := db.Execute([]string{"ASTEROIDS", "ast_bad"})
	// Should process 10 and -5. 10 > 5, so 10 becomes 5. Result: 5.
	if res != "-> 5" {
		t.Errorf("Asteroids non-int failed: %s", res)
	}

	// 2. Collision Top > Current
	db.Execute([]string{"MPUSH", "ast_1", "10", "-5"})
	if db.Execute([]string{"ASTEROIDS", "ast_1"}) != "-> 5" {
		t.Error("Asteroids Top > Current failed")
	}

	// 3. Collision Top < Current
	db.Execute([]string{"MPUSH", "ast_2", "5", "-10"})
	if db.Execute([]string{"ASTEROIDS", "ast_2"}) != "-> -5" {
		t.Error("Asteroids Top < Current failed")
	}

	// 4. Collision Equal
	db.Execute([]string{"MPUSH", "ast_3", "5", "-5"})
	if db.Execute([]string{"ASTEROIDS", "ast_3"}) != "-> " { // Empty result
		t.Error("Asteroids Equal failed")
	}

	// 5. No collision (Left then Right)
	db.Execute([]string{"MPUSH", "ast_4", "-5", "10"})
	if db.Execute([]string{"ASTEROIDS", "ast_4"}) != "-> -5 10" {
		t.Error("Asteroids No Collision failed")
	}

	// 6. Top < 0 (Top moving left)
	db.Execute([]string{"MPUSH", "ast_5", "-5", "-10"})
	if db.Execute([]string{"ASTEROIDS", "ast_5"}) != "-> -5 -10" {
		t.Error("Asteroids Top < 0 failed")
	}

	// Longest Substring coverage
	if db.Execute([]string{"LONGESTSUBSTR", ""}) != "-> 0" {
		t.Error("Longest Substring empty failed")
	}
	if db.Execute([]string{"LONGESTSUBSTR", "abcabcbb"}) != "-> 3" {
		t.Error("Longest Substring abcabcbb failed")
	}
	if db.Execute([]string{"LONGESTSUBSTR", "bbbbb"}) != "-> 1" {
		t.Error("Longest Substring bbbbb failed")
	}
	if db.Execute([]string{"LONGESTSUBSTR", "pwwkew"}) != "-> 3" {
		t.Error("Longest Substring pwwkew failed")
	}
}

func TestDBMS_UnknownCommand(t *testing.T) {
	db := NewDBMS()
	res := db.Execute([]string{"UNKNOWN_CMD", "name"})
	if res != "Error: Unknown command." {
		t.Errorf("Unknown command failed, got: %q", res)
	}
}

func TestDBMS_Execute_Errors_Extended(t *testing.T) {
	db := NewDBMS()

	// SADD
	if db.Execute([]string{"SADD", "s"}) != "Error: SADD requires a value." {
		t.Error("SADD error failed")
	}

	// SREM
	if db.Execute([]string{"SREM", "s"}) != "Error: SREM requires a value." {
		t.Error("SREM error failed")
	}
	if db.Execute([]string{"SREM", "s", "v"}) != "Error: Set 's' not found." {
		t.Error("SREM not found failed")
	}

	// SISMEMBER
	if db.Execute([]string{"SISMEMBER", "s"}) != "Error: SISMEMBER requires a value." {
		t.Error("SISMEMBER error failed")
	}
	if db.Execute([]string{"SISMEMBER", "s", "v"}) != "Error: Set 's' not found." {
		t.Error("SISMEMBER not found failed")
	}

	// CPUT
	if db.Execute([]string{"CPUT", "c", "k"}) != "Error: CPUT requires key and value." {
		t.Error("CPUT error failed")
	}

	// CGET
	if db.Execute([]string{"CGET", "c"}) != "Error: CGET requires a key." {
		t.Error("CGET error failed")
	}
	if db.Execute([]string{"CGET", "c", "k"}) != "Error: Cache 'c' not found." {
		t.Error("CGET not found failed")
	}

	// MINPARTITION
	if db.Execute([]string{"MINPARTITION", "in", "out1"}) != "Error: MINPARTITION requires input set and two output sets." {
		t.Error("MINPARTITION error failed")
	}
	if db.Execute([]string{"MINPARTITION", "in", "out1", "out2"}) != "Error: Set 'in' not found." {
		t.Error("MINPARTITION not found failed")
	}

	// FINDSUM
	if db.Execute([]string{"FINDSUM", "arr", "10"}) != "Error: FINDSUM requires target and output array." {
		t.Error("FINDSUM error failed")
	}
	if db.Execute([]string{"FINDSUM", "arr", "bad", "out"}) != "Error: Invalid target." {
		t.Error("FINDSUM invalid target failed")
	}
	if db.Execute([]string{"FINDSUM", "arr", "10", "out"}) != "Error: Array 'arr' not found." {
		t.Error("FINDSUM not found failed")
	}

	// ASTEROIDS
	if db.Execute([]string{"ASTEROIDS", "arr"}) != "Error: Array 'arr' not found." {
		t.Error("ASTEROIDS not found failed")
	}

	// QEMPTY
	if db.Execute([]string{"QEMPTY"}) != "Error: QEMPTY requires a name." {
		t.Error("QEMPTY error failed")
	}
	if db.Execute([]string{"QEMPTY", "q"}) != "Error: Queue 'q' not found." {
		t.Error("QEMPTY not found failed")
	}
}

func TestDBMS_Execute_MoreErrors(t *testing.T) {
	db := NewDBMS()

	// Array
	if db.Execute([]string{"MPUSH", "arr"}) != "Error: MPUSH requires at least one value." {
		t.Error("MPUSH error")
	}
	if db.Execute([]string{"MGET", "arr"}) != "Error: MGET requires an index." {
		t.Error("MGET error")
	}
	if db.Execute([]string{"MGET", "arr", "0"}) != "Error: Array 'arr' not found." {
		t.Error("MGET not found")
	}
	if db.Execute([]string{"MDEL", "arr"}) != "Error: MDEL requires an index." {
		t.Error("MDEL error")
	}
	if db.Execute([]string{"MDEL", "arr", "0"}) != "Error: Array 'arr' not found." {
		t.Error("MDEL not found")
	}
	if db.Execute([]string{"MSET", "arr"}) != "Error: MSET requires an index and a value." {
		t.Error("MSET error")
	}
	if db.Execute([]string{"MSET", "arr", "0", "v"}) != "Error: Array 'arr' not found." {
		t.Error("MSET not found")
	}
	if db.Execute([]string{"MINSERT", "arr"}) != "Error: MINSERT requires an index and a value." {
		t.Error("MINSERT error")
	}
	if db.Execute([]string{"MINSERT", "arr", "0", "v"}) != "Error: Array 'arr' not found." {
		t.Error("MINSERT not found")
	}

	// SinglyLinkedList
	if db.Execute([]string{"LPUSHFRONT", "l"}) != "Error: LPUSHFRONT requires a value." {
		t.Error("LPUSHFRONT error")
	}
	if db.Execute([]string{"LPUSHBACK", "l"}) != "Error: LPUSHBACK requires a value." {
		t.Error("LPUSHBACK error")
	}
	if db.Execute([]string{"LPOPFRONT", "l"}) != "Error: List 'l' not found." {
		t.Error("LPOPFRONT not found")
	}
	if db.Execute([]string{"LPOPBACK", "l"}) != "Error: List 'l' not found." {
		t.Error("LPOPBACK not found")
	}
	if db.Execute([]string{"LREMOVE", "l"}) != "Error: LREMOVE requires a value." {
		t.Error("LREMOVE error")
	}
	if db.Execute([]string{"LREMOVE", "l", "v"}) != "Error: List 'l' not found." {
		t.Error("LREMOVE not found")
	}
	if db.Execute([]string{"LFIND", "l"}) != "Error: LFIND requires a value." {
		t.Error("LFIND error")
	}
	if db.Execute([]string{"LFIND", "l", "v"}) != "Error: List 'l' not found." {
		t.Error("LFIND not found")
	}
	if db.Execute([]string{"LINSERT_AFTER", "l"}) != "Error: LINSERT_AFTER requires target and new value." {
		t.Error("LINSERT_AFTER error")
	}
	if db.Execute([]string{"LINSERT_AFTER", "l", "t", "v"}) != "Error: List 'l' not found." {
		t.Error("LINSERT_AFTER not found")
	}
	if db.Execute([]string{"LINSERT_BEFORE", "l"}) != "Error: LINSERT_BEFORE requires target and new value." {
		t.Error("LINSERT_BEFORE error")
	}
	if db.Execute([]string{"LINSERT_BEFORE", "l", "t", "v"}) != "Error: List 'l' not found." {
		t.Error("LINSERT_BEFORE not found")
	}
	if db.Execute([]string{"LREMOVE_AFTER", "l"}) != "Error: LREMOVE_AFTER requires a value." {
		t.Error("LREMOVE_AFTER error")
	}
	if db.Execute([]string{"LREMOVE_AFTER", "l", "v"}) != "Error: List 'l' not found." {
		t.Error("LREMOVE_AFTER not found")
	}
	if db.Execute([]string{"LREMOVE_BEFORE", "l"}) != "Error: LREMOVE_BEFORE requires a value." {
		t.Error("LREMOVE_BEFORE error")
	}
	if db.Execute([]string{"LREMOVE_BEFORE", "l", "v"}) != "Error: List 'l' not found." {
		t.Error("LREMOVE_BEFORE not found")
	}

	// DoublyLinkedList
	if db.Execute([]string{"DLPUSHFRONT", "dl"}) != "Error: DLPUSHFRONT requires a value." {
		t.Error("DLPUSHFRONT error")
	}
	if db.Execute([]string{"DLPUSHBACK", "dl"}) != "Error: DLPUSHBACK requires a value." {
		t.Error("DLPUSHBACK error")
	}
	if db.Execute([]string{"DLPOPFRONT", "dl"}) != "Error: List 'dl' not found." {
		t.Error("DLPOPFRONT not found")
	}
	if db.Execute([]string{"DLPOPBACK", "dl"}) != "Error: List 'dl' not found." {
		t.Error("DLPOPBACK not found")
	}
	if db.Execute([]string{"DLREMOVE", "dl"}) != "Error: DLREMOVE requires a value." {
		t.Error("DLREMOVE error")
	}
	if db.Execute([]string{"DLREMOVE", "dl", "v"}) != "Error: List 'dl' not found." {
		t.Error("DLREMOVE not found")
	}
	if db.Execute([]string{"DLFIND", "dl"}) != "Error: DLFIND requires a value." {
		t.Error("DLFIND error")
	}
	if db.Execute([]string{"DLFIND", "dl", "v"}) != "Error: List 'dl' not found." {
		t.Error("DLFIND not found")
	}
	if db.Execute([]string{"DLINSERT_AFTER", "dl"}) != "Error: DLINSERT_AFTER requires target and new value." {
		t.Error("DLINSERT_AFTER error")
	}
	if db.Execute([]string{"DLINSERT_AFTER", "dl", "t", "v"}) != "Error: List 'dl' not found." {
		t.Error("DLINSERT_AFTER not found")
	}
	if db.Execute([]string{"DLINSERT_BEFORE", "dl"}) != "Error: DLINSERT_BEFORE requires target and new value." {
		t.Error("DLINSERT_BEFORE error")
	}
	if db.Execute([]string{"DLINSERT_BEFORE", "dl", "t", "v"}) != "Error: List 'dl' not found." {
		t.Error("DLINSERT_BEFORE not found")
	}
	if db.Execute([]string{"DLREMOVE_AFTER", "dl"}) != "Error: DLREMOVE_AFTER requires a value." {
		t.Error("DLREMOVE_AFTER error")
	}
	if db.Execute([]string{"DLREMOVE_AFTER", "dl", "v"}) != "Error: List 'dl' not found." {
		t.Error("DLREMOVE_AFTER not found")
	}
	if db.Execute([]string{"DLREMOVE_BEFORE", "dl"}) != "Error: DLREMOVE_BEFORE requires a value." {
		t.Error("DLREMOVE_BEFORE error")
	}
	if db.Execute([]string{"DLREMOVE_BEFORE", "dl", "v"}) != "Error: List 'dl' not found." {
		t.Error("DLREMOVE_BEFORE not found")
	}

	// Stack
	if db.Execute([]string{"SPUSH", "s"}) != "Error: SPUSH requires a value." {
		t.Error("SPUSH error")
	}
	if db.Execute([]string{"SPOP", "s"}) != "Error: Stack 's' not found." {
		t.Error("SPOP not found")
	}
	if db.Execute([]string{"SEMPTY", "s"}) != "Error: Stack 's' not found." {
		t.Error("SEMPTY not found")
	}

	// Queue
	if db.Execute([]string{"QPUSH", "q"}) != "Error: QPUSH requires a value." {
		t.Error("QPUSH error")
	}
	if db.Execute([]string{"QPOP", "q"}) != "Error: Queue 'q' not found." {
		t.Error("QPOP not found")
	}

	// Tree
	if db.Execute([]string{"TINSERT", "t"}) != "Error: TINSERT requires a value." {
		t.Error("TINSERT error")
	}
	if db.Execute([]string{"TFIND", "t"}) != "Error: TFIND requires a value." {
		t.Error("TFIND error")
	}
	if db.Execute([]string{"TFIND", "t", "v"}) != "Error: Tree 't' not found." {
		t.Error("TFIND not found")
	}
	if db.Execute([]string{"TISFULL", "t"}) != "Error: Tree 't' not found." {
		t.Error("TISFULL not found")
	}
}

func TestDBMS_Execute_Success(t *testing.T) {
	db := NewDBMS()

	// PRINT
	if db.Execute([]string{"PRINT"}) != "" {
		t.Error("PRINT empty failed")
	}

	// SEMPTY
	db.Execute([]string{"SPUSH", "s", "1"})
	if db.Execute([]string{"SEMPTY", "s"}) != "-> FALSE" {
		t.Error("SEMPTY false failed")
	}
	db.Execute([]string{"SPOP", "s"})
	if db.Execute([]string{"SEMPTY", "s"}) != "-> TRUE" {
		t.Error("SEMPTY true failed")
	}
}

func TestDBMS_Execute_Coverage_Final(t *testing.T) {
	db := NewDBMS()

	// MINPARTITION with existing output sets
	db.Execute([]string{"SADD", "in", "1"})
	db.Execute([]string{"SADD", "out1", "2"})
	db.Execute([]string{"SADD", "out2", "3"})
	// MINPARTITION returns diff string
	if db.Execute([]string{"MINPARTITION", "in", "out1", "out2"}) == "" {
		t.Error("MINPARTITION existing failed")
	}

	// FINDSUM with existing output array
	db.Execute([]string{"MPUSH", "arr", "1"})
	db.Execute([]string{"MPUSH", "out", "2"})
	if db.Execute([]string{"FINDSUM", "arr", "1", "out"}) == "" {
		t.Error("FINDSUM existing failed")
	}
}

func TestStorageManager_Malformed(t *testing.T) {
	// Create file with malformed lines
	f, _ := os.Create("malformed.txt")
	f.WriteString("BAD_LINE\n")
	f.WriteString("Array arr\n")
	f.Close()
	defer os.Remove("malformed.txt")

	db := NewDBMS()
	sm := NewStorageManager("malformed.txt")
	sm.Load(db)
	// Should not crash.
	// Check if arr exists and is empty?
	// LoadStructure("Array", "arr", "") -> creates empty array.
	if db.Execute([]string{"MGET", "arr", "0"}) == "Error: Array 'arr' not found." {
		t.Error("Array should exist")
	}
}

func TestDBMS_Coverage(t *testing.T) {
	db := NewDBMS()

	// LoadStructure unknown type
	db.LoadStructure("UNKNOWN", "name", "data")
	// Should just log or ignore, ensure no panic

	// StorageManager Load file not exist
	sm := NewStorageManager("non_existent_file.txt")
	if err := sm.Load(db); err != nil {
		t.Error("Load non-existent file should not error")
	}

	// StorageManager Save error
	// Create a directory with same name
	os.Mkdir("bad_path", 0755)
	defer os.Remove("bad_path")
	smBad := NewStorageManager("bad_path")
	if err := smBad.Save(db); err == nil {
		t.Error("Save to directory should error")
	}
}
