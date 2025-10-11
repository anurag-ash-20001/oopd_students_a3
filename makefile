CXX = g++
CXXFLAGS = -std=c++17 -O2
TARGET = oopd_students_sys
SRC = main_sys.cpp
HEADERS = syscalls.h io_sys.h file_sys.h str_sys.h vector_sys.h except_sys.h student_sys.h studentdb_sys.h trie_sys.h sorting_sys.h stddef_sys.h

$(TARGET): $(SRC) $(HEADERS)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET)

clean:
	rm -f $(TARGET)
