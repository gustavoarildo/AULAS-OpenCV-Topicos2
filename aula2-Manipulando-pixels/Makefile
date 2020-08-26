.SUFFIXES:
.SUFFIXES: .cpp

GCC = g++	# or clang++

.cpp:
	$(GCC) -Wall -Wunused -std=c++11 -O2 $< -o $@ `pkg-config --libs opencv`

clean:
	rm -f *.o $@
