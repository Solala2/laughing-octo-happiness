CC = gcc
CCFLAGS = 

LD = gcc
LDFLAGS = -lnsl -lresolv

SRCS = $(shell find . -type f -iname "*.c")
OBJS = $(SRCS:%.c=%.o)

OUTPUT = tcpClient

all: $(OUTPUT)

test: $(OUTPUT)
	./$(OUTPUT)

%.o: %.c
	$(CC) -c $(CCFLAGS) -o$@ $<

$(OUTPUT): $(OBJS)
	$(LD) $(LDFLAGS) -o$@ $<

