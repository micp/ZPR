#include "mainapp.h"

MainApplication::MainApplication(const WEnvironment& env)

  : WApplication(env), session_( appRoot() + "dataBase.db")
{
 
  setTitle("Noughts & Crosses");  
  useStyleSheet("css/OXGame5.css");
  session_.login().changed().connect(this, &MainApplication::authEvent);
  OXGameServer& srv = OXGameServer::getServer();

  Auth::AuthWidget * authW = new Auth::AuthWidget(srv.getAuthService(), session_.users(), session_.login());
  authW->model()->addPasswordAuth( &srv.getPasswordService());
  authW->setRegistrationEnabled(true);
  authW->processEnvironment();
  authW->setMaximumSize(450,WLength::Auto);
  gameMenu = new menu(root(), &session_);	
  gameMenu->addWidget(authW);
  gameMenu->addWidgets();
}

void MainApplication::authEvent()
{
  if(session_.login().loggedIn())
  {
    std::cout<<"USER: "<<session_.login().user().id()<<" logged in"<<std::endl;
    std::cout<<session_.getUserName()<<std::endl;
    gameMenu->logInButton();
    gameMenu->setUserName(session_.getUserName());
  }
  else
  {
    std::cout<<"User logged out"<<std::endl;
    gameMenu->logOutButton();
  }
     
}
