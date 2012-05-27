/************************************************************************************
			Example of authorization widget for KUBIK
					//TODO kubik
************************************************************************************/

#ifndef __AUTHAPPH__
#define __AUTHAPPH__

#include <Wt/WApplication>
#include <Wt/WContainerWidget>
#include <Wt/WEnvironment>

#include <Wt/Auth/AuthWidget>
#include <Wt/Auth/PasswordService>

#include "session.h"
#include "oxgameserver.h"

using namespace Wt;

class AuthApp: public WApplication
{
public:
	AuthApp( const WEnvironment &env );
	
	void authEvent();

private:
	Session session_;
};
#endif
