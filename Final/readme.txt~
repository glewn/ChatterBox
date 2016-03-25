Run
    - Server
	build file using Source : [../Source/Server]
		make
	use source file [../Execute]
		"./server <port>"   
			: port number is optional. Default port is 7000.
    - Client
	build file using QT Creator
		Load the *.pro file and build.
	use source file [../Executable]
		- "./ChatterBox"  : It will open client UI
		- input IP address, Port number, nickname
		- press 'OK' button
		- Enjoy chatting

Directory
    - Document
	- Design Document
	- Test Document
	- User Manual
    - Source
	- Client
	    clientsrc.cpp/ clientsrc.h		 : Client socket related source
	    Value.h				 : defined value (shared value)
	    client.cpp / client.h / client.ui    : Client UI related source
	    connection.cpp / connection.h	 : connection between main window and others
	    main.cpp				 : start file
	    mainwindow.cpp/ mainwindow.h	 : UI main window files
	    
	- Server
	    server.cpp / server.h 		 : Server socket source
	    Value.h				 : defined value (shared value)
	    logfile.txt				 : server side log file to check message flow
	    makefile				 : easy to make object

    - Executable
	server					: server file
	ChatterBox				: client file
