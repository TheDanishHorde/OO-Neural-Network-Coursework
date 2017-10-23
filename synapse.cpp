// This file contains the function boidies of the synapse class

#include "synapse.h"

synapse::synapse() : forward_neuron_(std::shared_ptr<neuron>()), originating_neuron_(std::shared_ptr<neuron>()), weight_(0) {
	//std::cout << "synapse default constructor used" << std::endl;
}
synapse::synapse(std::shared_ptr<neuron> forward_neuron, std::shared_ptr<neuron> originating_neuron, double in_weight) : forward_neuron_(forward_neuron), originating_neuron_(originating_neuron), weight_(in_weight), old_weight_(0) {
	//std::cout << "Synapse parameterised constructor used: ptr = " << originating_neuron->get_coords_string() << std::endl;
}
synapse::~synapse() { 
	//std::cout << "Synapse destructor" << std::endl;
	forward_neuron_.reset();
	originating_neuron_.reset();
}
synapse::synapse(synapse const& original_synapse) { // Copy constructor for deep copying 
	//std::cout << "Synapse Copy Constructor Used" << std::endl;
	forward_neuron_ = original_synapse.forward_neuron_;
	originating_neuron_ = original_synapse.originating_neuron_;
	weight_ = original_synapse.weight_;
	old_weight_ = original_synapse.old_weight_;
}
synapse::synapse(synapse&& original_synapse) { // Move Constructor
	//std::cout << "Synapse Move Constructor Used" << std::endl;
	forward_neuron_ = original_synapse.forward_neuron_;
	originating_neuron_ = original_synapse.originating_neuron_;
	weight_ = original_synapse.weight_;
	old_weight_ = original_synapse.old_weight_;
	// Set all of original_synapse's data to nothing as it is now undefined
	original_synapse.forward_neuron_ = std::shared_ptr<neuron>();
	original_synapse.originating_neuron_ = std::shared_ptr<neuron>();
	original_synapse.weight_ = 0;
	original_synapse.old_weight_ = 0;
}
synapse& synapse::operator = (synapse& original_synapse) { // Assinment operator overload
	if (&original_synapse == this) { std::cout << "Synapse Self Assignment" << std::endl; return *this; } // Self Assignment
	//std::cout << "Synapse Assignment Operator Used" << std::endl;
	forward_neuron_ = original_synapse.forward_neuron_;
	originating_neuron_ = original_synapse.originating_neuron_;
	weight_ = original_synapse.weight_;
	old_weight_ = original_synapse.old_weight_;
	return *this;
}
synapse& synapse::operator = (synapse&& original_synapse) { // overload =, for moving
	//std::cout << "Synapse Move Assignment Operator Used" << std::endl;
	std::swap(this->forward_neuron_, original_synapse.forward_neuron_);
	std::swap(this->originating_neuron_, original_synapse.originating_neuron_);
	std::swap(this->weight_, original_synapse.weight_);
	std::swap(this->old_weight_, original_synapse.old_weight_);
	return *this;
}
std::shared_ptr<neuron> synapse::get_forward_neuron() { return forward_neuron_; }
std::shared_ptr<neuron> synapse::get_originating_neuron() { return originating_neuron_; }