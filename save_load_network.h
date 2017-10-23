#pragma once

#include "neural_network.h"

#include <string>
#include <sstream>
#include <map>
#include <iostream>
#include <vector>
#include <memory>

#include<fstream>


static void save_neural_network(std::string file_name, neural_network& network) {
	bool invalid_file{ true };
	while (invalid_file) {
		std::ofstream save_file(file_name);
		if (!save_file.good()) { // Unsuccessful opening of file
			std::cout << "Opening file " << file_name << " was unsuccessful" << std::endl;
			std::cout << "Please input a valid file name or enter 'x' to exit file saving: ";
			file_name = ""; std::cin.clear(); std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Clear in stream and the file name
			std::cin >> file_name;
			if (file_name == "x") { // exit the function
				invalid_file = false;
			}
		}
		else {
			std::cout << "Save file " << file_name << " created" << std::endl;
			save_file << "name_ " << network.name_ << '\n';
			save_file << "description_ " << network.description_ << '\n';
			save_file << "inputs_ " << network.inputs_ << '\n';
			save_file << "hidden_ " << network.hidden_ << '\n';
			save_file << "outputs_ " << network.outputs_ << '\n';
			save_file << "* * * * * * * * * *" << '\n';
			save_file << "input_neurons" << '\n';
			for (int i = 0; i < network.input_neurons_.size(); i++) {
				save_file << "* * * * * * * * * *" << '\n';
				save_file << "input_neuron[" << i << "]" << '\n';
				save_file << "inputs_ " << network.input_neurons_[i]->get_inputs() << '\n';
				save_file << "activation_value_ " << network.input_neurons_[i]->get_activation_value() << '\n';
				save_file << "threshold_ " << network.input_neurons_[i]->get_threshold() << '\n';
				save_file << "readiness_ " << network.input_neurons_[i]->get_readiness() << '\n';
				save_file << "activation_function_ " << act_func::activation_function_pointers[network.input_neurons_[i]->get_activation_function()] << '\n';
				save_file << "derivative_function_ " << act_func::activation_function_pointers[network.input_neurons_[i]->get_derivative_function()] << '\n';
				save_file << "coords_ " << network.input_neurons_[i]->get_coords_string() << '\n';
				for (int j = 0; j < network.input_neurons_[i]->get_synapses().size(); j++) {
					save_file << "forward_synapse " << network.input_neurons_[i]->get_synapses()[j].get()->forward_neuron_->get_coords_string() << '\n';
					save_file << "weight " << network.input_neurons_[i]->get_synapses()[j]->weight_ << '\n';
				}
			}
			save_file << "* * * * * * * * * *" << '\n';
			save_file << "hidden_neurons" << '\n';
			for (int i = 0; i < network.hidden_neurons_.size(); i++) {
				save_file << "* * * * * * * * * *" << '\n';
				save_file << "hidden_neuron[" << i << "]" << '\n';
				save_file << "inputs_ " << network.hidden_neurons_[i]->get_inputs() << '\n';
				save_file << "activation_value_ " << network.hidden_neurons_[i]->get_activation_value() << '\n';
				save_file << "threshold_ " << network.hidden_neurons_[i]->get_threshold() << '\n';
				save_file << "readiness_ " << network.hidden_neurons_[i]->get_readiness() << '\n';
				save_file << "activation_function_ " << act_func::activation_function_pointers[network.hidden_neurons_[i]->get_activation_function()] << '\n';
				save_file << "derivative_function_ " << act_func::activation_function_pointers[network.hidden_neurons_[i]->get_derivative_function()] << '\n';
				save_file << "coords_ " << network.hidden_neurons_[i]->get_coords_string() << '\n';
				for (int j = 0; j < network.hidden_neurons_[i]->get_synapses().size(); j++) {
					save_file << "forward_synapse " << network.hidden_neurons_[i]->get_synapses()[j].get()->forward_neuron_->get_coords_string() << '\n';
					save_file << "weight_ " << network.hidden_neurons_[i]->get_synapses()[j]->weight_ << '\n';
				}
			}
			save_file << "* * * * * * * * * *" << '\n';
			save_file << "output_neurons" << '\n';
			for (int i = 0; i < network.output_neurons_.size(); i++) {
				save_file << "* * * * * * * * * *" << '\n';
				save_file << "output_neuron[" << i << "]" << '\n';
				save_file << "inputs_ " << network.output_neurons_[i]->get_inputs() << '\n';
				save_file << "activation_value_ " << network.output_neurons_[i]->get_activation_value() << '\n';
				save_file << "threshold_ " << network.output_neurons_[i]->get_threshold() << '\n';
				save_file << "readiness_ " << network.output_neurons_[i]->get_readiness() << '\n';
				save_file << "activation_function_ " << act_func::activation_function_pointers[network.output_neurons_[i]->get_activation_function()] << '\n';
				save_file << "derivative_function_ " << act_func::activation_function_pointers[network.output_neurons_[i]->get_derivative_function()] << '\n';
				save_file << "coords_ " << network.output_neurons_[i]->get_coords_string() << '\n';
			}
			save_file.close();
			std::cout << "File Saved" << std::endl;
			invalid_file = false;
		}
	}
	return;
}


static neural_network load_neural_network(std::string file_name) {
	bool invalid_file{ true };
	neural_network network;
	while (invalid_file) {
		std::ifstream data_file(file_name);
		if (!data_file.good()) { // Unsuccessful opening of file
			std::cout << "Opening file " << file_name << " was unsuccessful" << std::endl;
			std::cout << "Please input a valid file name or enter 'x' to exit file loading: ";
			file_name = ""; std::cin.clear(); std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Clear in stream and the file name
			std::cin >> file_name;
			if (file_name == "x") { // exit the function
				invalid_file = false;
			}
		}
		else {
			std::cout << "File " << file_name << " was opened successfully" << std::endl;
			invalid_file = false;
			char bin;
			std::string in_name, in_description;
			int in_inputs, in_hidden, in_outputs;
			std::string line;
			getline(data_file, line); in_name = line.substr(6);
			getline(data_file, line); in_description = line.substr(13);
			getline(data_file, line); in_inputs = stoi(line.substr(8));
			getline(data_file, line); in_hidden = stoi(line.substr(8));
			getline(data_file, line); in_outputs = stoi(line.substr(9));
			getline(data_file, line);
			getline(data_file, line);
			network = neural_network{ in_name, in_description, in_inputs, in_hidden, in_outputs };
			std::map<std::vector<int>, std::shared_ptr<neuron>> neuron_map;
			double neuron_inputs, neuron_activation_value, neuron_threshold, neuron_readiness, synapse_weight;
			std::string neuron_activation_function, neuron_derivative_function;
			std::vector<int> neuron_coords;
			int coordinate;
			std::vector<std::vector<std::pair<std::vector<int>, double>>> input_forward_synapses;
			std::vector<std::pair<std::vector<int>, double>> forward_synapses;
			for (int i = 0; i < in_inputs; i++) {
				getline(data_file, line); // ignore "* * * * * * * * * *" line
				getline(data_file, line); // ignore first line with input_neuron[#]
				getline(data_file, line); neuron_inputs = stod(line.substr(8));
				getline(data_file, line); neuron_activation_value = stod(line.substr(18));
				getline(data_file, line); neuron_threshold = stod(line.substr(11));
				getline(data_file, line); neuron_readiness = stoi(line.substr(11));
				getline(data_file, line); neuron_activation_function = line.substr(21);
				getline(data_file, line); neuron_derivative_function = line.substr(21);
				getline(data_file, line);
				line = line.substr(8);
				while (line != "") {
					int ws_pos = line.find(' ');
					coordinate = stoi(line.substr(0, ws_pos + 1));
					line = line.substr(ws_pos + 1);
					neuron_coords.push_back(coordinate);
				}
				*network.get_input_neurons()[i] = input_neuron(neuron_inputs, neuron_activation_value, neuron_threshold, neuron_readiness, neuron_coords);
				neuron_map[neuron_coords] = network.get_input_neurons()[i];
				neuron_coords = {};
				// Now loop through forward neurons
				while (data_file.peek() != '*') {
					getline(data_file, line);
					line = line.substr(16);
					//std::cout << "coords = " << line << std::endl;
					while (line != "") {
						int ws_pos = line.find(' ');
						coordinate = stoi(line.substr(0, ws_pos + 1));
						line = line.substr(ws_pos + 1);
						neuron_coords.push_back(coordinate);
					}
					getline(data_file, line); synapse_weight = stod(line.substr(7));
					forward_synapses.emplace_back(std::pair<std::vector<int>, double>(neuron_coords, synapse_weight));
					neuron_coords = {};
				}
				input_forward_synapses.push_back(forward_synapses);
				forward_synapses.clear();
			}
			getline(data_file, line); // ignore "* * * * * * * * * *" line
			getline(data_file, line); // ignore "hidden_neurons"
			std::vector<std::vector<std::pair<std::vector<int>, double>>> hidden_forward_synapses;
			for (int i = 0; i < in_hidden; i++) {
				std::vector<std::pair<std::vector<int>, double>> forward_synapses;
				getline(data_file, line); // ignore "* * * * * * * * * *" line
				getline(data_file, line); // ignore first line with hidden_neuron[#]
				getline(data_file, line); neuron_inputs = stod(line.substr(8));
				getline(data_file, line); neuron_activation_value = stod(line.substr(18));
				getline(data_file, line); neuron_threshold = stod(line.substr(11));
				getline(data_file, line); neuron_readiness = stoi(line.substr(11));
				getline(data_file, line); neuron_activation_function = line.substr(21);
				getline(data_file, line); neuron_derivative_function = line.substr(21);
				getline(data_file, line);
				line = line.substr(8);
				while (line != "") {
					int ws_pos = line.find(' ');
					coordinate = stoi(line.substr(0, ws_pos + 1));
					line = line.substr(ws_pos + 1);
					neuron_coords.push_back(coordinate);
				}
				*network.get_hidden_neurons()[i] = neuron(neuron_inputs, neuron_activation_value, neuron_threshold, neuron_readiness,
					act_func::act_func_name[neuron_activation_function], neuron_coords);
				neuron_map[neuron_coords] = network.get_hidden_neurons()[i];
				neuron_coords = {};
				// Now loop through forward neurons
				while (data_file.peek() != '*') {
					getline(data_file, line);
					line = line.substr(16);
					while (line != "") {
						int ws_pos = line.find(' ');
						coordinate = stoi(line.substr(0, ws_pos + 1));
						line = line.substr(ws_pos + 1);
						neuron_coords.push_back(coordinate);
					}
					getline(data_file, line); synapse_weight = stod(line.substr(7));
					forward_synapses.emplace_back(std::pair<std::vector<int>, double>(neuron_coords, synapse_weight));
					neuron_coords = {};
				}
				hidden_forward_synapses.push_back(forward_synapses);
				forward_synapses.clear();
			}
			getline(data_file, line); // ignore "* * * * * * * * * *" line
			getline(data_file, line); // ignore "output_neurons"
									  // No Ouput forward synapses
			for (int i = 0; i < in_outputs; i++) {
				getline(data_file, line); // ignore "* * * * * * * * * *" line
				getline(data_file, line); // ignore first line with output_neuron[#]
				getline(data_file, line); neuron_inputs = stod(line.substr(8));
				getline(data_file, line); neuron_activation_value = stod(line.substr(18));
				getline(data_file, line); neuron_threshold = stod(line.substr(11));
				getline(data_file, line); neuron_readiness = stoi(line.substr(11));
				getline(data_file, line); neuron_activation_function = line.substr(21);
				getline(data_file, line); neuron_derivative_function = line.substr(21);
				getline(data_file, line);
				line = line.substr(8);
				while (line != "") {
					int ws_pos = line.find(' ');
					coordinate = stoi(line.substr(0, ws_pos + 1));
					line = line.substr(ws_pos + 1);
					neuron_coords.push_back(coordinate);
				}
				*network.get_output_neurons()[i] = output_neuron(neuron_inputs, neuron_activation_value, neuron_threshold, neuron_readiness,
					act_func::act_func_name[neuron_activation_function], neuron_coords);
				neuron_map[neuron_coords] = network.get_output_neurons()[i];
				neuron_coords = {};
			}
			// Now propagate forward_synapses, and hence back_synapses as well
			for (int i = 0; i < input_forward_synapses.size(); i++) { // propagate input forward synapses network.get_input_neurons().size()
				std::vector<std::pair<std::shared_ptr<neuron>, double>> in_synapses;
				for (int j = 0; j < input_forward_synapses[i].size(); j++) {
					in_synapses.push_back(std::pair<std::shared_ptr<neuron>, double>(neuron_map[input_forward_synapses[i][j].first], input_forward_synapses[i][j].second)); // Searches the neron_map for the neuron with the correct coords to pass in as a neuron pointer
				}
				network.get_input_neurons()[i]->set_synapses(in_synapses);
			}
			for (int i = 0; i < hidden_forward_synapses.size(); i++) { // propagate hidden forward synapses network.get_hidden_neurons().size()
				std::vector<std::pair<std::shared_ptr<neuron>, double>> in_synapses;
				for (int j = 0; j < hidden_forward_synapses[i].size(); j++) {
					in_synapses.push_back(std::pair<std::shared_ptr<neuron>, double>(neuron_map[hidden_forward_synapses[i][j].first], hidden_forward_synapses[i][j].second)); // Searches the neron_map for the neuron with the correct coords to pass in as a neuron pointer
				}
				network.get_hidden_neurons()[i]->set_synapses(in_synapses);
			}
			invalid_file = false;
		}
		invalid_file = false;
	}
	return network;
}