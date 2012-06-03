#include "menuWidget.h"
#include "../user.h"
#include <boost/lexical_cast.hpp>
#include <Wt/WText>
#include <Wt/WPushButton>
#include <Wt/WBreak>
#include <Wt/WLineEdit>
#include <Wt/WTable>
#include <Wt/WTableCell>
#include <Wt/WString>
#include <Wt/WAnchor>
#include <Wt/WAnchor>
#include <Wt/WText>
#include <Wt/WStackedWidget>
#include <Wt/WVBoxLayout>
#include <Wt/WHBoxLayout>
#include <Wt/WApplication>
#include <Wt/WInteractWidget>
#include <Wt/WEvent>
#include <Wt/WCompositeWidget>
#include <Wt/WLink>
#include <Wt/Ext/Button>
#include <Wt/WFont>
#include <iterator>
#include <string>
#include <vector>
#include <map>
using namespace Wt;
using namespace std;
menuWidget::menuWidget(WContainerWidget * parent, Session * session_tmp) : WContainerWidget(parent)
{
///////////////////////////////////////////////////////////////////
	sessionID = WApplication::instance()->sessionId();
WApplication::instance()->enableUpdates();
///////////////////////////////////////////////////////////////////
  setContentAlignment(AlignCenter);
  session_ = session_tmp;
  gamesConnector = &GamesConnector::getInstance();
  addBreaks();
  information = new WText("");
  addWidget(information);
  addBreaks(1);
  highScoresButton = new WPushButton("HighScores",this);
  
  everything = new WTable(this);
  newGameButton = new WPushButton("Create new game", this);
  newGameButton->clicked().connect(boost::bind(&menuWidget::processCreateNewGameButton,this,newGameButton));
  
  showGames = new WPushButton("Show available games",this);
  showGames->clicked().connect(boost::bind(&menuWidget::processShowGamesButton,this));
  addBreaks();
  startGame  = new WPushButton("Start the game",this);
  startGame->clicked().connect(boost::bind(&menuWidget::processStartGameButton,this));
  startGame->disable();
  startGame->hide();
  tmp2 = true; //Girls just wanna have fun
  joined = true; 
  ifCreator = false;
  gameEnded = false;
  endConnection = new WPushButton("Quit that game",this);
  endConnection->clicked().connect(boost::bind(&menuWidget::processEndGameConnection,this,endConnection));
  endConnection->hide();
  
  
  everything = new WTable(this);

  addWidget(endConnection);
  gamesAvailable = new WTable(this);
  gamesAvailable->setStyleClass("gamesAvailable");
  //gamesAvailable->setStyleClass("myStyle");
  addWidget(gamesAvailable);

  everything = new WTable(this);
  //everything->setStyleClass("myStyle");
  
  
  success = new WText("Game created correctly. Click \"Start Game \" to begin.");
  addWidget(success);
  success->hide();
  
  highScoresButton->clicked().connect(boost::bind(&menuWidget::processHighScoresButton,this,highScoresButton));
  highScores = new WTable(this);
  highScores->setStyleClass("gamesAvailable");
  highScores->hide();
  endGame = new WPushButton("End game");
  endGame->clicked().connect(boost::bind(&menuWidget::processEndGameButton,this));
  addWidget(endGame);
  endGame->hide();
  giveUp = new WPushButton("Give up");
  giveUp->clicked().connect(boost::bind(&menuWidget::processGiveUpButton,this,giveUp));
  addWidget(giveUp);
  giveUp->hide();
  giveUp->disable();
  revenge = new WPushButton("Take revenge"); 
  revenge->clicked().connect(boost::bind(&menuWidget::processRevengeButton,this,revenge));
  addWidget(revenge);
  revenge->hide();
}



void menuWidget::processHighScoresButton(WPushButton *b)
{
  information->setText("");
  success->hide();
  endConnection->hide();
  if(b->text() == "HighScores")
  {
    b->setText("Back to Game");
    newGameButton->hide();
    showGames->hide();
    startGame->hide();
    everything->hide(); //hide the elements of the game
    //Take the data from the server (10 users is enough)
    //Create the table
    highScores->clear();
    delete highScores;
    highScores = new WTable(this); //create new table of top gamers
    highScores->setStyleClass("gamesAvailable");
    highScores->setHeaderCount(1);
    addWidget(highScores);
    highScores->elementAt(0,0)->addWidget(new WText("List of top gamers"));

   std::vector<User> vec = session_->getHigestRankUsers(10);
   std::vector<User>::iterator it;
   int i = 1;
   for(it = vec.begin(); it!= vec.end();it++)
   {
     highScores->elementAt(i,0)->addWidget(new WText(it->getName()));
     highScores->elementAt(i,1)->addWidget(new WText(boost::lexical_cast<string>( it->getScore())));
     i++;
   } 
  }
  else if(b->text() == "Back to Game")
  {
    b->setText("HighScores");
    highScores->hide();
    newGameButton->show();
    showGames->show();
    everything->show(); //show the components of the game
  }
}

void menuWidget::processEndGameConnection(WPushButton *b)
{
  
  //close the connection with a decent game and let the user to choose another one
  information->setText("");
  startGame->hide();
  success->hide();
  endConnection->hide();
  showGames->enable();
  newGameButton->enable();
  gamesAvailable->clear();
  highScoresButton->show();
  delete gamesAvailable;
  gamesAvailable = new WTable(this);
  gamesAvailable->setStyleClass("gamesAvailable");
}

void menuWidget::processGiveUpButton(WPushButton*)
{
  information->setText("You gived up");
  endGame->enable();
  revenge->enable();
  giveUp->disable();
  map<Coordinates,WPushButton*>::iterator it;
  for(it = gameButtons.begin(); it != gameButtons.end();it++)
  {
    it->second->disable();
  }
  gamePointer->giveUp(*this);
}

void menuWidget::processCreateNewGameButton(WPushButton *b)
{
  //stuff that requires database connection
  //if everything's fine, database return info (let's say "true")
  //and we can proceed
  information->setText("New game created, waiting for another player");
  gamePointer =  gamesConnector->newGame(*this);
  joined = false; 
  ifCreator = true;
  endConnection->show();
  showGames->disable();
  gamesAvailable->hide();
  success->show();
  newGameButton->disable();
  startGame->disable();
  startGame->show();
  highScoresButton->hide();
  endGame->enable();
  endConnection->hide();
  success->hide();

  if(newGameButton) newGameButton->hide();
  showGames->hide();
  

  //preparing the game screen (nothing interesting above)
  gameButtons.clear();
  everything->clear();
  delete everything;
  everything = new WTable(this);
  //everything->setStyleClass("myStyle");
  for(int i = 0; i<SIZE; i++)
  {
    for(int j = 0; j<SIZE; j++){
      WPushButton * newButton = new WPushButton();
      newButton->resize(35,35); //images are 30x30, just in case
      newButton->disable();
      newButton->setVerticalAlignment(Wt::AlignMiddle);
      //newButton->setIcon(WLink("/white.jpg"));
      newButton->clicked().connect(boost::bind(&menuWidget::processClickButton,this,newButton,i,j));
      everything->elementAt(i,j)->addWidget(newButton);
      gameButtons.insert(make_pair(Coordinates(j,i),newButton));
    }//adding the noughts&crosses buttons (225)
  }
  map<Coordinates,WPushButton*>::iterator it;
  for(it  = gameButtons.begin(); it!=gameButtons.end();it++)
  {
    it->second->disable();
  }
  endGame->show();
  giveUp->show();
  revenge->show();
  revenge->disable();
  gamesConnector->unregister(*this);
  //adding two buttons on the right (End Game and Take Revenge)
}

void menuWidget::processShowGamesButton()
{
  information->setText("");
  std::cout<<"POCZATEK SHOW GAMES"<<std::endl;
  highScoresButton->hide();
  showGames->disable();
  newGameButton->disable();
  success->hide();
  joinButtons.clear();
  gamesAvailable->clear();
  
  delete gamesAvailable;
  gamesAvailable = new WTable(this);
  
  gamesAvailable->setStyleClass("gamesAvailable");
  addWidget(gamesAvailable);
  gamesAvailable->elementAt(0,0)->addWidget(new WText("List of all available games:"));
   //preparing whole "scene" (nothing interesting above)
  
  //TODO get data from the database. By now I just add some random values
  //Some loop gonna be necessary here
  int i = 1;
  gamesConnector->iterationBegin();
  GamesConnector::const_iterator cit = gamesConnector->begin();
  for(cit ; cit != gamesConnector->end(); ++cit)
  {   
    string gameName = *cit; //this will be returned by the database
    //gameName is important, 'cause it is the parameter of the join event
    gamesAvailable->elementAt(i,0)->addWidget(new WText(gameName));
    WPushButton * btn = new WPushButton("Join",this);
    btn->clicked().connect(boost::bind(&menuWidget::processChooseGameButton,this,btn));
    gamesAvailable->elementAt(i,1)->addWidget(btn);
    joinButtons.push_back(std::make_pair(btn,cit)); //vector of join buttons
    std::cout<<"ITERACJA"<<std::endl;
    i++;
  }  
  std::cout<<"PO ITERACJI"<<std::endl;
  gamesConnector->iterationEnd();
  gamesConnector->refRegister(*this); 
  
  WPushButton * hideGames = new WPushButton("Hide games list");
  hideGames->clicked().connect(boost::bind(&menuWidget::processHideListButton,this));
  gamesAvailable->elementAt(i,0)->addWidget(hideGames);
	std::cout<<"WYCHODZE"<<std::endl;
}  


void menuWidget::processStartGameButton()
{
    information->setText("Waiting for the opponent to start the game"); 
    startGame->disable();
    //endGame->disable();
    //giveUp->enable();
    map<Coordinates,WPushButton*>::iterator it;
    gamePointer->start(*this);
}

void menuWidget::processChooseGameButton(WPushButton *b)
{
  //stuff that requires database connection
  //if everything's fine, database return info (let's say "true")
  //and we can proceed
   information->setText("Press start game");
  endConnection->show();
  showGames->disable();
  gamesAvailable->hide();
  success->show();
  newGameButton->disable();
  startGame->enable();
  startGame->show();
  highScoresButton->hide();
  endGame->enable();
  endConnection->hide();
  success->hide();

  if(gamesAvailable) gamesAvailable->hide();
  if(newGameButton) newGameButton->hide();
  showGames->hide();
  int i;
  for( i = 0; i < joinButtons.size(); i++)
  {
    if(joinButtons[i].first == b)
      break;
  }
  
  gamePointer = gamesConnector->join(joinButtons[i].second, *this);
  //preparing the game screen (nothing interesting above)
  gameButtons.clear();
  removeWidget(everything);
  everything->clear();
  delete everything;
  everything = new WTable(this);
  //everything->setStyleClass("myStyle");
  for(int i = 0; i<SIZE; i++)
  {
    for(int j = 0; j<SIZE; j++){
      WPushButton * newButton = new WPushButton();
      newButton->resize(35,35); //images are 30x30, just in case
      newButton->disable();
      newButton->setVerticalAlignment(Wt::AlignMiddle);
      //newButton->setIcon(WLink("/white.jpg"));
      newButton->clicked().connect(boost::bind(&menuWidget::processClickButton,this,newButton,i,j));
      everything->elementAt(i,j)->addWidget(newButton);
      gameButtons.insert(make_pair(Coordinates(j,i),newButton));
    }//adding the noughts&crosses buttons (225)
  }
  endGame->show();
  giveUp->show();
  revenge->show();
  revenge->disable();
  addWidget(everything);
  //adding two buttons on the right (End Game and Take Revenge)
  vector<std::pair<WPushButton*,GamesConnector::const_iterator> >::iterator it;
  for(it = joinButtons.begin(); it!=joinButtons.end(); ++it)
  {
    it->first->disable();
    //(*it)->hide();
    it->first->hide();
  }
  
  //making all join buttons unavailable to touch
  
  //TODO Send the info to the database about which game was chosen

}

void menuWidget::processClickButton(WPushButton *b, int i, int j)
{
  //IMPORTANT PLACE
  //Send the info to the database that button has been clicked
  //change the image depends on who made a move
  //by now it is just - one time cross, one time circle
  gamePointer->putField(*this,j,i);
  success->hide();
  //b->disable();
}

void menuWidget::processHideListButton()
{
  information->setText("");
  success->hide();
  newGameButton->enable();
  gamesAvailable->hide();
  highScoresButton->show();
  showGames->enable();
  gamesConnector->unregister(*this);
  //nothin' special
}

void menuWidget::processEndGameButton()
{
  information->setText("");
  gamePointer->exit(*this);
  if(!joined)
  {
    gamesConnector->deleteGame(gamePointer);
    joined = true;
  }
  highScoresButton->show();
  everything->clear();
  delete everything;
  everything = new WTable(this);
  //everything->setStyleClass("myStyle");
  success->hide();
  newGameButton->show();
  newGameButton->enable();
  showGames->show();
  startGame->disable();
  startGame->hide();
  showGames->enable();
  if(gamesAvailable) gamesAvailable->hide();
  endGame->hide();
  giveUp->hide();
  revenge->hide();
  revenge->enable();
  //preparing the scene after "destroying" the current game
}

void menuWidget::processRevengeButton(WPushButton * b)
{
    information->setText("Revenge proposed. Press start to begin");
    gamePointer->revenge(*this);
    map<Coordinates,WPushButton*>::iterator it;
    for(it = gameButtons.begin(); it!=gameButtons.end(); it++)
    {
      removeWidget(it->second);
    }
    success->hide();  
    startGame->enable();
    //preparing the scene (nothing interesting above)
    gameButtons.clear();
    everything->clear();
    delete everything;
    everything = new WTable(this);
    //everything->setStyleClass("myStyle");
    for(int i = 0; i<SIZE; i++)
    {
    for(int j = 0; j<SIZE; j++){
      WPushButton * newButton = new WPushButton();
      newButton->disable();
      newButton->resize(35,35); //images are 30x30, just in case
      newButton->setVerticalAlignment(Wt::AlignMiddle);
      //newButton->setIcon(WLink("/white.jpg"));
      newButton->clicked().connect(boost::bind(&menuWidget::processClickButton,this,newButton,i,j));
      everything->elementAt(i,j)->addWidget(newButton);
      gameButtons.insert(make_pair(Coordinates(j,i),newButton));
    }//adding the noughts&crosses buttons (225)
  }
 
  revenge->disable();
  addWidget(endGame);
  addWidget(giveUp);
  addWidget(revenge);
  //loop is almost exactly the same as in processStartGameButton
  //we need to clear all modifications that was made on the game Buttons
  
}

void menuWidget::gameStarted()
{
  WApplication * app = WApplication::instance();
  std::cout<<"GAME STARTED"<<std::endl;
  gameEnded = false;
  if(ifCreator) 
  {
   information->setText("It's your turn");
  }
  endGame->disable();
  giveUp->enable();
  map<Coordinates,WPushButton*>::iterator it;
  for(it = gameButtons.begin(); it != gameButtons.end(); it++)
  {
    it->second->enable();
  }
  app->triggerUpdate();
}

void menuWidget::fieldChanged( FieldX &f , int x , int y )
{
  WApplication *app = WApplication::instance();
  gameButtons[Coordinates(x,y)]->setIcon(WLink("/cross.png"));
  ifCreator = !ifCreator;
  if(!gameEnded)
  {
  if(ifCreator) 
   information->setText("It's your turn");   
  else information->setText("Waiting for opponent move");
  }
  gameButtons[Coordinates(x,y)]->disable();
  app->triggerUpdate();
}
void menuWidget::fieldChanged( FieldO &f, int x, int y )
{
  WApplication *app = WApplication::instance();
  gameButtons[Coordinates(x,y)]->setIcon(WLink("/circle.png"));
  gameButtons[Coordinates(x,y)]->disable();
  ifCreator = !ifCreator;
  if(!gameEnded)
  {
  if(ifCreator)
    information->setText("It's your turn");
  else information->setText("Waiting for opponent move");
  }
  app->triggerUpdate();
}

void menuWidget::wonByGivingUp()
{
  WApplication *app = WApplication::instance();
  endGame->enable();
  information->setText("Your opponent has given up");
  revenge->enable();
  giveUp->disable();
  map<Coordinates,WPushButton*>::iterator it;
  for(it = gameButtons.begin(); it != gameButtons.end();it++)
  {
    it->second->disable();
  }
  app->triggerUpdate();
  std::cout<<"WON BY GIVING UP"<<std::endl;
}

void menuWidget::playerExited()
{
  WApplication *app = WApplication::instance();
  information->setText("Your opponent has exited");
  processEndGameButton();
  app->triggerUpdate();
}

void menuWidget::revengeProposed()
{
  WApplication *app = WApplication::instance();
  information->setText("Revenge was proposed. Press start to begin");
  startGame->enable();
  revenge->disable();
  giveUp->disable();
  endGame->enable();
      gameButtons.clear();
    everything->clear();
    delete everything;
    everything = new WTable(this);
    //everything->setStyleClass("myStyle");
    for(int i = 0; i<SIZE; i++)
    {
    for(int j = 0; j<SIZE; j++){
      WPushButton * newButton = new WPushButton();
      newButton->disable();
      newButton->resize(35,35); //images are 30x30, just in case
      newButton->setVerticalAlignment(Wt::AlignMiddle);
      //newButton->setIcon(WLink("/white.jpg"));
      newButton->clicked().connect(boost::bind(&menuWidget::processClickButton,this,newButton,i,j));
      everything->elementAt(i,j)->addWidget(newButton);
      gameButtons.insert(make_pair(Coordinates(j,i),newButton));
    }//adding the noughts&crosses buttons (225)
  }
 
  app->triggerUpdate();
}

void menuWidget::refreshGameList()
{
  WApplication *app = WApplication::instance();
  startGame->disable();
  processShowGamesButton();
  app->triggerUpdate();
}

bool operator<(const menuWidget::Coordinates& c1, const menuWidget::Coordinates& c2)
  {
    if (c1.x != c2.x) return (c1.x < c2.x);
    else return (c1.y < c2.y);
  }

void menuWidget::playerJoined()
{
	WApplication *app = WApplication::instance();
  information->setText("Opponent connected. Press start to begin");
  std::cout<<" MOJ "<<(GameListRefresher*)this<<std::endl;
  startGame->enable();
  startGame->refresh();
	app->triggerUpdate();
} 

void menuWidget::endedWithWin(int a, int b, int c, int d)
{
  WApplication *app = WApplication::instance();
  std::cout<<"ENDED WITH WIN"<<std::endl;
  gameEnded = true;
  information->setText("You won!!!");
  giveUp->disable();
  endGame->enable();
  revenge->enable();    
  map<Coordinates,WPushButton*>::iterator it;
  for(it = gameButtons.begin(); it != gameButtons.end();it++)
  {
    it->second->disable();
  }
  if(a == c)
  {
    while(b!=d)
    {
      gameButtons[Coordinates(a,b)]->setIcon("/black.png");
      b++;
    }
  }
  else if (b == d)
  {
    while(a!=c)
    {
      gameButtons[Coordinates(a,b)]->setIcon("/black.png");
      a++;
    }
  }
  else
  {
    if(a<c)
    {
      while (a!=c && b!=d)
      {
        gameButtons[Coordinates(a,b)]->setIcon("/black.png");
        a++;
        b++;
      }
    }
    else
    {
      while( a!=c && b!=d)
      {
       gameButtons[Coordinates(a,b)]->setIcon("/black.png");
       a--;
       b--;
      }
    }
  }
  app->triggerUpdate();
}

void menuWidget::endedWithDraw(int a, int b, int c, int d)
{
  WApplication *app = WApplication::instance();
  std::cout<<"ENDED WITH DRAW"<<std::endl;
  information->setText("Ended with draw");
  gameEnded = true;
  giveUp->disable();
  endGame->enable();
  revenge->enable(); 
  map<Coordinates,WPushButton*>::iterator it;

  app->triggerUpdate();
}

void menuWidget::endedWithLose(int a, int b, int c, int d)
{
  WApplication *app = WApplication::instance();
  information->setText("You loosed!");
  std:cout<<"ENDED WITH LOSE"<<std::endl;
  gameEnded = true;
  giveUp->disable();
  endGame->enable();
  revenge->enable(); 
  map<Coordinates,WPushButton*>::iterator it;
  for(it = gameButtons.begin(); it != gameButtons.end();it++)
  {
    it->second->disable();
  }
  if(a == c)
  {
    while(b!=d)
    {
      gameButtons[Coordinates(a,b)]->setIcon("/black.png");
      b++;
    }
  }
  else if (b == d)
  {
    while(a!=c)
    {
      gameButtons[Coordinates(a,b)]->setIcon("/black.png");
      a++;
    }
  }
  else
  {
    if(a<c)
    {
      while (a!=c && b!=d)
      {
        gameButtons[Coordinates(a,b)]->setIcon("/black.png");
        a++;
        b++;
      }
    }
    else
    {
      while( a!=c && b!=d)
      {
       gameButtons[Coordinates(a,b)]->setIcon("/black.png");
       a--;
       b--;
      }
    }
  }
  
  
  app->triggerUpdate();
}

std::string& menuWidget::getUserName()
{
  return userName;
}
void menuWidget::addBreaks(int num)
{
  for(int i = 0; i<=num; i++)
    addWidget(new WBreak(this));
} // helpful function




