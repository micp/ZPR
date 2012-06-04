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

/** This is server class wchich allows users to connect with this program
* through web browser */
class OXGameServer: public WServer
{
public:
	/** This is a Singleton class so you may get it's object only from 
	* this function */
	static OXGameServer& getServer();

	~OXGameServer(){ delete ps_; }
	/** Return AuthServis required for AuthWidget */
	Auth::AuthService& getAuthService() { return as_; }

	/** Return PassWordServis required for AuthWidget */
	Auth::PasswordService& getPasswordService() { return *ps_; }
	
private:
	/** Singleton so the constructor is private*/
	OXGameServer();

	Auth::AuthService as_;
	Auth::PasswordService *ps_;
	static OXGameServer * instance_;
	static interprocess_mutex mutex_;
};
#endif
