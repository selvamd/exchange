CC = g++ 
CPPFLAGS = -DHAVE_CONFIG_H  -D_REENTRANT \
	-std=c++17 -g -O2 -fvisibility=hidden \
	-Wall -Wno-overloaded-virtual  -Wno-deprecated-declarations \
	-I. -I../include -I../../libinclude/phoenix -I../../libinclude/quickfix
LDFLAGS = -L../../lib -lphoenix -lquickfix -lcurl -lm 

PROGS = matcher 

all : $(LIB) $(PROGS) 
	- rm -f *.o core > /dev/null 2>&1

$(PROGS) : %:%.cpp $(OBJS) 
	$(CC) $@.cpp $(CPPFLAGS) $(LDFLAGS) -o ../../bin/$@

clean : 
	- rm -f *.o core > /dev/null 2>&1
	- rm -f $(PROGS) 


