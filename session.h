#ifndef __SESSIONH__
#define __SESSIONH__

#include <Wt/Auth/Login>
#include <Wt/Auth/AbstractUserDatabase>
#include <Wt/Auth/Dbo/UserDatabase>
#include <Wt/Auth/Identity>

#include <Wt/Dbo/Session>
#include <Wt/Dbo/ptr>
#include <Wt/Dbo/backend/Sqlite3>

#include <string>

#include "user.h"

using namespace Wt;

typedef Auth::Dbo::UserDatabase<AuthInfo> UserDatabase;

class UserNotLoggedInException{};

//This class represents one sesion with database
//example of it's usage may be found in authapp class
class Session: public Dbo::Session
{
public:
	Session(const std::string &dbName );

	~Session() { delete db_; }

	Auth::AbstractUserDatabase& users() { return *db_; }

	Auth::Login& login() { return login_; }

	//functions for getting and setting some informations about currently logged user
	std::string getUserName() ;

	int getUserScore();

	void setScore( int score );
	
	void updateScore( int to_add );

	int getUserRanking();

	//function connected with players ranking
	std::vector<User> getHigestRankUsers( int how_many );

private:
	Wt::Dbo::ptr<User> getUser();
	Dbo::backend::Sqlite3 connection_;
	Auth::Login login_;
	UserDatabase *db_;
};
#endif
