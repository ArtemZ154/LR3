package datastructures

import (
	"errors"
	"strings"
)

type DNode struct {
	Value string
	Next  *DNode
	Prev  *DNode
}

type DoublyLinkedList struct {
	Head  *DNode
	Tail  *DNode
	Count int
}

func NewDoublyLinkedList() *DoublyLinkedList {
	return &DoublyLinkedList{}
}

func (l *DoublyLinkedList) PushFront(value string) {
	newNode := &DNode{Value: value, Next: l.Head}
	if l.Head != nil {
		l.Head.Prev = newNode
	} else {
		l.Tail = newNode
	}
	l.Head = newNode
	l.Count++
}

func (l *DoublyLinkedList) PushBack(value string) {
	newNode := &DNode{Value: value, Prev: l.Tail}
	if l.Tail != nil {
		l.Tail.Next = newNode
	} else {
		l.Head = newNode
	}
	l.Tail = newNode
	l.Count++
}

func (l *DoublyLinkedList) PopFront() (string, error) {
	if l.Head == nil {
		return "", errors.New("list is empty")
	}
	val := l.Head.Value
	l.Head = l.Head.Next
	if l.Head != nil {
		l.Head.Prev = nil
	} else {
		l.Tail = nil
	}
	l.Count--
	return val, nil
}

func (l *DoublyLinkedList) PopBack() (string, error) {
	if l.Tail == nil {
		return "", errors.New("list is empty")
	}
	val := l.Tail.Value
	l.Tail = l.Tail.Prev
	if l.Tail != nil {
		l.Tail.Next = nil
	} else {
		l.Head = nil
	}
	l.Count--
	return val, nil
}

func (l *DoublyLinkedList) RemoveValue(value string) {
	current := l.Head
	for current != nil {
		if current.Value == value {
			if current.Prev != nil {
				current.Prev.Next = current.Next
			} else {
				l.Head = current.Next
			}
			if current.Next != nil {
				current.Next.Prev = current.Prev
			} else {
				l.Tail = current.Prev
			}
			l.Count--
			return
		}
		current = current.Next
	}
}

func (l *DoublyLinkedList) Find(value string) bool {
	current := l.Head
	for current != nil {
		if current.Value == value {
			return true
		}
		current = current.Next
	}
	return false
}

func (l *DoublyLinkedList) InsertAfter(targetValue, newValue string) {
	current := l.Head
	for current != nil {
		if current.Value == targetValue {
			newNode := &DNode{Value: newValue, Next: current.Next, Prev: current}
			if current.Next != nil {
				current.Next.Prev = newNode
			} else {
				l.Tail = newNode
			}
			current.Next = newNode
			l.Count++
			return
		}
		current = current.Next
	}
}

func (l *DoublyLinkedList) InsertBefore(targetValue, newValue string) {
	current := l.Head
	for current != nil {
		if current.Value == targetValue {
			newNode := &DNode{Value: newValue, Next: current, Prev: current.Prev}
			if current.Prev != nil {
				current.Prev.Next = newNode
			} else {
				l.Head = newNode
			}
			current.Prev = newNode
			l.Count++
			return
		}
		current = current.Next
	}
}

func (l *DoublyLinkedList) RemoveAfter(targetValue string) {
	current := l.Head
	for current != nil {
		if current.Value == targetValue && current.Next != nil {
			toRemove := current.Next
			current.Next = toRemove.Next
			if toRemove.Next != nil {
				toRemove.Next.Prev = current
			} else {
				l.Tail = current
			}
			l.Count--
			return
		}
		current = current.Next
	}
}

func (l *DoublyLinkedList) RemoveBefore(targetValue string) {
	current := l.Head
	for current != nil {
		if current.Value == targetValue && current.Prev != nil {
			toRemove := current.Prev
			if toRemove.Prev != nil {
				toRemove.Prev.Next = current
			} else {
				l.Head = current
			}
			current.Prev = toRemove.Prev
			l.Count--
			return
		}
		current = current.Next
	}
}

func (l *DoublyLinkedList) Size() int {
	return l.Count
}

func (l *DoublyLinkedList) Serialize() string {
	var sb strings.Builder
	current := l.Head
	for current != nil {
		sb.WriteString(current.Value)
		if current.Next != nil {
			sb.WriteString(" ")
		}
		current = current.Next
	}
	return sb.String()
}

func (l *DoublyLinkedList) Deserialize(str string) {
	l.Head = nil
	l.Tail = nil
	l.Count = 0
	if str == "" {
		return
	}
	parts := strings.Split(str, " ")
	for _, p := range parts {
		l.PushBack(p)
	}
}
