#ifndef CONFIG_H
#define CONFIG_H

#include <QApplication>
#include <QSettings>
#include <QFileInfo>
#include <QDateTime>
class Config{
public:
    QString path = qApp->applicationDirPath()+"/conf.ini";
    bool develop_hidden,adobe,jetbrains,explorer,system,browser,maya,visual_stdio,game,utils,other,player;
    QDateTime loaded;
    Config(){}
    ~Config();
    bool check(){
        return QFileInfo(path).exists();
    }
    void save(){
        remove();
        QSettings settings(path, QSettings::IniFormat );
        settings.beginGroup("Develop");
        settings.setValue("hidden", develop_hidden);
        settings.endGroup();
        settings.beginGroup("Shortcut");
        settings.setValue("Explorer",explorer);
        settings.setValue("System",system);
        settings.setValue("Browser",browser);
        settings.setValue("Maya2016",maya);
        settings.setValue("Other",other);
        settings.setValue("AdobeCS5.5", adobe);
        settings.setValue("Jetbrains2016.2", jetbrains);
        settings.setValue("VisualStdio",visual_stdio);
        settings.setValue("Game",game);
        settings.setValue("Utils",utils);
        settings.setValue("Player",player);
        settings.endGroup();
        list();
    }
    void load(){
        QSettings settings(path, QSettings::IniFormat);
        settings.beginGroup("Develop");
        develop_hidden = settings.value("hidden").toBool();
        settings.endGroup();
        settings.beginGroup("Shortcut");
        explorer = settings.value("Explorer").toBool();
        system = settings.value("System").toBool();
        browser = settings.value("Browser").toBool();
        maya = settings.value("Maya2016").toBool();
        other = settings.value("Other").toBool();
        adobe = settings.value("AdobeCS5.5").toBool();
        jetbrains = settings.value("Jetbrains2016.2").toBool();
        visual_stdio = settings.value("VisualStdio").toBool();
        game = settings.value("Game").toBool();
        utils = settings.value("Utils").toBool();
        player = settings.value("Player").toBool();
        settings.endGroup();
        loaded = QDateTime::currentDateTime();
        list();
    }
    void remove(){
        QFile(path).remove();
    }
    void list(){
        QSettings settings(path, QSettings::IniFormat );
        for(QString key : settings.allKeys()) {
            qDebug() << key << settings.value(key);
        }
    }
};

#endif // CONFIG_H
