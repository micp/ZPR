src_main = "server.cpp "

src_ipaddress = "ipaddress.cpp "

src_server = "oxgameserver.cpp "

src_authapp = "mainapp.cpp session.cpp user.cpp "

src_connector = "gamesconnector.cpp "

src_game = "fields.cpp OXGame.cpp game.cpp endofgamelistener.cpp "
src_game_test = "OXGame_test.cpp"

src_kubik = " menuWidget/menuWidget.cpp accountWidget/accountWidget.cpp loginWidget/loginWidget.cpp menu.cpp "

liby = Split('boost_signals boost_regex boost_thread wthttp wt wtdbo wtdbosqlite3 boost_unit_test_framework ');

if ARGUMENTS.get('test', '0') == '1':
	print "* * *BUILDING TESTS* * *"
	Program( 'connectorTest' , Split( src_connector + 'user.cpp '  ) , LIBS = liby , CCFLAGS=' -D__GAMESCONNECTOR_TEST__')
	Program('OXGameTest', Split(src_game + src_game_test))

else:
	print "* * *BUILDING RELEASE* * *"
	Program('server' ,Split(src_main + src_server + src_ipaddress + src_authapp + src_kubik + src_connector + src_game) , LIBS = liby)
