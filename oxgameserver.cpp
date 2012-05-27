#include "oxgameserver.h"
//server implementation
//don't touch this
//leave this place jedi

//initialization of singleton instance
OXGameServer * OXGameServer::instance_ = NULL;

//declartion of static member of server class
interprocess_mutex OXGameServer::mutex_;

OXGameServer& OXGameServer::getServer()
{
	if( instance_ == NULL )
	{
		scoped_lock<interprocess_mutex> lock( mutex_ );
		if( instance_ == NULL ) instance_ = new OXGameServer();
	}
	return *instance_;
}
//constructs the server and all support needed for user authorization
OXGameServer::OXGameServer()
{
	as_.setAuthTokensEnabled(true , "OXGameCookie");	//remember me option
	as_.setEmailVerificationEnabled(false);

	Auth::PasswordVerifier *verifier = new Auth::PasswordVerifier();
	verifier->addHashFunction( new Auth::BCryptHashFunction( 8 ) );
	
	ps_ = new Auth::PasswordService(as_);
	ps_->setVerifier(verifier);
	ps_->setAttemptThrottlingEnabled(true); //to protect from brute force attackers
	ps_->setStrengthValidator( new Auth::PasswordStrengthValidator() );
}
