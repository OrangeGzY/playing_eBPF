SRC = fs.c
CC = gcc
CFLAGS = -g  
CFLAGS += -I/root/playing_eBPF
#CFLAGS += -I/kernel-src/tools/lib
#CFLAGS += -I/kernel-src/tools/perf
#CFLAGS += -I/kernel-src/tools/include
#CFLAGS += -I/kernel-src/tools/testing/selftests/bpf
#CFLAGS += -I/kernel-src/tools/lib/bpf
BIN = fs
OBJS = $(SRC:%.c=%.o)
all: $(BIN)
$(BIN): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm -rf *.o $(BIN)
