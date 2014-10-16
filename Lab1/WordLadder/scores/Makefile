PROGRAM = score
OBJECTS = score.o
CPPOPTIONS = -I../StanfordCPPLib
LIB  = -lStanfordCPPLib
LDOPTIONS = -L..

all: $(PROGRAM)

score: $(OBJECTS) 
	g++ -o $(PROGRAM)  $(OBJECTS) $(LIB) $(LDOPTIONS) 

score.o: score.cc  
	g++ -c $(CPPOPTIONS)  score.cc 

clean scratch: 
	rm -f *.o  $(PROGRAM)

