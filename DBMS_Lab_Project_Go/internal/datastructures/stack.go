package datastructures

import (
	"bytes"
	"errors"
)

type Stack struct {
	data []string
}

func NewStack() *Stack {
	return &Stack{
		data: make([]string, 0),
	}
}

func (s *Stack) Push(value string) {
	s.data = append(s.data, value)
}

func (s *Stack) Pop() (string, error) {
	if len(s.data) == 0 {
		return "", errors.New("stack is empty")
	}
	lastIndex := len(s.data) - 1
	val := s.data[lastIndex]
	s.data = s.data[:lastIndex]
	return val, nil
}

func (s *Stack) Peek() (string, error) {
	if len(s.data) == 0 {
		return "", errors.New("stack is empty")
	}
	return s.data[len(s.data)-1], nil
}

func (s *Stack) Empty() bool {
	return len(s.data) == 0
}

func (s *Stack) Serialize() string {
	var buf bytes.Buffer
	WriteSize(&buf, len(s.data))
	for _, v := range s.data {
		WriteString(&buf, v)
	}
	return buf.String()
}

func (s *Stack) Deserialize(str string) {
	s.data = make([]string, 0)
	if str == "" {
		return
	}
	buf := bytes.NewBufferString(str)
	count, _ := ReadSize(buf)
	for i := 0; i < count; i++ {
		v, _ := ReadString(buf)
		s.Push(v)
	}
}
