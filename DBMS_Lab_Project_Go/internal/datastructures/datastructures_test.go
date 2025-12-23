package datastructures

import (
	"testing"
)

// --- Array Tests ---
func TestArray(t *testing.T) {
	arr := NewArray()
	arr.PushBack("10")
	arr.PushBack("20")

	// Test Size
	if arr.Size() != 2 {
		t.Errorf("Expected size 2, got %d", arr.Size())
	}

	// Test Get
	val, err := arr.Get(1)
	if err != nil || val != "20" {
		t.Errorf("Expected 20, got %s, err: %v", val, err)
	}
	_, err = arr.Get(5)
	if err == nil {
		t.Error("Expected error for out of bounds Get")
	}

	// Test Insert
	arr.Insert(1, "15") // 10, 15, 20
	val, _ = arr.Get(1)
	if val != "15" {
		t.Errorf("Expected 15, got %s", val)
	}
	err = arr.Insert(10, "99")
	if err == nil {
		t.Error("Expected error for out of bounds Insert")
	}

	// Test Set
	arr.Set(0, "5") // 5, 15, 20
	val, _ = arr.Get(0)
	if val != "5" {
		t.Errorf("Expected 5, got %s", val)
	}
	err = arr.Set(10, "99")
	if err == nil {
		t.Error("Expected error for out of bounds Set")
	}

	// Test Remove
	arr.Remove(0) // 15, 20
	val, _ = arr.Get(0)
	if val != "15" {
		t.Errorf("Expected 15, got %s", val)
	}
	err = arr.Remove(10)
	if err == nil {
		t.Error("Expected error for out of bounds Remove")
	}
}

// --- Stack Tests ---
func TestStack(t *testing.T) {
	s := NewStack()
	if !s.Empty() {
		t.Error("Expected stack to be empty")
	}

	s.Push("1")
	s.Push("2")

	val, _ := s.Peek()
	if val != "2" {
		t.Errorf("Expected 2, got %s", val)
	}

	val, _ = s.Pop()
	if val != "2" {
		t.Errorf("Expected 2, got %s", val)
	}

	val, _ = s.Pop()
	if val != "1" {
		t.Errorf("Expected 1, got %s", val)
	}

	_, err := s.Pop()
	if err == nil {
		t.Error("Expected error when popping empty stack")
	}
}

// --- Queue Tests ---
func TestQueue(t *testing.T) {
	q := NewQueue()
	if !q.Empty() {
		t.Error("Expected queue to be empty")
	}

	q.Push("1")
	q.Push("2")

	val, _ := q.Pop()
	if val != "1" {
		t.Errorf("Expected 1, got %s", val)
	}

	val, _ = q.Pop()
	if val != "2" {
		t.Errorf("Expected 2, got %s", val)
	}

	_, err := q.Pop()
	if err == nil {
		t.Error("Expected error when popping empty queue")
	}
}

// --- SinglyLinkedList Tests ---
func TestSinglyLinkedList(t *testing.T) {
	l := NewSinglyLinkedList()
	l.PushBack("1")
	l.PushFront("0") // 0 -> 1

	if l.Head.Value != "0" {
		t.Errorf("Expected head 0, got %s", l.Head.Value)
	}
	if l.Tail.Value != "1" {
		t.Errorf("Expected tail 1, got %s", l.Tail.Value)
	}

	l.InsertAfter("0", "0.5") // 0 -> 0.5 -> 1
	if !l.Find("0.5") {
		t.Error("Expected to find 0.5")
	}

	l.RemoveValue("0.5") // 0 -> 1
	if l.Find("0.5") {
		t.Error("Expected 0.5 to be removed")
	}

	l.PopBack() // 0
	if l.Tail.Value != "0" {
		t.Errorf("Expected tail 0, got %s", l.Tail.Value)
	}

	l.PopFront() // empty
	if l.Head != nil {
		t.Error("Expected head to be nil")
	}

	_, err := l.PopFront()
	if err == nil {
		t.Error("Expected error popping from empty list")
	}
}

// --- DoublyLinkedList Tests ---
func TestDoublyLinkedList(t *testing.T) {
	l := NewDoublyLinkedList()
	l.PushBack("1")
	l.PushFront("0") // 0 <-> 1

	if l.Head.Value != "0" {
		t.Errorf("Expected head 0, got %s", l.Head.Value)
	}
	if l.Tail.Value != "1" {
		t.Errorf("Expected tail 1, got %s", l.Tail.Value)
	}

	l.InsertAfter("0", "0.5") // 0 <-> 0.5 <-> 1
	if !l.Find("0.5") {
		t.Error("Expected to find 0.5")
	}

	l.RemoveValue("0.5") // 0 <-> 1
	if l.Find("0.5") {
		t.Error("Expected 0.5 to be removed")
	}

	l.PopBack() // 0
	if l.Tail.Value != "0" {
		t.Errorf("Expected tail 0, got %s", l.Tail.Value)
	}

	l.PopFront() // empty
	if l.Head != nil {
		t.Error("Expected head to be nil")
	}
}

// --- HashTableChaining Tests ---
func TestHashTableChaining(t *testing.T) {
	ht := NewHashTableChaining(10)
	ht.Put("key1", "val1")
	ht.Put("key2", "val2")

	val, err := ht.Get("key1")
	if err != nil || val != "val1" {
		t.Errorf("Expected val1, got %s", val)
	}

	// Update existing
	ht.Put("key1", "val1_new")
	val, _ = ht.Get("key1")
	if val != "val1_new" {
		t.Errorf("Expected val1_new, got %s", val)
	}

	ht.Remove("key1")
	_, err = ht.Get("key1")
	if err == nil {
		t.Error("Expected error after removal")
	}
}

// --- HashTableOpenAddr Tests ---
func TestHashTableOpenAddr(t *testing.T) {
	ht := NewHashTableOpenAddr(10)
	ht.Put("key1", "val1")

	val, err := ht.Get("key1")
	if err != nil || val != "val1" {
		t.Errorf("Expected val1, got %s", val)
	}

	// Update existing
	ht.Put("key1", "val1_new")
	val, _ = ht.Get("key1")
	if val != "val1_new" {
		t.Errorf("Expected val1_new, got %s", val)
	}

	ht.Remove("key1")
	_, err = ht.Get("key1")
	if err == nil {
		t.Error("Expected error after removal")
	}
}

// --- FullBinaryTree Tests ---
func TestFullBinaryTree(t *testing.T) {
	tree := NewFullBinaryTree()
	tree.Insert("1")
	tree.Insert("2")
	tree.Insert("3") // Full tree with 3 nodes (root + 2 children)

	if !tree.Find("2") {
		t.Error("Expected to find 2")
	}
	if !tree.Find("3") {
		t.Error("Expected to find 3")
	}
	if tree.Find("99") {
		t.Error("Expected not to find 99")
	}

	if !tree.IsFull() {
		t.Error("Expected tree to be full")
	}

	tree.Insert("4")
	if tree.IsFull() {
		t.Error("Expected tree NOT to be full after adding 4th node")
	}
}

// --- Set Tests ---
func TestSet(t *testing.T) {
	s := NewSet(10)
	s.Add("1")
	s.Add("2")

	if !s.Contains("1") {
		t.Error("Expected to contain 1")
	}

	// Add duplicate
	s.Add("1")
	// Check size or elements if possible, but basic check is it doesn't crash and still contains
	if !s.Contains("1") {
		t.Error("Expected to contain 1")
	}

	s.Remove("1")
	if s.Contains("1") {
		t.Error("Expected not to contain 1")
	}

	s.Remove("99") // Remove non-existent
}

// --- LFUCache Tests ---
func TestLFUCache(t *testing.T) {
	c := NewLFUCache(2)
	c.Set("1", "val1")
	c.Set("2", "val2")

	// Access 1, freq becomes 2
	if c.Get("1") != "val1" {
		t.Error("Expected val1")
	}

	// Add 3, should evict 2 (freq 1) because 1 has freq 2
	c.Set("3", "val3")

	if c.Get("2") != "" {
		t.Error("Expected 2 to be evicted")
	}
	if c.Get("3") != "val3" {
		t.Error("Expected val3")
	}
	if c.Get("1") != "val1" {
		t.Error("Expected 1 to remain")
	}

	// Update existing
	c.Set("3", "val3_new")
	if c.Get("3") != "val3_new" {
		t.Error("Expected val3_new")
	}
}

// --- Serialization Tests ---

func TestArray_SerializeDeserialize(t *testing.T) {
	arr := NewArray()
	arr.PushBack("10")
	arr.PushBack("20")

	data := arr.Serialize()
	arr2 := NewArray()
	arr2.Deserialize(data)

	if arr2.Size() != 2 {
		t.Errorf("Expected size 2, got %d", arr2.Size())
	}
	val, _ := arr2.Get(0)
	if val != "10" {
		t.Errorf("Expected 10, got %s", val)
	}

	arr.Clear()
	if arr.Size() != 0 {
		t.Error("Expected empty array after Clear")
	}
}

func TestStack_SerializeDeserialize(t *testing.T) {
	s := NewStack()
	s.Push("1")
	s.Push("2")

	data := s.Serialize()
	s2 := NewStack()
	s2.Deserialize(data)

	val, _ := s2.Pop()
	if val != "2" {
		t.Errorf("Expected 2, got %s", val)
	}
}

func TestQueue_SerializeDeserialize(t *testing.T) {
	q := NewQueue()
	q.Push("1")
	q.Push("2")

	data := q.Serialize()
	q2 := NewQueue()
	q2.Deserialize(data)

	val, _ := q2.Pop()
	if val != "1" {
		t.Errorf("Expected 1, got %s", val)
	}
}

func TestSinglyLinkedList_SerializeDeserialize(t *testing.T) {
	l := NewSinglyLinkedList()
	l.PushBack("1")
	l.PushBack("2")

	data := l.Serialize()
	l2 := NewSinglyLinkedList()
	l2.Deserialize(data)

	if l2.Head.Value != "1" {
		t.Errorf("Expected head 1, got %s", l2.Head.Value)
	}
	if l2.Tail.Value != "2" {
		t.Errorf("Expected tail 2, got %s", l2.Tail.Value)
	}
}

func TestDoublyLinkedList_SerializeDeserialize(t *testing.T) {
	l := NewDoublyLinkedList()
	l.PushBack("1")
	l.PushBack("2")

	data := l.Serialize()
	l2 := NewDoublyLinkedList()
	l2.Deserialize(data)

	if l2.Head.Value != "1" {
		t.Errorf("Expected head 1, got %s", l2.Head.Value)
	}
	if l2.Tail.Value != "2" {
		t.Errorf("Expected tail 2, got %s", l2.Tail.Value)
	}
}

func TestHashTableChaining_SerializeDeserialize(t *testing.T) {
	ht := NewHashTableChaining(10)
	ht.Put("k1", "v1")

	data := ht.Serialize()
	ht2 := NewHashTableChaining(10)
	ht2.Deserialize(data)

	val, _ := ht2.Get("k1")
	if val != "v1" {
		t.Errorf("Expected v1, got %s", val)
	}
}

func TestHashTableOpenAddr_SerializeDeserialize(t *testing.T) {
	ht := NewHashTableOpenAddr(10)
	ht.Put("k1", "v1")

	data := ht.Serialize()
	ht2 := NewHashTableOpenAddr(10)
	ht2.Deserialize(data)

	val, _ := ht2.Get("k1")
	if val != "v1" {
		t.Errorf("Expected v1, got %s", val)
	}

	ht.Clear()
	_, err := ht.Get("k1")
	if err == nil {
		t.Error("Expected error after Clear")
	}
}

func TestFullBinaryTree_SerializeDeserialize(t *testing.T) {
	tree := NewFullBinaryTree()
	tree.Insert("1")
	tree.Insert("2")

	data := tree.Serialize()
	tree2 := NewFullBinaryTree()
	tree2.Deserialize(data)

	if !tree2.Find("2") {
		t.Error("Expected to find 2")
	}
}

func TestSet_SerializeDeserialize(t *testing.T) {
	s := NewSet(10)
	s.Add("1")

	data := s.Serialize()
	s2 := NewSet(10)
	s2.Deserialize(data)

	if !s2.Contains("1") {
		t.Error("Expected to contain 1")
	}

	s.Clear()
	if s.Contains("1") {
		t.Error("Expected empty set after Clear")
	}
}

func TestLFUCache_SerializeDeserialize(t *testing.T) {
	c := NewLFUCache(2)
	c.Set("k1", "v1")

	data := c.Serialize()
	c2 := NewLFUCache(2)
	c2.Deserialize(data)

	if c2.Get("k1") != "v1" {
		t.Errorf("Expected v1, got %s", c2.Get("k1"))
	}
}

// --- Additional Operations Tests ---

func TestSinglyLinkedList_AdditionalOps(t *testing.T) {
	l := NewSinglyLinkedList()
	l.PushBack("1")
	l.PushBack("2")
	l.PushBack("3") // 1 -> 2 -> 3

	l.InsertBefore("2", "1.5") // 1 -> 1.5 -> 2 -> 3
	if !l.Find("1.5") {
		t.Error("Expected to find 1.5")
	}

	l.RemoveAfter("1") // Removes 1.5 -> 1 -> 2 -> 3
	if l.Find("1.5") {
		t.Error("Expected 1.5 to be removed")
	}

	l.RemoveBefore("3") // Removes 2 -> 1 -> 3
	if l.Find("2") {
		t.Error("Expected 2 to be removed")
	}
}

func TestDoublyLinkedList_AdditionalOps(t *testing.T) {
	l := NewDoublyLinkedList()
	l.PushBack("1")
	l.PushBack("2")
	l.PushBack("3") // 1 <-> 2 <-> 3

	l.InsertBefore("2", "1.5") // 1 <-> 1.5 <-> 2 <-> 3
	if !l.Find("1.5") {
		t.Error("Expected to find 1.5")
	}

	l.RemoveAfter("1") // Removes 1.5 -> 1 <-> 2 <-> 3
	if l.Find("1.5") {
		t.Error("Expected 1.5 to be removed")
	}

	l.RemoveBefore("3") // Removes 2 -> 1 <-> 3
	if l.Find("2") {
		t.Error("Expected 2 to be removed")
	}
}

func TestSet_Operations(t *testing.T) {
	s1 := NewSet(10)
	s1.Add("1")
	s1.Add("2")

	s2 := NewSet(10)
	s2.Add("2")
	s2.Add("3")

	// Union: 1, 2, 3
	u := s1.Union(s2)
	if !u.Contains("1") || !u.Contains("2") || !u.Contains("3") {
		t.Error("Union failed")
	}

	// Intersection: 2
	i := s1.Intersection(s2)
	if i.Contains("1") || !i.Contains("2") || i.Contains("3") {
		t.Error("Intersection failed")
	}

	// Difference s1 - s2: 1
	d := s1.Difference(s2)
	if !d.Contains("1") || d.Contains("2") {
		t.Error("Difference failed")
	}

	elems := s1.GetElements()
	if len(elems) != 2 {
		t.Errorf("Expected 2 elements, got %d", len(elems))
	}
}

func TestHashTableOpenAddr_Rehash(t *testing.T) {
	// Initial capacity is small (e.g. 2 or 4 if we could set it, but it's 16 by default)
	// We need to fill it up to trigger rehash.
	// Default size 16, load factor 0.75 -> 12 elements triggers rehash.
	ht := NewHashTableOpenAddr(4) // Force small size if possible, but constructor takes size

	for i := 0; i < 10; i++ {
		ht.Put(string(rune('a'+i)), "val")
	}

	// Check if all are present
	for i := 0; i < 10; i++ {
		val, err := ht.Get(string(rune('a' + i)))
		if err != nil || val != "val" {
			t.Errorf("Missing key %c", 'a'+i)
		}
	}
}

func TestSet_Rehash(t *testing.T) {
	s := NewSet(4)
	for i := 0; i < 10; i++ {
		s.Add(string(rune('a' + i)))
	}

	for i := 0; i < 10; i++ {
		if !s.Contains(string(rune('a' + i))) {
			t.Errorf("Missing key %c", 'a'+i)
		}
	}
}

func TestDeserialize_Errors(t *testing.T) {
	arr := NewArray()
	// Malformed data
	arr.Deserialize("invalid")
	// Should not crash, maybe empty or partial

	l := NewSinglyLinkedList()
	l.Deserialize("invalid")

	ht := NewHashTableOpenAddr(10)
	ht.Deserialize("invalid")
}

func TestDataStructures_EdgeCases(t *testing.T) {
	// SinglyLinkedList Edge Cases
	sl := NewSinglyLinkedList()
	if _, err := sl.PopBack(); err == nil {
		t.Error("Expected error popping back from empty list")
	}
	sl.PushBack("1")
	if val, err := sl.PopBack(); err != nil || val != "1" {
		t.Error("Failed to pop back single element")
	}
	if sl.Head != nil {
		t.Error("List should be empty after popping single element")
	}

	sl.PushBack("1")
	sl.PushBack("2")
	sl.PushBack("3")
	sl.RemoveValue("1")
	if sl.Head.Value != "2" {
		t.Error("Head should be 2")
	}
	sl.RemoveValue("3")
	if sl.Tail.Value != "2" {
		t.Error("Tail should be 2")
	}
	sl.RemoveValue("missing")

	// DoublyLinkedList Edge Cases
	dl := NewDoublyLinkedList()
	if _, err := dl.PopBack(); err == nil {
		t.Error("Expected error popping back from empty dlist")
	}
	dl.PushBack("1")
	if val, err := dl.PopBack(); err != nil || val != "1" {
		t.Error("Failed to pop back single element from dlist")
	}
	if dl.Head != nil {
		t.Error("DList should be empty after popping single element")
	}

	dl.PushBack("1")
	dl.PushBack("2")
	dl.PushBack("3")
	dl.RemoveValue("1")
	if dl.Head.Value != "2" {
		t.Error("Head should be 2")
	}
	if dl.Head.Prev != nil {
		t.Error("Head prev should be nil")
	}
	dl.RemoveValue("3")
	if dl.Tail.Value != "2" {
		t.Error("Tail should be 2")
	}
	if dl.Tail.Next != nil {
		t.Error("Tail next should be nil")
	}
	dl.RemoveValue("missing")

	// Queue Edge Cases
	q := NewQueue()
	if _, err := q.Pop(); err == nil {
		t.Error("Expected error popping from empty queue")
	}

	// Stack Edge Cases
	s := NewStack()
	if _, err := s.Pop(); err == nil {
		t.Error("Expected error popping from empty stack")
	}

	// Set Edge Cases
	set := NewSet(10)
	set.Remove("missing")

	// HashTable Edge Cases
	ht := NewHashTableChaining(10)
	if _, err := ht.Get("missing"); err == nil {
		t.Error("Should not find missing key in hash table")
	}
	ht.Remove("missing")

	oht := NewHashTableOpenAddr(10)
	if _, err := oht.Get("missing"); err == nil {
		t.Error("Should not find missing key in open hash table")
	}
	oht.Remove("missing")

	// LFUCache Edge Cases
	cache := NewLFUCache(2)
	if val := cache.Get("missing"); val != "" {
		t.Error("Should not find missing key in cache")
	}
}

func TestSinglyLinkedList_Advanced(t *testing.T) {
	l := NewSinglyLinkedList()
	l.PushBack("1")
	l.PushBack("2")
	l.PushBack("3")

	// InsertBefore
	l.InsertBefore("1", "0") // 0 -> 1 -> 2 -> 3
	if l.Head.Value != "0" {
		t.Error("InsertBefore head failed")
	}

	l.InsertBefore("3", "2.5") // 0 -> 1 -> 2 -> 2.5 -> 3
	if l.Tail.Value != "3" {
		t.Error("Tail should be 3")
	}

	l.InsertBefore("missing", "x") // No change
	if l.Count != 5 {
		t.Error("Count should be 5")
	}

	// RemoveBefore
	l.RemoveBefore("0") // No change (nothing before head)
	if l.Head.Value != "0" {
		t.Error("RemoveBefore head failed")
	}

	l.RemoveBefore("1") // Removes 0. 1 -> 2 -> 2.5 -> 3
	if l.Head.Value != "1" {
		t.Error("RemoveBefore second element failed")
	}

	l.RemoveBefore("3") // Removes 2.5. 1 -> 2 -> 3
	if l.Count != 3 {
		t.Error("Count should be 3")
	}

	l.RemoveBefore("missing") // No change

	// RemoveAfter
	l.RemoveAfter("3") // Tail, nothing after. 1 -> 2 -> 3
	if l.Tail.Value != "3" {
		t.Error("RemoveAfter tail failed")
	}

	l.RemoveAfter("missing") // No change
}

func TestStack_Peek(t *testing.T) {
	s := NewStack()
	if _, err := s.Peek(); err == nil {
		t.Error("Peek empty should error")
	}
	s.Push("1")
	if val, err := s.Peek(); err != nil || val != "1" {
		t.Error("Peek failed")
	}
	// Ensure not popped
	if _, err := s.Pop(); err != nil {
		t.Error("Stack should not be empty after Peek")
	}
}
