package datastructures

import (
	"errors"
	"fmt"
	"strings"
)

type State int

const (
	EMPTY State = iota
	ACTIVE
	DELETED
)

type Entry struct {
	Key   string
	Value string
	State State
}

type HashTableOpenAddr struct {
	table         []Entry
	capacity      int
	numElements   int
	maxLoadFactor float64
}

func NewHashTableOpenAddr(cap int) *HashTableOpenAddr {
	if cap <= 0 {
		cap = 16
	}
	return &HashTableOpenAddr{
		table:         make([]Entry, cap),
		capacity:      cap,
		numElements:   0,
		maxLoadFactor: 0.6,
	}
}

func (ht *HashTableOpenAddr) manualHash(key string) int {
	var hash uint64 = 5381
	for _, c := range key {
		hash = ((hash << 5) + hash) + uint64(c)
	}
	return int(hash % uint64(ht.capacity))
}

func (ht *HashTableOpenAddr) rehash() {
	oldTable := ht.table
	ht.capacity *= 2
	ht.table = make([]Entry, ht.capacity)
	ht.numElements = 0

	for _, entry := range oldTable {
		if entry.State == ACTIVE {
			ht.Put(entry.Key, entry.Value)
		}
	}
}

func (ht *HashTableOpenAddr) Put(key, value string) {
	if float64(ht.numElements)/float64(ht.capacity) >= ht.maxLoadFactor {
		ht.rehash()
	}

	idx := ht.manualHash(key)
	for i := 0; i < ht.capacity; i++ {
		probeIdx := (idx + i) % ht.capacity
		if ht.table[probeIdx].State == EMPTY || ht.table[probeIdx].State == DELETED {
			ht.table[probeIdx] = Entry{Key: key, Value: value, State: ACTIVE}
			ht.numElements++
			return
		} else if ht.table[probeIdx].State == ACTIVE && ht.table[probeIdx].Key == key {
			ht.table[probeIdx].Value = value
			return
		}
	}
}

func (ht *HashTableOpenAddr) Get(key string) (string, error) {
	idx := ht.manualHash(key)
	for i := 0; i < ht.capacity; i++ {
		probeIdx := (idx + i) % ht.capacity
		if ht.table[probeIdx].State == EMPTY {
			return "", errors.New("Key not found")
		}
		if ht.table[probeIdx].State == ACTIVE && ht.table[probeIdx].Key == key {
			return ht.table[probeIdx].Value, nil
		}
	}
	return "", errors.New("Key not found")
}

func (ht *HashTableOpenAddr) Remove(key string) {
	idx := ht.manualHash(key)
	for i := 0; i < ht.capacity; i++ {
		probeIdx := (idx + i) % ht.capacity
		if ht.table[probeIdx].State == EMPTY {
			return
		}
		if ht.table[probeIdx].State == ACTIVE && ht.table[probeIdx].Key == key {
			ht.table[probeIdx].State = DELETED
			ht.numElements--
			return
		}
	}
}

func (ht *HashTableOpenAddr) Clear() {
	ht.table = make([]Entry, ht.capacity)
	ht.numElements = 0
}

func (ht *HashTableOpenAddr) Serialize() string {
	var sb strings.Builder
	first := true
	for _, entry := range ht.table {
		if entry.State == ACTIVE {
			if !first {
				sb.WriteString(" ")
			}
			sb.WriteString(fmt.Sprintf("%s:%s", entry.Key, entry.Value))
			first = false
		}
	}
	return sb.String()
}

func (ht *HashTableOpenAddr) Deserialize(str string) {
	ht.Clear()
	if str == "" {
		return
	}
	pairs := strings.Split(str, " ")
	for _, p := range pairs {
		kv := strings.SplitN(p, ":", 2)
		if len(kv) == 2 {
			ht.Put(kv[0], kv[1])
		}
	}
}
