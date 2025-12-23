package datastructures

import (
	"errors"
	"strings"
)

type SNode struct {
	Value string
	Next  *SNode
}

type SinglyLinkedList struct {
	Head  *SNode
	Tail  *SNode
	Count int
}

func NewSinglyLinkedList() *SinglyLinkedList {
	return &SinglyLinkedList{}
}

func (l *SinglyLinkedList) PushFront(value string) {
	newNode := &SNode{Value: value, Next: l.Head}
	l.Head = newNode
	if l.Tail == nil {
		l.Tail = newNode
	}
	l.Count++
}

func (l *SinglyLinkedList) PushBack(value string) {
	newNode := &SNode{Value: value}
	if l.Tail == nil {
		l.Head = newNode
		l.Tail = newNode
	} else {
		l.Tail.Next = newNode
		l.Tail = newNode
	}
	l.Count++
}

func (l *SinglyLinkedList) PopFront() (string, error) {
	if l.Head == nil {
		return "", errors.New("list is empty")
	}
	val := l.Head.Value
	l.Head = l.Head.Next
	if l.Head == nil {
		l.Tail = nil
	}
	l.Count--
	return val, nil
}

func (l *SinglyLinkedList) PopBack() (string, error) {
	if l.Head == nil {
		return "", errors.New("list is empty")
	}
	if l.Head == l.Tail {
		val := l.Head.Value
		l.Head = nil
		l.Tail = nil
		l.Count--
		return val, nil
	}
	current := l.Head
	for current.Next != l.Tail {
		current = current.Next
	}
	val := l.Tail.Value
	l.Tail = current
	l.Tail.Next = nil
	l.Count--
	return val, nil
}

func (l *SinglyLinkedList) RemoveValue(value string) {
	if l.Head == nil {
		return
	}
	if l.Head.Value == value {
		l.Head = l.Head.Next
		if l.Head == nil {
			l.Tail = nil
		}
		l.Count--
		return
	}
	current := l.Head
	for current.Next != nil {
		if current.Next.Value == value {
			current.Next = current.Next.Next
			if current.Next == nil {
				l.Tail = current
			}
			l.Count--
			return
		}
		current = current.Next
	}
}

func (l *SinglyLinkedList) Find(value string) bool {
	current := l.Head
	for current != nil {
		if current.Value == value {
			return true
		}
		current = current.Next
	}
	return false
}

func (l *SinglyLinkedList) InsertAfter(targetValue, newValue string) {
	current := l.Head
	for current != nil {
		if current.Value == targetValue {
			newNode := &SNode{Value: newValue, Next: current.Next}
			current.Next = newNode
			if current == l.Tail {
				l.Tail = newNode
			}
			l.Count++
			return
		}
		current = current.Next
	}
}

func (l *SinglyLinkedList) InsertBefore(targetValue, newValue string) {
	if l.Head == nil {
		return
	}
	if l.Head.Value == targetValue {
		l.PushFront(newValue)
		return
	}
	current := l.Head
	for current.Next != nil {
		if current.Next.Value == targetValue {
			newNode := &SNode{Value: newValue, Next: current.Next}
			current.Next = newNode
			l.Count++
			return
		}
		current = current.Next
	}
}

func (l *SinglyLinkedList) RemoveAfter(targetValue string) {
	current := l.Head
	for current != nil {
		if current.Value == targetValue && current.Next != nil {
			if current.Next == l.Tail {
				l.Tail = current
			}
			current.Next = current.Next.Next
			l.Count--
			return
		}
		current = current.Next
	}
}

func (l *SinglyLinkedList) RemoveBefore(targetValue string) {
	if l.Head == nil || l.Head.Next == nil {
		return
	}
	if l.Head.Next.Value == targetValue {
		l.PopFront()
		return
	}
	current := l.Head
	for current.Next != nil && current.Next.Next != nil {
		if current.Next.Next.Value == targetValue {
			current.Next = current.Next.Next
			l.Count--
			return
		}
		current = current.Next
	}
}

func (l *SinglyLinkedList) Serialize() string {
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

func (l *SinglyLinkedList) Deserialize(str string) {
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
