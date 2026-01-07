package datastructures
package datastructures

import (
	"encoding/binary"
	"io"
)

func WriteInt(w io.Writer, val int) error {
















































}	return string(buf), nil	}		return "", err	if _, err := io.ReadFull(r, buf); err != nil {	buf := make([]byte, length)	}		return "", nil	if length == 0 {	}		return "", err	if err != nil {	length, err := ReadSize(r)func ReadString(r io.Reader) (string, error) {}	return int(val), nil	}		return 0, err	if err := binary.Read(r, binary.LittleEndian, &val); err != nil {	var val uint32func ReadSize(r io.Reader) (int, error) {}	return int(val), nil	}		return 0, err	if err := binary.Read(r, binary.LittleEndian, &val); err != nil {	var val int32func ReadInt(r io.Reader) (int, error) {}	return nil	}		return err		_, err := w.Write([]byte(str))	if len(str) > 0 {	}		return err	if err := WriteSize(w, len(str)); err != nil {func WriteString(w io.Writer, str string) error {}	return binary.Write(w, binary.LittleEndian, uint32(val))func WriteSize(w io.Writer, val int) error {}	return binary.Write(w, binary.LittleEndian, int32(val))