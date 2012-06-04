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
  
  //some class for representing button coordinates
  struct Coordinates
  {
    Coordinates(int c_x, int c_y) { x = c_x; y = c_y;}
    int x;
    int y;
  };
  bool tmp;
  bool tmp2;
  /////////////////////////////////////////////////////////////////
 //		Inherited from GameListRefresher                //
///////////////////////////////////////////////////////////////// 
 
  virtual void gameStarted();
 
  virtual void fieldChanged( FieldX &f , int x , int y );
 
  virtual void fieldChanged( FieldO &f , int x , int y );
 
  virtual void wonByGivingUp();
 
  virtual void playerExited();
 
  virtual void revengeProposed();
 
  virtual void refreshGameList();
 
  virtual void playerJoined();  
 
  virtual void endedWithDraw(int, int , int , int); 
 
  virtual void endedWithWin(int , int , int , int ); 
 
  virtual void endedWithLose(int , int , int , int ); 
 
  virtual std::string& getUserName();
 
  virtual void setUserName(std::string user_name) { userName = user_name;}
 
  virtual std::string& getSessionID() {	return sessionID; 	}
  
  virtual Session* getSession() { 	return session_;	}
///////////////////////////////////////////////////////////

private :
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
  
  //some buttons
  WPushButton * newGameButton;
  WPushButton * startGame;
  WPushButton * showGames;
  WPushButton * endConnection;
  WPushButton * endGame;
  WPushButton * giveUp;
  WPushButton * revenge;
  WPushButton * highScoresButton;
  
  //some tables
  WTable * gamesAvailable;
  WTable * everything;
  WTable * endRevenge;
  WTable * highScores;
  
  //some texts
  WText * success;
  WText * failure;
  WText * information;


  vector<std::pair<WPushButton*,GamesConnector::const_iterator> > joinButtons;
  //some state bool's
  bool joined;
  bool ifCreator;
  bool gameEnded;
  //Pointer to games base
  GamesConnector * gamesConnector;
  
  //id of current usre's session
  std::string sessionID;
  
  //smart pointer to game object
  boost::shared_ptr<Game> gamePointer;	
  
  //map of buttons (x and o)
  map<Coordinates,WPushButton*> gameButtons;
  
  //pointer to session object to communicate with database
  Session * session_;
  std::string userName;
  const static int SIZE = 15;
};


#endif
