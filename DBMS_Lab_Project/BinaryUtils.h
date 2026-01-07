#pragma once
#include <string>
#include <sstream>
#include <iostream>

inline void writeInt(std::ostream& os, int value) {
    os.write(reinterpret_cast<const char*>(&value), sizeof(value));
}

inline void writeSize(std::ostream& os, size_t value) {
    uint32_t v = static_cast<uint32_t>(value);
    os.write(reinterpret_cast<const char*>(&v), sizeof(v));
}

inline void writeString(std::ostream& os, const std::string& str) {
    writeSize(os, str.size());
    if (!str.empty()) {
        os.write(str.data(), str.size());
    }
}

inline void readInt(std::istream& is, int& value) {
    is.read(reinterpret_cast<char*>(&value), sizeof(value));
}

inline void readSize(std::istream& is, size_t& value) {
    uint32_t v;
    is.read(reinterpret_cast<char*>(&v), sizeof(v));
    value = static_cast<size_t>(v);
}

inline void readString(std::istream& is, std::string& str) {
    size_t len;
    readSize(is, len);
    if (len > 0) {
        str.resize(len);
        is.read(&str[0], len);
    } else {
        str.clear();
    }
}

// Generic serializer for types
template<typename T>
void writeValue(std::ostream& os, const T& value);

template<>
inline void writeValue<std::string>(std::ostream& os, const std::string& value) {
    writeString(os, value);
}

template<>
inline void writeValue<int>(std::ostream& os, const int& value) {
    writeInt(os, value);
}

// Generic deserializer for types
template<typename T>
void readValue(std::istream& is, T& value);

template<>
inline void readValue<std::string>(std::istream& is, std::string& value) {
    readString(is, value);
}

template<>
inline void readValue<int>(std::istream& is, int& value) {
    readInt(is, value);
}
