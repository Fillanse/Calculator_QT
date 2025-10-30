#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>
#include <QFontDatabase>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

     fonts = {
        "./themes/fonts/SFMono-Regular.otf",
        "./themes/fonts/SegoeUI.ttf",
        "./themes/fonts/Ubuntu-Regular.ttf"};

    for (const QString &fontPath : fonts)
    {
        QFile fontFile(fontPath);
        if (!fontFile.open(QIODevice::ReadOnly))
        {
            qWarning() << "Failed to open font:" << fontPath;
            continue;
        }

        int fontId = QFontDatabase::addApplicationFontFromData(fontFile.readAll());
        if (fontId == -1)
        {
            qWarning() << "Error - can't open font:" << fontPath;
        }
        else
        {
            qDebug() << "Font loaded:" << QFontDatabase::applicationFontFamilies(fontId);
        }
        fontFile.close();
    }

    themeMap = {
        {ui->actionMacOS_Dark, "./themes/styles/macos_dark.qss"},
        {ui->actionMacOS_Light, "./themes/styles/macos_light.qss"},
        {ui->actionUbuntu_Dark, "./themes/styles/ubuntu_dark.qss"},
        {ui->actionUbuntu_Light, "./themes/styles/ubuntu_light.qss"},
        {ui->actionWindows_11_Dark, "./themes/styles/win11_dark.qss"},
        {ui->actionWindows_11_Light, "./themes/styles/win11_light.qss"}};

    for (QAction *action : themeMap.keys())
    {
        connect(action, &QAction::triggered, this, [this, action]()
                {
            const QString& themePath = themeMap[action];
            loadTheme(themePath); });
    }

    loadTheme("./themes/styles/macos_light.qss");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::loadTheme(const QString &themePath)
{
    QFile file(themePath);
    if (!file.open(QFile::ReadOnly | QFile::Text))
    {
        qWarning() << "Failed to open the theme:" << themePath;
        return;
    }

    QTextStream in(&file);
    QString stylesheet = in.readAll();
    file.close();

    qApp->setStyleSheet(stylesheet);
    for (QAction *action : themeMap.keys())
    {
        action->setChecked(themeMap[action] == themePath);
    }

    qDebug() << "Theme is loaded:" << themePath;
}