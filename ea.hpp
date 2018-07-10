#ifndef EA_HPP_
#define EA_HPP_

#include <algorithm>
#include <random>
#include <vector>

class EA
{
  public:
    using Weights = std::vector<
			std::vector<
			    std::vector<float>
			>
		    >;
  
  private: 
    unsigned weights_count_;
    std::mt19937 gen_{std::random_device()()};
    std::uniform_real_distribution<> 
	urd_{std::nextafterf(-1.0, -2.0), std::nextafterf(1.0, 2.0)};
  public: 
    const static unsigned parent_pop = 100;
    const static unsigned child_pop = 500;
    const static unsigned pop = parent_pop + child_pop;
    std::vector<Weights> people_weights;

    EA (const std::vector<Weights>& vw)
    {
	update_weights(vw);
    }

    void cross_over(const std::vector<float>& score, 
		    float cross_over_rate = 1.0)
    {
	for (size_t i(parent_pop); i < parent_pop + child_pop; i++)
	{
	    std::bernoulli_distribution bd1(0.5), bd2(cross_over_rate);

	    auto& f(people_weights[RW_select(score)]), 
		    m(people_weights[RW_select(score)]);

	    if (bd2(gen_))
	    {
		switch ((unsigned)bd1(gen_))
		{
		    case 0:
			people_weights[i] = f;
		    case 1:
			people_weights[i] = m;
		}
	    }
	}
    }

    void mutate(float mutate_rate = 1.0)
    {
	std::bernoulli_distribution bd(mutate_rate);
	std::uniform_int_distribution<> 
	    uid1(0, people_weights[0].size() - 1),
	    uid2(0, people_weights[0][0].size() - 1),
	    uid3(0, people_weights[0][0][0].size() - 1);

	for (size_t i(parent_pop); i < child_pop + parent_pop; i++)
	{
	    if (bd(gen_))
	    {
		size_t p1(uid1(gen_)), p2(uid2(gen_)), p3(uid3(gen_));
		people_weights[i][p1][p2][p3] += urd_(gen_);
	    }
	}	
    }

    void init_people()
    {
	for (size_t i(0); i < parent_pop; i++)
	{
	    for (auto& i : people_weights[i])
		for (auto& j : i)
		    for (auto& k : j)
			k = urd_(gen_);
	}
    }

    void selection_sort(std::vector<float>& score)
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
	    std::swap(people_weights[i], people_weights[max_idx]);
	}
    }

  private:
    void update_weights(const std::vector<Weights>& vw)
    {
	people_weights = vw;
	weights_count_ = vw[0].size() * vw[0][0].size() 
			* vw[0][0][0].size();
    }

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
    }
};

#endif
