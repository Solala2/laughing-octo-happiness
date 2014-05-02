CC = gcc
CCFLAGS =

LD = gcc
LDFLAGS = -lnsl -lresolv

RM = rm -rf

SRCS = $(shell find . -type f -iname "*.c")
OBJS = $(SRCS:%.c=%.o)

OUTPUT = tcpClient

all: $(OUTPUT)

test: $(OUTPUT)
	./$(OUTPUT)

clean:
	$(RM) $(OBJS) $(OUTPUT)

%.o: %.c
	$(CC) -c $(CCFLAGS) -o$@ $<

$(OUTPUT): $(OBJS)
	$(LD) $(LDFLAGS) -o$@ $<

