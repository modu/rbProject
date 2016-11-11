bbst : bbst.o RedBlackTree.o 
	g++ -o bbst RedBlackTree.o bbst.o

RedBlackTree.o : RedBlackTree.cpp
	g++ -c RedBlackTree.cpp

bbst.o : bbst.cpp
	g++ -c bbst.cpp
cl :
	rm *.o bbst
