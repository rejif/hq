#include "mainwindow.h"
#include <sys/time.h>


//Admin privilege checker string
#define ADMIN_TEST_STRING "HKEY_LOCAL_MACHINE"
#define ADMIN_DEFAULT_KEY "(Default)"
bool isUser(){//Check for admin privilege mode
    QSettings adminPrivSettings(ADMIN_TEST_STRING, QSettings::NativeFormat);
    QVariant currentAdminStatus = adminPrivSettings.value(ADMIN_DEFAULT_KEY);
    adminPrivSettings.setValue(ADMIN_DEFAULT_KEY, currentAdminStatus);
    adminPrivSettings.sync();
    return adminPrivSettings.status() == QSettings::AccessError;
    //return (adminPrivSettings.status() == QSettings::AccessError)?"Administrator privilege not found":"Administrator privilege was activated";
}
QString getUsername(){
    QString name = qgetenv("USER");
    return !name.isEmpty()?name:qgetenv("USERNAME");
}
template<typename Functor>
void win10(Functor f){
    if(QSysInfo::productVersion()=="10"){
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
QPushButton* createDetachBtn(QString btnText,QString command){return createLambdaActionButton(btnText,[=](){QProcess::startDetached(command);});}
QPushButton* createQDetachBtn(QString btnText,QString command){return createDetachBtn(btnText,"\""+command+"\"");}
QPushButton* createExecuteBtn(QString btnText,QString command){return createLambdaActionButton(btnText,[=](){QProcess::execute(command);});}
QPushButton* createExecuteCmdBtn(QString btnText,QString command){return createExecuteBtn(btnText,"C:/Windows/System32/cmd.exe /C "+command);}
QPushButton* createCommandBtn(QString btnText,QString command){
    return createExecuteCmdBtn(btnText,"cd \\ && "+command);
}
QPushButton* createCommandBtnFromHome(QString btnText,QString command){
    return createExecuteCmdBtn(btnText,"cd "+QStandardPaths::writableLocation(QStandardPaths::HomeLocation)+" && "+command);
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
    QVBoxLayout *vl = new QVBoxLayout(w);
    w->setLayout(vl);

    QList<QHostAddress> list = QNetworkInterface::allAddresses();
    for(int nIter=0; nIter<list.count(); nIter++){
      if(!list[nIter].isLoopback() && list[nIter].protocol() == QAbstractSocket::IPv4Protocol){
          vl->addWidget(new QLabel("IP @ "+list[nIter].toString()));
      }
    }
    vl->addWidget(
        createLambdaActionButton("EPOCID",[=](){
            QApplication::clipboard()->setText(QString::number(getEpoc()));
        })
    );
    vl->addWidget(
        createLambdaActionButton("DATEID",[=](){
            QApplication::clipboard()->setText(QDateTime::currentDateTime().toString("yyyyMMddhhmmsszzz"));
        })
    );
#if defined(_WIN32) || defined(_WIN64)

    vl->addWidget(new QLabel("Config @ "+config->loaded.toString("yyyy/MM/dd hh:mm:ss")));
    QHBoxLayout *hl_conf = new QHBoxLayout();
    hl_conf->addWidget(createLambdaActionButton("Load",[=](){createMenu();}));
    hl_conf->addWidget(createLambdaActionButton("Edit",[=](){
        if(!config->check()){
            config->save();
        }
        QProcess::execute("C:/Windows/System32/cmd.exe /C start "+config->path);
    }));
    hl_conf->addWidget(createLambdaActionButton("Save",[=](){config->save();}));
    vl->addLayout(hl_conf);

    vl->addWidget(new QLabel("Application"));
    QHBoxLayout *hl_app = new QHBoxLayout();
    hl_app->addWidget(createDetachBtn("Directory","\"C:/Windows/explorer.exe\" \""+qApp->applicationDirPath().replace("/","\\")+"\""));
    hl_app->addWidget(createExecuteBtn("Update","C:/Windows/System32/cmd.exe /C start https://ci.appveyor.com/project/onoie/hq/build/artifacts/"));
    vl->addWidget(createDetachBtn("SourceTree",QStandardPaths::writableLocation(QStandardPaths::HomeLocation)+"/AppData/Local/SourceTree/Update.exe --processStart \"SourceTree.exe\""));
    vl->addWidget(createDetachBtn("QtCreator","C:/Qt/Tools/QtCreator/bin/qtcreator.exe"));
    vl->addLayout(hl_app);

    if(!config->develop_hidden){
        vl->addWidget(new QLabel("Develop"));
        /**///vlay->addWidget(createQDetachBtn("GitBash","C:/Program Files/Git/git-bash.exe"));
        vl->addWidget(createCommandBtnFromHome("GitBash","start \" \" \"C:/Program Files/Git/git-bash.exe\""));
        vl->addWidget(createCommandBtn("PowerShell","start C:/Windows/System32/WindowsPowerShell/v1.0/powershell.exe"));// -Command \"ls\"
        vl->addWidget(createCommandBtn("CMD","start cmd"));
        vl->addWidget(
            createLambdaActionButton("QuietRun",[=](){
                QString cmd = QInputDialog::getText(this,"Run","CMD");
                QProcess::startDetached("C:/Windows/System32/cmd.exe /C cd \\ && " + cmd);
            })
        );
        win10([=](){vl->addWidget(createExecuteBtn("WSL","C:/Windows/System32/cmd.exe /C cd "+QStandardPaths::writableLocation(QStandardPaths::HomeLocation)+"/AppData/Local/Microsoft/WindowsApps"+" && start \" \" \"ubuntu.exe\""));});
        vl->addWidget(createDetachBtn("Library","explorer.exe ::{031E4825-7B94-4dc3-B131-E946B44C8DD5}"));
        vl->addWidget(createDetachBtn("RecycleBin","explorer.exe ::{645FF040-5081-101B-9F08-00AA002F954E}"));
        vl->addWidget(createDetachBtn("Service","\"C:/Windows/System32/mmc.exe\" \"C:/Windows/System32/services.msc\""));
        vl->addWidget(createDetachBtn("ControlPanel","control"));
        vl->addWidget(createLambdaActionButton("Restart",[=](){
            QProcess::startDetached(qApp->arguments()[0], qApp->arguments());
            QTimer::singleShot(1000, qApp, SLOT(quit()));
        }));
    }
    if(config->explorer){
        vl->addWidget(new QLabel("Explorer"));
        vl->addWidget(createDetachBtn("Explorer","C:/Windows/explorer.exe"));
        vl->addWidget(createDetachBtn("MyComputer","explorer.exe ::{20D04FE0-3AEA-1069-A2D8-08002B30309D}"));
        vl->addWidget(createDetachBtn("MyDocument","explorer.exe ::{450d8fba-ad25-11d0-98a8-0800361b1103}"));
        vl->addWidget(createDetachBtn("Home","\"C:/Windows/explorer.exe\" \""+QStandardPaths::writableLocation(QStandardPaths::HomeLocation).replace("/","\\")+"\""));
        vl->addWidget(createDetachBtn("Desktop","\"C:/Windows/explorer.exe\" \""+QStandardPaths::writableLocation(QStandardPaths::DesktopLocation).replace("/","\\")+"\""));
        /**///vlay->addWidget(createDetachBtn("Startup","\"C:/Windows/explorer.exe\" \"C:\\ProgramData\\Microsoft\\Windows\\Start Menu\\Programs\\Startup\""));
        vl->addWidget(createDetachBtn("Startup","\"C:/Windows/explorer.exe\" \""+QStandardPaths::writableLocation(QStandardPaths::HomeLocation).replace("/","\\")+"\\AppData\\Roaming\\Microsoft\\Windows\\Start Menu\\Programs\\Startup\""));
    }
    if(config->system){
        vl->addWidget(new QLabel("System"));
        vl->addWidget(createDetachBtn("Calc","calc"));
        vl->addWidget(createDetachBtn("TaskManager","taskmgr"));
        vl->addWidget(createDetachBtn("NetworkAndSharingCenter","control.exe /name Microsoft.NetworkAndSharingCenter"));
        vl->addWidget(createDetachBtn("MSInfo","msinfo32.exe"));
        vl->addWidget(
            createLambdaActionButton("ShutdownForce#sft0",[=]() {
                QMessageBox::StandardButton reply;
                 reply = QMessageBox::question(this,"Confirm","ShutdownForce?",QMessageBox::Ok|QMessageBox::Cancel);
                 if (reply == QMessageBox::Ok) {
                   QProcess::startDetached("C:/Windows/System32/Shutdown.exe /s /f /t 0");
                 }
            })
        );
        vl->addWidget(createDetachBtn("Shutdown#st7200","C:/Windows/System32/Shutdown.exe /s /t 7200"));
        vl->addWidget(createDetachBtn("ShutdownCancel","C:/Windows/System32/Shutdown.exe /a"));
    }
    if(config->work){
        vl->addWidget(new QLabel("Work"));
        vl->addWidget(createQDetachBtn("TeraTerm","C:/Program Files (x86)/teraterm/ttermpro.exe"));
        vl->addWidget(createQDetachBtn("WinSCP","C:/Program Files (x86)/WinSCP/WinSCP.exe"));
        vl->addWidget(createQDetachBtn("Thunderbird","C:/Program Files (x86)/Mozilla Thunderbird/thunderbird.exe"));
        vl->addWidget(createLambdaActionButton("Hosts",[=](){
            QProcess::startDetached(isUser()?"C:/Windows/System32/cmd.exe /C start C:/Windows/System32/drivers/etc":"notepad C:/Windows/System32/drivers/etc/hosts");}
        ));
        vl->addWidget(
            createLambdaActionButton("FlushDNS",[=](){
            QProcess process;
            process.start("C:/Windows/System32/cmd.exe /C cd \\ && ipconfig /flushdns");
            process.waitForFinished();
            int code = process.exitCode();
                QMessageBox::information(this,"FlashDNS","Done:"+code);
            })
        );
    }
    if(config->editor){
        vl->addWidget(new QLabel("Editor"));
        vl->addWidget(createDetachBtn("Notepad","notepad"));
        vl->addWidget(createDetachBtn("Atom",QStandardPaths::writableLocation(QStandardPaths::HomeLocation)+"/AppData/Local/atom/atom.exe"));
        vl->addWidget(createQDetachBtn("VisualStdioCode","C:/Program Files/Microsoft VS Code/Code.exe"));
        vl->addWidget(createQDetachBtn("SublimeText3","C:/Program Files/Sublime Text 3/sublime_text.exe"));
        vl->addWidget(createQDetachBtn("Brackets","C:/Program Files (x86)/Brackets/Brackets.exe"));
    }
    if(config->browser){
        vl->addWidget(new QLabel("Browser"));
        vl->addWidget(createQDetachBtn("InternetExplorer","C:/Program Files/Internet Explorer/iexplore.exe"));
        vl->addWidget(createExecuteBtn("MicrosoftEdge","C:/Windows/System32/cmd.exe /C cd \\ && start microsoft-edge:"));
        vl->addWidget(createQDetachBtn("GoogleChrome","C:/Program Files (x86)/Google/Chrome/Application/chrome.exe"));
        vl->addWidget(createQDetachBtn("Firefox","C:/Program Files/Mozilla Firefox/firefox.exe"));
        vl->addWidget(createDetachBtn("Vivaldi",QStandardPaths::writableLocation(QStandardPaths::HomeLocation)+"/AppData/Local/Vivaldi/Application/vivaldi.exe"));
    }
    if(config->office){
        vl->addWidget(new QLabel("Office"));
        vl->addWidget(createExecuteBtn("Word","C:/Windows/System32/cmd.exe /C cd \\ && start Winword"));
        vl->addWidget(createExecuteBtn("Excel","C:/Windows/System32/cmd.exe /C cd \\ && start excel"));
        vl->addWidget(createExecuteBtn("PowerPoint","C:/Windows/System32/cmd.exe /C cd \\ && start powerpnt"));
    }
    if(config->maya){
        vl->addWidget(new QLabel("Maya"));
        vl->addWidget(createQDetachBtn("Maya2016","C:/Program Files/Autodesk/Maya2016/bin/maya.exe"));
        vl->addWidget(createDetachBtn("MayaDirectory","\"C:/Windows/explorer.exe\" \""+QString(QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation)+"/maya/").replace("/","\\")+"\""));
        vl->addWidget(createDetachBtn("MayaDefaultProject","\"C:/Windows/explorer.exe\" \""+QString(QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation)+"/maya/projects/default/").replace("/","\\")+"\""));
        vl->addWidget(createExecuteBtn("MayaFCheck","C:/Windows/System32/cmd.exe /C cd C:/Program Files/Autodesk/Maya2016/bin/ && start fcheck.exe"));
        vl->addWidget(createQDetachBtn("MayaMatchMover2014","C:/Program Files/Autodesk/MatchMover2014/MatchMoverApp.exe"));
        vl->addWidget(createExecuteBtn("Blender","C:/Windows/System32/cmd.exe /C cd \"C:/Program Files/Blender Foundation/Blender/\" && blender"));
    }
    if(config->other){
        vl->addWidget(new QLabel("Other"));
        vl->addWidget(createDetachBtn("Slack",QStandardPaths::writableLocation(QStandardPaths::HomeLocation)+"/AppData/Local/slack/slack.exe"));
        vl->addWidget(createQDetachBtn("NetLimiter4","C:/Program Files/Locktime Software/NetLimiter 4/NLClientApp.exe"));
        vl->addWidget(createQDetachBtn("TeamViewer","C:/Program Files (x86)/TeamViewer/TeamViewer.exe"));
        vl->addWidget(createQDetachBtn("DbVisualizer","C:/Program Files/DbVisualizer/dbvis.exe"));
        vl->addWidget(createQDetachBtn("PaintstormStdio","C:/Program Files/Paintstorm Studio/Paintstorm.exe"));
        vl->addWidget(createQDetachBtn("PowerMate","C:/Program Files (x86)/Griffin Technology/PowerMate/PowerMate.exe"));
        vl->addWidget(createQDetachBtn("WacomTabletUtility","C:/Program Files/Tablet/Wacom/32/PrefUtil.exe"));
        vl->addWidget(createQDetachBtn("WacomDesktopCenter","C:/Program Files/Tablet/Wacom/32/WacomDesktopCenter.exe"));
    }
    if(config->utils){
        vl->addWidget(new QLabel("Utils"));
        vl->addWidget(createQDetachBtn("Everything","C:/Program Files/Everything/Everything.exe"));
        vl->addWidget(createQDetachBtn("DaemonToolsLite","C:/Program Files (x86)/DAEMON Tools Lite/DTLite.exe"));
        vl->addWidget(createQDetachBtn("ImgBurn","C:/Program Files (x86)/ImgBurn/ImgBurn.exe"));
        vl->addWidget(createQDetachBtn("Audacity","C:/Program Files (x86)/Audacity/audacity.exe"));
        vl->addWidget(createQDetachBtn("LICEcap","C:/Program Files (x86)/LICEcap/licecap.exe"));
        vl->addWidget(createQDetachBtn("SDFormatter","C:/Program Files (x86)/SDA/SD Formatter/SDFormatter.exe"));
        vl->addWidget(createQDetachBtn("Kindle","C:/Program Files (x86)/Amazon/Kindle/Kindle.exe"));
        vl->addWidget(createQDetachBtn("AndroidStdio","C:/Program Files/Android/Android Studio/bin/studio64.exe"));
        vl->addWidget(createQDetachBtn("XMind","C:/Program Files (x86)/XMind/XMind.exe"));
    }
    if(config->player){
        vl->addWidget(new QLabel("Player"));
        vl->addWidget(createQDetachBtn("WindowMediaPlayer","C:/Program Files (x86)/Windows Media Player/wmplayer.exe"));
        vl->addWidget(createQDetachBtn("QuickTimePlayer","C:/Program Files (x86)/QuickTime/QuickTimePlayer.exe"));
        vl->addWidget(createQDetachBtn("MPC-HC","C:/Program Files/MPC-HC/mpc-hc64.exe"));
        vl->addWidget(createQDetachBtn("VLC","C:/Program Files (x86)/VideoLAN/VLC/vlc.exe"));
    }
    if(config->adobe){
        vl->addWidget(new QLabel("Adobe"));
        vl->addWidget(createQDetachBtn("Photoshop64bit","C:/Program Files/Adobe/Adobe Photoshop CS5.1 (64 Bit)/Photoshop.exe"));
        vl->addWidget(createQDetachBtn("Illustrator","C:/Program Files (x86)/Adobe/Adobe Illustrator CS5.1/Support Files/Contents/Windows/Illustrator.exe"));
        vl->addWidget(createQDetachBtn("InDesign","C:/Program Files (x86)/Adobe/Adobe InDesign CS5.5/InDesign.exe"));
        vl->addWidget(createQDetachBtn("Audition","C:/Program Files (x86)/Adobe/Adobe Audition CS5.5/Adobe Audition.exe"));
        vl->addWidget(createQDetachBtn("AfterEffect","C:/Program Files/Adobe/Adobe After Effects CS5.5/Support Files/AfterFX.exe"));
        vl->addWidget(createQDetachBtn("Premiere","C:/Program Files/Adobe/Adobe Premiere Pro CS5.5/Adobe Premiere Pro.exe"));
        vl->addWidget(createQDetachBtn("MediaEncoder","C:/Program Files/Adobe/Adobe Media Encoder CS5.5/Adobe Media Encoder.exe"));
    }
    if(config->jetbrains){
        vl->addWidget(new QLabel("Jetbrains"));
        vl->addWidget(createQDetachBtn("IntelliJIDEA","C:/Program Files (x86)/JetBrains/IntelliJ IDEA 2016.2.5\bin/idea64.exe"));
        vl->addWidget(createQDetachBtn("Clion","C:/Program Files (x86)/JetBrains/CLion 2016.2.3/bin/clion64.exe"));
        vl->addWidget(createQDetachBtn("DataGrip","C:/Program Files (x86)/JetBrains/DataGrip 2016.2.6/bin/datagrip64.exe"));
        vl->addWidget(createQDetachBtn("PhpStorm","C:/Program Files (x86)/JetBrains/PhpStorm 2016.2.2/bin/PhpStorm64.exe"));
        vl->addWidget(createQDetachBtn("PyCharm","C:/Program Files (x86)/JetBrains/PyCharm 2016.2.3/bin/pycharm64.exe"));
        vl->addWidget(createQDetachBtn("RubyMine","C:/Program Files (x86)/JetBrains/RubyMine 2016.2.5/bin/rubymine64.exe"));
        vl->addWidget(createQDetachBtn("WebStorm","C:/Program Files (x86)/JetBrains/WebStorm 2016.2.4/bin/WebStorm64.exe"));
    }
    if(config->visual_stdio){
        vl->addWidget(new QLabel("VisualStdio"));
        vl->addWidget(createQDetachBtn("VS2013","C:/Program Files (x86)/Microsoft Visual Studio 12.0/Common7/IDE/WDExpress.exe"));
        vl->addWidget(createQDetachBtn("VS2015","C:/Program Files (x86)/Microsoft Visual Studio 14.0/Common7/IDE/devenv.exe"));
        vl->addWidget(createQDetachBtn("VS2017","C:/Program Files (x86)/Microsoft Visual Studio/2017/Community/Common7/IDE/devenv.exe"));
    }
    if(config->game){
        vl->addWidget(new QLabel("Game"));
        vl->addWidget(createQDetachBtn("Unity","C:/Program Files/Unity/Editor/Unity.exe"));
        vl->addWidget(createQDetachBtn("EpicGameLauncher","C:/Program Files (x86)/Epic Games/Launcher/Portal/Binaries/Win64/EpicGamesLauncher.exe"));
        vl->addWidget(createQDetachBtn("Steam","C:/Program Files (x86)/Steam/Steam.exe"));
        vl->addWidget(createQDetachBtn("Origin","C:/Program Files (x86)/Origin/Origin.exe"));
        vl->addWidget(createQDetachBtn("Uplay","C:/Program Files (x86)/Ubisoft/Ubisoft Game Launcher/upc.exe"));
        vl->addWidget(createQDetachBtn("Oculus","C:/Program Files/Oculus/Support/oculus-client/OculusClient.exe"));
    }
#else
    qDebug() << "not win";
#endif
    return w;
}
