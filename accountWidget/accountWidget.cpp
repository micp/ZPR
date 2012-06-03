
#include <Wt/WText>
#include <Wt/WPushButton>
#include <Wt/WTable>
#include <Wt/WLineEdit>
#include <Wt/WLabel>
#include "accountWidget.h"
using namespace Wt;
using namespace std;

accountWidget::accountWidget(WContainerWidget *parent) : WContainerWidget(parent)
{
  setContentAlignment(AlignCenter);
  layout = new WTable(this);
  usernameLabel = new WLabel("Your username: ",layout->elementAt(0,0));
  username = new WLineEdit(layout->elementAt(0,1));
  username->setFocus(true);
  usernameLabel->setBuddy(username);
  passwordLabel = new WLabel("Your password: ", layout->elementAt(1,0));
  password = new WLineEdit(layout->elementAt(1,1));
  password->setEchoMode(WLineEdit::Password);
  passwordLabel->setBuddy(password);  
  create = new WPushButton("Create",layout->elementAt(3,0));
  create->clicked().connect(boost::bind(&accountWidget::processCreateButton,this,create));
  isAvailable = new WPushButton("Check",layout->elementAt(0,2));
  isAvailable->clicked().connect(boost::bind(&accountWidget::processCheckButton,this,isAvailable));
  back = new WPushButton("Back",layout->elementAt(4,0));
  back->clicked().connect(boost::bind(&accountWidget::processBackButton,this,back));
  
}

void accountWidget::processCheckButton(WPushButton *b)
{
  
  //TODO  everything
  
  //connect with database and check if this login is available
  //for now let's say that is available
  
}

void accountWidget::processBackButton(WPushButton *b)
{
  password->setText("");
  username->setText("");
  backer_.emit();
}

void accountWidget::processCreateButton(WPushButton *b)
{
  accountWidget::processCheckButton(b);
  
  //TODO Create new account in database
  
  //If create was successful
  password->setText("");
  username->setText("");
  creator_.emit();
}