package datastructures

import (
	"bytes"
	"errors"
)

type Array struct {
	data []string
}

func NewArray() *Array {
	return &Array{
		data: make([]string, 0),
	}
}

func (a *Array) PushBack(value string) {
	a.data = append(a.data, value)
}

func (a *Array) Insert(index int, value string) error {
	if index < 0 || index > len(a.data) {
		return errors.New("index out of bounds")
	}
	if index == len(a.data) {
		a.PushBack(value)
		return nil
	}
	a.data = append(a.data[:index+1], a.data[index:]...)
	a.data[index] = value
	return nil
}

func (a *Array) Get(index int) (string, error) {
	if index < 0 || index >= len(a.data) {
		return "", errors.New("index out of bounds")
	}
	return a.data[index], nil
}

func (a *Array) Remove(index int) error {
	if index < 0 || index >= len(a.data) {
		return errors.New("index out of bounds")
	}
	a.data = append(a.data[:index], a.data[index+1:]...)
	return nil
}

func (a *Array) Set(index int, value string) error {
	if index < 0 || index >= len(a.data) {
		return errors.New("index out of bounds")
	}
	a.data[index] = value
	return nil
}

func (a *Array) Size() int {
	return len(a.data)
}

func (a *Array) Clear() {
	a.data = make([]string, 0)
}

func (a *Array) Serialize() string {
	var buf bytes.Buffer
	WriteSize(&buf, len(a.data))
	for _, s := range a.data {
		WriteString(&buf, s)
	}
	return buf.String()
}

func (a *Array) Deserialize(str string) {
	a.Clear()
	if str == "" {
		return
	}
	buf := bytes.NewBufferString(str)
	count, _ := ReadSize(buf)
	for i := 0; i < count; i++ {
		s, _ := ReadString(buf)
		a.PushBack(s)
	}
}
