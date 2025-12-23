package main

import (
	"bufio"
	"dbms_lab_project/internal/dbms"
	"fmt"
	"os"
)

func main() {
	db := dbms.NewDBMS()
	sm := dbms.NewStorageManager("database.txt")

	if err := sm.Load(db); err != nil {
		fmt.Println("Error loading database:", err)
	}

	scanner := bufio.NewScanner(os.Stdin)
	for {
		fmt.Print("> ")
		if !scanner.Scan() {
			break
		}
		line := scanner.Text()
		if line == "EXIT" {
			break
		}

		parts := dbms.Parse(line)
		result := db.Execute(parts)
		fmt.Println(result)
	}

	if err := sm.Save(db); err != nil {
		fmt.Println("Error saving database:", err)
	}
}
