CPP 	= g++ -g --std=c++11 -march=corei7 -Wno-return-local-addr -fPIC -rdynamic -fmax-errors=10 -I/usr/local/include
LDFLAGS = #-L/usr/local/lib -lgmpxx -lgmp -lflint
MOD 	= ../base.so

all: $(MOD)

%.o:%.cpp %.hpp
	$(CPP) -c $< -o $@

$(MOD): init.cpp array.o kernel.o module.o string.o integer.o
	$(CPP) -shared  $(LDFLAGS) $^ -o $@

clean:
	-$(RM) *~
	-$(RM) $(MOD) *.o
