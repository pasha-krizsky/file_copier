#include "dialog.h"
#include "ui_dialog.h"
#include <QMessageBox>

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{

    ui->setupUi(this);

    this->setFixedSize(this->width(),this->height());

    // Init progress bar
    ui->progressBar->reset();
    ui->progressBar->setMinimum(0);
    ui->progressBar->setMaximum(100);

    is_pause = false;
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::on_pauseButton_clicked()
{
    if (!is_pause) {
        is_pause = true;
        emit sendPauseSignal();
        ui->pauseButton->setText("Resume");
    } else {
        is_pause = false;
        emit sendResumeSignal();
        ui->pauseButton->setText("Pause");
    }
}

void Dialog::getProgressSlot(float pr)
{
    ui->progressBar->setValue(pr * 100);
}

void Dialog::getPathSlot(QString p1, QString p2)
{
    this->file_from_path = p1;
    this->file_to_path = p2;
}

void Dialog::copySlot()
{
    // Create thread to copy files
    CopyThread* copyThread = new CopyThread();

    // Connect the dialog and thread to send path to files
    QObject::connect(this, SIGNAL(sendPathSignal(QString, QString)),
                         copyThread, SLOT(getPathSlot(QString, QString)));

    // Connect the dialog and thread to get progress from thread
    QObject::connect(copyThread, SIGNAL(sendProgressSignal(float)),
                         this, SLOT(getProgressSlot(float)));

    // Connect the dialog and thread to send pause
    QObject::connect(this, SIGNAL(sendPauseSignal()),
                         copyThread, SLOT(pauseSlot()));

    // Connect the dialog and thread to undo pause
    QObject::connect(this, SIGNAL(sendResumeSignal()),
                         copyThread, SLOT(resumeSlot()));

    // Connect the dialog and thread to end copy
    QObject::connect(copyThread, SIGNAL(endCopySignal()),
                         this, SLOT(endCopySlot()));

    // Connect the dialog and thread to show dialog if files are opened
    QObject::connect(copyThread, SIGNAL(showDialogSignal()),
                         this, SLOT(show()));

    // Connect the dialog and thread to show error message
    QObject::connect(copyThread, SIGNAL(errorSignal()),
                         this, SLOT(errorMessageSlot()));

    // Connect the dialog and thread to stop copy
    QObject::connect(this, SIGNAL(stopCopySignal()),
                         copyThread, SLOT(stopCopySlot()));

    // Send path to "CopyThread"
    emit sendPathSignal(file_from_path, file_to_path);

    // Copy!
    copyThread->start();
}

void Dialog::endCopySlot()
{
    is_pause = false;
    ui->pauseButton->setText("Pause");
    close();
}

void Dialog::errorMessageSlot()
{
    // Path doesn't exist
    QMessageBox::information(
            this,
            tr("Ops..."),
            tr("Can't find the file...") );
    close();
}

void Dialog::on_cancelButton_clicked()
{
    emit stopCopySignal();
}
