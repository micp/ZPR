#include <Wt/WAnchor>
#include <Wt/WText>
#include <Wt/WStackedWidget>
#include <Wt/WVBoxLayout>
#include <Wt/WHBoxLayout>
#include <Wt/WApplication>
#include <Wt/WPushButton>
#include <Wt/WLabel>
#include <Wt/WLineEdit>
#include <iostream>
#include "menu.h"
#include "loginWidget/loginWidget.h"
#include "menuWidget/menuWidget.h"
#include "accountWidget/accountWidget.h"

using namespace Wt;

void menu::logInButton()
{
      menu_->show();
}

void menu::logOutButton()
{
     menu_->clear();
     delete menu_;
     menu_ = new menuWidget(this, session_);
     menu_->hide();
}
void menu::createAccountButton()
{
      login_->hide();
      menu_->hide();
      account_->show();
}

void menu::accountCreated()
{
    account_->hide();
    login_->show();
    login_->accountSuccessful();
}
void menu::backMenuButton()
{
    menu_->clear();
    delete menu_;
    menu_ = new menuWidget(this, session_);
    menu_->hide();
}

void menu::backAccountButton()
{
}
menu::menu(WContainerWidget *parent, Session * session_tmp) : WContainerWidget(parent)
{
  setContentAlignment(AlignCenter);
  session_ = session_tmp;
  title = new WText("Noughts & Crosses");
  title->decorationStyle().font().setSize(20);
  addWidget(title);
  //menu_ = new menuWidget(this);
  //menu_->hide();
}

void menu::addWidgets()
{
  menu_ = new menuWidget(this, session_);
  menu_->hide();
}
menu::~menu()
{
  delete menu_;
}