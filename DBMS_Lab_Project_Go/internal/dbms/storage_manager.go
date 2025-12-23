package dbms

import (
	"bufio"
	"os"
	"strings"
)

type StorageManager struct {
	filepath string
}

func NewStorageManager(path string) *StorageManager {
	return &StorageManager{filepath: path}
}

func (sm *StorageManager) Load(db *DBMS) error {
	file, err := os.Open(sm.filepath)
	if err != nil {
		if os.IsNotExist(err) {
			return nil // File doesn't exist, start with empty DB
		}
		return err
	}
	defer file.Close()

	db.Clear()
	scanner := bufio.NewScanner(file)
	for scanner.Scan() {
		line := scanner.Text()
		parts := strings.SplitN(line, " ", 3)
		if len(parts) < 2 {
			continue
		}
		typeStr := parts[0]
		name := parts[1]
		data := ""
		if len(parts) == 3 {
			data = parts[2]
		}
		db.LoadStructure(typeStr, name, data)
	}
	return scanner.Err()
}

func (sm *StorageManager) Save(db *DBMS) error {
	file, err := os.Create(sm.filepath)
	if err != nil {
		return err
	}
	defer file.Close()

	content := db.SerializeAll()
	_, err = file.WriteString(content)
	return err
}
