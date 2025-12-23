package datastructures

import (
	"testing"
)

func TestDoublyLinkedList_PopFront_Error(t *testing.T) {
	l := NewDoublyLinkedList()
	_, err := l.PopFront()
	if err == nil {
		t.Error("Expected error when popping from empty list")
	}
}

func TestFullBinaryTree_IsFull_Broken(t *testing.T) {
	tree := NewFullBinaryTree()
	// Manually construct a non-full tree
	//      A
	//     /
	//    B
	tree.Root = &BTNode{Data: "A"}
	tree.Root.Left = &BTNode{Data: "B"}

	if tree.IsFull() {
		t.Error("Tree with single child should not be full")
	}

	// Test the other side
	tree.Root.Left = nil
	tree.Root.Right = &BTNode{Data: "B"}
	if tree.IsFull() {
		t.Error("Tree with single right child should not be full")
	}
}

func TestSinglyLinkedList_EdgeCases_2(t *testing.T) {
	// PopBack empty
	l := NewSinglyLinkedList()
	_, err := l.PopBack()
	if err == nil {
		t.Error("Expected error when popping back from empty list")
	}

	// PopBack single element
	l.PushBack("A")
	val, err := l.PopBack()
	if err != nil {
		t.Errorf("Unexpected error: %v", err)
	}
	if val != "A" {
		t.Errorf("Expected A, got %s", val)
	}
	if l.Head != nil || l.Tail != nil {
		t.Error("Head and Tail should be nil")
	}

	// RemoveValue head (single element)
	l.PushBack("A")
	l.RemoveValue("A")
	if l.Head != nil || l.Tail != nil {
		t.Error("Head and Tail should be nil after removing only element")
	}

	// RemoveValue tail
	l.PushBack("A")
	l.PushBack("B")
	l.RemoveValue("B")
	if l.Tail.Value != "A" {
		t.Errorf("Expected Tail to be A, got %s", l.Tail.Value)
	}
	if l.Tail.Next != nil {
		t.Error("Tail.Next should be nil")
	}

	// InsertAfter tail
	l.InsertAfter("A", "C") // List is A -> C
	if l.Tail.Value != "C" {
		t.Errorf("Expected Tail to be C, got %s", l.Tail.Value)
	}
}

func TestSinglyLinkedList_MoreCoverage(t *testing.T) {
	// PopBack multiple elements
	l := NewSinglyLinkedList()
	l.PushBack("A")
	l.PushBack("B")
	l.PushBack("C")
	val, err := l.PopBack() // Removes C
	if err != nil {
		t.Errorf("Unexpected error: %v", err)
	}
	if val != "C" {
		t.Errorf("Expected C, got %s", val)
	}
	if l.Tail.Value != "B" {
		t.Errorf("Expected Tail to be B, got %s", l.Tail.Value)
	}

	// RemoveValue Head (multiple elements)
	l = NewSinglyLinkedList()
	l.PushBack("A")
	l.PushBack("B")
	l.RemoveValue("A")
	if l.Head.Value != "B" {
		t.Errorf("Expected Head to be B, got %s", l.Head.Value)
	}

	// RemoveValue Middle
	l = NewSinglyLinkedList()
	l.PushBack("A")
	l.PushBack("B")
	l.PushBack("C")
	l.RemoveValue("B")
	if l.Head.Next.Value != "C" {
		t.Errorf("Expected A -> C, got A -> %s", l.Head.Next.Value)
	}

	// RemoveValue Not Found (Multiple elements)
	l = NewSinglyLinkedList()
	l.PushBack("A")
	l.PushBack("C")
	l.RemoveValue("B")
	if l.Count != 2 {
		t.Error("Count changed when removing non-existent value from multi-element list")
	}

	// RemoveValue Not Found
	l = NewSinglyLinkedList()
	l.PushBack("A")
	l.RemoveValue("B")
	if l.Count != 1 {
		t.Error("Count changed when removing non-existent value")
	}

	// RemoveValue Empty
	l = NewSinglyLinkedList()
	l.RemoveValue("A")
	if l.Count != 0 {
		t.Error("Count changed when removing from empty list")
	}

	// InsertAfter Middle
	l = NewSinglyLinkedList()
	l.PushBack("A")
	l.PushBack("B")
	l.InsertAfter("A", "C") // A -> C -> B
	if l.Head.Next.Value != "C" {
		t.Errorf("Expected C after A, got %s", l.Head.Next.Value)
	}

	// InsertAfter Not Found
	l = NewSinglyLinkedList()
	l.PushBack("A")
	l.InsertAfter("B", "C")
	if l.Count != 1 {
		t.Error("Count changed when inserting after non-existent value")
	}
}
