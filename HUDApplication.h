/*===================================================================
APM_PLANNER Open Source Ground Control Station

(c) 2014 Patrick Duffy

This file is part of the APM_PLANNER project

    APM_PLANNER is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    APM_PLANNER is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with APM_PLANNER. If not, see <http://www.gnu.org/licenses/>.

======================================================================*/

#ifndef HUDAPPLICATION_H
#define HUDAPPLICATION_H

#include <QApplication>
#include <QMainWindow>

class HUDApplication: public QApplication
{
	Q_OBJECT

public:
    HUDApplication(int &argc, char **argv):QApplication(argc,argv){}
	void sendCloseSignal() { emit close(); }

signals:
	bool close();
};

class MainWindow : public QMainWindow {

    Q_OBJECT;

public:
	MainWindow(QWidget *parent = 0, Qt::WindowFlags flags = 0) : QMainWindow(parent, flags) {}

protected:
     void closeEvent(QCloseEvent *pEvent);
};

extern QMainWindow *theMainWindow;

#endif
