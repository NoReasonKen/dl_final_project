#include <iostream>
#include <vector>
#include <random>
#include <iomanip>

class NN
{

	/*
	 * after create NN, you should use init() to initialize all parameter of all weight
	 * use forward() will calculate all neuron
	 * use delete_w(layer, input_neu, output_neu) will make the corresponding weight zero
	 */
  public:
	NN(uint32_t layer, uint32_t neu): weight(layer - 2, std::vector<std::vector<double>>(neu, std::vector<double>(neu))), 
		neuron(layer - 1, std::vector<double>(neu)), out_neuron(3), out_weight(3, std::vector<double>(neu)){}

	// initialize, you should input the input neuron
	void init(std::vector<double> input_neuron)
	{
		neuron[0] = std::move(input_neuron);
		
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_real_distribution<> dis(-1.0, 1.0);

		for(auto &i : weight)
		{
			for(auto &j : i)
			{
				for(auto &k : j)
					k = dis(gen);
			}
		}

		for(auto &i : out_weight)
			for(auto &j : i)
				j = dis(gen);
	}

	void forward()
	{
		for(uint32_t i = 0; i != neuron.size(); ++i)
			for(uint32_t j = 0; j != neuron[i].size(); ++j)
			{
					if(i != neuron.size() - 1)
						neuron[i + 1][j] = dot(neuron[i], weight[i][j]);
			}

		for(uint32_t i = 0; i != out_neuron.size(); ++i)
			out_neuron[i] = dot(neuron[neuron.size() - 1], out_weight[i]);
	}


	//delete weight[layer][input_neu][output_neu] 
	void delete_w(uint32_t layer, uint32_t input_neu, uint32_t output_neu)
	{

		if(layer == weight.size())
			out_weight[output_neu][input_neu] = 0;

		else
			weight[layer][output_neu][input_neu] = 0;
	}

	friend std::ostream& operator << (std::ostream& os, NN n)
	{
		os << "neuron" << std::endl;
		for(uint32_t i = 0; i != n.neuron.size(); ++i)
		{
			os << "layer" << i << std::endl;
			for(auto & j : n.neuron[i])
				os << std::setw(10) << j << "\t";
			os << std::endl;
		}


		os << "layer" << n.neuron.size() << std::endl;
		for(auto &i : n.out_neuron)
			os << std::setw(10) << i << "\t";
		os << std::endl;

		os << std::endl << "weight" << std::endl;

		
		for(uint32_t i = 0; i != n.weight.size(); ++i)
		{
			os << "layer" << i << std::endl;
			for(auto & j : n.weight[i])
			{
				for(auto &k : j)
					os << std::setw(10) << k << "\t";

				os << std::endl;
			}
		}

		os << "layer" << n.weight.size() << std::endl;
		for(auto &i : n.out_weight)
		{
			for(auto &j : i)
				os << std::setw(10) << j << "\t";
			os << std::endl;
		}
		return os;
	}

	std::vector<std::vector<std::vector<double>>> weight;
	std::vector<std::vector<double>> neuron;
	std::vector<double> out_neuron;
	std::vector<std::vector<double>> out_weight;

	
	double dot(std::vector<double> v1, std::vector<double> v2)
	{
		if(v1.size() != v2.size())
			std::cerr << "two vectors are different size!" << std::endl;

		double dot_sum = 0.0;
		for(uint32_t i = 0; i != v1.size(); ++i)
			dot_sum += v1[i] * v2[i];

		return dot_sum;
	}
};
