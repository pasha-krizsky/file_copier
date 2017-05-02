#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QString>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setFixedSize(this->width(),this->height());

    // Set some text...
    ui->fileFrom->setText("C:\\Users\\Pasha\\Downloads\\720P_1500K_107484062.mp4");
    ui->fileTo->setText("D:\\gallery\\videos\\stuff\\awesome_copied.mp4");

    // Create new dialog with progress bar and two buttons
    dialog = new Dialog();
    dialog->setModal(true);

    // Connect "MainWindow" and "Dialog" to send path
    QObject::connect(this, SIGNAL(sendPathSignal(QString, QString)),
                         dialog, SLOT(getPathSlot(QString, QString)));

    // Connect main window and dialog to send "copy request"
    QObject::connect(this, SIGNAL(startCopySignal()),
                         dialog, SLOT(copySlot()));
}

MainWindow::~MainWindow()
{
    delete ui;
    delete dialog;
}

void MainWindow::on_startButton_clicked()
{
    // Get path
    QString fileFromPath = ui->fileFrom->text();
    QString fileToPath   = ui->fileTo->text();

    // Send path to dialog
    emit sendPathSignal(fileFromPath, fileToPath);

    // Copy!
    emit startCopySignal();
}
