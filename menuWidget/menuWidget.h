#ifndef  MENUWIDGET_H_
#define  MENUWIDGET_H_

#include <Wt/WContainerWidget>
#include <Wt/WLabel>
#include <Wt/WLineEdit>

#include <vector>
#include <map>

#include "../fields.h"
#include "../game.h"
#include "../gamelistrefresher.h"
#include "../gamesconnector.h"
#include "../session.h"
using namespace Wt;
using namespace std;
class menuWidget  : public WContainerWidget, public GameListRefresher
{
public:
  menuWidget(WContainerWidget * parent, Session * session_tmp);
  //Wt::Signal<void>& backer() { return backer_; } 
  struct Coordinates
  {
    Coordinates(int c_x, int c_y) { x = c_x; y = c_y;}
    int x;
    int y;
  };
  bool tmp;
  bool tmp2;
  virtual void gameStarted();
  virtual void fieldChanged( FieldX &f , int x , int y );
  virtual void fieldChanged( FieldO &f , int x , int y );
  virtual void wonByGivingUp();
  virtual void playerExited();
  virtual void revengeProposed();
  virtual void refreshGameList();
  virtual void playerJoined();  
  virtual void endedWithDraw(int, int , int , int);  //TODO
	virtual void endedWithWin(int , int , int , int ); //TODO
	virtual void endedWithLose(int , int , int , int ); //TODO
virtual std::string& getUserName() { static std::string a("kopytko"); return a; } //TODO	
virtual std::string&getSessionID() {return sessionID; }
private :
	std::string sessionID;
//  Signal<void> backer_;
  void processBackButton(WPushButton *b);
  void processCreateNewGameButton(WPushButton *b);
  void processShowGamesButton();
  void processChooseGameButton(WPushButton *b);
  void processStartGameButton();
  void processClickButton(WPushButton *b, int i, int j);
  void processHideListButton();
  void processEndGameButton();
  void processRevengeButton(WPushButton *b);
  void processEndGameConnection(WPushButton *b);
  void processHighScoresButton(WPushButton *b);
  void processGiveUpButton(WPushButton *b);
  void addBreaks(int num = 0);
  WPushButton * newGameButton;
  //WPushButton * backButton;
  WPushButton * startGame;
  WPushButton * showGames;
  WPushButton * endConnection;
  WPushButton * endGame;
  WPushButton * giveUp;
  WPushButton * revenge;
  WPushButton * highScoresButton;
  WTable * gamesAvailable;
  WTable * everything;
  WTable * endRevenge;
  WTable * highScores;
  WText * success;
  WText * failure;
  WText * playerExitedText;
  WText * revengeProposedText;
  vector<std::pair<WPushButton*,GamesConnector::const_iterator> > joinButtons;
  bool ifRevenge;
  GamesConnector * gamesConnector;
  boost::shared_ptr<Game> gamePointer;	
  map<Coordinates,WPushButton*> gameButtons;
  Session * session_;
  const static int SIZE = 15;
};


#endif
