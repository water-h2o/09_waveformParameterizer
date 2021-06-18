CC=g++
CPPFLAGS=-c -g -Wall -std=c++17 `root-config --cflags` -I/usr/include/eigen3/ -I./include -I/usr/include
LDFLAGS=`root-config --glibs` -L/usr/lib/x86_64-linux-gnu -lgsl -lgslcblas -lm -lstdc++fs
#SOURCES= ./src/main.cpp ./src/getFilesInDir.cpp 
OBJECTS=main.o readANTSoutput.o getFilesInDir.o waveformParametrizer.o consoleMessages.o parameterFeaturizer.o fileOutput.o
EXECUTABLE=test

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@

consoleMessages.o: ./src/consoleMessages.o
	$(CC) ./src/consoleMessages.cpp $(CPPFLAGS) -o $@

waveformParametrizer.o: ./src/waveformParametrizer.cpp
	$(CC) ./src/waveformParametrizer.cpp $(CPPFLAGS) -o $@

parameterFeaturizer.o: ./src/parameterFeaturizer.cpp
	$(CC) ./src/parameterFeaturizer.cpp $(CPPFLAGS) -o $@

getFilesInDir.o: ./src/getFilesInDir.cpp
	$(CC) ./src/getFilesInDir.cpp $(CPPFLAGS) -o $@

readANTSoutput.o: ./src/readANTSoutput.cpp
	$(CC) ./src/readANTSoutput.cpp $(CPPFLAGS) -o $@

fileOutput.o: ./src/fileOutput.cpp
	$(CC) ./src/fileOutput.cpp $(CPPFLAGS) -o $@

main.o: ./src/main.cpp
	$(CC) ./src/main.cpp $(CPPFLAGS) -o $@

clean:
	rm ./*~ ./*.o ./main
