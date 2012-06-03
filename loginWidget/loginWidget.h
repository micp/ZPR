#ifndef  LOGINWIDGET_H_
#define  LOGINWIDGET_H_

#include <Wt/WContainerWidget>
#include <Wt/WPushButton>
#include <Wt/WBreak>
using namespace Wt;


class loginWidget  : public WContainerWidget
{
public:
  loginWidget(WContainerWidget * parent = 0);
  Signal<void>& changer() { return changer_; } 
  Signal<void>& creator() { return creator_; }
  
  WLineEdit* getLogin() { return username;}
  WLineEdit* getPassword() { return password;}
  void setErrorMessage() { errorMessage->show();}
  void disableErrorMessage() { errorMessage->hide();}
  void clearPassword() { password->setText("");}
  void accountSuccessful() { errorMessage->hide(), accountCreatedWell->show();}
private :
  Signal<void> changer_;
  Signal<void> creator_;
  void addBreaks(int num = 0);
  void processLogInButton(WPushButton *b);
  void processCreateAccountButton(WPushButton *b);
  WText * title;
  WLabel * usernameLabel;
  WLabel * passwordLabel;
  WLineEdit * username;
  WLineEdit * password;
  WTable * layout;
  WPushButton * login;
  WText * errorMessage;
  WText * accountCreatedWell;
  WPushButton * create;
  WText * upText;
  WText * upText2;
  WBreak * myBreak;
  
};



#endif