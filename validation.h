// This contains all of the Validation Functions which are mainly used within the user interface
#pragma once
#include "extra_exceptions.h"

#include <string> // strings and their associated functions
#include <vector> // In order to use vectors, erase(), push_back() etc
#include <iostream> // cout, cin
#include <algorithm> // for the find function

template <class T> T validate_input(){ // templated function to return a validated input of class T
	T return_value;
	bool is_invalid_input{ true };
	while (is_invalid_input){
		try{
			std::cin >> return_value;
			if (std::cin.fail()){ // Failed to insert input into return_value
				throw cin_fail(); 
			}
			if (std::cin.peek() != '\n'){ // Only part of the input was inserted into return_value, occurs for integers when '2a' is inputted for example. 
				throw not_eof();
			}
			is_invalid_input = false;
			return return_value;
		}
		catch (cin_fail error){
			std::cout << error.what() << " Invalid " << typeid(T).name() << ". Please input a valid value: ";
		}
		catch (not_eof error){
			std::cout << error.what() << " Invalid " << typeid(T).name() << ". Please input a valid value: ";
		}
		catch (std::out_of_range error){
			std::cout << error.what() << " Input too large to be represented by type " << typeid(T).name() << ". Please input a valid value: " << std::endl;
		}
		std::cin.clear(); std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Clear cin
	}
}

template <class T> T limited_input(T min_value, bool min_equals, T max_value, bool max_equals){ // templated function to check that an input of a particular type is within some specified limits
	// To use only one limit input the max or min value associated for that data types by using <limits>
	T return_value;
	bool is_invalid_input{ true };
	while (is_invalid_input){
		try{
			return_value = validate_input<T>();
			if (min_equals){ if (return_value < min_value){ throw too_small(); } }
			else{ if (return_value <= min_value){ throw too_small(); } }
			if (max_equals){ if (return_value > max_value){ throw too_large(); } }
			else{ if (return_value >= max_value){ throw too_large(); } }
			is_invalid_input = false;
			return return_value;
		}
		catch (too_small error){
			std::cout << error.what() << " Input is less than specified limits, please input a valid value: ";
		}
		catch (too_large error){
			std::cout << error.what() << " Input is greater than specified limits, please input a valid value: ";
		}
		std::cin.clear(); std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Clear cin
	}
}

bool is_option(std::string question, std::vector <std::string> true_parameters, std::vector <std::string> false_parameters){ // Inputs are vectors so that multiple conditions can be true or false
	// Gives a true or false value depeding on whether user input matches the true or false conditions as well as validates the inputs.
	bool option{ true }; // Initalise as true
	bool is_input_failure{ true };
	while (is_input_failure){
		std::cout << question << std::endl;
		std::string input{ "" };
		std::getline(std::cin, input);
		std::string temp{ "" };
		for (unsigned int i = 0; i < input.size(); i++){ // convert each character in the string to lower
			temp += tolower(input[i]);
		}
		if (std::find(true_parameters.begin(), true_parameters.end(), temp) != true_parameters.end()){ // True condition satisfied 
			is_input_failure = false; // exit input failure loop as true condition satisfied
			option = true;
		}
		else if (std::find(false_parameters.begin(), false_parameters.end(), temp) != false_parameters.end()){ // False condition satisfied
			is_input_failure = false; // exit input failure loop as false conditon satisfied
			option = false;
		}
		else{ // invalid input detected, ask user again.
			std::cout << "Please enter a valid input: ";
			std::cin.clear(); std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		}
	}
	return option;
}

bool is_yes(std::string question){
	bool option{ true };
	std::cout << question << std::endl;
	bool is_input_failure{ true };
	while (is_input_failure){
		char input;
		try{
			std::cin >> input;
			if (std::cin.fail()){ throw cin_fail(); }
			input = tolower(input);
			if (input == 'y'){
				is_input_failure = false;
				option = true;
			}
			else if (input == 'n'){
				is_input_failure = false;
				option = false;
			}
			else{ // invalid input detected, ask user again.
				std::cout << "Please enter yes (y) or no (n): ";
			}
		}
		catch (cin_fail e){
			std::cout << e.what() << "Pease enter yes(y) or no(n)" << std::endl;
		}
		std::cin.clear(); std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	}
	return option;
}

int execution_option(std::vector<std::string> options) {
	std::cout << "Please Choose from the following options: " << std::endl;
	std::vector<int> int_choices;
	int choice;
	int option_number{ 1 };
	for (auto&& i : options) {
		std::cout << i << std::endl;
		int_choices.push_back(option_number);
		option_number++;
	}
	bool is_invalid{ true };
	while (is_invalid) {
		choice = validate_input<int>(); // gets the choice as an input, validate input will only accept int inputs here
		if (std::find(int_choices.begin(), int_choices.end(), choice) != int_choices.end()) {
			is_invalid = false;
		}
		else {
			std::cout << "Please Select a valid option from above" << std::endl;
		}
		std::cin.clear(); std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Clear cin
	}
	return choice;
}