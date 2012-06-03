#include "authapp.h"
#include <iostream> //temporary just for testing

AuthApp::AuthApp( const WEnvironment &env ): WApplication(env) , session_( appRoot() + "dataBase.db")
{
	session_.login().changed().connect( this,  &AuthApp::authEvent ); //sth like addActionListener
	
	//it should be thar same as in main
	OXGameServer& srv = OXGameServer::getServer();
	
	//adding default authentication widget
	//to change if not a default widget should be used
	//TODO kubik
	Auth::AuthWidget *authW = new Auth::AuthWidget(
	 				srv.getAuthService() , session_.users() , session_.login() );

	authW->model()->addPasswordAuth(  &srv.getPasswordService() );
	authW->setRegistrationEnabled(true);

	authW->processEnvironment();

	root()->addWidget( authW );
}

//function takes control when some authentication event has been propagated (when user logged in or logged out)
//TODO kubik
//do with this what's you want 
//cout's are just for testnig to remove later
void AuthApp::authEvent()
{
	if( session_.login().loggedIn() )
	{
		 std::cout<< "User: "<<session_.login().user().id()<<" logged in"<<std::endl;
		std::cout<<session_.getUserName()<<std::endl;
		session_.setScore(0);
	}
	else std::cout<<"User logged out"<<std::endl;
}

