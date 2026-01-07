package datastructures

import (
	"bytes"
)

type Set struct {
	table         []Entry
	capacity      int
	numElements   int
	maxLoadFactor float64
}

func NewSet(cap int) *Set {
	if cap <= 0 {
		cap = 16
	}
	return &Set{
		table:         make([]Entry, cap),
		capacity:      cap,
		numElements:   0,
		maxLoadFactor: 0.6,
	}
}

func (s *Set) manualHash(value string) int {
	var hash uint64 = 5381
	for _, c := range value {
		hash = ((hash << 5) + hash) + uint64(c)
	}
	return int(hash % uint64(s.capacity))
}

func (s *Set) rehash() {
	oldTable := s.table
	s.capacity *= 2
	s.table = make([]Entry, s.capacity)
	s.numElements = 0

	for _, entry := range oldTable {
		if entry.State == ACTIVE {
			s.Add(entry.Value) // Value is stored in Value field of Entry, Key is unused or same
		}
	}
}

func (s *Set) Add(value string) {
	if s.Contains(value) {
		return
	}
	if float64(s.numElements)/float64(s.capacity) >= s.maxLoadFactor {
		s.rehash()
	}

	idx := s.manualHash(value)
	for i := 0; i < s.capacity; i++ {
		probeIdx := (idx + i) % s.capacity
		if s.table[probeIdx].State == EMPTY || s.table[probeIdx].State == DELETED {
			s.table[probeIdx] = Entry{Value: value, State: ACTIVE}
			s.numElements++
			return
		}
	}
}

func (s *Set) Remove(value string) {
	idx := s.manualHash(value)
	for i := 0; i < s.capacity; i++ {
		probeIdx := (idx + i) % s.capacity
		if s.table[probeIdx].State == EMPTY {
			return
		}
		if s.table[probeIdx].State == ACTIVE && s.table[probeIdx].Value == value {
			s.table[probeIdx].State = DELETED
			s.numElements--
			return
		}
	}
}

func (s *Set) Contains(value string) bool {
	idx := s.manualHash(value)
	for i := 0; i < s.capacity; i++ {
		probeIdx := (idx + i) % s.capacity
		if s.table[probeIdx].State == EMPTY {
			return false
		}
		if s.table[probeIdx].State == ACTIVE && s.table[probeIdx].Value == value {
			return true
		}
	}
	return false
}

func (s *Set) Size() int {
	return s.numElements
}

func (s *Set) Clear() {
	s.table = make([]Entry, s.capacity)
	s.numElements = 0
}

func (s *Set) GetElements() []string {
	elems := make([]string, 0, s.numElements)
	for _, entry := range s.table {
		if entry.State == ACTIVE {
			elems = append(elems, entry.Value)
		}
	}
	return elems
}

func (s *Set) Union(other *Set) *Set {
	res := NewSet(s.capacity + other.capacity)
	for _, v := range s.GetElements() {
		res.Add(v)
	}
	for _, v := range other.GetElements() {
		res.Add(v)
	}
	return res
}

func (s *Set) Intersection(other *Set) *Set {
	res := NewSet(s.capacity) // Estimate
	for _, v := range s.GetElements() {
		if other.Contains(v) {
			res.Add(v)
		}
	}
	return res
}

func (s *Set) Difference(other *Set) *Set {
	res := NewSet(s.capacity)
	for _, v := range s.GetElements() {
		if !other.Contains(v) {
			res.Add(v)
		}
	}
	return res
}

func (s *Set) Serialize() string {
	var buf bytes.Buffer
	WriteSize(&buf, s.numElements)
	for _, entry := range s.table {
		if entry.State == ACTIVE {
			WriteString(&buf, entry.Value)
		}
	}
	return buf.String()
}

func (s *Set) Deserialize(str string) {
	s.Clear()
	if str == "" {
		return
	}
	buf := bytes.NewBufferString(str)
	count, _ := ReadSize(buf)
	for i := 0; i < count; i++ {
		val, _ := ReadString(buf)
		s.Add(val)
	}
}
