/*
Mustafa Yetişir

02.12.2022

Main File
*/


#include <iostream>
#include <iomanip>
#include <fstream>
#include <cassert>
#include <random>
#include <assert.h>
#include <string.h>
#include "linked_list.cpp"
#include <chrono>

using namespace std;

struct EPC_data//epc data struct to keep the data we will use (eg: date, hour, gap, grp etc.)
{
	string date, hour;
	double gap, grp, v, gi;
};

class DataProcessor//class to keep data features and estimators' functions
{
private:
	string feature;
	string starting_date;
	string starting_hour;
	string last_retrieved_date;
	string last_retrieved_hour;

	int estimator_count;
	string* estimators;

	int committed_data_size = 0;
	LinkedList elements;			//linked list called elements, so that we can
public:
	DataProcessor(const string& feature, int estimator_count, string* estimators) 
		: 
		feature(feature), 
		estimator_count(estimator_count), 
		estimators(estimators) {};

	double extract_selected_feature(const EPC_data& data);	//Function to extract selected feature of the data (e.g. gap, grp,vi)
	double get_estimator_value(string& estimator_name);		//Function to call selected estimator (e.g. max,std,mean...)

	void add(const EPC_data& data);
	void print();

	double get_mean();
	double get_max(); 
	double get_min();
	double get_std();
	double get_q1();
	double get_median();
	double get_q3();
};

void DataProcessor::print()//print function
{
	cout 
		<< starting_date << "," 
		<< starting_hour << "," 
		<< last_retrieved_date << ","
		<< last_retrieved_hour;

	int t = 0;
	auto estimator = estimators;
	while (t < estimator_count)
	{
		string estimator_name = *estimator;
		cout << "," << get_estimator_value(estimator_name);
		estimator++;
		t++;
	}
	cout << "\n";

}

double DataProcessor::get_estimator_value(string& estimator_name)//Function to call selected estimator and return that estimator's function 
{
	if (estimator_name == "max")
		return get_max();
	else if (estimator_name == "mean")
		return get_mean();
	else if (estimator_name == "std")
		return get_std();
	else if (estimator_name == "min")
		return get_min();
	else if (estimator_name == "firstq")
		return get_q1();
	else if (estimator_name == "median")
		return get_median();
	else if (estimator_name == "thirdq")
		return get_q3();
	return 0.0;
} 


double DataProcessor::extract_selected_feature(const EPC_data& data)//function to extract selected feature and return it
{
	if (feature == "gap") return data.gap;
	else if (feature == "grp") return data.grp;
	else if (feature == "v") return data.v;
	else if (feature == "gi") return data.gi;
	assert(0);
}


void DataProcessor::add(const EPC_data& data)//add function to call with epc data and insert it into elements list's appropriate position
{
	if (committed_data_size == 0)
	{
		starting_date = data.date;
		starting_hour = data.hour;
	}

	last_retrieved_date = data.date;
	last_retrieved_hour = data.hour;

	double value = extract_selected_feature(data);
	elements.insert(value);
	committed_data_size += 1;
}

////////estimator functions below:
double DataProcessor::get_std()//function to calculate std
{
	double total = 0.0;
	auto it = elements.get_head();
	double mean = get_mean();
	while (it != nullptr)	//goes from head to the end of the list and calculates the variance and then std. Works in O(n) complexity.
	{
		double mean_dist = mean - it->get_value();  
		total += (mean_dist * mean_dist); 
		it = it->get_next();
	}

	double variance = total / (committed_data_size - 1);
	return sqrt(variance);
}


double DataProcessor::get_q1()
{
	if (committed_data_size < 4)//if committed data size is less than 4 we calculate the q1 as below
	{
		double median = get_median();
		double mn = get_min();

		return (median + mn) / 2.0;
	}

	double q1_index = (committed_data_size+1) / 4.0;
	if (q1_index == floor(q1_index))
		return elements.get_kth((int)q1_index);
	else
		return elements.get_average_of_two_at((int)q1_index);	
}


double DataProcessor::get_median()
{
	if (committed_data_size % 2 == 0)
		return elements.get_average_of_two_at(committed_data_size/2 - 1);
	else
		return elements.get_kth(committed_data_size - 1);
}


double DataProcessor::get_q3() 
{
	double q3_index = (committed_data_size+1) / 4.0;

	if (q3_index == floor(q3_index))
		return elements.get_kth((int)q3_index);
	else
		return elements.get_average_of_two_at((int)q3_index);	
}

double DataProcessor::get_max()//since we're using linked list structure, head contains the max element so this particular function takes O(1) time complexity
{
	return elements.get_head()->get_value();
}


double DataProcessor::get_min()//function to get min, since we're using linked this this particular function returns the tail with O(n) time complexity 
{
	auto it = elements.get_head();
	while (it->get_next()) it = it->get_next();
	return it->get_value();
}


double DataProcessor::get_mean()//mean calculator. We traverse the whole list and divide, so it is O(n)
{
	double total = 0.0;
	auto it = elements.get_head();
	while (it != nullptr)
	{
		total += it->get_value();
		it = it->get_next();
	}
	return total / committed_data_size;;
}


int main(int argc, char* argv[])
{
	auto start = std::chrono::high_resolution_clock::now();//clock start, to calculate duration
	string input_filename = "";

	if (argc != 2)
	{
		cout << "usage: ./homework input_file" << endl;
		return 0;
	}

	input_filename = argv[1];
	// INPUT
	ifstream input(input_filename);

	int estimator_count;
	input >> estimator_count;

	string estimators[estimator_count];
	if(input.is_open()){
		for (int i = 0; i < estimator_count; i++)
			input >> estimators[i];
		
		input.ignore();

		int query_count;
		input >> query_count;
		string feature, dummy;
		
		input.ignore();
		input >> feature;
		input.ignore(); 

		DataProcessor processor(feature, estimator_count, estimators);

		int t = 0;
		while (!input.eof())
		{
			string op;
			input >> op;
			input.ignore();

			if (op == "add")
			{
				EPC_data data;
				getline(input, data.date, ',');
				getline(input, data.hour, ',');
				// input >> data.date;
				// input.ignore();
				// input >> data.hour;
				// input.ignore();
				input >> data.gap;
				input.ignore();
				input >> data.grp;
				input.ignore();
				input >> data.v;
				input.ignore();
				input >> data.gi;

				processor.add(data);

			}
			else if(op == "print")
			{
				processor.print();
			}
			else
			{
				cerr << "ERROR: Undefined operation!! [ " << op << " ]" << endl;
			}
		}
	}
	else{	//file is not openned error
		cerr<<"ERROR: Couldn't open the file: " << input_filename <<" " <<endl;
	}

	auto stop = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);

	cout <<" DURATION: "<< duration.count() << "ms " <<endl;
	return 0;
}