package datastructures

import "bytes"

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
	var buf bytes.Buffer
	var helper func(*BTNode)
	helper = func(n *BTNode) {
		if n == nil {
			buf.WriteByte(0)
			return
		}
		buf.WriteByte(1)
		WriteString(&buf, n.Data)
		helper(n.Left)
		helper(n.Right)
	}
	helper(t.Root)
	return buf.String()
}

func (t *FullBinaryTree) Deserialize(str string) {
	t.Root = nil
	if str == "" {
		return
	}
	buf := bytes.NewBufferString(str)
	var helper func() *BTNode
	helper = func() *BTNode {
		b, err := buf.ReadByte()
		if err != nil || b == 0 {
			return nil
		}
		val, _ := ReadString(buf)
		node := &BTNode{Data: val}
		node.Left = helper()
		node.Right = helper()
		return node
	}
	t.Root = helper()
}
