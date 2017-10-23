#pragma once
#include "validation.h"
#include "network_algorithms.h"
#include "save_load_network.h"

#include <iostream> // cout, cin

// Ask the user if they would like to initalise a new network themselves or from a file or see and example

static void main_menu() {
	neural_network network;
	bool is_using_main_menu{ true };
	while (is_using_main_menu) {
		int execution{ execution_option({"Define own Neural Network (Demonstrates construction and saving to file) [1] ","Load Neural Network from file [2] ","Display Example Network (Demonstrates some of the class features) [3]", "Sorting example [4]", "Exit Menu [5]"}) };
		if (execution == 1) { // User defined network setup
			// Ask user for parameters
			std::cout << "Please enter the name of the network (one word): ";
			std::string name{ validate_input<std::string>() };
			std::cout << std::endl << "Please enter the description of the network: ";
			std::string description;
			std::cin >> description;
			std::cin.clear(); std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Clear cin
			std::cout << std::endl << "Please enter activation function (sigmoid, h_tan, linear):  ";
			std::string activation_function{ validate_input<std::string>() };
			std::cout << std::endl << "Please enter the neuron threshold = ";
			double in_threshold{ validate_input<double>() };
			std::cout << std::endl << "Please enter the number of input neurons = ";
			int input_neurons{ limited_input<int>(1, true, 100, true) };
			std::cout << std::endl << "Please enter the number of hidden neuron layers = ";
			int hidden_layers{ limited_input<int>(1, true, 100, true) };
			std::cout << std::endl << "Please enter the amount of hidden neurons per layer = ";
			int hidden_per_layer{ limited_input<int>(1, true, 100, true) };
			std::cout << std::endl << "Please enter the number of output neurons = ";
			int output_neurons{ limited_input<int>(1, true, 100, true) };
			network = neural_network{ name, description, act_func::act_func_name[activation_function], in_threshold, input_neurons, hidden_layers, hidden_per_layer, output_neurons };
			std::vector<double> inputs(input_neurons); // assign the vector to the size of the number of input neurons
			std::cout << std::endl << "Please enter " << network.get_input_neurons().size() << " doubles (press enter between each input)" << std::endl;
			for (int i = 0; i < inputs.size(); i++) {
				std::cout << "input [" << i << "] = ";
				inputs[i] = validate_input<double>();
			}
			std::vector<double> out;
			out = forward_propagation(network, inputs);
			std::cout << "Test output values from forward propagation: " << std::endl;
			for (int i = 0; i < out.size(); i++) {
				std::cout << "O[" << i << "] = " << out[i] << std::endl;
			}
			std::string file_name;
			std::cout << "Please input file name to save to: ";
			std::cin >> file_name;
			std::cin.clear(); std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 
			save_neural_network(file_name, network);
		}
		else if (execution == 2) { // Load Network from file
			// Ask user for name of file
			std::string file_name;
			std::cout << "Please input the file name: ";
			std::cin >> file_name;
			network = load_neural_network(file_name);
			std::vector<double> inputs(network.get_input_neurons().size()); // assign the vector to the size of the number of input neurons
			// get inputs
			std::cout << std::endl << "Please enter " << network.get_input_neurons().size() << " doubles (press enter between each input)" << std::endl;
			for (int i = 0; i < inputs.size(); i++) {
				std::cout << "input [" << i << "] = ";
				inputs[i] = validate_input<double>();
			}
			std::vector<double> out;
			out = forward_propagation(network, inputs);
			std::cout << "Test output values from forward propagation: " << std::endl;
			for (int i = 0; i < out.size(); i++) {
				std::cout << "O[" << i << "] = " << out[i] << std::endl;
			}
		}
		else if (execution == 3) { // Display example network
			// Display network and then end program (or take them back to menu)
			network = neural_network{ "Example Network" };
			std::cout << network.get_description() << std::endl;
			std::vector<double> in { 1, 0 };
			std::vector<double> outputs{ forward_propagation(network, in) };
			std::cout << "Result of first forward propagation: " << std::endl;
			for (int i = 0; i < outputs.size(); i++) {
				std::cout << "O[" << i << "] = " << outputs[i] << std::endl;
			}
			// remove a neuron
			network.remove_neuron({ 1,1 });
			outputs = forward_propagation(network, in);
			std::cout << "Result of second forward propagation: " << std::endl;
			for (int i = 0; i < outputs.size(); i++) {
				std::cout << "O[" << i << "] = " << outputs[i] << std::endl;
			}
			save_neural_network("synapse_removal.txt", network);
		}
		else if (execution == 4) { // Display example network
			neural_network network;
			network = load_neural_network("sorting_example.txt");
			std::cout << network.get_name() << std::endl;
			for (auto&& iterator : network.get_input_neurons()) {
				std::cout << "coords = " << iterator->get_coords_string() << std::endl;
			}
			for (auto&& iterator : network.get_hidden_neurons()) {
				std::cout << "coords = " << iterator->get_coords_string() << std::endl;
			}
			for (auto&& iterator : network.get_output_neurons()) {
				std::cout << "coords = " << iterator->get_coords_string() << std::endl;;
			}
			network.sort_neurons();
			// output all coords
			std::cout << "sorting..." << std::endl;
			for (auto&& iterator : network.get_input_neurons()) {
				std::cout << "coords = " << iterator->get_coords_string() << std::endl;
			}
			for (auto&& iterator : network.get_hidden_neurons()) {
				std::cout << "coords = " << iterator->get_coords_string() << std::endl;
			}
			for (auto&& iterator : network.get_output_neurons()) {
				std::cout << "coords = " << iterator->get_coords_string() << std::endl;;
			}
		}
		else {
			is_using_main_menu = false;
		}
	}
	return;
}