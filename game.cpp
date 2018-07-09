#include "snake.hpp"
#include "NN.hpp"
#include "ea.hpp"
#include <algorithm>

#define H_LAYER_NUM 3
#define H_LAYER_CH 6
#define EPOCH 100

int main()
{
    unsigned population = EA::parent_population + EA::child_population;

    EA ea
    (
	std::vector<EA::Weights> 
	(EA::parent_population + EA::child_population, 
	    EA::Weights
	    (H_LAYER_NUM + 2, std::vector<std::vector<float>>
		(H_LAYER_CH, std::vector<float>(H_LAYER_CH))
	    )
	)
    );
    ea.init_people();
    ea.cross_over();
    ea.mutate();

    std::vector<unsigned> score(population);
    Snake::Info info;
    Snake::Direction dir(Snake::Direction::right);
    for (unsigned epoch(0); epoch < EPOCH; epoch++)
    {
	for (unsigned num_person(0); num_person < population; num_person++)
	{
	    Snake game;
	    NN nn(H_LAYER_NUM + 2, H_LAYER_CH, ea.people_weights[num_person]);

	    info = game.get_info(); 
	    nn.init(info);

	    while (!info.is_over)
	    {
		nn.forward();
		info = game.a_step
		(
		    (Snake::Direction)std::distance
		    (
			nn.out_neuron.begin(), 
			std::max_element(nn.out_neuron.begin(), nn.out_neuron.end())
		    );
		)
		nn.init(info);
	    }

	    score[num_person] = game.get_score();
	}

	sort();
	ea.cross_over();
	ea.mutate();	
    }

}
