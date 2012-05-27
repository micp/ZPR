#include "session.h"

Session::Session( const std::string &dbName ): connection_ (dbName)
{
	setConnection(connection_);

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

Dbo::ptr<User> Session::user() const
{
	if( login_.loggedIn() )
	{
		Dbo::ptr<AuthInfo> user_data = db_->find(login_.user());
    		return user_data->user();
	}
	else return Dbo::ptr<User>();
}
