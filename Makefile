CC=g++
CFLAGS=-Wall -std=gnu++17 -fopenmp
TARGET=prog
SRCS=$(wildcard *.cpp)
OBJS=$(SRCS:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@
	rm -f $(OBJS)

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJS)
