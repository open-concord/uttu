INC = -I./../build/exe/inc
LINK = -L./../build/exe -luttu -lpthread -lcryptopp

GCC = g++ -std=c++20
# net
n:
	$(GCC) $(INC) net.cpp -o bin/n.out $(LINK)

# sec
s:
	$(GCC) $(INC) sec.cpp -o bin/s.out $(LINK)
	
# protocol
p:
	$(GCC) $(INC) prot.cpp -o bin/p.out $(LINK)
