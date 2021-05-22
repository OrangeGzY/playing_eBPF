SRC = fs.c
CC = gcc

CFLAGS = -g

BIN = fs

OBJS = $(SRC:%.c=%.o) ../../bpf.o

all: $(BIN)

$(BIN):$(OBJS)
	$(CC) $(CFLAGS) $^ -o $@
clean:
	rm -rf *.o $(BIN)
