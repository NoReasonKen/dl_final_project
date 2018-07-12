#include "snake.hpp"
#include "nn.hpp"
#include "ea.hpp"
#include <algorithm>

#define I_CH 4
#define H_LAYER_NUM 1
#define H_LAYER_CH 4
#define O_CH 4
#define EPOCH 10000
#define LR 0.5
#define BP_ERR_THREDHOLD 0.1

void play(Snake&, Snake::Info&, NN&);
inline float relu(const float);
std::vector<float> calculate_predict(Snake&);
Snake::Direction choose_dir(Snake&, std::vector<float>&);

int main()
{
    std::vector<float> score(EA::pop);
    std::vector<unsigned> eat(EA::pop);
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
	play(game, info, nn);
	nn.get_weight(ea.people_weights[p_idx]);
	score[p_idx] = game.get_score();
	eat[p_idx] = game.get_eat();
    } 

    //ea.cross_over(score);
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
	    eat[p_idx] = game.get_eat();
	}

	ea.selection_sort(score, eat);

	std::cout << "eat: ";
	for (size_t i(0); i < EA::parent_pop; i++)
	{
	    std::cout << score[i] << " ";
	}
	std::cout << "\n";

	//std::string str;
	//std::cin >> str;

	//ea.cross_over(score);
	ea.mutate();

	if (epoch % 500 == 0)
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
    std::vector<float> weight(32);
    Snake::Point head, food;

    while (!info.is_over)
    {
	nn.init(info);
	nn.forward();
	std::vector<float> out(nn.out_neuron);
	std::vector<float> v(calculate_predict(game));

	nn.back_prop(v, BP_ERR_THREDHOLD);
	nn.get_weight(weight);

	while (true)
	{
	    float sum(0);
	    for (size_t i(0); i < v.size(); i++)
	    {
		sum += std::pow(v[i] - out[i], 2);
	    }
	    
	    if (std::sqrt(sum) < BP_ERR_THREDHOLD)
	    {
		break;
	    }
	    else
	    {
		nn.back_prop(v, LR);
		nn.forward();
		out = nn.out_neuron;
	    }
	}	
	info = game.a_step(choose_dir(game, out));
    }
}

inline float relu(const float x)
{
    if (x < 0)
    {
	return 0;
    }
    else
    {
	return x;
    }
}

std::vector<float> calculate_predict(Snake& game)
{
    auto head(game.get_body().front()), food(game.get_food());
    auto dist(Snake::point_dist(head, food));

    std::vector<float> v
    {
	relu((int)head.first - (int)food.first) / dist, 
	relu((int)food.first - (int)head.first) / dist, 
	relu((int)food.second - (int)head.second) / dist, 
	relu((int)head.second - (int)food.second) / dist
    };
    
    if ((int)head.first - 1 < 0 || 
	game.get_board()[head.first - 1][head.second]
	    == Snake::Content::body)
    {
	v[0] = -0.01;
    }
    if (head.first + 1 >= game.get_board().size() || 
	game.get_board()[head.first + 1][head.second]
	    == Snake::Content::body)
    {
	v[1] = -0.01;
    }
    if (head.second + 1 >= game.get_board()[0].size() || 
	game.get_board()[head.first][head.second + 1]
	    == Snake::Content::body)
    {
	v[2] = -0.01;
    }
    if ((int)head.second - 1 < 0 || 
	game.get_board()[head.first][head.second - 1]
	    == Snake::Content::body)
    {
	v[3] = -0.01;
    }

    return v;
}

Snake::Direction choose_dir(Snake& game, std::vector<float>& out)
{
    Snake::Direction d;
    d = (Snake::Direction)std::distance
	(
	    out.begin(), 
	    std::max_element
	    (
	        out.begin(), 
		out.end()
	    )
	);
    if (game.is_backward(d))
    {
	if ((int)d <= 1)
	{
	    if (out[2] > out[3])
	    {
		d = (Snake::Direction)2;
	    }
	    else
	    {
		d = (Snake::Direction)3;
	    }
	}
	else
	{
	    if (out[0] > out[1])
	    {
		d = (Snake::Direction)0;
	    }
	    else
	    {
		d = (Snake::Direction)1;
	    }
	}
    }

    return d; 
}
