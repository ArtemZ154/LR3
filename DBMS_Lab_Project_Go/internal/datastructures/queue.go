package datastructures

import (
	"bytes"
	"errors"
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
	var buf bytes.Buffer
	WriteSize(&buf, len(q.data))
	for _, v := range q.data {
		WriteString(&buf, v)
	}
	return buf.String()
}

func (q *Queue) Deserialize(str string) {
	q.data = make([]string, 0)
	if str == "" {
		return
	}
	buf := bytes.NewBufferString(str)
	count, _ := ReadSize(buf)
	for i := 0; i < count; i++ {
		v, _ := ReadString(buf)
		q.Push(v)
	}
}
