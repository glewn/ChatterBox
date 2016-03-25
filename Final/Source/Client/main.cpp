/*------------------------------------------------------------------------------------------------------------------
-- SOURCE FILE: main.cpp
--
-- PROGRAM: ChatterBox
--
-- FUNCTIONS:
-- int main(int argc, char *argv[])
--
-- DATE: March 09, 2016
--
-- DESIGNER: Gabriel Lee
--
-- PROGRAMMER: Gabriel Lee
--
-- NOTES:
-- The entry point of the program.
----------------------------------------------------------------------------------------------------------------------*/
#include "mainwindow.h"
#include <QApplication>

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: main
--
-- DATE: March 09, 2016
--
-- REVISIONS: (Date and Description)
-- March 09, 2016 - Function created.
--
-- DESIGNER: Gabriel Lee
--
-- PROGRAMMER: Gabriel Lee
--
-- INTERFACE: int main(int argc, char *argv[])
--
-- RETURNS: int - the exit code of the program
--
-- NOTES:
-- The main entry point of the program
----------------------------------------------------------------------------------------------------------------------*/
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
