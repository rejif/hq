#include "mainwindow.h"
#include <sys/time.h>
QString getUsername(){
    QString name = qgetenv("USER");
    return !name.isEmpty()?name:qgetenv("USERNAME");
}
template<typename Functor>
void win10(Functor f){
    if(QSysInfo::productVersion()==10){
        f();
    }
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
    setWindowIcon(QIcon(":/hq.png"));
    QWidget *w = createMenu();
    setCentralWidget(w);
}
unsigned long long MainWindow::getEpoc(){
    struct timeval tv;
    gettimeofday(&tv, NULL);
    unsigned long long millisecondsSinceEpoch =
        (unsigned long long)(tv.tv_sec) * 1000 +
        (unsigned long long)(tv.tv_usec) / 1000;
    //printf("%llu\n", millisecondsSinceEpoch);
    return millisecondsSinceEpoch;
}
MainWindow::~MainWindow(){}
QWidget* MainWindow::createMenu(){
    config->load();

    QWidget* w = new QWidget();
    setCentralWidget(w);
    QVBoxLayout *vlay = new QVBoxLayout(w);
    w->setLayout(vlay);

    QList<QHostAddress> list = QNetworkInterface::allAddresses();
    for(int nIter=0; nIter<list.count(); nIter++){
      if(!list[nIter].isLoopback() && list[nIter].protocol() == QAbstractSocket::IPv4Protocol){
          vlay->addWidget(new QLabel("IP @ "+list[nIter].toString()));
      }
    }
    vlay->addWidget(
        createLambdaActionButton("EPOCID",[=](){
            QApplication::clipboard()->setText(QString::number(getEpoc()));
        })
    );
    vlay->addWidget(
        createLambdaActionButton("DATEID",[=](){
            QApplication::clipboard()->setText(QDateTime::currentDateTime().toString("yyyyMMddhhmmsszzz"));
        })
    );
#if defined(_WIN32) || defined(_WIN64)
    vlay->addWidget(new QLabel("Config @ "+config->loaded.toString("yyyy/MM/dd hh:mm:ss")));
    vlay->addWidget(createDetachBtn("ApplicationDirectory","\"C:/Windows/explorer.exe\" \""+qApp->applicationDirPath().replace("/","\\")+"\""));
    vlay->addWidget(createExecuteBtn("ApplicationUpdate","C:/Windows/System32/cmd.exe /C start https://ci.appveyor.com/project/onoie/hq/build/artifacts/"));
    if(!config->develop_hidden){
        vlay->addWidget(new QLabel("Develop"));
        /**///vlay->addWidget(createQDetachBtn("GitBash","C:/Program Files/Git/git-bash.exe"));
        vlay->addWidget(createExecuteBtn("GitBash","C:/Windows/System32/cmd.exe /C cd "+QStandardPaths::writableLocation(QStandardPaths::HomeLocation)+" && start \" \" \"C:/Program Files/Git/git-bash.exe\""));
        vlay->addWidget(createExecuteBtn("PowerShell","C:/Windows/System32/cmd.exe /C cd \\ && start C:/Windows/System32/WindowsPowerShell/v1.0/powershell.exe"));
        vlay->addWidget(createExecuteBtn("CMD","C:/Windows/System32/cmd.exe /C cd \\ && start cmd"));
        win10([=](){vlay->addWidget(createExecuteBtn("WSL","C:/Windows/System32/cmd.exe /C cd "+QStandardPaths::writableLocation(QStandardPaths::HomeLocation)+"/AppData/Local/Microsoft/WindowsApps"+" && start \" \" \"ubuntu.exe\""));});
        vlay->addWidget(
            createLambdaActionButton("QuietRun",[=](){
                QString cmd = QInputDialog::getText(this,"Run","CMD");
                QProcess::startDetached("C:/Windows/System32/cmd.exe /C cd \\ && " + cmd);
            })
        );
        vlay->addWidget(
            createLambdaActionButton("FlushDNS",[=](){
            QProcess process;
            process.start("C:/Windows/System32/cmd.exe /C cd \\ && ipconfig /flushdns");
            process.waitForFinished();
            int code = process.exitCode();
                QMessageBox::information(this,"FlashDNS","Done:"+code);
            })
        );
        vlay->addWidget(createDetachBtn("Notepad","notepad"));
        vlay->addWidget(createDetachBtn("MSInfo","msinfo32.exe"));
        vlay->addWidget(createDetachBtn("Service","\"C:/Windows/System32/mmc.exe\" \"C:/Windows/System32/services.msc\""));
        vlay->addWidget(createDetachBtn("SourceTree",QStandardPaths::writableLocation(QStandardPaths::HomeLocation)+"/AppData/Local/SourceTree/Update.exe --processStart \"SourceTree.exe\""));
        vlay->addWidget(createDetachBtn("QtCreator","C:/Qt/Tools/QtCreator/bin/qtcreator.exe"));
        vlay->addWidget(createLambdaActionButton("Restart",[=](){
            QProcess::startDetached(qApp->arguments()[0], qApp->arguments());
            QTimer::singleShot(1000, qApp, SLOT(quit()));
        }));
        vlay->addWidget(createLambdaActionButton("Hosts",[=](){QProcess::startDetached("C:/Windows/System32/cmd.exe /C start C:/Windows/System32/drivers/etc");}));
        vlay->addWidget(createLambdaActionButton("ConfigLoad",[=](){createMenu();}));
        vlay->addWidget(createLambdaActionButton("ConfigEdit",[=](){if(config->check()){QProcess::execute("C:/Windows/System32/cmd.exe /C start "+config->path);}}));
        vlay->addWidget(createLambdaActionButton("ConfigSave",[=](){config->save();}));
        vlay->addWidget(createLambdaActionButton("ConfigRemove",[=](){config->remove();}));
    }
    if(config->explorer){
        vlay->addWidget(new QLabel("Explorer"));
        vlay->addWidget(createDetachBtn("ControlPanel","control"));
        vlay->addWidget(createDetachBtn("Explorer","C:/Windows/explorer.exe"));
        vlay->addWidget(createDetachBtn("MyComputer","explorer.exe ::{20D04FE0-3AEA-1069-A2D8-08002B30309D}"));
        vlay->addWidget(createDetachBtn("MyDocument","explorer.exe ::{450d8fba-ad25-11d0-98a8-0800361b1103}"));
        vlay->addWidget(createDetachBtn("Home","\"C:/Windows/explorer.exe\" \""+QStandardPaths::writableLocation(QStandardPaths::HomeLocation).replace("/","\\")+"\""));
        vlay->addWidget(createDetachBtn("Desktop","\"C:/Windows/explorer.exe\" \""+QStandardPaths::writableLocation(QStandardPaths::DesktopLocation).replace("/","\\")+"\""));
        /**///vlay->addWidget(createDetachBtn("Startup","\"C:/Windows/explorer.exe\" \"C:\\ProgramData\\Microsoft\\Windows\\Start Menu\\Programs\\Startup\""));
        vlay->addWidget(createDetachBtn("Startup","\"C:/Windows/explorer.exe\" \""+QStandardPaths::writableLocation(QStandardPaths::HomeLocation).replace("/","\\")+"\\AppData\\Roaming\\Microsoft\\Windows\\Start Menu\\Programs\\Startup\""));
    }
    if(config->system){
        vlay->addWidget(new QLabel("System"));
        vlay->addWidget(createDetachBtn("RecycleBin","explorer.exe ::{645FF040-5081-101B-9F08-00AA002F954E}"));
        vlay->addWidget(createDetachBtn("NetworkAndSharingCenter","control.exe /name Microsoft.NetworkAndSharingCenter"));
        vlay->addWidget(createDetachBtn("TaskManager","taskmgr"));
        vlay->addWidget(createDetachBtn("Calc","calc"));
        vlay->addWidget(
            createLambdaActionButton("ShutdownForce",[=]() {
                QMessageBox::StandardButton reply;
                 reply = QMessageBox::question(this,"Confirm","ShutdownForce?",QMessageBox::Ok|QMessageBox::Cancel);
                 if (reply == QMessageBox::Ok) {
                   QProcess::startDetached("C:/Windows/System32/Shutdown.exe /s /f /t 0");
                 }
            })
        );
        vlay->addWidget(createDetachBtn("Shutdown2h","C:/Windows/System32/Shutdown.exe /s /t 7200"));
        vlay->addWidget(createDetachBtn("ShutdownCancel","C:/Windows/System32/Shutdown.exe /a"));
    }
    if(config->work){
        vlay->addWidget(new QLabel("Work"));
        vlay->addWidget(createQDetachBtn("TeraTerm","C:/Program Files (x86)/teraterm/ttermpro.exe"));
        vlay->addWidget(createQDetachBtn("WinSCP","C:/Program Files (x86)/WinSCP/WinSCP.exe"));
        vlay->addWidget(createQDetachBtn("Thunderbird","C:/Program Files (x86)/Mozilla Thunderbird/thunderbird.exe"));
        vlay->addWidget(createQDetachBtn("XMind","C:/Program Files (x86)/XMind/XMind.exe"));
    }
    if(config->editor){
        vlay->addWidget(new QLabel("Editor"));
        vlay->addWidget(createDetachBtn("Atom",QStandardPaths::writableLocation(QStandardPaths::HomeLocation)+"/AppData/Local/atom/atom.exe"));
        vlay->addWidget(createQDetachBtn("VisualStdioCode","C:/Program Files/Microsoft VS Code/Code.exe"));
        vlay->addWidget(createQDetachBtn("SublimeText3","C:/Program Files/Sublime Text 3/sublime_text.exe"));
    }
    if(config->browser){
        vlay->addWidget(new QLabel("Browser"));
        vlay->addWidget(createQDetachBtn("InternetExplorer","C:/Program Files/Internet Explorer/iexplore.exe"));
        vlay->addWidget(createExecuteBtn("MicrosoftEdge","C:/Windows/System32/cmd.exe /C cd \\ && start microsoft-edge:"));
        vlay->addWidget(createQDetachBtn("GoogleChrome","C:/Program Files (x86)/Google/Chrome/Application/chrome.exe"));
        vlay->addWidget(createQDetachBtn("Firefox","C:/Program Files/Mozilla Firefox/firefox.exe"));
        vlay->addWidget(createDetachBtn("Vivaldi",QStandardPaths::writableLocation(QStandardPaths::HomeLocation)+"/AppData/Local/Vivaldi/Application/vivaldi.exe"));
    }
    if(config->office){
        vlay->addWidget(new QLabel("Office"));
        vlay->addWidget(createExecuteBtn("Word","C:/Windows/System32/cmd.exe /C cd \\ && start Winword"));
        vlay->addWidget(createExecuteBtn("Excel","C:/Windows/System32/cmd.exe /C cd \\ && start excel"));
        vlay->addWidget(createExecuteBtn("PowerPoint","C:/Windows/System32/cmd.exe /C cd \\ && start powerpnt"));
    }
    if(config->maya){
        vlay->addWidget(new QLabel("Maya"));
        vlay->addWidget(createQDetachBtn("Maya2016","C:/Program Files/Autodesk/Maya2016/bin/maya.exe"));
        vlay->addWidget(createDetachBtn("MayaDirectory","\"C:/Windows/explorer.exe\" \""+QString(QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation)+"/maya/").replace("/","\\")+"\""));
        vlay->addWidget(createDetachBtn("MayaDefaultProject","\"C:/Windows/explorer.exe\" \""+QString(QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation)+"/maya/projects/default/").replace("/","\\")+"\""));
        vlay->addWidget(createExecuteBtn("MayaFCheck","C:/Windows/System32/cmd.exe /C cd C:/Program Files/Autodesk/Maya2016/bin/ && start fcheck.exe"));
        vlay->addWidget(createQDetachBtn("MayaMatchMover2014","C:/Program Files/Autodesk/MatchMover2014/MatchMoverApp.exe"));
        vlay->addWidget(createExecuteBtn("Blender","C:/Windows/System32/cmd.exe /C cd \"C:/Program Files/Blender Foundation/Blender/\" && blender"));
    }
    if(config->other){
        vlay->addWidget(new QLabel("Other"));
        vlay->addWidget(createDetachBtn("Slack",QStandardPaths::writableLocation(QStandardPaths::HomeLocation)+"/AppData/Local/slack/slack.exe"));
        vlay->addWidget(createQDetachBtn("NetLimiter4","C:/Program Files/Locktime Software/NetLimiter 4/NLClientApp.exe"));
        vlay->addWidget(createQDetachBtn("TeamViewer","C:/Program Files (x86)/TeamViewer/TeamViewer.exe"));
        vlay->addWidget(createQDetachBtn("DbVisualizer","C:/Program Files/DbVisualizer/dbvis.exe"));
        vlay->addWidget(createQDetachBtn("PaintstormStdio","C:/Program Files/Paintstorm Studio/Paintstorm.exe"));
        vlay->addWidget(createQDetachBtn("PowerMate","C:/Program Files (x86)/Griffin Technology/PowerMate/PowerMate.exe"));
        vlay->addWidget(createQDetachBtn("WacomTabletUtility","C:/Program Files/Tablet/Wacom/32/PrefUtil.exe"));
        vlay->addWidget(createQDetachBtn("WacomDesktopCenter","C:/Program Files/Tablet/Wacom/32/WacomDesktopCenter.exe"));
    }
    if(config->utils){
        vlay->addWidget(new QLabel("Utils"));
        vlay->addWidget(createQDetachBtn("Everything","C:/Program Files/Everything/Everything.exe"));
        vlay->addWidget(createQDetachBtn("DaemonToolsLite","C:/Program Files (x86)/DAEMON Tools Lite/DTLite.exe"));
        vlay->addWidget(createQDetachBtn("ImgBurn","C:/Program Files (x86)/ImgBurn/ImgBurn.exe"));
        vlay->addWidget(createQDetachBtn("Audacity","C:/Program Files (x86)/Audacity/audacity.exe"));
        vlay->addWidget(createQDetachBtn("LICEcap","C:/Program Files (x86)/LICEcap/licecap.exe"));
        vlay->addWidget(createQDetachBtn("SDFormatter","C:/Program Files (x86)/SDA/SD Formatter/SDFormatter.exe"));
        vlay->addWidget(createQDetachBtn("Kindle","C:/Program Files (x86)/Amazon/Kindle/Kindle.exe"));
        vlay->addWidget(createQDetachBtn("AndroidStdio","C:/Program Files/Android/Android Studio/bin/studio64.exe"));
    }
    if(config->player){
        vlay->addWidget(new QLabel("Player"));
        vlay->addWidget(createQDetachBtn("WindowMediaPlayer","C:/Program Files (x86)/Windows Media Player/wmplayer.exe"));
        vlay->addWidget(createQDetachBtn("QuickTimePlayer","C:/Program Files (x86)/QuickTime/QuickTimePlayer.exe"));
        vlay->addWidget(createQDetachBtn("MPC-HC","C:/Program Files/MPC-HC/mpc-hc64.exe"));
        vlay->addWidget(createQDetachBtn("VLC","C:/Program Files (x86)/VideoLAN/VLC/vlc.exe"));
    }
    if(config->adobe){
        vlay->addWidget(new QLabel("Adobe"));
        vlay->addWidget(createQDetachBtn("Photoshop64bit","C:/Program Files/Adobe/Adobe Photoshop CS5.1 (64 Bit)/Photoshop.exe"));
        vlay->addWidget(createQDetachBtn("Illustrator","C:/Program Files (x86)/Adobe/Adobe Illustrator CS5.1/Support Files/Contents/Windows/Illustrator.exe"));
        vlay->addWidget(createQDetachBtn("InDesign","C:/Program Files (x86)/Adobe/Adobe InDesign CS5.5/InDesign.exe"));
        vlay->addWidget(createQDetachBtn("Audition","C:/Program Files (x86)/Adobe/Adobe Audition CS5.5/Adobe Audition.exe"));
        vlay->addWidget(createQDetachBtn("AfterEffect","C:/Program Files/Adobe/Adobe After Effects CS5.5/Support Files/AfterFX.exe"));
        vlay->addWidget(createQDetachBtn("Premiere","C:/Program Files/Adobe/Adobe Premiere Pro CS5.5/Adobe Premiere Pro.exe"));
        vlay->addWidget(createQDetachBtn("MediaEncoder","C:/Program Files/Adobe/Adobe Media Encoder CS5.5/Adobe Media Encoder.exe"));
    }
    if(config->jetbrains){
        vlay->addWidget(new QLabel("Jetbrains"));
        vlay->addWidget(createQDetachBtn("IntelliJIDEA","C:/Program Files (x86)/JetBrains/IntelliJ IDEA 2016.2.5\bin/idea64.exe"));
        vlay->addWidget(createQDetachBtn("Clion","C:/Program Files (x86)/JetBrains/CLion 2016.2.3/bin/clion64.exe"));
        vlay->addWidget(createQDetachBtn("DataGrip","C:/Program Files (x86)/JetBrains/DataGrip 2016.2.6/bin/datagrip64.exe"));
        vlay->addWidget(createQDetachBtn("PhpStorm","C:/Program Files (x86)/JetBrains/PhpStorm 2016.2.2/bin/PhpStorm64.exe"));
        vlay->addWidget(createQDetachBtn("PyCharm","C:/Program Files (x86)/JetBrains/PyCharm 2016.2.3/bin/pycharm64.exe"));
        vlay->addWidget(createQDetachBtn("RubyMine","C:/Program Files (x86)/JetBrains/RubyMine 2016.2.5/bin/rubymine64.exe"));
        vlay->addWidget(createQDetachBtn("WebStorm","C:/Program Files (x86)/JetBrains/WebStorm 2016.2.4/bin/WebStorm64.exe"));
    }
    if(config->visual_stdio){
        vlay->addWidget(new QLabel("VisualStdio"));
        vlay->addWidget(createQDetachBtn("VS2013","C:/Program Files (x86)/Microsoft Visual Studio 12.0/Common7/IDE/WDExpress.exe"));
        vlay->addWidget(createQDetachBtn("VS2015","C:/Program Files (x86)/Microsoft Visual Studio 14.0/Common7/IDE/devenv.exe"));
        vlay->addWidget(createQDetachBtn("VS2017","C:/Program Files (x86)/Microsoft Visual Studio/2017/Community/Common7/IDE/devenv.exe"));
    }
    if(config->game){
        vlay->addWidget(new QLabel("Game"));
        vlay->addWidget(createQDetachBtn("Unity","C:/Program Files/Unity/Editor/Unity.exe"));
        vlay->addWidget(createQDetachBtn("EpicGameLauncher","C:/Program Files (x86)/Epic Games/Launcher/Portal/Binaries/Win64/EpicGamesLauncher.exe"));
        vlay->addWidget(createQDetachBtn("Steam","C:/Program Files (x86)/Steam/Steam.exe"));
        vlay->addWidget(createQDetachBtn("Origin","C:/Program Files (x86)/Origin/Origin.exe"));
        vlay->addWidget(createQDetachBtn("Uplay","C:/Program Files (x86)/Ubisoft/Ubisoft Game Launcher/upc.exe"));
        vlay->addWidget(createQDetachBtn("Oculus","C:/Program Files/Oculus/Support/oculus-client/OculusClient.exe"));
    }
#else
    qDebug() << "not win";
#endif
    return w;
}
