#ifndef  ACCOUNTWIDGET_H_
#define  ACCOUNTWIDGET_H_

#include <Wt/WContainerWidget>
#include <Wt/WPushButton>
using namespace Wt;


class accountWidget  : public WContainerWidget
{
public:
  accountWidget(WContainerWidget * parent = 0);
  Signal<void>& creator() { return creator_; } 
  Signal<void>& backer()  { return backer_; }
  WLineEdit* getLogin() { return username;}
  WLineEdit* getPassword() { return password;}
  void disableLogin() { login->disable();}
  void setErrorMessage() { errorMessage->show();}
  void disableErrorMessage() { errorMessage->hide();}
private :
  Signal<void> checker_;
  Signal<void> backer_;
  Signal<void> creator_;
  void processCheckButton(WPushButton *b);
  void processBackButton(WPushButton *b);
  void processCreateButton(WPushButton *b);
  WLabel * usernameLabel;
  WLabel * passwordLabel;
  WLineEdit * username;
  WLineEdit * password;
  WTable * layout;
  WPushButton * login;
  WText * errorMessage;
  WPushButton * create;
  WPushButton * isAvailable;
  WPushButton * back;
  
};



#endif