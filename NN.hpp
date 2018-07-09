#include <iostream>
#include <vector>
#include <random>
#include <iomanip>
#include "ea.hpp"
#include "snake.hpp"


class NN
{

	/*
	 * after create NN, you should use init() to initialize all parameter of all weight
	 * use forward() will calculate all neuron
	 * use delete_w(layer, input_neu, output_neu) will make the corresponding weight zero
	 */
  public:
	NN(uint32_t layer, uint32_t neu, std::vector<std::vector<std::vector<float>>> w): 
		weight(w), 
		neuron(layer - 1, std::vector<float>(neu)), out_neuron(6){}

	// initialize, you should input the input neuron
	void init(Snake::Info info)
	{
		neuron[0][0] = info.food_pos.first;
		neuron[0][1] = info.food_pos.second;
		neuron[0][2] = info.distance[0];
		neuron[0][3] = info.distance[1];
		neuron[0][4] = info.distance[2];
		neuron[0][5] = info.distance[3];
		//neuron[0] = std::move(input_neuron);
		
		/*
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
				j = dis(gen);*/
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
			out_neuron[i] = dot(neuron[neuron.size() - 1], weight[neuron.size() - 1][i]);
	}


	/*
	//delete weight[layer][input_neu][output_neu] 
	void delete_w(uint32_t layer, uint32_t input_neu, uint32_t output_neu)
	{

		if(layer == weight.size())
			out_weight[output_neu][input_neu] = 0;

		else
			weight[layer][output_neu][input_neu] = 0;
	}*/

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
/*
		os << "layer" << n.weight.size() << std::endl;
		for(auto &i : n.out_weight)
		{
			for(auto &j : i)
				os << std::setw(10) << j << "\t";
			os << std::endl;
		}*/
		return os;
	}

	std::vector<std::vector<std::vector<float>>> weight;
	std::vector<std::vector<float>> neuron;
	std::vector<float> out_neuron;
	std::vector<std::vector<float>> out_weight;

	
	float dot(std::vector<float> v1, std::vector<float> v2)
	{
		if(v1.size() != v2.size())
			std::cerr << "two vectors are different size!" << std::endl;

		float dot_sum = 0.0;
		for(uint32_t i = 0; i != v1.size(); ++i)
			dot_sum += v1[i] * v2[i];

		return dot_sum;
	}
};
