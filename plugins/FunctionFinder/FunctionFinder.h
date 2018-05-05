/*
Copyright (C) 2006 - 2015 Evan Teran
                          evan.teran@gmail.com

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef FUNCTIONFINDER_20060430_H_
#define FUNCTIONFINDER_20060430_H_

#include "IPlugin.h"

class QMenu;
class QDialog;

namespace FunctionFinderPlugin {

class FunctionFinder : public QObject, public IPlugin {
	Q_OBJECT
	Q_INTERFACES(IPlugin)
	Q_PLUGIN_METADATA(IID "edb.IPlugin/1.0")
	Q_CLASSINFO("author", "Evan Teran")
	Q_CLASSINFO("url", "http://www.codef00.com")

public:
	FunctionFinder();
	virtual ~FunctionFinder();

public:
	virtual QMenu *menu(QWidget *parent = 0);

public Q_SLOTS:
	void show_menu();

private:
	QMenu *   menu_;
	QPointer<QDialog> dialog_;
};

}


#endif
