CC=g++

LIBDIR=../netio
LIB=$(LIBDIR)/libnetio.a
LIBINC=-I$(LIBDIR)/include -I./http-parser
CFLAGS =-c -g $(LIBINC)
CCFLAGS=-c -std=c++11 -g $(LIBINC)
#LDFLAGS=-Wl,--no-as-needed -lpthread -lrt -lnetio
LDFLAGS=-pthread -lrt -lnetio

vpath %.hpp ../netio/include
vpath %.c ./http-parser

SRVOBJS=http_parser.o HttpSession.o HttpServer.o ResponseHelper.o main.o

SRVTRG=main

$(SRVTRG) :$(SRVOBJS) $(LIB)
	$(CC) -o $(SRVTRG) $(SRVOBJS) -L $(LIBDIR) $(LDFLAGS) 

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
