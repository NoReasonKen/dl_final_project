#include <iostream>
#include <vector>
#include <random>
#include <iomanip>

class nn
{
  public:
	nn(int layer, int neu): weight(layer - 2, std::vector<std::vector<double>>(neu, std::vector<double>(neu))),
		neuron(layer - 1, std::vector<double>(neu)), out_neuron(3), out_weight(3, std::vector<double>(neu)){}

	void init(std::vector<double> input_neuron)
	{
		neuron[0] = std::move(input_neuron);

		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_real_distribution<> dis(0.0, 1.0);

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

	double dot(std::vector<double> v1, std::vector<double> v2)
	{
		if(v1.size() != v2.size())
			std::cerr << "two vectors are different size!" << std::endl;

		double dot_sum = 0.0;
		for(size_t i = 0; i != v1.size(); ++i)
			dot_sum += v1[i] * v2[i];

		return dot_sum;
	}

	void forward()
	{
		for(size_t i = 0; i != neuron.size(); ++i)
			for(size_t j = 0; j != neuron[i].size(); ++j)
			{
					if(i != neuron.size() - 1)
						neuron[i + 1][j] = dot(neuron[i], weight[i][j]);
			}

		for(size_t i = 0; i != out_neuron.size(); ++i)
			out_neuron[i] = dot(neuron[neuron.size() - 1], out_weight[i]);
	}


	//delete weight[layer][input_neu][output_neu]
	void delete_w(int32_t layer, int32_t input_neu, int32_t output_neu)
	{
		weight[layer][output_neu][input_neu] = 0;
	}

	friend std::ostream& operator << (std::ostream& os, nn n)
	{
		os << "neuron" << std::endl;
		for(auto & i : n.neuron)
		{
			for(auto & j : i)
				os << std::setw(10) << j << "\t";
			std::cout << std::endl;
		}

		os << "weight" << std::endl;

		int cnt = 0;
		for(auto & i : n.weight)
		{
			os << "layer" << cnt << std::endl;
			for(auto & j : i)
			{
				for(auto &k : j)
					os << std::setw(10) << k << "\t";

				os << std::endl;
			}
			os << std::endl << std::endl;
			++cnt;
		}

		os << "output weight" << std::endl;
		for(auto &i : n.out_weight)
		{
			for(auto &j : i)
				os << std::setw(10) << j << "\t";
			os << std::endl;
		}

		os << "output neuron" << std::endl;
		for(auto &i : n.out_neuron)
			os << std::setw(10) << i << "\t";
		os << std::endl;

		return os;
	}

  private:
	std::vector<std::vector<std::vector<double>>> weight;
	std::vector<std::vector<double>> neuron;
	std::vector<double> out_neuron;
	std::vector<std::vector<double>> out_weight;

};
