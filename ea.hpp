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
	urd_{std::nextafter(-1, -2), std::nextafter(1, 2)};
  public: 
    const static unsigned parent_population = 10;
    const static unsigned child_population = 50;
    const static unsigned population = parent_population
				    + child_population;
    std::vector<Weights> people_weights;

    EA (const std::vector<Weights>& vw)
    {
	update_weights(vw);
    }

    void cross_over(float cross_over_rate = 1.0)
    {
	for (size_t i(parent_population)
	    ; i < parent_population + child_population
	    ; i++)
	{
	    std::uniform_int_distribution<> 
		uid1(0, parent_population - 1);
	    std::bernoulli_distribution bd1(0.5), bd2(cross_over_rate);

	    auto& f(people_weights[uid1(gen_)]), 
		    m(people_weights[uid1(gen_)]);

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

	for (size_t i(parent_population)
	    ; i < child_population + parent_population
	    ; i++)
	{
	    if (bd(gen_))
	    {
		size_t p1(uid1(gen_)), p2(uid2(gen_)), p3(uid3(gen_));
		people_weights[i][p1][p2][p3] += urd_(gen_);
		/*
		people_weights_[i][uid1(gen_)][uid2(gen_)][uid3(gen_)]
		    += urd(gen_);
		*/
	    }
	}	
    }

    void init_people()
    {
	for (size_t i(0); i < parent_population; i++)
	{
	    for (auto& i : people_weights[i])
		for (auto& j : i)
		    for (auto& k : j)
			k = urd_(gen_);
	}
    }

    void selection_sort(std::vector<unsigned>& score)
    {
	for (size_t i(0); i < parent_population; i++)
	{
	    size_t max_idx(i);
	    for (size_t j(i + 1); j < population; j++)
	    {
		if (score[i] < score[j])
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
};
