#ifndef MENU_H_
#define MENU_H_
#include <Wt/WText>
#include "session.h"
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
private:
  loginWidget * login_;
  menuWidget  * menu_;
  accountWidget * account_;
  Wt::WText * title;
  Session * session_;
};


#endif
