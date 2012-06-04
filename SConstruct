src_main = "server.cpp "

src_ipaddress = "ipaddress.cpp "

src_server = "oxgameserver.cpp "

src_authapp = "mainapp.cpp session.cpp user.cpp "

src_connector = "gamesconnector.cpp "

src_game = "fields.cpp WinningLine.cpp OXGame.cpp game.cpp endofgamelistener.cpp "
src_game_test = "OXGame_test.cpp "

src_kubik = " menuWidget/menuWidget.cpp  menu.cpp "

liby = Split('boost_signals boost_regex boost_thread wthttp wt wtdbo wtdbosqlite3 boost_unit_test_framework ');

if ARGUMENTS.get('test', '0') == '1':
	print "* * *BUILDING TESTS* * *"
	Program( 'connectorTest'   , Split( src_connector + 'user.cpp'), CCFLAGS=' -D__GAMESCONNECTOR_TEST__' , LIBS = liby )
elif ARGUMENTS.get('test', '0') == '2':	
	print "* * *BUILDING TESTS* * *"
	Program('OXGameTest' ,  Split(src_connector +src_game + src_game_test +'user.cpp '+ 'session.cpp ' ), LIBS = liby)

else:
	print "* * *BUILDING RELEASE* * *"
	Program('server' ,Split(src_main + src_server + src_ipaddress + src_authapp + src_kubik + src_connector + src_game) , LIBS = liby)
