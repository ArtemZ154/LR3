cmake_minimum_required(VERSION 3.10)
project(DBMS_Lab_Project)

# Устанавливаем стандарт C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# --- Настройка Clang-Tidy ---
# Эта команда найдет clang-tidy и будет запускать его при каждой сборке.
# Предупреждения будут выводиться вместе с ошибками компиляции.
find_program(CLANG_TIDY_EXE clang-tidy)
if(CLANG_TIDY_EXE)
    set(CMAKE_CXX_CLANG_TIDY "${CLANG_TIDY_EXE}")
    message(STATUS "Clang-Tidy found and enabled: ${CLANG_TIDY_EXE}")
else()
    message(WARNING "Clang-Tidy not found. Linter checks will be skipped.")
endif()


# Включаем текущую директорию в пути поиска заголовков,
# чтобы работали инклюды вида #include "DataStructures/Array.h"
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# Определяем исходные файлы для структур данных
set(DATA_STRUCTURES_SRCS
        DataStructures/Array.cpp
        DataStructures/Stack.cpp
        DataStructures/Queue.cpp
        DataStructures/SinglyLinkedList.cpp
        DataStructures/DoublyLinkedList.cpp
        DataStructures/FullBinaryTree.cpp
        DataStructures/Set.cpp
        DataStructures/LFUCache.cpp
        DataStructures/HashTableChaining.cpp
        DataStructures/HashTableOpenAddr.cpp
)

# Определяем основные исходные файлы проекта
set(MAIN_SRCS
        main.cpp
        DBMS.cpp
        StorageManager.cpp
        CommandParser.cpp
)

# Создаем исполняемый файл lab_dbms из всех исходников
add_executable(lab_dbms ${MAIN_SRCS} ${DATA_STRUCTURES_SRCS})

# --- Настройка Google Test ---
include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/release-1.12.1.zip
)
set(FETCHCONTENT_TRY_FIND_PACKAGE_MODE NEVER) 
FetchContent_MakeAvailable(googletest)

# Включаем систему тестов CTest
enable_testing()

# Определяем исходные файлы для тестов
set(TEST_SRCS
        tests/DBMS_test.cpp
        tests/CommandParser_test.cpp
        tests/StorageManager_test.cpp
        tests/Array_test.cpp
        tests/Stack_test.cpp
        tests/Queue_test.cpp
        tests/SinglyLinkedList_test.cpp
        tests/DoublyLinkedList_test.cpp
        tests/FullBinaryTree_test.cpp
        tests/Set_test.cpp
        tests/LFUCache_test.cpp
        tests/HashTableChaining_test.cpp
        tests/HashTableOpenAddr_test.cpp
        tests/Set_Int_test.cpp
)

# Исходники, которые нужны для тестов (без main.cpp)
set(TEST_DEPENDENCY_SRCS
        DBMS.cpp
        StorageManager.cpp
        CommandParser.cpp
        ${DATA_STRUCTURES_SRCS}
)

# Создаем исполняемый файл для тестов
add_executable(runTests ${TEST_SRCS} ${TEST_DEPENDENCY_SRCS})

# Линкуем тесты с Google Test
target_link_libraries(
        runTests
        PRIVATE
        GTest::gtest_main
)

# Интеграция с CTest для автоматического обнаружения тестов
include(GoogleTest)
gtest_discover_tests(runTests)

# --- Настройка Code Coverage (LLVM) ---
if(CMAKE_BUILD_TYPE STREQUAL "Debug" OR NOT CMAKE_BUILD_TYPE)
    message(STATUS "LLVM code coverage flags enabled for Debug build.")
    target_compile_options(runTests PRIVATE -fprofile-instr-generate -fcoverage-mapping)
    target_link_options(runTests PRIVATE -fprofile-instr-generate)
endif()
