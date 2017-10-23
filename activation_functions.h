// This contains all the activation functions which are referenced to in later code. A name space is used to avoid naming conflicts in later code
#pragma once

#include <iostream> // cout, cin
#include <map> // In order to use the map class
#include <cmath> // for mathematical functions such as exp()

// Herein lies void pointer voodoo

using generic_object = const void*;
using generic_function = void*; // This is a pointer to a function that is either global or declared static within a class as they use the same pointer
using generic_function_double = double(*)(double); // This is what I should use double(*) the pointer type, (double) is the return type
using generic_function_double_ref = double(*)(const double&); 

// inline will dump this code in the space it is called 
namespace act_func {
	inline double input(const double& input) {
		return input;
	}

	inline double input_der(const double& input) { // Dummy function for use in the map below
		return 0;
	}

	inline double null_func(const double& input) {
		return 0;
	}

	inline double null_func_der(const double& input) { // Dummy function for use in the map below
		return 0;
	}

	inline double sigmoid(const double& input) { 
		return 1 / (1 + exp(-1 * input));
	}

	inline double sigmoid_derivative(const double& input) {
		return exp(-1 * input) / pow(1 + exp(-1 * input), 2);
	}

	inline double linear(const double& input) {
		double gradient{ 1 };
		return gradient*input;
	}

	inline double linear_derivative(const double& input) {
		double gradient{ 1 };
		return gradient;
	}

	inline double h_tan(const double& input) {
		return tanh(input);
	}
	
	inline double h_tan_derivative(const double& input) {
		return 1 - pow(tanh(input), 2);
	}
	static std::map<generic_function_double_ref, std::string> activation_function_pointers{ // Returns the name of the pointer but providing the function pointer in access operator []
		// Contains all the pointers to functions in a map so they can be saved, also allows new functions to be easily selected by user as well as hooking in new functions
		// *** FUNCTION NAMES SHOULD NOT CONTAIN THE CHARACTER '*' AS BREAKS LOADER ** //
		// NAMES for derivative functions must be the same at the function it derives from but with "_derivative" at the end
		{&input, "input" },
		{&input_der, "input_derivative" },
		{&null_func, "null_func" },
		{&null_func_der, "null_func_derivative" },
		{&sigmoid, "sigmoid" },
		{&sigmoid_derivative, "sigmoid_derivative" },
		{&linear, "linear" },
		{&linear_derivative, "linear_derivative" },
		{&h_tan, "h_tan" },
		{&h_tan_derivative, "h_tan_derivative" }
	};

	static std::map<std::string, generic_function_double_ref> act_func_name{ // Returns the function pointer when the name is provided into the access oparator []
		// *** FUNCTION NAMES SHOULD NOT CONTAIN THE CHARACTER '*' AS BREAKS LOADER ** //
		{ "input", &input },
		{ "input_derivative", &input_der },
		{ "null_func", &null_func },
		{ "null_func_derivative", &null_func_der },
		{ "sigmoid", &sigmoid },
		{ "sigmoid_derivative", &sigmoid_derivative },
		{ "linear", &linear },
		{ "linear_derivative", &linear_derivative },
		{ "h_tan", &h_tan },
		{ "h_tan_derivative", &h_tan_derivative }
	};
	
	static generic_function_double_ref activation_pointer_derivative(generic_function_double_ref in_activation_function) {
		std::string name{ activation_function_pointers[in_activation_function] };
		name += "_derivative";
		try {
			return act_func_name[name];
		}
		catch (std::out_of_range) {
			std::cout << "Activation function derivative could not be found, please check the activation function entered" << std::endl;
			return null_func; // In case of failure return the null function
		}
	}
}
