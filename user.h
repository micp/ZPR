#ifndef __USERH__
#define __USERH__

#include <Wt/Dbo/Types>
#include <Wt/WGlobal>
#include <string>

using namespace Wt;
//This is the class representing an user (player)
//all it's fields are saved to database
//TODO ups
//add some fields to make ranking and game search
class User
{
public:
	User() {};
	User( std::string name): name_(name){}

	void setName(std::string name) { name_ = name; }

	void setPoints( int points ) { points_ = points; }

	int getPoints() { return points_; }
	std::string& getName() { return name_; }

	template<class Action>
	void persist( Action &a )
	{
		Dbo::field(a , name_, "name");
		Dbo::field(a , points_ , "points" );
	}

private:
	std::string name_;
	int points_;
};

typedef Auth::Dbo::AuthInfo<User> AuthInfo;
#endif	
