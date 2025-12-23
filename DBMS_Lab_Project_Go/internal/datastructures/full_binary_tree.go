package datastructures

import (
	"strings"
)

type BTNode struct {
	Data  string
	Left  *BTNode
	Right *BTNode
}

type FullBinaryTree struct {
	Root *BTNode
}

func NewFullBinaryTree() *FullBinaryTree {
	return &FullBinaryTree{}
}

func (t *FullBinaryTree) Insert(value string) {
	if t.Root == nil {
		t.Root = &BTNode{Data: value}
		return
	}
	queue := []*BTNode{t.Root}
	for len(queue) > 0 {
		current := queue[0]
		queue = queue[1:]

		if current.Left == nil {
			current.Left = &BTNode{Data: value}
			return
		} else {
			queue = append(queue, current.Left)
		}

		if current.Right == nil {
			current.Right = &BTNode{Data: value}
			return
		} else {
			queue = append(queue, current.Right)
		}
	}
}

func (t *FullBinaryTree) Find(value string) bool {
	return t.findRecursive(t.Root, value)
}

func (t *FullBinaryTree) findRecursive(node *BTNode, value string) bool {
	if node == nil {
		return false
	}
	if node.Data == value {
		return true
	}
	return t.findRecursive(node.Left, value) || t.findRecursive(node.Right, value)
}

func (t *FullBinaryTree) IsFull() bool {
	return t.isFullRecursive(t.Root)
}

func (t *FullBinaryTree) isFullRecursive(node *BTNode) bool {
	if node == nil {
		return true
	}
	if node.Left == nil && node.Right == nil {
		return true
	}
	if node.Left != nil && node.Right != nil {
		return t.isFullRecursive(node.Left) && t.isFullRecursive(node.Right)
	}
	return false
}

func (t *FullBinaryTree) Serialize() string {
	var sb strings.Builder
	t.serializeRecursive(t.Root, 0, &sb)
	res := sb.String()
	if len(res) > 0 && res[len(res)-1] == '|' {
		res = res[:len(res)-1]
	}
	return res
}

func (t *FullBinaryTree) serializeRecursive(node *BTNode, depth int, sb *strings.Builder) {
	if node == nil {
		return
	}
	sb.WriteString(strings.Repeat(".", depth))
	sb.WriteString(node.Data)
	sb.WriteString("|")
	t.serializeRecursive(node.Left, depth+1, sb)
	t.serializeRecursive(node.Right, depth+1, sb)
}

func (t *FullBinaryTree) Deserialize(str string) {
	t.Root = nil
	if str == "" {
		return
	}
	lines := strings.Split(str, "|")
	if len(lines) == 0 {
		return
	}

	t.Root = &BTNode{Data: lines[0]} // First line has 0 dots
	parentStack := []*BTNode{t.Root}

	for i := 1; i < len(lines); i++ {
		line := lines[i]
		depth := 0
		for j := 0; j < len(line); j++ {
			if line[j] == '.' {
				depth++
			} else {
				break
			}
		}
		value := line[depth:]
		newNode := &BTNode{Data: value}

		if depth-1 < 0 || depth-1 >= len(parentStack) {
			// Should not happen if serialized correctly
			continue
		}
		parent := parentStack[depth-1]

		if parent.Left == nil {
			parent.Left = newNode
		} else {
			parent.Right = newNode
		}

		if len(parentStack) <= depth {
			parentStack = append(parentStack, newNode)
		} else {
			parentStack[depth] = newNode
		}
	}
}
