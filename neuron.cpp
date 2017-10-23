// This file contains the function boidies of the neuron class

#include "neuron.h"


neuron::neuron() : inputs_(0), activation_value_(0), threshold_(0), readiness_(0), delta_sum_(0), act_func_pointer_(&act_func::null_func), derivative_func_pointer_(act_func::null_func), synapses_({}), back_synapses_({}), coords_({}) {} // Default constructor																																															   
neuron::neuron(double in_threshold, std::vector<std::shared_ptr<neuron>> forward_neurons, generic_function_double_ref activation_function, std::vector<int> in_coords) : delta_sum_(0) { // Parameterised constructor with random weights in the synapses																																													 
	inputs_ = 0;
	activation_value_ = 0;
	threshold_ = in_threshold;
	readiness_ = 0;
	delta_sum_ = 0;
	act_func_pointer_ = activation_function;
	derivative_func_pointer_ = act_func::activation_pointer_derivative(activation_function); 
	coords_ = in_coords;  
	synapses_.reserve(forward_neurons.size());
	back_synapses_ = {};
	for (int i = 0; i < forward_neurons.size(); i++) {
		synapses_.emplace_back(std::make_shared<synapse>());
	}
	for (int i = 0; i < forward_neurons.size(); i++) {
		synapses_[i] = std::make_shared<synapse>(forward_neurons[i], std::make_shared<neuron>(*this), distribution(generator));
		forward_neurons[i]->back_synapses_.push_back(synapses_[i]);
	}
}
neuron::neuron(double in_threshold, std::vector<std::pair<std::shared_ptr<neuron>, double>> forward_neurons, generic_function_double_ref activation_function, std::vector<int> in_coords) : delta_sum_(0) { // Overload for specifying the synapse weights																																																//std::cout << "Neuron Parameterised Constructor (Set Weights)" << std::endl;
	inputs_ = 0;
	activation_value_ = 0;
	threshold_ = in_threshold;
	readiness_ = 0;
	delta_sum_ = 0;
	act_func_pointer_ = activation_function;
	derivative_func_pointer_ = act_func::activation_pointer_derivative(activation_function);
	coords_ = in_coords;
	synapses_.reserve(forward_neurons.size());
	back_synapses_ = {};
	for (int i = 0; i < forward_neurons.size(); i++) {
		synapses_.emplace_back(std::make_shared<synapse>());
	}
	for (int i = 0; i < forward_neurons.size(); i++) {
		synapses_[i] = std::make_shared<synapse>( forward_neurons[i].first, std::make_shared<neuron>(*this), forward_neurons[i].second );
		forward_neurons[i].first->back_synapses_.push_back(synapses_[i]);
	}
}
neuron::neuron(double in_inputs, double in_activation_value, double in_threshold, int in_readiness, generic_function_double_ref activation_function, std::vector<int> in_coords): delta_sum_(0) { // Constructor used in the loader, doesn't define any forward synapses. This is initialised later
	//std::cout << "Neuron File Loader Constructor" << std::endl;
	inputs_ = in_inputs;
	activation_value_ = in_activation_value;
	threshold_ = in_threshold;
	readiness_ = in_readiness;
	delta_sum_ = 0;
	act_func_pointer_ = activation_function;
	derivative_func_pointer_ = act_func::activation_pointer_derivative(activation_function); //act_func::act_func_name[act_func::activation_function_pointers[activation_function] += "_derivative"]; // gets the name of the activation function and adds "_derivative" on the end in order to reference the correct derivative function;
	coords_ = in_coords;
	synapses_ = {};
	back_synapses_ = {};
}
neuron::neuron(neuron const& original_neuron) { // Copy constructor for deep copying 
	//std::cout << "Neuron Copy Constructor Used" << std::endl;
	inputs_ = original_neuron.inputs_;
	activation_value_ = original_neuron.activation_value_;
	threshold_ = original_neuron.threshold_;
	readiness_ = original_neuron.readiness_;
	delta_sum_ = original_neuron.delta_sum_;
	act_func_pointer_ = original_neuron.act_func_pointer_;
	derivative_func_pointer_ = original_neuron.derivative_func_pointer_;
	synapses_ = original_neuron.synapses_;
	back_synapses_ = original_neuron.back_synapses_;
	coords_ = original_neuron.coords_;
}
neuron::neuron(neuron&& original_neuron) { // Move Constructor
	//std::cout << "Neuron Move Constructor Used" << std::endl;
	inputs_ = original_neuron.inputs_;
	activation_value_ = original_neuron.activation_value_;
	threshold_ = original_neuron.threshold_;
	readiness_ = original_neuron.readiness_;
	delta_sum_ = original_neuron.delta_sum_;
	act_func_pointer_ = original_neuron.act_func_pointer_;
	derivative_func_pointer_ = original_neuron.derivative_func_pointer_;
	synapses_ = original_neuron.synapses_;
	if (!original_neuron.back_synapses_.empty()) { // Only move over the back_synapses_ if original neurons' is not empty
		back_synapses_ = original_neuron.back_synapses_;
		original_neuron.back_synapses_ = {};
	}
	coords_ = original_neuron.coords_;
	// Set all of original_neuron's data to nothing as it is now undefined
	original_neuron.inputs_ = 0;
	original_neuron.activation_value_ = 0;
	original_neuron.threshold_ = 0;
	original_neuron.readiness_ = 0;
	original_neuron.delta_sum_ = 0;
	original_neuron.act_func_pointer_ = nullptr;
	original_neuron.derivative_func_pointer_ = nullptr;
	original_neuron.synapses_ = {};
	original_neuron.coords_.erase(original_neuron.coords_.begin(), original_neuron.coords_.end());
}
neuron& neuron::operator = (neuron & original_neuron) { // Assignment operator overloaded definition, this will deep copy the matrix on the RHS into the LHS matrix
	if (&original_neuron == this) { std::cout << "Neuron Self Assignment" << std::endl; return *this; } // Self Assignment
	//std::cout << "Neuron Assignment Operator Used" << std::endl;
	inputs_ = original_neuron.inputs_;
	activation_value_ = original_neuron.activation_value_;
	threshold_ = original_neuron.threshold_;
	readiness_ = original_neuron.readiness_;
	delta_sum_ = original_neuron.delta_sum_;
	act_func_pointer_ = original_neuron.act_func_pointer_;
	derivative_func_pointer_ = original_neuron.derivative_func_pointer_;
	synapses_ = original_neuron.synapses_;
	back_synapses_ = original_neuron.back_synapses_;
	coords_ = original_neuron.coords_;
	return *this;
}
neuron& neuron::operator = (neuron&& original_neuron) { // overload =, for moving
	//std::cout << "Neuron Move Assignment Operator Used" << std::endl;
	std::swap(this->inputs_, original_neuron.inputs_);
	std::swap(this->activation_value_, original_neuron.activation_value_);
	std::swap(this->threshold_, original_neuron.threshold_);
	std::swap(this->readiness_, original_neuron.readiness_);
	std::swap(this->delta_sum_, original_neuron.delta_sum_);
	std::swap(this->act_func_pointer_, original_neuron.act_func_pointer_);
	std::swap(this->derivative_func_pointer_, original_neuron.derivative_func_pointer_);
	std::swap(this->synapses_, original_neuron.synapses_);
	if (!original_neuron.back_synapses_.empty()) { // Do not swap back synapses data if empty as this deletes previously setup backwards connection
		std::swap(this->back_synapses_, original_neuron.back_synapses_);
	}
	std::swap(this->coords_, original_neuron.coords_);
	return *this;
}
neuron::~neuron() { /*std::cout << "Neuron destructor called" << std::endl;*/ }
// Member functions
void neuron::sum_inputs(const double& input) { // const ensure the value of input is unchanged
	if (readiness_ == 0) { // update the value of inputs to zero if this is a new round of receiving inputs
		inputs_ = 0;
	}
	inputs_ += input;
}
void neuron::update_readiness(std::queue<neuron*> & in_chain) {
	readiness_++;
	if (readiness_ == back_synapses_.size()) { 
		in_chain.push(this); // Add this neuron to the queue as it is now ready to fire to the queue
		readiness_ = 0; // reset the readiness counter to zero
	}
}
void neuron::activate(std::queue<neuron*> & in_chain) {
	activation_value_ = act_func_pointer_(inputs_); // use a move constructor?
	//std::cout << "neuron with coords = " << this->get_coords_string() << " activated with value: " << activation_value_ << " inputs = " << inputs_ << std::endl;
	if (activation_value_ < threshold_) { // Don't fire the neuron, but do update the readiness of downstream neurons
		for (auto iterator = synapses_.begin(); iterator != synapses_.end(); iterator++) {
			(*iterator).get()->forward_neuron_->update_readiness(in_chain);
		}
	}
	else { // Fire the neuron
		for (auto iterator = synapses_.begin(); iterator != synapses_.end(); iterator++) {		
			(*iterator).get()->forward_neuron_->sum_inputs((*iterator)->weight_ * this->activation_value_); // updates the target neuron's inputs_, check what net_value is
			(*iterator).get()->forward_neuron_->update_readiness(in_chain);
		}
	}
}
double neuron::derivative(double input) { return derivative_func_pointer_(input); }
void neuron::set_threshold(double new_threshold) {
	threshold_ = new_threshold;
}
void neuron::set_coords(std::vector<int> new_coords) {
	coords_ = new_coords;
}
void neuron::set_synapses(std::vector <std::pair<std::shared_ptr<neuron>, double>> forward_neurons) {
	synapses_ = {};
	synapses_.reserve(forward_neurons.size());
	for (int i = 0; i < forward_neurons.size(); i++) {
		synapses_.push_back(std::make_shared<synapse>(forward_neurons[i].first, std::make_shared<neuron>(*this), forward_neurons[i].second));
		forward_neurons[i].first->back_synapses_.push_back(synapses_[i]);
	}
}
void neuron::remove_synapses() {
	for (auto&& it : back_synapses_) {
		std::vector<std::shared_ptr<synapse>>::iterator end_of_vector = std::remove(it->originating_neuron_->synapses_.begin(), it->originating_neuron_->synapses_.end(), it); // Remove forward synapses to this neuron
		it->originating_neuron_->synapses_.erase(end_of_vector, it->originating_neuron_->synapses_.end()); // Must manually erase all the now empty elements as remove deletes relevant elements then shuffles the rest down but does not resize the container
	}
	for (auto&& it : synapses_) {
		std::vector<std::shared_ptr<synapse>>::iterator end_of_vector = std::remove(it->forward_neuron_->back_synapses_.begin(), it->forward_neuron_->back_synapses_.end(), it); // Remove synapses which can reference back toward this neuron
		it->forward_neuron_->back_synapses_.erase(end_of_vector, it->forward_neuron_->back_synapses_.end());
	}
}
void neuron::clear_back_synapses() { 
	back_synapses_.clear();
}
void neuron::propagate_synapses() {
	for (int i = 0; i < synapses_.size(); i++) {
		synapses_[i]->forward_neuron_->back_synapses_.push_back(synapses_[i]);
	}
}
void neuron::set_activation_function(generic_function_double_ref new_function) { // Allows the activation function of this neuron to be changed
	act_func_pointer_ = new_function;
}
void neuron::set_derivative_function(generic_function_double_ref new_function) { // Allows the derivative of the activation function to be changed
	derivative_func_pointer_ = new_function;
}
int neuron::number_of_back_connections() {
	return back_synapses_.size();
}
void neuron::load_input(const double& input) {
	inputs_ = input;
}
double neuron::get_output() {
	return activation_value_; // returns the activation value of the output neuron
}
double neuron::get_inputs() { return inputs_; }
double neuron::get_activation_value() { return activation_value_; }
double neuron::get_threshold() { return threshold_; }
double neuron::get_readiness() { return readiness_; }
generic_function_double_ref neuron::get_activation_function() { return act_func_pointer_; }
generic_function_double_ref neuron::get_derivative_function() { return derivative_func_pointer_; }
std::vector<int> neuron::get_coords() { return coords_; }
std::string neuron::get_coords_string() {
	std::stringstream return_string;
	for (auto it = coords_.begin(); it != coords_.end(); it++) {
		return_string << *it << " ";
	}
	return return_string.str();
}
std::vector<std::shared_ptr<synapse>> neuron::get_synapses() { return synapses_; }
std::vector<std::shared_ptr<synapse>> neuron::get_back_synapses() { return back_synapses_; } 

input_neuron::input_neuron() : neuron() {
	//std::cout << "Input Neuron default constructor" << std::endl;
	act_func_pointer_ = &act_func::input;
}
input_neuron::input_neuron(double in_threshold, std::vector<std::shared_ptr<neuron>> forward_neurons, std::vector<int> in_coords) :
	neuron(in_threshold, forward_neurons, &act_func::input, in_coords) {
	//std::cout << "Input Neuron Parameterised Constructor (Random Weights)" << std::endl;
}
input_neuron::input_neuron(double in_threshold, std::vector<std::pair<std::shared_ptr<neuron>, double>> forward_neurons, std::vector<int> in_coords) :
	neuron(in_threshold, forward_neurons, &act_func::input, in_coords) {
	//std::cout << "Input Neuron Parameterised Constructor (Set Weights)" << std::endl;
}
input_neuron::input_neuron(double in_inputs, double in_activation_value, double in_threshold, int in_readiness, std::vector<int> in_coords) : // Constructor for use when loading from file, forward neurons are initialised later
	neuron(in_inputs, in_activation_value, in_threshold, in_readiness, &act_func::input, in_coords) {}
input_neuron::~input_neuron() { /*std::cout << "input_neuron destructor" << std::endl;*/
	for (auto&& i : synapses_) {
		i.reset();
	}
	for (auto&& i : back_synapses_) {
		i.reset();
	}
}
void input_neuron::load_input(const double& input) {
	inputs_ = input;
}

std::vector<std::shared_ptr<neuron>> empty_vec;
output_neuron::output_neuron() : neuron() {
	//std::cout << "output_neuron default constructor" << std::endl;
	threshold_ = 0; // or something what will mean it will never fail to activate.
}
output_neuron::output_neuron(double in_threshold, generic_function_double_ref activation_function, std::vector<int> in_coords) :
	neuron(in_threshold, empty_vec, activation_function, in_coords) {
	//std::cout << "Output Neuron Parameterised Constructor (Random Weights)" << std::endl;
}
output_neuron::output_neuron(double in_inputs, double in_activation_value, double in_threshold, int in_readiness, generic_function_double_ref activation_function, std::vector<int> in_coords) : // Constructor for use when loading from file
	neuron(in_inputs, in_activation_value, in_threshold, in_readiness, activation_function, in_coords) {}
output_neuron::~output_neuron() { /*std::cout << "output_neuron default destructor" << std::endl;*/ 
	for (auto&& i : synapses_) {
		i.reset();
	}
	for (auto&& i : back_synapses_) {
		i.reset();
	}
}
double output_neuron::get_output() {
	return activation_value_; // returns the activation value of the output neuron
}