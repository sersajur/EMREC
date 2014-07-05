#include "View/mainwindow.h"
#include <QApplication>
#include "Presenter/presenter.h"

int main(int argc, char *argv[])
{
    QDir appDir = QFileInfo(QString::fromLocal8Bit(argv[0])).absolutePath();
    QString tmpPath = appDir.absolutePath();
    QApplication::addLibraryPath(tmpPath+"/plugins");
    QApplication a(argc, argv);
    MainWindow w;
    Presenter p(&w);
    w.show();
    return a.exec();
}
