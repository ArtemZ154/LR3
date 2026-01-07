package datastructures

import (
	"bytes"
	"container/list"
)

type ValFreq struct {
	Value string
	Freq  int
}

type LFUCache struct {
	capacity      int
	size          int
	minFreq       int
	keyToValFreq  map[string]ValFreq
	freqToKeys    map[int]*list.List
	keyToIterator map[string]*list.Element
}

func NewLFUCache(cap int) *LFUCache {
	return &LFUCache{
		capacity:      cap,
		size:          0,
		minFreq:       0,
		keyToValFreq:  make(map[string]ValFreq),
		freqToKeys:    make(map[int]*list.List),
		keyToIterator: make(map[string]*list.Element),
	}
}

func (c *LFUCache) updateFrequency(key string) {
	valFreq := c.keyToValFreq[key]
	freq := valFreq.Freq
	c.keyToValFreq[key] = ValFreq{Value: valFreq.Value, Freq: freq + 1}

	// Remove from old freq list
	oldList := c.freqToKeys[freq]
	elem := c.keyToIterator[key]
	oldList.Remove(elem)
	if oldList.Len() == 0 {
		delete(c.freqToKeys, freq)
		if c.minFreq == freq {
			c.minFreq++
		}
	}

	// Add to new freq list
	newFreq := freq + 1
	if _, exists := c.freqToKeys[newFreq]; !exists {
		c.freqToKeys[newFreq] = list.New()
	}
	newList := c.freqToKeys[newFreq]
	newElem := newList.PushBack(key)
	c.keyToIterator[key] = newElem
}

func (c *LFUCache) evict() {
	if c.size == 0 {
		return
	}
	minList := c.freqToKeys[c.minFreq]
	evictElem := minList.Front() // LRU in this freq bucket
	evictKey := evictElem.Value.(string)
	minList.Remove(evictElem)
	if minList.Len() == 0 {
		delete(c.freqToKeys, c.minFreq)
	}

	delete(c.keyToValFreq, evictKey)
	delete(c.keyToIterator, evictKey)
	c.size--
}

func (c *LFUCache) Get(key string) string {
	if _, exists := c.keyToValFreq[key]; !exists {
		return "" // Or error
	}
	val := c.keyToValFreq[key].Value
	c.updateFrequency(key)
	return val
}

func (c *LFUCache) Set(key, value string) {
	if c.capacity == 0 {
		return
	}
	if _, exists := c.keyToValFreq[key]; exists {
		c.keyToValFreq[key] = ValFreq{Value: value, Freq: c.keyToValFreq[key].Freq}
		c.updateFrequency(key)
		return
	}

	if c.size >= c.capacity {
		c.evict()
	}

	c.keyToValFreq[key] = ValFreq{Value: value, Freq: 1}
	if _, exists := c.freqToKeys[1]; !exists {
		c.freqToKeys[1] = list.New()
	}
	c.freqToKeys[1].PushBack(key)
	c.keyToIterator[key] = c.freqToKeys[1].Back()
	c.minFreq = 1
	c.size++
}

func (c *LFUCache) Serialize() string {
	var buf bytes.Buffer
	WriteInt(&buf, c.capacity)
	WriteSize(&buf, len(c.keyToValFreq))
	for key, vf := range c.keyToValFreq {
		WriteString(&buf, key)
		WriteString(&buf, vf.Value)
		WriteInt(&buf, vf.Freq)
	}
	return buf.String()
}

func (c *LFUCache) Deserialize(str string) {
	c.size = 0
	c.minFreq = 0
	c.keyToValFreq = make(map[string]ValFreq)
	c.freqToKeys = make(map[int]*list.List)
	c.keyToIterator = make(map[string]*list.Element)

	if str == "" {
		return
	}
	buf := bytes.NewBufferString(str)
	c.capacity, _ = ReadInt(buf)
	count, _ := ReadSize(buf)
	for i := 0; i < count; i++ {
		key, _ := ReadString(buf)
		val, _ := ReadString(buf)
		freq, _ := ReadInt(buf)
		c.internalSet(key, val, freq)
	}
}

func (c *LFUCache) internalSet(key, value string, freq int) {
	c.keyToValFreq[key] = ValFreq{Value: value, Freq: freq}
	if _, exists := c.freqToKeys[freq]; !exists {
		c.freqToKeys[freq] = list.New()
	}
	c.freqToKeys[freq].PushBack(key)
	c.keyToIterator[key] = c.freqToKeys[freq].Back()
	c.size++
	// Recompute minFreq is hard here without scanning all freqs.
	// But usually deserialize is followed by usage.
	// Let's try to maintain minFreq.
	if c.minFreq == 0 || freq < c.minFreq {
		c.minFreq = freq
	}
}
