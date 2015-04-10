
CC=g++

LIBDIR=../netio
LIB=$(LIBDIR)/libnetio.a
LIBINC=-I$(LIBDIR)/include
CFLAGS =-c -O3 $(LIBINC)
CCFLAGS=-c -std=c++11 -O3 $(LIBINC)
#LDFLAGS=-Wl,--no-as-needed -lpthread -lrt -lnetio
LDFLAGS=-pthread -lrt -lnetio

vpath %.hpp ../netio/include

OBJS=Proxy.o 

SRVOBJS=Proxy.o
CLIOBJS=Client.o

SRVTRG=proxy
CLITRG=client

$(SRVTRG) $(CLITRG):$(SRVOBJS) $(CLIOBJS) $(LIB)
	$(CC) -o $(SRVTRG) $(SRVOBJS) -L $(LIBDIR) $(LDFLAGS) 
	$(CC) -o $(CLITRG) $(CLIOBJS) -L $(LIBDIR) $(LDFLAGS)

$(LIB):
	make -C $(LIBDIR)

%.o : %.c
	$(CC) $(CFLAGS) $<

%.o : %.cpp
	$(CC) $(CCFLAGS) $<

%.d : %.cpp
	set -e; rm -f $@; \
	$(CC) -MM $(CCFLAGS) $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$

%.d: %.c
	set -e; rm -f $@; \
	$(CC) -MM $(CFLAGS) $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$

-include $(LIBOBJS:.o=.d)


.PHONY:clean
clean:
	rm -f $(SRVTRG) $(CLITRG) $(SRVOBJS) $(CLIOBJS) $(CLIOBJS:.o=.d) $(SRVOBJS:.o=.d) core *.log
	make -C $(LIBDIR) clean
