INC = -I./../build/exe/inc
LINK = -L./../build/exe -luttu -lpthread -lcryptopp

GCC = g++ -std=c++20

# sec
s:
	$(GCC) $(INC) sec.cpp -o bin/s.out $(LINK)
	
# flag
f:
	$(GCC) $(INC) flag.cpp -o bin/f.out $(LINK)

# timeout
t:
	$(GCC) $(INC) tout.cpp -o bin/t.out $(LINK)
