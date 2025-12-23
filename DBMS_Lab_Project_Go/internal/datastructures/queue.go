package datastructures

import (
	"errors"
	"strings"
)

type Queue struct {
	data []string
}

func NewQueue() *Queue {
	return &Queue{
		data: make([]string, 0),
	}
}

func (q *Queue) Push(value string) {
	q.data = append(q.data, value)
}

func (q *Queue) Pop() (string, error) {
	if len(q.data) == 0 {
		return "", errors.New("queue is empty")
	}
	val := q.data[0]
	q.data = q.data[1:]
	return val, nil
}

func (q *Queue) Empty() bool {
	return len(q.data) == 0
}

func (q *Queue) Serialize() string {
	return strings.Join(q.data, " ")
}

func (q *Queue) Deserialize(str string) {
	q.data = make([]string, 0)
	if str == "" {
		return
	}
	parts := strings.Split(str, " ")
	for _, p := range parts {
		q.Push(p)
	}
}
