# DEVS-FSM2-Cadmium
Author: Maaz Jamal

Folder Structure:

├── DEVSmodelsForm_FSM2-CADMIUM.doc
├── FSM2-CADMIUM-REPORT.doc
├── ModelDescription-FSM2-CADMIUM.xml
├── README.txt
├── atomics
│   └── moore.h
├── bin (created when make command run for first time. Contains binries)
├── build (created when make command run for first time. Contains object files)
├── data_structures
│   ├── message.cpp
│   └── message.hpp
├── input_data (stores user input to the models)
│   ├── main_ATM.txt
│   ├── main_serial_adder.txt
│   ├── main_telephone.txt
│   ├── main_vendor.txt
│   ├── moore_input_test.txt
│   ├── moore_input_test_transition.txt
│   └── simplemoore_test.txt
├── makefile
├── simulation_results ( the results of the simulation are stored here)
│   ├── ATOMIC_TEST_MOORE_outputs.txt
│   ├── ATOMIC_TEST_MOORE_state.txt
│   ├── COUPLED_ATM_outputs.txt
│   ├── COUPLED_ATM_state.txt
│   ├── COUPLED_TELEPHONE_outputs.txt
│   ├── COUPLED_TELEPHONE_state.txt
│   ├── COUPLED_TEST_SIMPLEMOORE_outputs.txt
│   ├── COUPLED_TEST_SIMPLEMOORE_state.txt
│   ├── COUPLED_VENDOR_outputs.txt
│   ├── COUPLED_VENDOR_state.txt
│   ├── COUPLED_serial_adder_outputs.txt
│   └── COUPLED_serial_adder_state.txt
├── test (contains atomic test for moore model and a coupled test using simple moore machine)
│   ├── main_test_moore.cpp
│   └── main_test_simplemoore.cpp
├── top_model (contains couples models)
│   ├── main_ATM.cpp
│   ├── main_Telephone.cpp
│   ├── main_serial_adder.cpp
│   └── main_vendor.cpp
└── vendor (external useful libraries)
    ├── NDTime.hpp
    └── iestream.hpp

1) compiling the program

	- Open the root workspce folder and run the command:
		 make all
	- This will generate all the binaries in bin file

2) Running the programs:

	- There are 6 binaries in bin folder.
	- Each binary corresponds to one of the coupled or atomic models.
	- On Ubuntu use the command ls in bash to list all the binaries
	- To run one of the binary open bin folder.
	- Open terminal in bin folder
	- run the following command 
		./binary_name

3) Inputs and Outputs:
	
	- The program takes inputs from txt files in input_data folder
	- The program outputs result of simulation in the simulation_results folder.

4) Chaging input data:
	- The programs are hardwired to accept inputs from txt file in input_data. To 
	change the input. Change the input txt files. Make sure to create a backup of previous
	file in case of errors.
	
├── input_data (stores user input to the models)
    ├── main_ATM.txt 						for ATM coupled model ( main_ATM.cpp  ./main_ATM)
    ├── main_serial_adder.txt				for serial_adder coupled model ( main_serial_adder.cpp ./main_serial_adder)
    ├── main_telephone.txt					for TELEPHONE coupled model ( main_Telephone.cpp  ./main_Telephone)
    ├── main_vendor.txt						for vendor coupled model (main_vendor.cpp ./main_vendor)
    ├── moore_input_test.txt				for moore atomic model test ( main_test_moore.cpp ./main_test_moore)
    ├── moore_input_test_transition.txt		for // 		//		//		//		//
    └── simplemoore_test.txt				for simplemoore coupled model ( main_test_simplemoore.cpp ./main_test_simplemoore)

Troubleshooting:

	While an attempt has been made to make sure the program is able to compile  
	out of the box, provided the CADMIUM libraries are installed, following the provided 
	instructions for Ubuntu. 
	If Make cannot find the CADMIUM libraries. Please alter the makefile and change the values of 
	
	INCLUDECADMIUM=-I ~/CADMIUM/Cadmium-Simulation-Environment/cadmium/include
	INCLUDEDESTIMES=-I ~/CADMIUM/Cadmium-Simulation-Environment/DESTimes/include
	
	To the appropriate location in your computer. 
	
	For Cygwin:
	The cygwin installation may not contain the initial CADMIUM folder and the include path may be:

	INCLUDECADMIUM=-I ~/Cadmium-Simulation-Environment/cadmium/include
	INCLUDEDESTIMES=-I ~/Cadmium-Simulation-Environment/DESTimes/include

	The program has been tested on Cygwin and WSL Ubuntu20 and has succeslfully compiled,
	run and tested on those platforms. 
	
