#pragma once

// This contains all the algorithms for use in actually running a certain network

#include "neural_network.h"
#include <vector>
#include <queue> // allows a queue system to be implemented which is used in the forward propagation function to load the next neuron to be fired

static std::vector<double> forward_propagation(neural_network& network, std::vector<double>& data_input) {
	std::queue<neuron*> chain;
	std::vector<double> output_values;
	for (int i = 0; i < network.inputs_; i++) {
		network.input_neurons_[i]->load_input(data_input[i]);
		chain.push(network.input_neurons_[i].get());
	}
	while (!chain.empty()) {
		chain.front()->activate(chain); // Apply the activation function to the inputs_ within this neuron and then this will then fire on all the connected neurons 
		chain.pop(); // Remove the neuron that has been fired
	}
	for (auto&& it : network.output_neurons_) {
		output_values.push_back(it->get_output());
	}
	return output_values;
}



