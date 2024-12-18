CC = g++ 
CPPFLAGS = -DHAVE_CONFIG_H -I. -I../include -I../libinclude/phoenix -I../libinclude/quickfix -std=c++11 -std=c++14 -std=c++17 -g -O2 -Wall -Wno-overloaded-virtual  -Wno-deprecated-declarations -fvisibility=hidden -D_REENTRANT
LDFLAGS = -L../lib -lphoenix -lquickfix -lcurl -lm 

#Compilation flags 
# ifeq ($(UNAME), Linux)
# CPPFLAGS = $(CCINCLUDE) -D_REENTRANT -std=c++11 -Wall -g -L$(LIB_INSTALL_DIR)
# endif
# ifeq ($(UNAME), Darwin)
# CPPFLAGS = $(CCINCLUDE) -D_REENTRANT -std=c++11 -Wall -g
# endif
PROGS = exchcli matcher fixgateway
OBJS = qfixapp.o 

all : $(LIB) $(PROGS) 
	- rm -f *.o core > /dev/null 2>&1

$(PROGS) : %:%.cpp $(OBJS) 
	$(CC) $@.cpp $(CPPFLAGS) $(OBJS) $(LDFLAGS) -o $@

%.o : %.cpp %.hpp
	$(CC) $(CPPFLAGS) -c $< -o $@

clean : 
	- rm -f *.o core > /dev/null 2>&1
	- rm -f $(PROGS) 
