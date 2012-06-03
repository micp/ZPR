#ifndef MENU_H_
#define MENU_H_
#include <Wt/WText>
#include "session.h"
#include "menuWidget/menuWidget.h"
class loginWidget;
class menuWidget;
class accountWidget;

class menu : public Wt::WContainerWidget
{
public:
  menu(Wt::WContainerWidget *parent, Session * session_tmp);
  ~menu();
  void logInButton();
  void logOutButton();
  void createAccountButton();
  void backMenuButton();
  void backAccountButton();
  void accountCreated();
  void addWidgets();
  void setUserName(std::string user_name) { menu_->setUserName(user_name);}
private:
  loginWidget * login_;
  menuWidget  * menu_;
  accountWidget * account_;
  Wt::WText * title;
  Session * session_;
};


#endif
