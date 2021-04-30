CC=g++
CFLAGS=-std=c++17

INCLUDECADMIUM=-I ~/CADMIUM/Cadmium-Simulation-Environment/cadmium/include
INCLUDEDESTIMES=-I ~/CADMIUM/Cadmium-Simulation-Environment/DESTimes/include

#CREATE BIN AND BUILD FOLDERS TO SAVE THE COMPILED FILES DURING RUNTIME
bin_folder := $(shell mkdir -p bin)
build_folder := $(shell mkdir -p build)
results_folder := $(shell mkdir -p simulation_results)

#TARGET TO COMPILE ALL THE TESTS TOGETHER (NOT SIMULATOR)
message.o: data_structures/message.cpp
	$(CC) -g -c $(CFLAGS) $(INCLUDECADMIUM) $(INCLUDEDESTIMES) data_structures/message.cpp -o build/message.o

	
main_vendor.o: top_model/main_vendor.cpp
	$(CC) -g -c $(CFLAGS) $(INCLUDECADMIUM) $(INCLUDEDESTIMES) top_model/main_vendor.cpp -o build/main_vendor.o

main_test_moore.o: test/main_test_moore.cpp
	$(CC) -g -c $(CFLAGS) $(INCLUDECADMIUM) $(INCLUDEDESTIMES) test/main_test_moore.cpp -o build/main_test_moore.o

main_test_simplemoore.o: test/main_test_simplemoore.cpp
	$(CC) -g -c $(CFLAGS) $(INCLUDECADMIUM) $(INCLUDEDESTIMES) test/main_test_simplemoore.cpp -o build/main_test_simplemoore.o
main_ATM.o: top_model/main_ATM.cpp
	$(CC) -g -c $(CFLAGS) $(INCLUDECADMIUM) $(INCLUDEDESTIMES) top_model/main_ATM.cpp -o build/main_ATM.o
main_Telephone.o: top_model/main_Telephone.cpp
	$(CC) -g -c $(CFLAGS) $(INCLUDECADMIUM) $(INCLUDEDESTIMES) top_model/main_Telephone.cpp -o build/main_Telephone.o
main_serial_adder.o: top_model/main_serial_adder.cpp
	$(CC) -g -c $(CFLAGS) $(INCLUDECADMIUM) $(INCLUDEDESTIMES) top_model/main_serial_adder.cpp -o build/main_serial_adder.o

tests: main_test_moore.o main_test_simplemoore.o  
		$(CC) -g -o bin/main_test_moore build/main_test_moore.o build/message.o
		$(CC) -g -o bin/main_test_simplemoore build/main_test_simplemoore.o build/message.o 
	

#TARGET TO COMPILE ONLY OFFLINE_FILE_TRANSFER SIMULATOR
simulator: main_vendor.o main_ATM.o main_Telephone.o main_serial_adder.o message.o 
	$(CC) -g -o bin/main_vendor build/main_vendor.o build/message.o 
	$(CC) -g -o bin/main_ATM build/main_ATM.o build/message.o
	$(CC) -g -o bin/main_Telephone build/main_Telephone.o build/message.o
	$(CC) -g -o bin/main_serial_adder build/main_serial_adder.o build/message.o
	
#TARGET TO COMPILE EVERYTHING (OFFLINE_FILE_TRANSFER SIMULATOR + TESTS TOGETHER)
all: simulator tests

#CLEAN COMMANDS
clean: 
	rm -f bin/* build/*
