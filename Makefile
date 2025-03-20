CPP_FLAGS=-std=c++17
main: main.cpp tree.o search_tree.o treenode.o
	c++ ${CPP_FLAGS} ./main.cpp ./build/treenode.o ./build/tree.o ./build/search_tree.o -o main

tree.o: tree.cpp
	c++ ${CPP_FLAGS} -c tree.cpp -o ./build/tree.o

search_tree.o: search_tree.cpp
	c++ ${CPP_FLAGS} -c search_tree.cpp -o ./build/search_tree.o

treenode.o: treenode.cpp
	c++ ${CPP_FLAGS} -c treenode.cpp -o ./build/treenode.o

clean:
	rm ./main
	rm ./build/*.o
