PROGRAM = main
MAIN = main.cpp
SOURCE = bptree.cpp dataSystem.cpp
TEST_1 = test1
T1_CPP = test1.cpp
TEST_2 = test2
T2_CPP = test2.cpp
TEST_3 = test3
T3_CPP = test3.cpp
TEST_4 = test4
T4_CPP = test4.cpp

all:	main	test1	test2	test3	test4

main:
	g++ -g -o $(PROGRAM) $(MAIN) $(SOURCE)

test1:
	g++ -g -o $(TEST_1) $(T1_CPP) $(SOURCE)

test2:
	g++ -g -o $(TEST_2) $(T2_CPP) $(SOURCE)

test3:
	g++ -g -o $(TEST_3) $(T3_CPP) $(SOURCE)

test4:
	g++ -g -o $(TEST_4) $(T4_CPP) $(SOURCE)

clean:
	rm $(PROGRAM) $(TEST_1) $(TEST_2) $(TEST_3) $(TEST_4)