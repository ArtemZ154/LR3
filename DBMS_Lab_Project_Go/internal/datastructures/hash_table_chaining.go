package datastructures

import (
	"bytes"
	"errors"
)

type KV struct {
	Key   string
	Value string
}

type HashTableChaining struct {
	table    [][]KV
	capacity int
}

func NewHashTableChaining(cap int) *HashTableChaining {
	if cap <= 0 {
		cap = 16
	}
	return &HashTableChaining{
		table:    make([][]KV, cap),
		capacity: cap,
	}
}

func (ht *HashTableChaining) manualHash(key string) int {
	var hash uint64 = 5381
	for _, c := range key {
		hash = ((hash << 5) + hash) + uint64(c)
	}
	return int(hash % uint64(ht.capacity))
}

func (ht *HashTableChaining) Put(key, value string) {
	idx := ht.manualHash(key)
	for i, kv := range ht.table[idx] {
		if kv.Key == key {
			ht.table[idx][i].Value = value
			return
		}
	}
	ht.table[idx] = append(ht.table[idx], KV{Key: key, Value: value})
}

func (ht *HashTableChaining) Get(key string) (string, error) {
	idx := ht.manualHash(key)
	for _, kv := range ht.table[idx] {
		if kv.Key == key {
			return kv.Value, nil
		}
	}
	return "", errors.New("Key not found")
}

func (ht *HashTableChaining) Remove(key string) {
	idx := ht.manualHash(key)
	for i, kv := range ht.table[idx] {
		if kv.Key == key {
			// Remove element
			ht.table[idx] = append(ht.table[idx][:i], ht.table[idx][i+1:]...)
			return
		}
	}
}

func (ht *HashTableChaining) Serialize() string {
	var buf bytes.Buffer
	total := 0
	for _, chain := range ht.table {
		total += len(chain)
	}
	WriteSize(&buf, total)
	for _, chain := range ht.table {
		for _, kv := range chain {
			WriteString(&buf, kv.Key)
			WriteString(&buf, kv.Value)
		}
	}
	return buf.String()
}

func (ht *HashTableChaining) Deserialize(str string) {
	ht.table = make([][]KV, ht.capacity)
	if str == "" {
		return
	}
	buf := bytes.NewBufferString(str)
	count, _ := ReadSize(buf)
	for i := 0; i < count; i++ {
		k, _ := ReadString(buf)
		v, _ := ReadString(buf)
		ht.Put(k, v)
	}
}
