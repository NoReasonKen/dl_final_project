#include "snake.hpp"
#include "nn.hpp"
#include "ea.hpp"
#include <algorithm>

#define H_LAYER_NUM 3
#define H_LAYER_CH 6
#define EPOCH 10000

void play(Snake&, Snake::Info&, NN&);

int main()
{
    std::vector<float> score(EA::pop);
    Snake::Info info;

    EA ea
    (
	std::vector<EA::Weights> 
	(EA::parent_pop + EA::child_pop, 
	    EA::Weights
	    (H_LAYER_NUM + 2, std::vector<std::vector<float>>
		(H_LAYER_CH, std::vector<float>(H_LAYER_CH))
	    )
	)
    );
    ea.init_people();

    for (unsigned p_idx(0); p_idx < EA::parent_pop; p_idx++)
    {
	Snake game;
	NN nn(H_LAYER_NUM + 2, H_LAYER_CH, ea.people_weights[p_idx]);

	info = game.get_info(); 
	nn.init(info);
	play(game, info, nn);
	score[p_idx] = game.get_score();
    } 

    ea.cross_over(score);
    ea.mutate();

    for (unsigned epoch(0); epoch < EPOCH; epoch++)
    {
	std::cout << "EPOCH " << epoch << ":\n";

	for (unsigned p_idx(EA::parent_pop); p_idx < EA::pop; p_idx++)
	{
	    Snake game;
	    NN nn
	    (
		H_LAYER_NUM + 2, H_LAYER_CH, 
		ea.people_weights[p_idx]
	    );

	    info = game.get_info(); 
	    nn.init(info);
	    play(game, info, nn);
	    score[p_idx] = game.get_score();
	}

	ea.selection_sort(score);

	std::cout << "score: ";
	for (size_t i(0); i < EA::parent_pop; i++)
	{
	    std::cout << score[i] << " ";
	}
	std::cout << "\n";

	//std::string str;
	//std::cin >> str;

	ea.cross_over(score);
	ea.mutate();	
    }
}

void play(Snake& game, Snake::Info& info, NN& nn)
{
    while (!info.is_over)
    {
	nn.forward();
	info = game.a_step
	(
	    (Snake::Direction)std::distance
	    (
		nn.out_neuron.begin(), 
		std::max_element
		(
		    nn.out_neuron.begin(), 
		    nn.out_neuron.begin() + 4
		)
	    )
	);
	nn.init(info);
    }
}
