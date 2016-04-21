CPP 	= g++ -fmax-errors=10
INCDIR  =  -I/usr/local/opt/readline/include -I/usr/local/include/
LIBDIR  =  -L/usr/local/opt/readline/lib -L/usr/local/lib/
CPPFLAG = -g --std=c++11 -O3 $(INCDIR)
LDFLAG	= -rdynamic -lgmpxx -lgmp -lflint -lreadline -ldl $(LIBDIR)
EXE 	= gomu


all: $(EXE)

doc: array.hpp dictionnary.hpp interpreter.hpp kernel.hpp module.hpp
	doxygen doc/Doxyfile

%.o:%.cpp %.hpp
	$(CPP) $(CPPFLAG) -o $@ -c $<

$(EXE) : module.o kernel.o interpreter.o  main.cpp
	$(CPP) $(CPPFLAG) $(LDFLAG) $^ -o $(EXE)

clean:
	-$(RM) *.o
	-$(RM) $(EXE)
