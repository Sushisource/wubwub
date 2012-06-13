#include "ssmp.h"
#include <QtGui/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	ssmp w;	
	w.show();
	return a.exec();
}
