src_main = "server.cpp "

src_ipaddress = "ipaddress.cpp "

src_server = "oxgameserver.cpp "

src_authapp = "authapp.cpp session.cpp user.cpp "

liby = Split('boost_regex boost_thread wthttp wt wtdbo wtdbosqlite3');

Program('server' ,Split(src_main + src_server + src_ipaddress + src_authapp) , LIBS = liby)
