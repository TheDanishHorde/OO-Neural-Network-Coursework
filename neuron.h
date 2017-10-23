// Contains the forward defintions of the Neuron class, the implementation of which is contained within neuron.cpp
// There are two derived classes of inout and output neurons which are deived from the base class neuron
// This includes the header file synapse and then implements there setup via neuron construction

#pragma once

#include "activation_functions.h"
#include "synapse.h"

#include <vector> // To use vectors
#include <iostream> // For using cout, cin, cerr...
#include <sstream> // To use stringstreams and their associated functions
#include <queue> // GTo utilise the queue that occurs during use of forward propagation
#include <time.h> // for using time
#include <random> // to generate random values
#include <memory> // to use shared pointers
#include <algorithm> // for remove

static std::minstd_rand0 generator(time(NULL)); // This creates the random number generator and seeds it 
static std::normal_distribution<double> distribution(0.0, 1.0); // Random number from normal distribution, mean 0, standard deviation of one

class synapse;

class neuron { // Base Neuron class, this is in general used a 'hidden' neuron with input neuron and output neuron being specialisations of this 
protected:
	double inputs_; // This is the input from other neurons into this one, but can mean the actual starting inputs in the case of the derived input neuron class
	double activation_value_; // This is the result of the activation function upon the inputs_
	double threshold_; // This is the threshold value that the neuron must exceed in order to proceed to fire
	int readiness_; // This is how many of the backwards connected neurons have fired into this neuron
	double delta_sum_;
	generic_function_double_ref act_func_pointer_;
	generic_function_double_ref derivative_func_pointer_;
	std::vector<int> coords_;
	std::vector<std::shared_ptr<synapse>> synapses_; // This contains all the synapses that this neuron has forward connected to
	std::vector<std::shared_ptr<synapse>> back_synapses_; // This contains all the synapses that this neuron has backward connected which points to the shared_ptrs which other neurons hold that point to this
public:
	neuron(); // Default constructor
	neuron(double, std::vector<std::shared_ptr<neuron>>, generic_function_double_ref, std::vector<int>); // Parameterised constructor with random weights in the synapses
	neuron(double, std::vector<std::pair<std::shared_ptr<neuron>, double>>, generic_function_double_ref, std::vector<int>); // Overload for specifying the synapse weights
	neuron(double, double, double, int, generic_function_double_ref, std::vector<int>); // Constructor used in the loader, doesn't define any forward synapses. This is initialised later
	neuron(neuron const&); // Copy constructor for deep copying 
	neuron(neuron&&); // Move Constructor
	neuron& operator = (neuron &); // Assignment operator overloaded definition, this will deep copy the matrix on the RHS into the LHS matrix
	neuron& operator = (neuron&&); // overload =, for moving
	virtual ~neuron();
	// Member functions
	void sum_inputs(const double& input);
	void update_readiness(std::queue<neuron*> &);
	void activate(std::queue<neuron*> &);
	double derivative(double input);
	void set_threshold(double new_threshold);
	void set_coords(std::vector<int> new_coords);
	void set_synapses(std::vector <std::pair<std::shared_ptr<neuron>, double>>);
	virtual void remove_synapses();
	void clear_back_synapses();
	void propagate_synapses();
	void set_activation_function(generic_function_double_ref); // Allows the activation function of this neuron to be changed
	void set_derivative_function(generic_function_double_ref); // Allows the derivative of the activation function to be changed
	int number_of_back_connections();
	virtual void load_input(const double&);
	virtual double get_output();
	double get_inputs();
	double get_activation_value();
	double get_threshold();
	double get_readiness();
	generic_function_double_ref get_activation_function();
	generic_function_double_ref get_derivative_function();
	std::vector<int> get_coords();
	std::string get_coords_string();
	std::vector<std::shared_ptr<synapse>> get_synapses();
	std::vector<std::shared_ptr<synapse>> get_back_synapses();
};

// The following classes are specialisations of the neuron class which contain specific additonal functions and overidden functions related
// to operations that can only be perfomed by certain kinds of neurons, for example data return can only be handled by output neurons

class input_neuron : public neuron {
public:
	input_neuron();
	input_neuron(double, std::vector<std::shared_ptr<neuron>>, std::vector<int>);
	input_neuron(double, std::vector<std::pair<std::shared_ptr<neuron>, double>>, std::vector<int>);
	input_neuron(double, double, double, int, std::vector<int>); // Constructor for use when loading from file, forward neurons are initialised later
	~input_neuron();
	void load_input(const double&);
};

class output_neuron : public neuron {
public:
	std::vector<std::shared_ptr<neuron>> empty_vec;
	output_neuron();
	output_neuron(double, generic_function_double_ref, std::vector<int>);
	output_neuron(double, double, double, int, generic_function_double_ref, std::vector<int>); // Constructor for use when loading from file
	~output_neuron();
	double get_output(); // returns the activation value of the output neuron
};



