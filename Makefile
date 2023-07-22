CStackRunner: Functions.o CStack.o Factory.o main.o
	wg++ -fno-elide-constructors -std=c++11 Factory.o Functions.o CStack.o main.o -fopenmp

main.o: main.cpp CStack.h Factory.h Functions.h
	wg++ -fno-elide-constructors -std=c++11  main.cpp -fopenmp -c

CStack.o: CStack.cpp CStack.h Factory.h
	wg++ -fno-elide-constructors -std=c++11  CStack.cpp -fopenmp -c

Factory.o: Factory.cpp Factory.h CStack.h
	wg++ -fno-elide-constructors -std=c++11  Factory.cpp -fopenmp -c

Functions.o: Functions.cpp Functions.h
	wg++ -fno-elide-constructors -std=c++11  Functions.cpp -fopenmp -c

clean:
	rm -f *.o
	rm -f *.out
	rm -f file*