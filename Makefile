all: webserver

OBJS = webserver.o request.o
CC = clang
CFLAGS = -g -Wall

webserver: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

request.o: request.h

webserver.o: request.h

clean:
	rm -f webserver $(OBJS)