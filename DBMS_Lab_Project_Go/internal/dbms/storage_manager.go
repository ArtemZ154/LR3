package dbms

import (
	"dbms_lab_project/internal/datastructures"
	"io"
	"os"
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
	total, err := datastructures.ReadSize(file)
	if err != nil {
		if err == io.EOF {
			return nil
		}
		return err
	}

	for i := 0; i < total; i++ {
		typeStr, err := datastructures.ReadString(file)
		if err != nil { return err }
		name, err := datastructures.ReadString(file)
		if err != nil { return err }
		data, err := datastructures.ReadString(file)
		if err != nil { return err }
		db.LoadStructure(typeStr, name, data)
	}
	return nil
}

func (sm *StorageManager) Save(db *DBMS) error {
	file, err := os.Create(sm.filepath)
	if err != nil {
		return err
	}
	defer file.Close()

	_, err = file.WriteString(db.SerializeAll())
	return err
}
	if err != nil {
		return err
	}
	defer file.Close()

	content := db.SerializeAll()
	_, err = file.WriteString(content)
	return err
}
