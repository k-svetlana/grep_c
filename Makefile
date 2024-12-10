CC = gcc
FLAGS = -Wall -Wextra -Werror -std=c11
TARGET = grep

all: $(TARGET)

$(TARGET): $(TARGET).c
	$(CC) $(FLAGS) $(TARGET).c -o $(TARGET)

clang:
	clang-format -i *.c *.h

cpp: 
	cppcheck --enable=all --suppress=missingIncludeSystem $(TARGET).c

test: all
	bash grep.sh > test_log.txt

clean:
	rm -f $(TARGET) *.o

rebuild: clean all
