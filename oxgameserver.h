#ifndef __OXGAMESERVERH__
#define __OXGAMESERVERH__

#include <Wt/WServer>
#include <Wt/WApplication>
#include <Wt/WEnvironment>

#include <Wt/Auth/AuthService>
#include <Wt/Auth/HashFunction>
#include <Wt/Auth/PasswordService>
#include <Wt/Auth/PasswordStrengthValidator>
#include <Wt/Auth/PasswordVerifier>

#include <boost/interprocess/sync/scoped_lock.hpp>
#include <boost/interprocess/sync/interprocess_mutex.hpp>

using namespace boost::interprocess;
using namespace Wt;

//Our brand new http server class supporting user authentication
//don't touch this, leave this place jedi
class OXGameServer: public WServer
{
public:
	static OXGameServer& getServer();

	~OXGameServer(){ delete ps_; }
	
	Auth::AuthService& getAuthService() { return as_; }

	Auth::PasswordService& getPasswordService() { return *ps_; }
	
private:
	OXGameServer();

	Auth::AuthService as_;
	Auth::PasswordService *ps_;
	static OXGameServer * instance_;
	static interprocess_mutex mutex_;
};
#endif
