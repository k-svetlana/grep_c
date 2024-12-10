#!/bin/bash

echo "Test 'pattern + file'"
diff <(./grep and test1.txt) <(grep and test1.txt)

echo "Test 'e + pattern + file'"
diff <(./grep -e and test1.txt) <(grep -e and test1.txt)

echo "Test 'e + pattern + e + pattern + file'"
diff <(./grep -e and -e several test1.txt) <(grep -e and -e several test1.txt)

echo "Test 'e + pattern + e + pattern + two files'"
diff <(./grep -e and -e several test1.txt test2.txt) <(grep -e and -e several test1.txt test2.txt)

echo "Test 'i + pattern + file'"
diff <(./grep -i aNd test1.txt) <(grep -i aNd test1.txt)

echo "Test 'v + pattern + file'"
diff <(./grep -v and test1.txt) <(grep -v and test1.txt)

echo "Test 'c + pattern + file'"
diff <(./grep -c and test1.txt) <(grep -c and test1.txt)

echo "Test 'l + pattern + two files'"
diff <(./grep -l and test1.txt test2.txt) <(grep -l and test1.txt test2.txt)

echo "Test 'n + pattern + two files'"
diff <(./grep -n several test1.txt test2.txt) <(grep -n several test1.txt test2.txt)
