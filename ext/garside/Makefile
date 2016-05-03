CPP 	= g++ -g --std=c++11 -march=corei7 -Wno-return-local-addr -fPIC -rdynamic -fmax-errors=10 -I/usr/local/include
LDFLAGS = #-L/usr/local/lib -lgmpxx -lgmp -lflint
MOD 	= ../garside.so
APP	= garside

all: $(MOD)

%.o:%.cpp %.hpp
	$(CPP) -c $< -o $@

$(MOD): init.cpp stacked_list.o braids.o monoid.o 
	$(CPP) -shared  $(LDFLAGS) $^ -o $@

$(APP): main.cpp stacked_list.o braids.o monoid.o
	$(CPP) $(LDFLAGS) $^ -o $@

clean:
	-$(RM) *~
	-$(RM) *.o
	-$(RM) $(MOD) 
