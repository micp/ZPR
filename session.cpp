#include "session.h"

Session::Session( const std::string &dbName ): connection_ (dbName)
{
	setConnection(connection_);
	connection_.setProperty("show-queries" , "true");

	mapClass<User>("user");
	mapClass<AuthInfo>("auth_info");
	mapClass<AuthInfo::AuthIdentityType>("auth_identity");
	mapClass<AuthInfo::AuthTokenType>("auth_token");

	try
	{
		createTables(); //excpetion is thrown if base exist
	} catch(std::exception &e) {}

	db_ = new UserDatabase( *this);
}

Dbo::ptr<User> Session::getUser() 
{
	if( login_.loggedIn() )
	{
		Dbo::ptr<AuthInfo> user_data = db_->find(login_.user());
    		Dbo::ptr<User> user = user_data->user();
		
		if( !user )
		{
			user = add( new User() );
			user_data.modify()->setUser( user );
		}
	
		return user;
	}
	else throw UserNotLoggedInException();
}

std::string Session::getUserName() 
{
	if( login_.loggedIn() ) return login_.user().identity( Auth::Identity::LoginName ).toUTF8();
	else throw UserNotLoggedInException();
}

int Session::getUserScore() 
{
	Dbo::ptr<User> user = getUser();
	return user->getScore();
}
	
void Session::setScore( int score )
{
	Dbo::Transaction transaction( *this );

	Dbo::ptr<User> user = getUser();
	user.modify()->setScore( score );

	transaction.commit();
}
	
void Session::updateScore( int to_add )
{
	Dbo::Transaction transaction( *this );

	Dbo::ptr<User> user = getUser();
	user.modify()->setScore( user->getScore() + to_add );

	transaction.commit();
}
	
int Session::getUserRanking() 
{
	Dbo::Transaction transaction( *this );
	
	Dbo::ptr<User> user = getUser();
	int rank;
	
	rank = query<int>("select distinct count(score) from user").where("score > ?").bind( user->getScore() );

	transaction.commit();

	return rank + 1; //small normalization
}

std::vector<User> Session::getHigestRankUsers( int how_many )
{
	Dbo::Transaction transaction( *this );
	
	Dbo::collection<Dbo::ptr<User> > top;
	top = find<User>().orderBy("score desc").limit( how_many );

	std::vector<User> result;
	result.reserve( how_many );
	for( Dbo::collection<Dbo::ptr<User> >::const_iterator it = top.begin(); it != top.end(); ++it )
	{
		result.push_back( **it );
		
		result.back().setName( ( *(*it)->getAuthInfo() ).identity( Auth::Identity::LoginName ).toUTF8() );
	}

	transaction.commit();

	return result;
}


