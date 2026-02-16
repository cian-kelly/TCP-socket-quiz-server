# Makefile for Unix Programming Quiz Client-Server Application
# Supports: client, server, debug builds, configurable IP/PORT
CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -D_GNU_SOURCE -MMD -MP
LDFLAGS =
LDLIBS =

CLIENT = client
SERVER = server

CLIENT_SRC = client.c
SERVER_SRC = server.c

CLIENT_OBJ = $(CLIENT_SRC:.c=.o)
SERVER_OBJ = $(SERVER_SRC:.c=.o)

DEPS = $(CLIENT_OBJ:.o=.d) $(SERVER_OBJ:.o=.d)

IP ?= 127.0.0.1
PORT ?= 8080

all: $(CLIENT) $(SERVER)

$(CLIENT): $(CLIENT_OBJ)
	$(CC) $(LDFLAGS) -o $@ $^ $(LDLIBS)

$(SERVER): $(SERVER_OBJ)
	$(CC) $(LDFLAGS) -o $@ $^ $(LDLIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(CLIENT_OBJ) $(SERVER_OBJ) $(DEPS)

cleanall: clean
	rm -f $(CLIENT) $(SERVER)

rebuild: cleanall all

debug: CFLAGS := -Wall -Wextra -std=c11 -D_GNU_SOURCE -O0 -g3 -MMD -MP
debug: all

run-server: $(SERVER)
	./$(SERVER) $(IP) $(PORT)

run-client: $(CLIENT)
	./$(CLIENT) $(IP) $(PORT)

help:
	@echo "Available targets:"
	@echo "  all        - Build both client and server (default)"
	@echo "  client     - Build only the client"
	@echo "  server     - Build only the server"
	@echo "  clean      - Remove object files"
	@echo "  cleanall   - Remove object files and executables"
	@echo "  rebuild    - Clean and rebuild everything"
	@echo "  debug      - Build with debug symbols (-g3 -O0)"
	@echo "  run-server - Build and run the server on 127.0.0.1:8080"
	@echo "  run-client - Build and run the client connecting to 127.0.0.1:8080"
	@echo "  help       - Display this help message"
	@echo ""
	@echo "Configurable variables:"
	@echo "  IP         - Server IP address (default: 127.0.0.1)"
	@echo "  PORT       - Server port number (default: 8080)"
	@echo ""
	@echo "Examples:"
	@echo "  make                              # Build everything"
	@echo "  make debug                        # Build with debugging symbols"
	@echo "  make run-server IP=0.0.0.0 PORT=9000"
	@echo "  make run-client IP=192.168.1.10 PORT=9000"

.PHONY: all clean cleanall rebuild debug run-server run-client help

-include $(DEPS)