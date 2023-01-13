CPP := g++ -w -std=c++17 -ofast
INCLUDE := -I/usr/include/v8
CFLAGS := -g -pthread -lsfml-graphics -lsfml-window -lsfml-system -lX11 -lzmq -lv8
LIB := -L/usr/lib/x86_64-linux-gnu

ALL_SRC := src
ALL_OBJ := objs
SERV_SRC := $(ALL_SRC)/server_src
SERV_OBJ := $(ALL_OBJ)/server_obj
CLI_SRC := $(ALL_SRC)/client_src
CLI_OBJ := $(ALL_OBJ)/client_obj
COMM_SRC := $(ALL_SRC)/common_src
COMM_OBJ := $(ALL_OBJ)/common_obj
COMP_SRC := $(ALL_SRC)/common_src/components
COMP_OBJ := $(ALL_OBJ)/common_obj/components

SERV_SOURCES := $(wildcard $(SERV_SRC)/*.cpp)
SERV_OBJECTS := $(patsubst $(SERV_SRC)%, $(SERV_OBJ)%, $(patsubst %.cpp, %.o, $(SERV_SOURCES)))

CLI_SOURCES := $(wildcard $(CLI_SRC)/*.cpp)
CLI_OBJECTS := $(patsubst $(CLI_SRC)%, $(CLI_OBJ)%, $(patsubst %.cpp, %.o, $(CLI_SOURCES)))

COMM_SOURCES := $(wildcard $(COMM_SRC)/*.cpp)
COMM_OBJECTS := $(patsubst $(COMM_SRC)%, $(COMM_OBJ)%, $(patsubst %.cpp, %.o, $(COMM_SOURCES)))

COMP_SOURCES := $(wildcard $(COMP_SRC)/*.cpp)
COMP_OBJECTS := $(patsubst $(COMP_SRC)%, $(COMP_OBJ)%, $(patsubst %.cpp, %.o, $(COMP_SOURCES)))

# HEADERS := $(wildcard $(SERV_SRC)/*.hpp)
# HEADER := $(SERV_SRC)/SERV_OBJECTS.hpp

all: client server

rebuild: clean all

client: $(CLI_OBJECTS) $(COMM_OBJECTS) $(COMP_OBJECTS)
	$(CPP) $(CLI_OBJECTS) $(COMM_OBJECTS) $(COMP_OBJECTS) -o $@ $(CFLAGS) $(LIB)

$(CLI_OBJ)/%.o: $(CLI_SRC)/%.cpp
	mkdir -p $(ALL_OBJ)
	mkdir -p $(CLI_OBJ)
	$(CPP) $< -c -g -o $@ $(INCLUDE) $(CFLAGS)

server: $(SERV_OBJECTS) $(COMM_OBJECTS) $(COMP_OBJECTS)
	$(CPP) $(SERV_OBJECTS) $(COMM_OBJECTS) $(COMP_OBJECTS) -o $@ $(CFLAGS) $(LIB)

$(SERV_OBJ)/%.o: $(SERV_SRC)/%.cpp
	mkdir -p $(ALL_OBJ)
	mkdir -p $(SERV_OBJ)
	$(CPP) $< -c -g -o $@ $(INCLUDE) $(CFLAGS)

$(COMM_OBJ)/%.o: $(COMM_SRC)/%.cpp
	mkdir -p $(ALL_OBJ)
	mkdir -p $(COMM_OBJ)
	$(CPP) $< -c -g -o $@ $(INCLUDE) $(CFLAGS)

$(COMP_OBJ)/%.o: $(COMP_SRC)/%.cpp
	mkdir -p $(ALL_OBJ)
	mkdir -p $(COMM_OBJ)
	mkdir -p $(COMP_OBJ)
	$(CPP) $< -c -g -o $@ $(INCLUDE) $(CFLAGS)

.PHONY: clean
clean:
	rm -f ./server
	rm -f ./client
	rm -r -f $(ALL_OBJ)
