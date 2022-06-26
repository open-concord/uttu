G=g++ -std=c++20 -c
H=inc/csp.hpp
default: $(H)
	$(G) src/csp.cpp -o bin/csp.o; cp bin/csp.o ../../build/bin/.



# == this is what you would do with another protocol, but csp is provided as default ==
#ar cr bin/libcsp.a bin/csp.o
#ranlib bin/libcsp.a
