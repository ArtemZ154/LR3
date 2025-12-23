package datastructures

import (
	"strings"
	"testing"
)

func TestArray_Coverage(t *testing.T) {
	arr := NewArray()
	// Insert at 0 when empty
	arr.Insert(0, "a")
	// Insert at end
	arr.Insert(1, "b")
	// Insert in middle
	arr.Insert(1, "c") // a, c, b

	if val, _ := arr.Get(1); val != "c" {
		t.Error("Insert middle failed")
	}

	// Deserialize empty
	arr.Deserialize("")
	if arr.Size() != 0 {
		t.Error("Deserialize empty failed")
	}
}

func TestDoublyLinkedList_Coverage(t *testing.T) {
	// PushFront empty vs non-empty
	dl := NewDoublyLinkedList()
	dl.PushFront("a") // Hits else (tail = newNode)
	dl.PushFront("b") // Hits if (head.prev = newNode)

	// PopFront until empty
	dl.PopFront() // b removed, a remains. Head != nil
	dl.PopFront() // a removed. Head == nil. Hits else (tail = nil)

	// InsertAfter
	dl.PushBack("a")
	dl.PushBack("b")
	dl.InsertAfter("a", "c") // a -> c -> b. Middle insert
	dl.InsertAfter("b", "d") // a -> c -> b -> d. Tail insert

	// InsertBefore
	dl.InsertBefore("d", "e") // ... -> b -> e -> d. Middle insert
	dl.InsertBefore("a", "f") // f -> a ... Head insert

	// RemoveAfter
	dl = NewDoublyLinkedList()
	dl.PushBack("a")
	dl.PushBack("b")
	dl.PushBack("c")
	dl.RemoveAfter("a") // Removes b. Middle remove. a -> c
	dl.RemoveAfter("a") // Removes c. Tail remove. a -> nil

	// RemoveBefore
	dl = NewDoublyLinkedList()
	dl.PushBack("a")
	dl.PushBack("b")
	dl.PushBack("c")
	dl.RemoveBefore("c") // Removes b. Middle remove. a -> c
	dl.RemoveBefore("a") // Nothing before a
	dl.RemoveBefore("c") // Removes a. Head remove. c

	// Deserialize empty
	dl.Deserialize("")
	if dl.Size() != 0 {
		t.Error("Deserialize empty failed")
	}
}

func TestSinglyLinkedList_Coverage(t *testing.T) {
	// PushFront is usually simple, but let's check
	sl := NewSinglyLinkedList()
	sl.PushFront("a")
	sl.PushFront("b")

	// InsertAfter
	sl = NewSinglyLinkedList()
	sl.PushBack("a")
	sl.InsertAfter("a", "b") // Tail insert

	// InsertBefore
	sl = NewSinglyLinkedList()
	sl.PushBack("a")
	sl.InsertBefore("a", "b") // Head insert
	sl.PushBack("c")
	sl.InsertBefore("c", "d") // Middle insert

	// RemoveAfter
	sl = NewSinglyLinkedList()
	sl.PushBack("a")
	sl.PushBack("b")
	sl.RemoveAfter("a") // Tail remove

	// RemoveBefore
	sl = NewSinglyLinkedList()
	sl.PushBack("a")
	sl.PushBack("b")
	sl.RemoveBefore("b") // Head remove

	// Deserialize empty
	sl.Deserialize("")
	if sl.Count != 0 {
		t.Error("Deserialize empty failed")
	}
}

func TestHashTableChaining_Coverage(t *testing.T) {
	ht := NewHashTableChaining(0) // Invalid cap -> default
	if ht == nil {
		t.Error("NewHashTableChaining failed")
	}
	ht.Deserialize("")
}

func TestHashTableOpenAddr_Coverage(t *testing.T) {
	ht := NewHashTableOpenAddr(0) // Invalid cap -> default
	if ht == nil {
		t.Error("NewHashTableOpenAddr failed")
	}
	ht.Deserialize("")
}

func TestSet_Coverage(t *testing.T) {
	s := NewSet(0) // Invalid cap -> default
	if s == nil {
		t.Error("NewSet failed")
	}
	s.Deserialize("")
}

func TestQueue_Coverage(t *testing.T) {
	q := NewQueue()
	q.Deserialize("")
}

func TestStack_Coverage(t *testing.T) {
	s := NewStack()
	s.Deserialize("")
}

func TestLFUCache_Coverage(t *testing.T) {
	c := NewLFUCache(2)
	c.Deserialize("")

	// Evict when empty (should be no-op)
	c.evict() // size 0

	// Set update existing
	c.Set("a", "1")
	c.Set("a", "2") // Update
}

func TestFullBinaryTree_Coverage(t *testing.T) {
	tree := NewFullBinaryTree()
	tree.Deserialize("")

	// IsFull recursive coverage
	// Manually create a non-full tree (1 child)
	tree.Root = &BTNode{Data: "A"}
	tree.Root.Left = &BTNode{Data: "B"}
	if tree.IsFull() {
		t.Error("Tree with 1 child should not be full")
	}
}

func TestFullBinaryTree_IsFull_RightOnly(t *testing.T) {
	tree := NewFullBinaryTree()
	tree.Root = &BTNode{Data: "A"}
	tree.Root.Right = &BTNode{Data: "B"}
	if tree.IsFull() {
		t.Error("Tree with right child only should not be full")
	}
}

func TestFullBinaryTree_IsFull_Complex(t *testing.T) {
	// Left child not full
	tree := NewFullBinaryTree()
	tree.Root = &BTNode{Data: "A"}
	tree.Root.Left = &BTNode{Data: "B"}
	tree.Root.Right = &BTNode{Data: "C"}
	tree.Root.Left.Left = &BTNode{Data: "D"}
	if tree.IsFull() {
		t.Error("Tree with left child not full should not be full")
	}

	// Right child not full
	tree = NewFullBinaryTree()
	tree.Root = &BTNode{Data: "A"}
	tree.Root.Left = &BTNode{Data: "B"}
	tree.Root.Right = &BTNode{Data: "C"}
	tree.Root.Right.Left = &BTNode{Data: "E"}
	if tree.IsFull() {
		t.Error("Tree with right child not full should not be full")
	}
}

func TestHashTableOpenAddr_Full(t *testing.T) {
	ht := NewHashTableOpenAddr(2)
	ht.Put("a", "1")
	ht.Put("b", "2")
	// Table is full. Search for missing key.
	if _, err := ht.Get("c"); err == nil {
		t.Error("Should not find c")
	}
}

func TestLFUCache_ZeroCap(t *testing.T) {
	c := NewLFUCache(0)
	c.Set("a", "1")
	if c.Get("a") != "" {
		t.Error("Should not store in zero cap cache")
	}
}

func TestDoublyLinkedList_RemoveAfterTail(t *testing.T) {
	dl := NewDoublyLinkedList()
	dl.PushBack("a")
	dl.RemoveAfter("a") // No-op
	if dl.Size() != 1 {
		t.Error("RemoveAfter tail should be no-op")
	}
}

func TestSinglyLinkedList_RemoveAfterTail(t *testing.T) {
	sl := NewSinglyLinkedList()
	sl.PushBack("a")
	sl.RemoveAfter("a") // No-op
	if sl.Count != 1 {
		t.Error("RemoveAfter tail should be no-op")
	}
}

func TestSinglyLinkedList_Coverage_Extended(t *testing.T) {
	// RemoveValue
	l := NewSinglyLinkedList()
	l.RemoveValue("a") // Empty list

	l.PushBack("a")
	l.RemoveValue("a") // Head match, becomes empty
	if l.Count != 0 || l.Head != nil || l.Tail != nil {
		t.Error("RemoveValue head failed")
	}

	l.PushBack("a")
	l.PushBack("b")
	l.RemoveValue("b") // Tail match
	if l.Count != 1 || l.Tail.Value != "a" {
		t.Error("RemoveValue tail failed")
	}

	// InsertAfter
	l = NewSinglyLinkedList()
	l.PushBack("a")
	l.InsertAfter("a", "b") // Insert after tail
	if l.Tail.Value != "b" {
		t.Error("InsertAfter tail failed")
	}

	// InsertBefore
	l = NewSinglyLinkedList()
	l.InsertBefore("a", "b") // Empty list

	l.PushBack("a")
	l.InsertBefore("a", "b") // Head match
	if l.Head.Value != "b" {
		t.Error("InsertBefore head failed")
	}

	l = NewSinglyLinkedList()
	l.PushBack("a")
	l.PushBack("b")
	l.InsertBefore("b", "c") // Middle match
	if l.Head.Next.Value != "c" {
		t.Error("InsertBefore middle failed")
	}

	// RemoveBefore
	l = NewSinglyLinkedList()
	l.RemoveBefore("a") // Empty

	l.PushBack("a")
	l.RemoveBefore("a") // 1 element

	l.PushBack("b")
	l.RemoveBefore("b") // Head match (a is before b)
	if l.Head.Value != "b" {
		t.Error("RemoveBefore head failed")
	}

	l = NewSinglyLinkedList()
	l.PushBack("a")
	l.PushBack("b")
	l.PushBack("c")
	l.RemoveBefore("c") // Middle match (b is before c)
	// List: a -> b -> c. Remove b. Result: a -> c.
	if l.Head.Next.Value != "c" {
		t.Error("RemoveBefore middle failed")
	}
}

func TestSet_Coverage_Extended(t *testing.T) {
	s := NewSet(16)
	if s.Size() != 0 {
		t.Error("Size should be 0")
	}
	if s.Serialize() != "" {
		t.Error("Serialize empty failed")
	}
	s.Add("a")
	if s.Serialize() != "a" {
		t.Error("Serialize 1 failed")
	}
	s.Add("b")
	// Order depends on hash, but contains space
	if !strings.Contains(s.Serialize(), " ") {
		t.Error("Serialize 2 failed")
	}
}

func TestHashTableChaining_Serialize(t *testing.T) {
	ht := NewHashTableChaining(16)
	if ht.Serialize() != "" {
		t.Error("Serialize empty failed")
	}
	ht.Put("a", "1")
	if ht.Serialize() != "a:1" {
		t.Error("Serialize 1 failed")
	}
	ht.Put("b", "2")
	if !strings.Contains(ht.Serialize(), " ") {
		t.Error("Serialize 2 failed")
	}
}

func TestHashTableOpenAddr_Serialize(t *testing.T) {
	ht := NewHashTableOpenAddr(16)
	if ht.Serialize() != "" {
		t.Error("Serialize empty failed")
	}
	ht.Put("a", "1")
	if ht.Serialize() != "a:1" {
		t.Error("Serialize 1 failed")
	}
	ht.Put("b", "2")
	if !strings.Contains(ht.Serialize(), " ") {
		t.Error("Serialize 2 failed")
	}
}

func TestLFUCache_Serialize(t *testing.T) {
	c := NewLFUCache(2)
	if c.Serialize() != "2|" {
		t.Error("Serialize empty failed")
	}
	c.Set("a", "1")
	if c.Serialize() != "2|a:1:1" { // key:val:freq
		t.Error("Serialize 1 failed")
	}
	c.Set("b", "2")
	if !strings.Contains(c.Serialize(), " ") {
		t.Error("Serialize 2 failed")
	}
}

func TestDatastructures_Deserialize_Coverage(t *testing.T) {
	// LFUCache
	c := NewLFUCache(2)
	c.Deserialize("invalid") // No pipe
	// Should not crash

	c.Deserialize("3|") // Empty data
	if c.capacity != 3 {
		t.Error("LFUCache capacity deserialize failed")
	}

	// FullBinaryTree
	tree := NewFullBinaryTree()
	tree.Deserialize("")
	if tree.Root != nil {
		t.Error("Tree deserialize empty failed")
	}
	tree.Deserialize("A|") // Root A, empty second part
	if tree.Root == nil || tree.Root.Data != "A" {
		t.Error("Tree deserialize root failed")
	}
}

func TestSet_Collision(t *testing.T) {
	s := NewSet(1) // Force collisions
	s.Add("a")
	s.Add("b")
	if !s.Contains("a") {
		t.Error("Contains a failed")
	}
	if !s.Contains("b") {
		t.Error("Contains b failed")
	}
	s.Remove("a")
	if s.Contains("a") {
		t.Error("Contains a after remove failed")
	}
	if !s.Contains("b") {
		t.Error("Contains b after remove a failed")
	}
}

func TestSinglyLinkedList_MiddleOps_2(t *testing.T) {
	// RemoveValue middle
	l := NewSinglyLinkedList()
	l.PushBack("a")
	l.PushBack("b")
	l.PushBack("c")
	l.RemoveValue("b")
	if l.Count != 2 || l.Head.Next.Value != "c" {
		t.Error("RemoveValue middle failed")
	}

	// InsertAfter middle
	l = NewSinglyLinkedList()
	l.PushBack("a")
	l.PushBack("b")
	l.InsertAfter("a", "c") // a -> c -> b
	if l.Count != 3 || l.Head.Next.Value != "c" || l.Tail.Value != "b" {
		t.Error("InsertAfter middle failed")
	}
}

func TestSinglyLinkedList_MiddleOps(t *testing.T) {
	// RemoveValue middle
	l := NewSinglyLinkedList()
	l.PushBack("a")
	l.PushBack("b")
	l.PushBack("c")
	l.RemoveValue("b")
	if l.Count != 2 || l.Head.Next.Value != "c" {
		t.Error("RemoveValue middle failed")
	}

	// InsertAfter middle
	l = NewSinglyLinkedList()
	l.PushBack("a")
	l.PushBack("b")
	l.InsertAfter("a", "c") // a -> c -> b
	if l.Count != 3 || l.Head.Next.Value != "c" || l.Tail.Value != "b" {
		t.Error("InsertAfter middle failed")
	}
}
