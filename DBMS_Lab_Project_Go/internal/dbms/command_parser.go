package dbms

import (
	"strings"
)

type CommandParser struct{}

func Parse(query string) []string {
	return strings.Fields(query)
}
