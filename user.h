#ifndef __USERH__
#define __USERH__

#include <Wt/Dbo/Types>
#include <Wt/Dbo/Dbo>
#include <Wt/WGlobal>
#include <Wt/Dbo/WtSqlTraits>
#include <Wt/Dbo/Impl>
#include <Wt/Auth/Dbo/AuthInfo>

#include <string>

using namespace Wt;
//This is the class representing an user (player)
//all it's fields are saved to database
//TODO ups
//add some fields to make ranking and game search
class User;
typedef Auth::Dbo::AuthInfo<User> AuthInfo;
/** This is the temporary version of one user record from data base
*/

class User: public Dbo::Dbo<User>
{
public:
	User(): score_(0)  {};
	User(const std::string name): name_(name), score_(0){}

	void setName(std::string name) { name_ = name; }

	const std::string& getName() const { return name_; }

	void setScore( int score ) { score_ = score; }

	int getScore() const { return score_; }

	Wt::Dbo::ptr<AuthInfo> getAuthInfo() const { return *authInfos.begin(); }
	
	template<class Action>
	void persist( Action &a )
	{
		Wt::Dbo::field(a , score_ , "score" );
		
		Wt::Dbo::hasMany(a, authInfos , Wt::Dbo::ManyToOne, "user");
	}

private:
	Wt::Dbo::collection< Wt::Dbo::ptr<AuthInfo> > authInfos;
	std::string name_;
	int score_;
};

DBO_EXTERN_TEMPLATES(User);

#endif	
