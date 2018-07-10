#ifndef NN_HPP_
#define NN_HPP_

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
	NN(uint32_t layer, uint32_t in_neu, uint32_t neu, uint32_t out_neu, const std::vector<float>& w): 
	    input_weight(neu, std::vector<float>(in_neu)), 
	    weight(layer - 3, std::vector<std::vector<float>>(neu, std::vector<float>(neu))),
	    out_weight(out_neu, std::vector<float>(neu)),
	    input_neuron(in_neu),
	    neuron(layer - 2, std::vector<float>(neu)),
	    out_neuron(out_neu)
    {
	uint32_t cnt = 0;
	for(uint32_t i = 0; i != neu; ++i)
	    for(uint32_t j = 0; j != in_neu; ++j, ++cnt)
		input_weight[i][j] = w[cnt];

	for(uint32_t i = 0; i != weight.size(); ++i)
	    for(uint32_t j = 0; j != weight[i].size(); ++j)
		for(uint32_t k = 0; k != weight[i][j].size(); ++k, ++cnt)
		    weight[i][j][k] = w[cnt];

	for(uint32_t i = 0; i != out_weight.size(); ++i)
	    for(uint32_t j = 0; j != out_weight[i].size(); ++j, ++cnt)
		out_weight[i][j] = w[cnt];

    }

	// initialize, you should input the input neuron
	void init(Snake::Info info)
	{
	    for (size_t i(0); i < 4; i++)
	    {
		input_neuron[i] = info.dist[i];
	    }
	    for (size_t i(4); i < 8; i++)
	    {
		input_neuron[i] = info.food_dist[i];
	    }
	}

	void forward()
	{
	    for(uint32_t i = 0; i != neuron.size(); ++i)
		for(uint32_t j = 0; j != neuron[i].size(); ++j)
		{
		    if(!i)
			neuron[0][j] = dot(input_neuron, input_weight[j]);

		    else
			neuron[i][j] = dot(neuron[i - 1], weight[i - 1][j]);
		}

	    for(uint32_t i = 0; i != out_neuron.size(); ++i)
		out_neuron[i] = dot(neuron[neuron.size() - 1], out_weight[i]);
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

	    return os;
	}

	std::vector<std::vector<float>> input_weight;
	std::vector<std::vector<std::vector<float>>> weight;
	std::vector<std::vector<float>> out_weight;
	std::vector<float> input_neuron;
	std::vector<std::vector<float>> neuron;
	std::vector<float> out_neuron;

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

#endif
