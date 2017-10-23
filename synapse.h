#pragma once

#include "neuron.h"
#include <vector> // To use vectors
#include <iostream> // For using cout, cin, cerr...
#include <memory> // Smart pointers, shared_ptr is used extensively in containg the forward and backward neurons

class neuron;

class synapse {
public:
	std::shared_ptr<neuron> forward_neuron_;
	std::shared_ptr<neuron> originating_neuron_;
	double weight_;
	double old_weight_; // This is only used in back propagation
	synapse();
	synapse(std::shared_ptr<neuron>, std::shared_ptr<neuron>, double);
	~synapse();
	synapse(synapse const& original_synapse); // Copy constructor for deep copying 
	synapse(synapse&& original_synapse); // Move Constructor
	synapse& operator = (synapse& original_synapse); // Assinment operator overload
	synapse& operator = (synapse&& original_synapse); // overload =, for moving
	std::shared_ptr<neuron> get_forward_neuron();
	std::shared_ptr<neuron> get_originating_neuron();
};

