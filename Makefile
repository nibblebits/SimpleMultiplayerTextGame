
OBJECT_FILES = ./build/server.o ./build/client.o ./build/clientmanager.o ./build/player.o ./build/itemhandler.o ./build/inventory.o
OBJECT_FILE_FLAGS = -c -g -fPIC -std=c++14 -g
INCLUDES = ./include

./build/server.o: ./src/server.cpp
	g++ -I ${INCLUDES} ./src/server.cpp -o ./build/server.o ${OBJECT_FILE_FLAGS}

./build/client.o: ./src/client.cpp
	g++ -I ${INCLUDES} ./src/client.cpp -o ./build/client.o ${OBJECT_FILE_FLAGS}

./build/clientmanager.o: ./src/clientmanager.cpp
	g++ -I ${INCLUDES} ./src/clientmanager.cpp -o ./build/clientmanager.o ${OBJECT_FILE_FLAGS}

./build/player.o: ./src/player.cpp
	g++ -I ${INCLUDES} ./src/player.cpp -o ./build/player.o ${OBJECT_FILE_FLAGS}

./build/itemhandler.o: ./src/itemhandler.cpp
	g++ -I ${INCLUDES} ./src/itemhandler.cpp -o ./build/itemhandler.o ${OBJECT_FILE_FLAGS}

./build/inventory.o: ./src/inventory.cpp
	g++ -I ${INCLUDES} ./src/inventory.cpp -o ./build/inventory.o ${OBJECT_FILE_FLAGS}

all: ${OBJECT_FILES}
	g++ -g -I ${INCLUDES} ./src/main.cpp ${OBJECT_FILES} -std=c++14 -pthread -o ./bin/server;

clean:
	rm -f ${OBJECT_FILES}