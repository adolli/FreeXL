PROGNAME = 007
OUTNAME = ./$(PROGNAME)_bin
LIBS = -lstdc++

SRC=\
$(PROGNAME).cpp 

OBJ= $(SRC:.cpp=.o)

$(PROGNAME):$(OBJ) 
	g++ $^ -o $(OUTNAME) $(LIBS)

%.o:	%.cpp
	@echo \< $*.cpp \>
	g++ -c $*.cpp -o $@
