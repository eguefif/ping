CC = gcc
CFLAGS = -Wall -Wextra -O2

SRC_DIR := src
SRCS := $(wildcard $(SRC_DIR)/*.c)
OBJS := $(SRCS:$(SRC_DIR)/%.c=%.o)

TARGET = ping

all: $(TARGET)


$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $(OBJS)

%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean
run:
	./ping 192.168.2.1
