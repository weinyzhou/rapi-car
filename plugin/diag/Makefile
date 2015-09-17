 CC = gcc

XX = g++
CFLAGS = -Wall -O -std=c99 -I./ 
LDFLAGS = -lwiringPi -lpthread
TARGET = ./diag
EXECPERM =          755
DESTDIR=/usr/local/bin

%.o: %.c
	$(CC) $(CFLAGS)  -c $< -o $@ 

%.o:%.cpp
	$(XX) $(CFLAGS)  -c $< -o $@



SOURCES = $(wildcard *.c *.cpp)
OBJS = $(patsubst %.c,%.o,$(patsubst %.cpp,%.o,$(SOURCES)))


$(TARGET) : $(OBJS)
	$(XX) $(OBJS) -o $(TARGET) $(LDFLAGS)
	chmod a+x $(TARGET)

install: $(TARGET)
	mkdir -p $(DESTDIR)
	cp $(TARGET) $(DESTDIR)
	chmod $(EXECPERM) $(DESTDIR)/$(TARGET)

clean:
	rm -rf *.o diag