# Please implement your Makefile rules and targets below.
all: bin/simulation

# Step 1: Linking all object files to create the final executable.
bin/simulation: clean mkdir bin/files
		@echo 'Linking'
		g++ -o bin/simulation bin/Settlement.o bin/Facility.o bin/Plan.o bin/SelectionPolicy.o bin/Auxiliary.o bin/Action.o bin/Simulation.o bin/main.o

# Step 2: Compiling each source file into an object file.
bin/files:
		g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/Settlement.o src/Settlement.cpp
		g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/Facility.o src/Facility.cpp
		g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/Plan.o src/Plan.cpp
		g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/SelectionPolicy.o src/SelectionPolicy.cpp
		g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/Auxiliary.o src/Auxiliary.cpp
		g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/Action.o src/Action.cpp
		g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/Simulation.o src/Simulation.cpp
		g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/main.o src/main.cpp


mkdir:
	mkdir bin

clean:
	@echo 'Cleaning'
	rm -f -r bin
