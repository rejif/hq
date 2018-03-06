#include "mainwindow.h"
#include <QVBoxLayout>
#include <QDebug>
#include <QProcess>
#include <QPushButton>
#include <QStandardPaths>

QPushButton* createQProcBtn(QString btnText,QString command){
  QPushButton *qProcBtn;
  qProcBtn = new QPushButton(btnText);
  QObject::connect(qProcBtn, &QPushButton::clicked, [=]() {
       QProcess::startDetached("\""+command+"\"");
  });
  return qProcBtn;
}
QPushButton* createServiceBtn(QString btnText){
  QPushButton *qProcBtn;
  qProcBtn = new QPushButton(btnText);
  QObject::connect(qProcBtn, &QPushButton::clicked, [=]() {
      QProcess::startDetached("\"C:/Windows/System32/mmc.exe\" \"C:/Windows/System32/services.msc\"");
   });
  return qProcBtn;
}
QPushButton* createCmdBtn(QString btnText,QString cmd){
  QPushButton *qProcBtn;
  qProcBtn = new QPushButton(btnText);
  QObject::connect(qProcBtn, &QPushButton::clicked, [=]() {
      QProcess::startDetached(cmd);
   });
  return qProcBtn;
}
QPushButton* openCommandPrompt(){
  QPushButton *qProcBtn;
  qProcBtn = new QPushButton("CMD");
  QObject::connect(qProcBtn, &QPushButton::clicked, [=]() {
      QProcess::execute("C:/windows/system32/cmd.exe /C cd \\ && start cmd");
   });
  return qProcBtn;
}

MainWindow::MainWindow(QWidget *parent):QMainWindow(parent){
    setWindowTitle("HQ");
    QWidget * wdg = new QWidget(this);
    QVBoxLayout *vlay = new QVBoxLayout(wdg);
    wdg->setLayout(vlay);
    setCentralWidget(wdg);
#if defined(_WIN32) || defined(_WIN64)
    vlay->addWidget(openCommandPrompt());
    vlay->addWidget(createQProcBtn("Notepad","notepad"));
    vlay->addWidget(createQProcBtn("Explorer","C:/Windows/explorer.exe"));
    vlay->addWidget(createServiceBtn("Service"));
    vlay->addWidget(createQProcBtn("WacomTabletUtility","C:/Program Files/Tablet/Wacom/32/PrefUtil.exe"));
    vlay->addWidget(createQProcBtn("PowerMate","C:/Program Files (x86)/Griffin Technology/PowerMate/PowerMate.exe"));
    vlay->addWidget(createQProcBtn("Everything","C:/Program Files/Everything/Everything.exe"));
    vlay->addWidget(createCmdBtn("Shutdown1h","C:/Windows/System32/Shutdown.exe -s -t 3600"));
    vlay->addWidget(createCmdBtn("ShutdownCancel","C:/Windows/System32/Shutdown.exe -a"));
    vlay->addWidget(createQProcBtn("GitBash","C:/Program Files/Git/git-bash.exe"));
    vlay->addWidget(createQProcBtn("GoogleChrome","C:/Program Files (x86)/Google/Chrome/Application/chrome.exe"));
    vlay->addWidget(createQProcBtn("Firefox","C:/Program Files/Mozilla Firefox/firefox.exe"));
    vlay->addWidget(createQProcBtn("Vivaldi",QStandardPaths::writableLocation(QStandardPaths::HomeLocation)+"/AppData/Local/Vivaldi/Application/vivaldi.exe"));
    vlay->addWidget(createQProcBtn("Photoshop","C:/Program Files/Adobe/Adobe Photoshop CS5.1 (64 Bit)/Photoshop.exe"));
    vlay->addWidget(createQProcBtn("Illustrator","C:/Program Files (x86)/Adobe/Adobe Illustrator CS5.1/Support Files/Contents/Windows/Illustrator.exe"));
    vlay->addWidget(createQProcBtn("InDesign","C:/Program Files (x86)/Adobe/Adobe InDesign CS5.5/InDesign.exe"));
    vlay->addWidget(createQProcBtn("Audition","C:/Program Files (x86)/Adobe/Adobe Audition CS5.5/Adobe Audition.exe"));
    vlay->addWidget(createQProcBtn("AfterEffect","C:/Program Files/Adobe/Adobe After Effects CS5.5/Support Files/AfterFX.exe"));
    vlay->addWidget(createQProcBtn("Premiere","C:/Program Files/Adobe/Adobe Premiere Pro CS5.5/Adobe Premiere Pro.exe"));
    vlay->addWidget(createQProcBtn("MediaEncoder","C:/Program Files/Adobe/Adobe Media Encoder CS5.5/Adobe Media Encoder.exe"));
    vlay->addWidget(createQProcBtn("TeamViewer","C:/Program Files (x86)/TeamViewer/TeamViewer.exe"));
    vlay->addWidget(createQProcBtn("IntelliJIDEA","C:/Program Files (x86)/JetBrains/IntelliJ IDEA 13.0.4/bin/idea64.exe"));
    vlay->addWidget(createQProcBtn("Clion","C:/Program Files (x86)/JetBrains/CLion 2016.2.3/bin/clion64.exe"));
    vlay->addWidget(createQProcBtn("DataGrip","C:/Program Files (x86)/JetBrains/DataGrip 2016.2.6/bin/datagrip64.exe"));
    vlay->addWidget(createQProcBtn("PhpStorm","C:/Program Files (x86)/JetBrains/PhpStorm 2016.2.2/bin/PhpStorm64.exe"));
    vlay->addWidget(createQProcBtn("PyCharm","C:/Program Files (x86)/JetBrains/PyCharm 2016.2.3/bin/pycharm64.exe"));
    vlay->addWidget(createQProcBtn("RubyMine","C:/Program Files (x86)/JetBrains/RubyMine 2016.2.5/bin/rubymine64.exe"));
    vlay->addWidget(createQProcBtn("WebStorm","C:/Program Files (x86)/JetBrains/WebStorm 2016.2.4/bin/WebStorm64.exe"));
    vlay->addWidget(createQProcBtn("DbVisualizer","C:/Program Files/DbVisualizer/dbvis.exe"));
    vlay->addWidget(createQProcBtn("Maya2016","C:/Program Files/Autodesk/Maya2016/bin/maya.exe"));
    vlay->addWidget(createQProcBtn("QtCreator","C:/Qt/Tools/QtCreator/bin/qtcreator.exe"));
#else
    qDebug() << "not win";
#endif
}
MainWindow::~MainWindow(){}
