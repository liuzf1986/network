
CC=g++

LIBDIR=../netio
LIB=$(LIBDIR)/libnetio.a
LIBINC=$(LIBDIR)/include
CFLAGS =-c -O3 -I$(LIBINC)
CCFLAGS=-c -std=c++11 -O3 -I$(LIBINC)
LDFLAGS=-Wl,--no-as-needed -lpthread -lrt -lnetio

vpath %.hpp ../netio/include

OBJS=test.o 

TARGET=proxy

$(TARGET):$(OBJS) $(LIB)
	$(CC) -o $(TARGET) $(OBJS) -L $(LIBDIR) $(LDFLAGS) 

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
	rm -f $(TARGET) $(OBJS) $(OBJS:.o=.d) core *.log
	make -C $(LIBDIR) clean
