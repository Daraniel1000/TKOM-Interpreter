all: Interpreter clean

CXX = g++
CFLAGS = -Wall

Interpreter: FileReader.o Lekser.o Runner.o Parser.o Tracer.o
	$(CXX) -obin/Release/Interpreter -O src/projekt.cpp FileReader.o Lekser.o Runner.o Parser.o Tracer.o $(CFLAGS)

FileReader.o: src/FileReader/FileReader.cpp
	$(CXX) -c src/FileReader/FileReader.cpp $(CFLAGS)

Lekser.o: src/Lekser/Lekser.cpp
	$(CXX) -c src/Lekser/Lekser.cpp $(CFLAGS)

Runner.o: src/Runner/Runner.cpp
	$(CXX) -c src/Runner/Runner.cpp $(CFLAGS)

Parser.o: src/parser/Parser.cpp
	$(CXX) -c src/parser/Parser.cpp $(CFLAGS)

Tracer.o: src/tracer/Tracer.cpp
	$(CXX) -c src/tracer/Tracer.cpp $(CFLAGS)

Test: FileReader.o Lekser.o Runner.o Parser.o Tracer.o
	$(CXX) -obin/Test/Test -g src/Tests/tests.cpp FileReader.o Lekser.o Runner.o Parser.o Tracer.o $(CFLAGS)

Debug: FileReader.o Lekser.o Runner.o Parser.o Tracer.o
	$(CXX) -obin/Debug/Interpreter -g src/projekt.cpp FileReader.o Lekser.o Runner.o Parser.o Tracer.o $(CFLAGS)

clean:
	rm *.o
