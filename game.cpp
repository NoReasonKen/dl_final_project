#include "snake.hpp"
#include "nn.hpp"
#include "ea.hpp"
#include <algorithm>

#define I_CH 8
#define H_LAYER_NUM 3
#define H_LAYER_CH 8
#define O_CH 4
#define EPOCH 10000

void play(Snake&, Snake::Info&, NN&);

int main()
{
    std::vector<float> score(EA::pop);
    Snake::Info info;

    const unsigned weight_count((I_CH + (H_LAYER_NUM - 1) 
				* H_LAYER_CH + O_CH) * H_LAYER_CH);

    EA ea
    (
	std::vector<EA::Weights>(EA::pop, EA::Weights(weight_count))
    );
    ea.init_people();

    for (unsigned p_idx(0); p_idx < EA::parent_pop; p_idx++)
    {
	Snake game;
	NN nn(H_LAYER_NUM + 2, I_CH, H_LAYER_CH, O_CH, 
		ea.people_weights[p_idx]);

	info = game.get_info(); 
	nn.init(info);
	play(game, info, nn);
	score[p_idx] = game.get_score();
    } 

    ea.cross_over(score);
    ea.mutate();

    for (unsigned epoch(1); epoch < EPOCH; epoch++)
    {
	std::cout << "EPOCH " << epoch << ":\n";

	for (unsigned p_idx(EA::parent_pop); p_idx < EA::pop; p_idx++)
	{
	    Snake game;
	    NN nn(H_LAYER_NUM + 2, I_CH, H_LAYER_CH, O_CH, 
		    ea.people_weights[p_idx]);

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

	if (epoch % 100 == 0)
	{
	    std::cout << "save: s, exit: e\ninput :";
	    std::cout.flush();
	    std::string input;
	    std::cin >> input;
	    if (input == "s")
	    {
		std::cout << "file name: ";
		std::cout.flush();
		std::cin >> input;
		ea.save_weight(input);
	    }
	    else if (input == "e")
	    {
		return 0;
	    }
	    else
	    {
		continue;
	    }
	}
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
