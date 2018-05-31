#include "mainwindow.h"
#include <QApplication>
#include <QDesktopWidget>
#include <QRect>
#include <QSystemTrayIcon>
int main(int argc, char *argv[]){
    QApplication a(argc, argv);

    Q_INIT_RESOURCE(resource);

    //SystemTray
    if (!QSystemTrayIcon::isSystemTrayAvailable()){
        QMessageBox::critical(0,
            QObject::tr("Critical Error"),
            QObject::tr("I couldn't detect any system tray on this system."));
        qApp->quit();
    }else{
        QSystemTrayIcon *trayIcon = new QSystemTrayIcon();
        trayIcon->setIcon(QIcon(":/hq.png"));
        QObject::connect(trayIcon, &QSystemTrayIcon::activated, [=](QSystemTrayIcon::ActivationReason reason) {
            if (reason == QSystemTrayIcon::DoubleClick){
                //qDebug()<<"SystemTrayDoubleClickTrigger";
            }
        });
        trayIcon->show();
    }

    QRect aa(QApplication::desktop()->availableGeometry());
    MainWindow mw;
    mw.show();
    mw.move(aa.width()-mw.width(),0);//TopRight Bottom:aa.height()-mw.height()

    return a.exec();
}
