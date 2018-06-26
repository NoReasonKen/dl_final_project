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
	Point food_pos;
	std::vector<unsigned> distance;
	bool is_over;
    };
    enum class Direction {left, right, up, down, end};
    enum class Content {empty, body, food, end};
    
  private:
    const unsigned body_reserve_length = 32;
    const std::vector<Point> default_snake_pos 
	{ {8, 20}, {8, 19}, {8, 18} };

    std::vector<std::vector<Content>> board_;
    std::vector<Point> body_;
    Point food_;
    unsigned score_;
    Direction dir_;
    Info info_;

  public:
    Snake()
	: score_(0)
    {
	init_board();
    }

    Snake(unsigned board_h, unsigned board_w)
	: score_(0)
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

	for (size_t i(0); i < sn.board_[0].size() + 2; i++)
	{
	    buf.append("-");
	}
	buf.append("\n");

	for (size_t i(0); i < sn.board_.size(); i++)
	{
	    buf.append("|");
	    for (size_t j(0); j < sn.board_[0].size(); j++)
	    {
		switch (sn.board_[i][j])
		{
		    case Content::empty:
			buf.append(" ");
			break;
		    case Content::body:
			buf.append("#");
			break;
		    case Content::food:
			buf.append("@");
			break;
		    default:
			std::cerr << "Content reach end while normal using\n";
		}
	    }
	    buf.append("|\n");
	}
	
	for (size_t i(0); i < sn.board_[0].size() + 2; i++)
	{
	    buf.append("-");
	}
	buf.append("\n");

	os << buf;
	return os;
    }

    Info a_step(Direction dir)
    {
	if ((int)dir == (int)dir_ + 2 ||
	    (int)dir == (int)dir_ - 2)
	{
	    dir = dir_;
	}

	auto& head(body_.front());

	switch (dir)
	{
	    case Direction::left:
		if (head.first == 0)
		{
		    info_.is_over = true;
		    return info_;
		}
		check_next_space(board_[head.first - 1][head.second]);
		break;
	    case Direction::right:
		if (head.first == board_[0].size() - 1)
		{
		    info_.is_over = true;
		    return info_;
		}
		check_next_space(board_[head.first + 1][head.second]);
		break;
	    case Direction::up:
		if (head.second == 0)
		{
		    info_.is_over = true;
		    return info_;
		}
		check_next_space(board_[head.first][head.second - 1]);
		break;
	    case Direction::down:
		if (head.second == board_.size() - 1)
		{
		    info_.is_over = true;
		    return info_;
		}
		check_next_space(board_[head.first][head.second + 1]);
		break;
	    default:
		std::cerr << "Content reach end while normal using\n";
	}

	return info_;
    }

    std::vector<std::vector<Content>> get_board()
    {
	return board_;
    }

    std::vector<Point> get_body()
    {
	return body_;
    }

    Point get_food()
    {
	return food_;
    }

    unsigned get_score()
    {
	return score_;
    }

    Direction get_dir()
    {
	return dir_;
    }

    void set_dir(Direction d)
    {
	dir_ = d;
    }

    Info get_info()
    {
	return info_;
    }

  private:
    void init_board(unsigned board_h = 20, unsigned board_w = 50)
    {
	board_.resize(board_h, std::vector<Content>(board_w, Content::empty));
	body_ = default_snake_pos;
	body_.reserve(body_reserve_length);
	dir_ = Direction::right;
	gen_food();
	info_.food_pos = food_;
	info_.distance.resize(4);
	info_.is_over = false;
	for (unsigned d(0); d < (unsigned)Direction::end; d++)
	{
	    info_.distance[d] = distance_to_obstacle((Direction)d);
	}
    }

    void gen_food()
    {
	std::random_device rd;
	std::mt19937 gen(rd);
	std::uniform_int_distribution<size_t> 
	    uid1(0, board_.size() - 1), uid2(0, board_[0].size());

	food_.first = uid1(gen);
	food_.second = uid2(gen);
    }
    
    unsigned distance_to_obstacle(Direction d)
    {
	unsigned dis(0);
	auto& head(body_.front());
	
	switch (d)
	{
	    case Direction::left:
		for (auto i((int)head.first - 1); i >= 0; i--)
		{
		    dis++;
		    if (board_[i][head.second] != Content::empty)
		    {
			break;
		    }
		}
		break;
	    case Direction::right:
		for (auto i(head.first + 1); i < board_[0].size(); i++)
		{
		    dis++;
		    if (board_[i][head.second] != Content::empty)
		    {
			break;
		    }
		}
		break;
	    case Direction::up:
		for (auto i((int)head.second + 1); i >= 0; i--)
		{
		    dis++;
		    if (board_[head.first][i] != Content::empty)
		    {
			break;
		    }
		}
		break;
	    case Direction::down:
		for (auto i(head.second - 1); i < board_.size(); i++)
		{
		    dis++;
		    if (board_[head.first][i] != Content::empty)
		    {
			break;
		    }
		}
		break;
	    default:
		std::cerr << "Content reach end while normal using\n";
	}

	return dis;
    }

    void check_next_space(Content c)
    {
	auto& head(body_.front());

	switch(c)
	{
	    case Content::empty:
		body_.pop_back();
		body_.insert(body_.begin(), Point(head.first - 1, head.second));
		break;
	    case Content::food:
		body_.insert(body_.begin(), Point(head.first - 1, head.second));
		score_++;
		gen_food();
		info_.food_pos = food_;
		break;
	    case Content::body:
		info_.is_over = true;
		return;
	    default:
		std::cerr << "Content reach end while normal using\n";
	}
	
	for (unsigned i(0); i < (unsigned)Direction::end; i++)
	{
	    info_.distance[i] = distance_to_obstacle((Direction)i);
	}
    }
};
