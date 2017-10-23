// NeuralNetwork
// Alex Koch May 2017
// Object Oriented Programming in C++

// This is a project implementing and demonstrating a codebase build for creating and using artificial neural networks.
// The header network_alogorithms.h includes all the functions and classes that make up an artificial neural network, whilst the 
// user_interface.h header contains all functions necessary for the user interface 

#include <iostream> // For cout, cerr & cin
#include <string> // In order to use strings
#include <vector>
#include <memory>

#include "user_interface.h"

int main(){
	bool is_{ true };
	while (is_) {
		main_menu();
		is_ = is_yes("Run again (y/n) ?");
	}
	return 0;
}