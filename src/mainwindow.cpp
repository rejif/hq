#include "mainwindow.h"
#include <QString>
#include <QDebug>
#include <QObject>
#include <QAction>
#include <QVBoxLayout>
#include <QProcess>
#include <QPushButton>
#include <QStandardPaths>
QString getUsername(){
    QString name = qgetenv("USER");
    return !name.isEmpty()?name:qgetenv("USERNAME");
}
template<typename Functor>
QAction* createLambdaAction(const QString actText,Functor f){
    QAction* act = new QAction(actText);
    QObject::connect(act,&QAction::triggered,f);
    return act;
}
template<typename Functor>
QPushButton* createLambdaActionButton(QString btnText,Functor f){
   QPushButton *lambdaBtn = new QPushButton(btnText);
   QAction* lambdaAct = createLambdaAction(btnText+"Action",f);
   QObject::connect(
       lambdaBtn,SIGNAL(clicked(bool)),
       lambdaAct,SLOT(trigger())
   );
   return lambdaBtn;
}
QPushButton* createExecuteBtn(QString btnText,QString command){
    return createLambdaActionButton(btnText,[=]() { QProcess::execute(command); });
}
QPushButton* createDetachBtn(QString btnText,QString command){
    return createLambdaActionButton(btnText,[=]() { QProcess::startDetached(command); });
}
QPushButton* createQDetachBtn(QString btnText,QString command){
    return createDetachBtn(btnText,"\""+command+"\"");
}
MainWindow::MainWindow(QWidget *parent):QMainWindow(parent){
    setWindowTitle("HQ");
    QWidget * wdg = new QWidget(this);
    QVBoxLayout *vlay = new QVBoxLayout(wdg);
    wdg->setLayout(vlay);
    setCentralWidget(wdg);
#if defined(_WIN32) || defined(_WIN64)
    vlay->addWidget(createExecuteBtn("CMD","C:/Windows/System32/cmd.exe /C cd \\ && start cmd"));
    vlay->addWidget(createExecuteBtn("PowerShell","C:/Windows/System32/cmd.exe /C cd \\ && start C:/Windows/System32/WindowsPowerShell/v1.0/powershell.exe"));
    vlay->addWidget(createDetachBtn("Desktop","\"C:/Windows/explorer.exe\" \""+QStandardPaths::writableLocation(QStandardPaths::DesktopLocation).replace("/","\\")+"\""));
    vlay->addWidget(createDetachBtn("Notepad","notepad"));
    vlay->addWidget(createDetachBtn("Explorer","C:/Windows/explorer.exe"));
    vlay->addWidget(createDetachBtn("Service","\"C:/Windows/System32/mmc.exe\" \"C:/Windows/System32/services.msc\""));
    vlay->addWidget(createQDetachBtn("WacomTabletUtility","C:/Program Files/Tablet/Wacom/32/PrefUtil.exe"));
    vlay->addWidget(createQDetachBtn("Everything","C:/Program Files/Everything/Everything.exe"));
    vlay->addWidget(createQDetachBtn("PowerMate","C:/Program Files (x86)/Griffin Technology/PowerMate/PowerMate.exe"));
    vlay->addWidget(createDetachBtn("Shutdown1h","C:/Windows/System32/Shutdown.exe -s -t 3600"));
    vlay->addWidget(createDetachBtn("ShutdownCancel","C:/Windows/System32/Shutdown.exe -a"));
    vlay->addWidget(createQDetachBtn("GitBash","C:/Program Files/Git/git-bash.exe"));
    vlay->addWidget(createQDetachBtn("GoogleChrome","C:/Program Files (x86)/Google/Chrome/Application/chrome.exe"));
    vlay->addWidget(createQDetachBtn("Firefox","C:/Program Files/Mozilla Firefox/firefox.exe"));
    vlay->addWidget(createDetachBtn("Vivaldi",QStandardPaths::writableLocation(QStandardPaths::HomeLocation)+"/AppData/Local/Vivaldi/Application/vivaldi.exe"));
    vlay->addWidget(createQDetachBtn("Photoshop","C:/Program Files/Adobe/Adobe Photoshop CS5.1 (64 Bit)/Photoshop.exe"));
    vlay->addWidget(createQDetachBtn("Illustrator","C:/Program Files (x86)/Adobe/Adobe Illustrator CS5.1/Support Files/Contents/Windows/Illustrator.exe"));
    vlay->addWidget(createQDetachBtn("InDesign","C:/Program Files (x86)/Adobe/Adobe InDesign CS5.5/InDesign.exe"));
    vlay->addWidget(createQDetachBtn("Audition","C:/Program Files (x86)/Adobe/Adobe Audition CS5.5/Adobe Audition.exe"));
    vlay->addWidget(createQDetachBtn("AfterEffect","C:/Program Files/Adobe/Adobe After Effects CS5.5/Support Files/AfterFX.exe"));
    vlay->addWidget(createQDetachBtn("Premiere","C:/Program Files/Adobe/Adobe Premiere Pro CS5.5/Adobe Premiere Pro.exe"));
    vlay->addWidget(createQDetachBtn("MediaEncoder","C:/Program Files/Adobe/Adobe Media Encoder CS5.5/Adobe Media Encoder.exe"));
    vlay->addWidget(createQDetachBtn("TeamViewer","C:/Program Files (x86)/TeamViewer/TeamViewer.exe"));
    vlay->addWidget(createQDetachBtn("IntelliJIDEA","C:/Program Files (x86)/JetBrains/IntelliJ IDEA 2016.2.5\bin/idea64.exe"));
    vlay->addWidget(createQDetachBtn("Clion","C:/Program Files (x86)/JetBrains/CLion 2016.2.3/bin/clion64.exe"));
    vlay->addWidget(createQDetachBtn("DataGrip","C:/Program Files (x86)/JetBrains/DataGrip 2016.2.6/bin/datagrip64.exe"));
    vlay->addWidget(createQDetachBtn("PhpStorm","C:/Program Files (x86)/JetBrains/PhpStorm 2016.2.2/bin/PhpStorm64.exe"));
    vlay->addWidget(createQDetachBtn("PyCharm","C:/Program Files (x86)/JetBrains/PyCharm 2016.2.3/bin/pycharm64.exe"));
    vlay->addWidget(createQDetachBtn("RubyMine","C:/Program Files (x86)/JetBrains/RubyMine 2016.2.5/bin/rubymine64.exe"));
    vlay->addWidget(createQDetachBtn("WebStorm","C:/Program Files (x86)/JetBrains/WebStorm 2016.2.4/bin/WebStorm64.exe"));
    vlay->addWidget(createQDetachBtn("DbVisualizer","C:/Program Files/DbVisualizer/dbvis.exe"));
    vlay->addWidget(createQDetachBtn("Maya2016","C:/Program Files/Autodesk/Maya2016/bin/maya.exe"));
    vlay->addWidget(createDetachBtn("QtCreator","C:/Qt/Tools/QtCreator/bin/qtcreator.exe"));
#else
    qDebug() << "not win";
#endif
}
MainWindow::~MainWindow(){}
