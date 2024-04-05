CC=g++-13
CFLAGS=-Wall -std=gnu++17 -fopenmp
TARGET=prog

all: $(TARGET)

$(TARGET): main.cpp
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm -f $(TARGET)
