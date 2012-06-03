
#include <Wt/WApplication>
#include <Wt/WContainerWidget>
#include <Wt/WLineEdit>
#include <Wt/WPushButton>
#include <Wt/WText>
#include <Wt/WEnvironment>
#include <Wt/Auth/AuthWidget>
#include <Wt/Auth/PasswordService>

#include "menu.h"
#include "session.h"
#include "oxgameserver.h"

using namespace Wt;

class MainApplication : public WApplication
{
public:
  MainApplication(const WEnvironment& env);

  void authEvent();
private:
  Session session_;
  menu * gameMenu;
};
