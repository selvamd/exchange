CC = g++ 
CPPFLAGS = -DHAVE_CONFIG_H  -D_REENTRANT \
	-std=c++17 -g -O2 -fvisibility=hidden \
	-Wall -Wno-overloaded-virtual  -Wno-deprecated-declarations \
	-Imain -Iinclude -I../libinclude/phoenix -I../libinclude/quickfix 
LDFLAGS = -L../lib -lphoenix -lquickfix -lcurl -lm 

PROGS = exchcli matcher fixgateway
OBJS = qfixapp.o 

all : $(LIB) $(PROGS) 
	- rm -f *.o core > /dev/null 2>&1

$(PROGS) : %:%.cpp $(OBJS) 
	$(CC) $@.cpp $(CPPFLAGS) $(OBJS) $(LDFLAGS) -o $@

clean : 
	- rm -f *.o core > /dev/null 2>&1
	- rm -f $(PROGS) 


