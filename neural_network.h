// This header file contains the neural network class which constructs and holds the neurons within the whole network

#pragma once

#include "neuron.h" // Contains all the neuron classes
#include <string> // To use strings and their associated functions
#include <sstream> // For suing stringstreams, mainly within getting the coords as a string
//#include <map> 
#include <iostream> // cout, cin
#include <vector> // Vectors and their associated functions, remove, erase, oush_back etc.
#include <memory> // To use smart pointers, mainly shared_ptr
#include <algorithm> // for find

class neural_network { // This encapsulates a whole Artifical Neural Network so that multiple networks can be used as the same time.
private:
	std::string name_, description_; // The description allows one to double check the purpose of this particular network 
	std::vector<std::shared_ptr<neuron>> input_neurons_;
	std::vector<std::shared_ptr<neuron>> hidden_neurons_;
	std::vector<std::shared_ptr<neuron>> output_neurons_;
	int inputs_, hidden_, outputs_; // Records the number of input, hidden and output neurons respectively
public:
	neural_network() { // default constructor
		std::cout << "Unparameterised constructor for the network " << name_ << " used" << std::endl;
		name_ = "Empty Network";
		description_ = "No Description";
		input_neurons_ = {};
		hidden_neurons_ = {};
		output_neurons_ = {};
		inputs_ = 0;
		hidden_ = 0;
		outputs_ = 0;
	}
	neural_network(std::string in_name, std::string in_description, generic_function_double_ref in_activation_function, double in_threshold,int inputs, int hidden_layers, int hidden_per_layer, int outputs) { // Constructor for a classic fully connected network separated into layers
		name_ = in_name;
		description_ = in_description;
		inputs_ = inputs;
		hidden_ = hidden_layers * hidden_per_layer;
		outputs_ = outputs;
		// Must use default constructors for all the neurons prior to proper contruction as pointers to other neurons are required in the parameterised constructors
		for (int i = 0; i < inputs_; i++) {
			input_neurons_.push_back(std::make_shared<input_neuron>());
		}
		for (int i = 0; i < hidden_; i++) {
			hidden_neurons_.push_back(std::make_shared<neuron>());
		}
		for (int i = 0; i < outputs_; i++) {
			output_neurons_.push_back(std::make_shared<output_neuron>());
		}
		for (int i = 0; i < inputs_; i++) { // Construct inputs neurons
			std::vector<std::shared_ptr<neuron>> forward_connections(hidden_neurons_.begin(), hidden_neurons_.begin() + hidden_per_layer); // &hidden_neurons_[0], &hidden_neurons_[hidden_per_layer - 1] -1
			*input_neurons_[i] = input_neuron(0, forward_connections, { 0, i });
		}
		// Construct hidden neurons
		for (int i = 0; i < hidden_per_layer; i++) { 
			std::vector<std::shared_ptr<neuron>> forward_connections(hidden_neurons_.begin() + hidden_per_layer, hidden_neurons_.begin() + 2*hidden_per_layer); 
			*hidden_neurons_[i] = neuron(0, forward_connections, in_activation_function,{ 1, i });
		}
		if (hidden_layers > 2) {
			for (int j = 1; j < hidden_layers - 1; j++) {
				for (int i = 0; i < hidden_per_layer; i++) {
					std::vector<std::shared_ptr<neuron>> forward_connections(hidden_neurons_.begin() + (j + 1)*hidden_per_layer, hidden_neurons_.begin() + (j + 2)*hidden_per_layer); 
					*hidden_neurons_[i + hidden_per_layer*j] = neuron(0, forward_connections, in_activation_function, { j + 1, i });
				}
			}
		}
		for (int i = 0; i < hidden_per_layer; i++) { 
			*hidden_neurons_[i + hidden_per_layer*(hidden_layers - 1)] = neuron(0, output_neurons_, in_activation_function, { hidden_layers, i });
		}
		for (int i = 0; i < outputs_; i++) { // Construct inputs neurons
			*output_neurons_[i] = output_neuron(0, in_activation_function, { hidden_layers + 1, i });
		}
		std::cout << "Network " << name_ << " constructed as fully connected layered network with " << inputs_ << " input neurons, " <<
			hidden_ << " neurons organised into " << hidden_layers << " layers with " << outputs_ << " output neurons." << std::endl;
	}
	neural_network(std::string in_name, std::string in_description, int in_inputs, int in_hidden, int in_outputs) : // Constructor for a general network being loaded in
		name_(in_name), description_(in_description), inputs_(in_inputs), hidden_(in_hidden), outputs_(in_outputs)	{
		for (int i = 0; i < inputs_; i++) {
			input_neurons_.push_back(std::make_shared<input_neuron>());
		}
		for (int i = 0; i < hidden_; i++) {
			hidden_neurons_.push_back(std::make_shared<neuron>());
		}
		for (int i = 0; i < outputs_; i++) {
			output_neurons_.push_back(std::make_shared<output_neuron>());
		}	
		// These neurons must be setup up afterwards, most likely by loading in a file
	}
	neural_network(std::string in_name) { // This is an example network which has 2 inputs, 3 hidden and 1 output
		name_ = in_name;
		description_ = "This is an example network which has 2 inputs, 3 hidden and 1 output and a threshold value of 0 and the sigmoid activation function setup with random weights";
		inputs_ = 2;
		hidden_ = 3;
		outputs_ = 1;
		for (int i = 0; i < 2; i++) {
			input_neurons_.push_back(std::make_shared<input_neuron>());
		}
		for (int i = 0; i < 3; i++) {
			hidden_neurons_.push_back(std::make_shared<neuron>());
		}
		output_neurons_.push_back(std::make_shared<output_neuron>());
		for (int i = 0; i < inputs_; i++) { // Construct inputs neurons
			*input_neurons_[i] = input_neuron(0, hidden_neurons_, { 0, i });
		}
		for (int i = 0; i < hidden_; i++) { // Construct Hidden Neurons
			*hidden_neurons_[i] = neuron(0, output_neurons_, &act_func::sigmoid, { 1, i });
		}
		for (int i = 0; i < outputs_; i++) { // Construct output neurons
			*output_neurons_[i] = output_neuron(0, &act_func::sigmoid, { 2, i });
		}
	}
	neural_network(neural_network const& original_network) { // Copy constructor for deep copying 
		std::cout << "Neural Network Copy Constructor Used" << std::endl;
		name_ = original_network.name_;
		description_ = original_network.description_;
		inputs_ = original_network.inputs_;
		hidden_ = original_network.hidden_;
		outputs_ = original_network.outputs_;
		input_neurons_ = original_network.input_neurons_;
		hidden_neurons_ = original_network.hidden_neurons_;
		output_neurons_ = original_network.output_neurons_;
	}
	neural_network(neural_network&& original_network) { // Move Constructor
		std::cout << "Neural Network Move Constructor Used" << std::endl;
		name_ = original_network.name_;
		description_ = original_network.description_;
		inputs_ = original_network.inputs_;
		hidden_ = original_network.hidden_;
		outputs_ = original_network.outputs_;
		input_neurons_ = original_network.input_neurons_;
		hidden_neurons_ = original_network.hidden_neurons_;
		output_neurons_ = original_network.output_neurons_;
		// Set all of original_network's data to nothing as it is now undefined
		original_network.name_ = "";
		original_network.description_ = "";
		original_network.inputs_ = 0;
		original_network.hidden_ = 0;
		original_network.outputs_ = 0;
		original_network.input_neurons_ = {};
		original_network.hidden_neurons_ = {};
		original_network.output_neurons_ = {};
	}
	neural_network & operator = (neural_network & original_network) { // Assignment operator overloaded definition, this will deep copy the matrix on the RHS into the LHS matrix
		if (&original_network == this) { std::cout << "Neural Network Self Assignment" << std::endl; return *this; } // Self Assignment
		std::cout << "Neural Network Assignment Operator Used" << std::endl;
		name_ = original_network.name_;
		description_ = original_network.description_;
		inputs_ = original_network.inputs_;
		hidden_ = original_network.hidden_;
		outputs_ = original_network.outputs_;
		input_neurons_ = original_network.input_neurons_;
		hidden_neurons_ = original_network.hidden_neurons_;
		output_neurons_ = original_network.output_neurons_;
		return *this;
	}
	neural_network& operator = (neural_network&& original_network) { // overload =, for moving
		std::cout << "Neural Network Move Assignment Operator Used" << std::endl;
		std::swap(this->name_, original_network.name_);
		std::swap(this->description_, original_network.name_);
		std::swap(this->input_neurons_, original_network.input_neurons_);
		std::swap(this->hidden_neurons_, original_network.hidden_neurons_);
		std::swap(this->output_neurons_, original_network.output_neurons_);
		std::swap(this->inputs_, original_network.inputs_);
		std::swap(this->hidden_, original_network.hidden_);
		std::swap(this->outputs_, original_network.outputs_);
		return *this;
	}
	~neural_network() { 
		std::cout << "neural_network destructor for the network " << name_ << " called" << std::endl; 
		for (auto&& i : input_neurons_) {
			i.reset();
		}
		for (auto&& i : hidden_neurons_) {
			i.reset();
		}
		for (auto&& i : output_neurons_) {
			i.reset();
		}
	}
	std::vector<std::shared_ptr<neuron>>  get_input_neurons() { // return by reference so that they can be modified
		return input_neurons_;
	}
	std::vector<std::shared_ptr<neuron>> get_hidden_neurons() {
		return hidden_neurons_;
	}
	std::vector<std::shared_ptr<neuron>> get_output_neurons() {
		return output_neurons_;
	}
	void remove_neuron(std::vector<int> coordinates) {
		std::shared_ptr<neuron> ptr;
		for (auto&& i : hidden_neurons_) {
			if (i->get_coords() == coordinates) {
				ptr = i;
			}
		}
		if (ptr.get() == nullptr) {
			std::cout << "Invalid coordinates, no neuron found" << std::endl;
		}
		else {
			std::cout << "removing neuron with coords: " << ptr->get_coords_string() << std::endl;
			ptr->remove_synapses();
			// Now remove references to this neuron in here
			std::vector<std::shared_ptr<neuron>>::iterator end_of_vector = std::remove( hidden_neurons_.begin(), hidden_neurons_.end(), ptr ); 
			hidden_neurons_.erase(end_of_vector, hidden_neurons_.end());
		}
	}
	void sort_neurons() { // Sorts the vectors containing the neurons according to this coordinates
		std::sort(input_neurons_.begin(), input_neurons_.end(), [](const std::shared_ptr<neuron> & first, const std::shared_ptr<neuron> & last) { // Lambda function example
			for (int i = 0; i < first->get_coords().size(); i++) { // go through the coordinates with the first elements first
				if (first->get_coords()[i] != last->get_coords()[i]) { // go to comparing the next elements along if these elements are the same
					return first->get_coords()[i] < last->get_coords()[i]; // return a bool of true or false, depending on whether the first element is larger or smaller than the second element. 
				}
			}
		});
		std::sort(hidden_neurons_.begin(), hidden_neurons_.end(), [](const std::shared_ptr<neuron> & first, const std::shared_ptr<neuron> & last) {
			for (int i = 0; i < first->get_coords().size(); i++) {
				if (first->get_coords()[i] != last->get_coords()[i]) {
					return first->get_coords()[i] < last->get_coords()[i];
				}
			}
		});
		std::sort(output_neurons_.begin(), output_neurons_.end(), [](const std::shared_ptr<neuron> & first, const std::shared_ptr<neuron> & last) {
			for (int i = 0; i < first->get_coords().size(); i++) {
				if (first->get_coords()[i] != last->get_coords()[i]) {
					return first->get_coords()[i] < last->get_coords()[i];
				}
			}
		});
	}
	std::string get_name() { return name_; }
	std::string get_description() { return description_; }
	friend std::vector<double> forward_propagation(neural_network&, std::vector<double>&);
	friend void save_neural_network(std::string, neural_network&);
};



