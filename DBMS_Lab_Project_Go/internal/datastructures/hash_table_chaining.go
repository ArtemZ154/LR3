package datastructures

import (
	"errors"
	"fmt"
	"strings"
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
	var sb strings.Builder
	first := true
	for _, bucket := range ht.table {
		for _, kv := range bucket {
			if !first {
				sb.WriteString(" ")
			}
			sb.WriteString(fmt.Sprintf("%s:%s", kv.Key, kv.Value))
			first = false
		}
	}
	return sb.String()
}

func (ht *HashTableChaining) Deserialize(str string) {
	// Clear table
	ht.table = make([][]KV, ht.capacity)
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
