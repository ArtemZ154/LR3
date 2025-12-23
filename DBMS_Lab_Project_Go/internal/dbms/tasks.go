package dbms

import (
	"dbms_lab_project/internal/datastructures"
	"math"
	"strconv"
	"strings"
)

func solveAsteroids(input *datastructures.Array) *datastructures.Array {
	stack := datastructures.NewStack()
	var asteroids []int

	for i := 0; i < input.Size(); i++ {
		valStr, _ := input.Get(i)
		val, err := strconv.Atoi(valStr)
		if err == nil {
			asteroids = append(asteroids, val)
		}
	}

	for _, ast := range asteroids {
		currentAst := ast
		for currentAst < 0 && !stack.Empty() {
			topStr, _ := stack.Peek()
			top, _ := strconv.Atoi(topStr)
			if top > 0 {
				if top > -currentAst {
					// Top is bigger
					newSize := top - (-currentAst)
					stack.Pop()
					stack.Push(strconv.Itoa(newSize))
					currentAst = 0
				} else if top < -currentAst {
					// Current is bigger
					newSize := (-currentAst) - top
					stack.Pop()
					currentAst = -newSize
				} else {
					// Equal
					stack.Pop()
					currentAst = 0
				}
			} else {
				break
			}
		}
		if currentAst != 0 {
			stack.Push(strconv.Itoa(currentAst))
		}
	}

	result := datastructures.NewArray()
	// Stack is reversed (Top -> Bottom), we need Bottom -> Top
	// My Stack implementation: Serialize gives Bottom -> Top.
	// But here we want to push to Array in order.
	// Stack Pop gives Top.
	// So we pop all to a temp stack to reverse, then pop to array.
	tempStack := datastructures.NewStack()
	for !stack.Empty() {
		val, _ := stack.Pop()
		tempStack.Push(val)
	}
	for !tempStack.Empty() {
		val, _ := tempStack.Pop()
		result.PushBack(val)
	}
	return result
}

func solveMinPartition(input *datastructures.Set, s1, s2 *datastructures.Set) string {
	var nums []int
	var numStrs []string
	totalSum := 0

	elements := input.GetElements()
	for _, s := range elements {
		num, err := strconv.Atoi(s)
		if err == nil {
			nums = append(nums, num)
			numStrs = append(numStrs, s)
			totalSum += num
		}
	}

	n := len(nums)
	targetSum := totalSum / 2
	dp := make([]bool, targetSum+1)
	parent := make([]int, targetSum+1)
	for i := range parent {
		parent[i] = -1
	}
	dp[0] = true

	for i := 0; i < n; i++ {
		for j := targetSum; j >= nums[i]; j-- {
			if dp[j-nums[i]] && !dp[j] {
				dp[j] = true
				parent[j] = i
			}
		}
	}

	s1Sum := 0
	for j := targetSum; j >= 0; j-- {
		if dp[j] {
			s1Sum = j
			break
		}
	}

	s1Indices := make([]bool, n)
	currSum := s1Sum
	for currSum > 0 && parent[currSum] != -1 {
		index := parent[currSum]
		s1Indices[index] = true
		s1.Add(numStrs[index])
		currSum -= nums[index]
	}

	for i := 0; i < n; i++ {
		if !s1Indices[i] {
			s2.Add(numStrs[i])
		}
	}

	s2Sum := totalSum - s1Sum
	diff := int(math.Abs(float64(s1Sum - s2Sum)))
	return strconv.Itoa(diff)
}

func solveFindSum(input *datastructures.Array, target int, output *datastructures.Array) bool {
	var nums []int
	var numStrs []string
	for i := 0; i < input.Size(); i++ {
		s, _ := input.Get(i)
		num, err := strconv.Atoi(s)
		if err == nil {
			nums = append(nums, num)
			numStrs = append(numStrs, s)
		}
	}

	prefixSumMap := make(map[int][]int)
	prefixSumMap[0] = append(prefixSumMap[0], -1)

	currentSum := 0
	found := false

	for i := 0; i < len(nums); i++ {
		currentSum += nums[i]
		diff := currentSum - target
		if indices, ok := prefixSumMap[diff]; ok {
			for _, startIndex := range indices {
				found = true
				var sb strings.Builder
				sb.WriteString("{")
				for j := startIndex + 1; j <= i; j++ {
					sb.WriteString(numStrs[j])
					if j != i {
						sb.WriteString(", ")
					}
				}
				sb.WriteString("}")
				output.PushBack(sb.String())
			}
		}
		prefixSumMap[currentSum] = append(prefixSumMap[currentSum], i)
	}
	return found
}

func solveLongestSubstring(s string) int {
	if s == "" {
		return 0
	}
	charMap := datastructures.NewHashTableOpenAddr(16)
	left := 0
	maxLength := 0

	for right := 0; right < len(s); right++ {
		charStr := string(s[right])
		foundIndexStr, err := charMap.Get(charStr)
		if err == nil {
			lastPos, _ := strconv.Atoi(foundIndexStr)
			if lastPos >= left {
				left = lastPos + 1
			}
		}
		charMap.Put(charStr, strconv.Itoa(right))
		if right-left+1 > maxLength {
			maxLength = right - left + 1
		}
	}
	return maxLength
}
