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

/** Rerepsents user not logged in exception
 */
class UserNotLoggedInException{};

/**This class represents one sesion with database
*/
class Session: public Dbo::Session
{
public:
	Session(const std::string &dbName );

	~Session() { delete db_; }
	/** This functions returns abstraction of data base */
	Auth::AbstractUserDatabase& users() { return *db_; }
	/** This functions returns login object */	
	Auth::Login& login() { return login_; }

	//functions for getting and setting some informations about currently logged user
	/** Returns name of currently logged user */
	std::string getUserName() ;
	
	/** Returns score of currently logged user */
	int getUserScore();

	/** Sets score of currently logged user */
	void setScore( int score );
	
	/** Updates score of currently logged user */
	void updateScore( int to_add );

	/**What is the position  of currently logged user */
	int getUserRanking();

	/**Returns vector of top how_many players */
	std::vector<User> getHigestRankUsers( int how_many );

private:
	/** Allows to access pointer to record from data base */
	Wt::Dbo::ptr<User> getUser();
	Dbo::backend::Sqlite3 connection_;
	Auth::Login login_;
	UserDatabase *db_;
};
#endif
