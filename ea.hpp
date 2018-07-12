#ifndef EA_HPP_
#define EA_HPP_

#include <algorithm>
#include <fstream>
#include <random>
#include <string>
#include <vector>

class EA
{
  public:
    using Weights = std::vector<float>;
  
  private: 
    std::mt19937 gen_{std::random_device()()};
    std::uniform_real_distribution<> 
	urd_{std::nextafterf(-1.0, -2.0), std::nextafterf(1.0, 2.0)};
  public: 
    const static unsigned parent_pop = 50;
    const static unsigned child_pop = 450;
    const static unsigned pop = parent_pop + child_pop;
    std::vector<Weights> people_weights;

    EA (const std::vector<Weights>& vw)
    {
	people_weights = vw;
    }

    void cross_over(const std::vector<float>& score, 
		    float cross_over_rate = 1.0)
    {
	for (size_t i(parent_pop); i < parent_pop + child_pop; i++)
	{
	    std::bernoulli_distribution bd1(0.5), bd2(cross_over_rate);
	    std::uniform_int_distribution<> 
		uid(0, people_weights[0].size() - 1);

	    auto& f(people_weights[RW_select(score)]), 
		    m(people_weights[RW_select(score)]);

	    if (bd2(gen_))
	    {
		size_t pos(uid(gen_));
		switch ((unsigned)bd1(gen_))
		{
		    case 0:
			people_weights[i] = f;

			for (size_t j(pos); j < people_weights[i].size(); j++)
			{
			    people_weights[i][j] = m[j];
			}

			break;
		    case 1:
			people_weights[i] = m;

			for (size_t j(pos); j < people_weights[i].size(); j++)
			{
			    people_weights[i][j] = f[j];
			}

			break;
		}
	    }
	}
    }

    void mutate(float mutate_rate = 1.0)
    {
	std::bernoulli_distribution bd(mutate_rate);
	std::uniform_int_distribution<> 
	    uid(0, people_weights[0].size() - 1);

	for (size_t i(parent_pop); i < child_pop + parent_pop; i++)
	{
	    if (bd(gen_))
	    {
		size_t pos(uid(gen_));
		people_weights[i][pos] = urd_(gen_);
	    }
	}	
    }

    void init_people()
    {
	for (size_t i(0); i < parent_pop; i++)
	{
	    for (auto& j : people_weights[i])
	    {
		j = urd_(gen_);
	    }
	}
    }

    void selection_sort(std::vector<float>& score, std::vector<unsigned>& eat)
    {
	for (size_t i(0); i < parent_pop; i++)
	{
	    size_t max_idx(i);
	    for (size_t j(i + 1); j < pop; j++)
	    {
		if (score[max_idx] < score[j])
		{
		    max_idx = j;
		}
	    }

	    std::swap(score[i], score[max_idx]);
	    std::swap(eat[i], eat[max_idx]);
	    std::swap(people_weights[i], people_weights[max_idx]);
	}
    }

    void save_weight(const std::string& file_name)
    {
	std::ofstream ofs(file_name + ".w");

	for (size_t i(0); i < parent_pop; i++)
	{
	    for (auto& j : people_weights[i])
	    {
		ofs << j << " ";
	    }
	    ofs << "\n";
	}
    }

  private:
    size_t RW_select(const std::vector<float>& score)
    {
	auto min(std::min_element(score.begin(), 
		score.begin() + parent_pop));
	float count(*min * parent_pop);	
	for (auto i : score)
	{
	    count += i;
	}

	std::uniform_real_distribution<> 
	    urd(0, std::nextafterf(count, count + 1));
	float target(urd(gen_));

	count = 0;
	for (size_t i(0); i < score.size(); i++)
	{
	    count += score[i] + *min;
	    if (count > target)
	    {
		return i;
	    }
	}

	return 0;
    }
};

#endif
