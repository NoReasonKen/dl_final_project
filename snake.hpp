#ifndef SNAKE_HPP_
#define SNAKE_HPP_

#include <cmath>
#include <iostream>
#include <random>
#include <string>
#include <utility>
#include <vector>

class Snake
{
  public:
    using Point = std::pair<size_t, size_t>;
    struct Info
    {
	std::vector<unsigned> food_dist;
	//std::vector<unsigned> dist;
	bool is_over;
    };
    enum class Direction {left, right, up, down, end};
    enum class Content {empty, body, food, end};
    
  private:
    const static unsigned body_reserve_length = 32;
    const std::vector<Point> default_snake_pos 
	{ {20, 8}, {19, 8}, {18, 8} };

    std::vector<std::vector<Content>> board_;
    std::vector<Point> body_;
    Point food_;
    float score_;
    unsigned eat_;
    float time_;
    Direction dir_;
    Info info_;

  public:
    Snake()
	: score_(0.0)
    {
	init_board();
    }

    Snake(unsigned board_h, unsigned board_w)
	: score_(0.0)
    {
	init_board(board_h, board_w);
    }

    Snake(const Snake& sn)
	: board_(sn.board_)
	, body_(sn.body_)
	, food_(sn.food_)
	, score_(sn.score_)
	, dir_(sn.dir_)
	, info_(sn.info_)
    {
    }

    Snake(Snake&& sn)
	: board_(std::move(sn.board_))
	, body_(std::move(sn.body_))
	, food_(std::move(sn.food_))
	, score_(std::move(sn.score_))
	, dir_(std::move(sn.dir_))
	, info_(std::move(sn.info_))
    {
    }

    Snake& operator=(const Snake& sn)
    {
	this->board_ = sn.board_;
	this->body_ = sn.body_;
	this->food_ = sn.food_;
	this->score_ = sn.score_;
	this->dir_ = sn.dir_;
	this->info_ = sn.info_;

	return *this;
    }

    Snake& operator=(Snake&& sn)
    {
	this->board_ = std::move(sn.board_);
	this->body_ = std::move(sn.body_);
	this->food_ = std::move(sn.food_);
	this->score_ = std::move(sn.score_);
	this->dir_ = std::move(sn.dir_);
	this->info_ = std::move(sn.info_);

	return *this;
    }

    friend std::ostream& operator<<(std::ostream& os, const Snake& sn)
    {
	std::string buf("");
	buf.reserve((sn.board_.size() + 2) * (sn.board_[0].size() + 3));

	for (size_t i(0); i < sn.board_.size() + 2; i++)
	{
	    buf.append("-");
	}
	buf.append("\n");

	for (size_t y(sn.board_[0].size() - 1); (int)y > -1; y--)
	{
	    buf.append("|");
	    for (size_t x(0); x < sn.board_.size(); x++)
	    {
		switch (sn.board_[x][y])
		{
		    case Content::empty:
			buf.append(" ");
			break;
		    case Content::body:
			if (sn.body_.front().first == x &&
			    sn.body_.front().second == y)
			{
			    buf.append("@");
			}
			else
			{
			    buf.append("#");
			}
			break;
		    case Content::food:
			buf.append("$");
			break;
		    default:
			std::cerr << "Content reach end while normal using\n";
		}
	    }
	    buf.append("|\n");
	}
	
	for (size_t i(0); i < sn.board_.size() + 2; i++)
	{
	    buf.append("-");
	}
	buf.append("\n");

	os << buf;
	return os;
    }

    Info a_step(Direction dir)
    {
	if (is_backward(dir))
	{
	    dir = dir_;
	}
	else
	{
	    dir_ = dir;
	}

	auto& head(body_.front());

	switch (dir_)
	{
	    case Direction::left:
		check_next_point(Point(head.first - 1, head.second));
		break;
	    case Direction::right:
		check_next_point(Point(head.first + 1, head.second));
		break;
	    case Direction::up:
		check_next_point(Point(head.first, head.second + 1));
		break;
	    case Direction::down:
		check_next_point(Point(head.first, head.second - 1));
		break;
	    default:
		std::cerr << "Content reach end while normal using\n";
	}

	return info_;
    }

    inline static float point_dist(Point a, Point b)
    {
	return std::sqrt(std::pow(a.first - b.first, 2) +
			std::pow(a.second - b.second, 2));
    }

    inline std::vector<std::vector<Content>> get_board()
    {
	return board_;
    }

    inline std::vector<Point> get_body()
    {
	return body_;
    }

    inline Point get_food()
    {
	return food_;
    }

    inline float get_score()
    {
	return score_;
    }

    inline unsigned get_eat()
    {
	return eat_;
    }

    inline Direction get_dir()
    {
	return dir_;
    }

    inline void set_dir(Direction d)
    {
	dir_ = d;
    }

    inline Info get_info()
    {
	return info_;
    }

  private:
    void init_board(unsigned x = 50, unsigned y = 20)
    {
	board_.resize(x, std::vector<Content>(y, Content::empty));

	body_ = default_snake_pos;
	body_.reserve(body_reserve_length);
	dir_ = Direction::right;
	food_ = std::pair<size_t, size_t>(23, 8);
	time_ = 0.0;
	eat_ = 0;
	info_.food_dist.resize(4);
	//info_.dist.resize(4);
	info_.is_over = false;

	update_board();
	for (unsigned d(0); d < (unsigned)Direction::end; d++)
	{
	    //info_.dist[d] = distance_to((Direction)d, Content::body);
	    info_.food_dist[d] = distance_to((Direction)d, Content::food);
	}
    }

    void gen_food()
    {
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<size_t> 
	    uid1(0, board_.size() - 1), uid2(0, board_[0].size() - 1);

	food_.first = uid1(gen);
	food_.second = uid2(gen);
    }
    
    unsigned distance_to(Direction d, Content c)
    {
	//unsigned dis(0);
	auto& head(body_.front());
	
	switch (d)
	{
	    case Direction::left:
		if (food_.first < head.first)
		{
		    return head.first - food_.first;
		}
		else
		{
		    return 0;
		}
		/*
		for (size_t i(head.first - 1); (int)i > -1; i--)
		{
		    if (board_[i][head.second] == c)
		    {
			return dis;
		    }
		    dis++;
		}
		return dis;
		*/
	    case Direction::right:
		if (food_.first > head.first)
		{
		    return food_.first - head.first;
		}
		else
		{
		    return 0;
		}
		/*
		for (size_t i(head.first + 1); i < board_.size(); i++)
		{
		    if (board_[i][head.second] == c)
		    {
			return dis;
		    }
		    dis++;
		}
		return dis;
		*/
	    case Direction::up:
		if (food_.second > head.second)
		{
		    return food_.second - head.second;
		}
		else
		{
		    return 0;
		}
		/*
		for (size_t i(head.second + 1); i < board_[0].size(); i++)
		{
		    if (board_[head.first][i] == c)
		    {
			return dis;
		    }
		    dis++;
		}
		return dis;
		*/
	    case Direction::down:
		if (food_.second < head.second)
		{
		    return head.second - food_.second;
		}
		else
		{
		    return 0;
		}
		/*
		for (size_t i(head.second - 1); (int)i > -1; i--)
		{
		    if (board_[head.first][i] == c)
		    {
			return dis;
		    }
		    dis++;
		}
		return dis;
		*/
	    default:
		std::cerr << "Content reach end while normal using\n";
		return 0;
	}
    }

    void check_next_point(Point p)
    {
	if ((int)p.first < 0 || p.first > board_.size() - 1 ||
	    (int)p.second < 0 || p.second > board_[0].size() - 1)
	{
	    info_.is_over = true;
	    return;
	}

	switch(board_[p.first][p.second])
	{
	    case Content::empty:
		if (++time_ == 1000)
		{
		    info_.is_over = true;
		    return;
		}
		else
		{
		    board_[body_.back().first][body_.back().second] 
			    = Content::empty;
		    body_.pop_back();
		    body_.insert(body_.begin(), p);
		    break;
		}
	    case Content::food:
		body_.insert(body_.begin(), p);
		eat_++;
		score_+= 1 - time_ / 500;
		time_ = 0.0;
		gen_food();
		break;
	    case Content::body:
		info_.is_over = true;
		return;
	    default:
		std::cerr << "Content reach end while normal using\n";
	}
	
	update_board();
	for (unsigned i(0); i < (unsigned)Direction::end; i++)
	{
	    //info_.dist[i] = distance_to((Direction)i, Content::body);
	    info_.food_dist[i] = distance_to((Direction)i, Content::food);
	}
    }

    void update_board()
    {
	for (auto& i : body_)
	{
	    board_[i.first][i.second] = Content::body;
	}
	board_[food_.first][food_.second] = Content::food;
    }

    bool is_backward(Direction dir)
    {
	if ((int)dir_ % 2 == 0)
	{
	    if ((int)dir == (int)dir_ + 1)
	    {
		return true;
	    }
	}
	else
	{
	    if ((int)dir == (int)dir_ - 1)
	    {
		return true;
	    }
	}

	return false;
    }
};

#endif
