G=g++ -std=c++20 -c
H=inc/tcp.hpp
default: $(H)
	$(G) src/tcp.cpp -o bin/tcp.o
	ar cr bin/libutcp.a bin/tcp.o
	ranlib bin/libutcp.a
