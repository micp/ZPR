
#include <Wt/WText>
#include <Wt/WPushButton>
#include <Wt/WTable>
#include <Wt/WLineEdit>
#include <Wt/WLabel>
#include <Wt/WText>
#include <Wt/WBreak>
#include "loginWidget.h"
using namespace Wt;
using namespace std;




loginWidget::loginWidget(WContainerWidget * parent) : WContainerWidget(parent)
{
  setContentAlignment(AlignCenter);
  upText = new WText("Log in");
  addWidget(upText);
  addBreaks(1);
  usernameLabel = new WLabel("Username: ");
  usernameLabel->setStyleClass("alignLeft");
  addWidget(usernameLabel);
  username = new WLineEdit();
  username->setFocus(true);
  username->setStyleClass("alignRight");
  addWidget(username);
  usernameLabel->setBuddy(username);
  addBreaks(1);
  passwordLabel = new WLabel("Password: ");
  passwordLabel->setStyleClass("alignLeft");
  addWidget(passwordLabel);
  password = new WLineEdit();
  password->setStyleClass("alignRight");
  addWidget(password);
  password->setEchoMode(WLineEdit::Password);
  passwordLabel->setBuddy(password);
  addBreaks(1);
  login = new WPushButton("Log in");
  login->clicked().connect(boost::bind(&loginWidget::processLogInButton,this,login));
  addWidget(login);
  addBreaks(1);
  accountCreatedWell = new WText(
    "Account Created. Now log in to start a new game");
  addWidget(accountCreatedWell);
  accountCreatedWell->hide();
  errorMessage = new WText(
    "There is no user with that username and/or password");
  addWidget(errorMessage);
  errorMessage->hide();
  addBreaks();
  create = new WPushButton("Create Account");
  create->clicked().connect(boost::bind(&loginWidget::processCreateAccountButton,this,create));
  addWidget(create);
  username->enterPressed().connect
    (boost::bind(&loginWidget::processLogInButton, this, login));
  password->enterPressed().connect
    (boost::bind(&loginWidget::processLogInButton, this, login));
}

void loginWidget::processLogInButton(WPushButton *b)
{
  //TODO connection with database and check, whether login and password are correct
  if((username->text() == "kopytko") && (password->text() == "tak"))
    changer_.emit();
  else 
  {
    errorMessage->show();
    username->setFocus(true);
  }
  accountCreatedWell->hide();
  password->setText("");
  username->setText("");
  //nothin' special (of course despite this "sending to database" part)
}

void loginWidget::processCreateAccountButton(WPushButton *b)
{
  accountCreatedWell->hide();
  creator_.emit();
}

void loginWidget::addBreaks(int num)
{
  for(int i = 0; i<=num; i++)
    addWidget(new WBreak(this));
} 