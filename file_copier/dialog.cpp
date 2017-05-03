#include "dialog.h"
#include "ui_dialog.h"
#include <QMessageBox>

// ---------------------------------------------------------------
Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    this->setFixedSize(this->width(),this->height());
    this->setWindowTitle("Copy dialog");

    // Init progress bar
    ui->progressBar->reset();
    ui->progressBar->setMinimum(0);
    ui->progressBar->setMaximum(100);

    m_is_pause   = false;
    m_is_copied  = false;

    QObject::connect(this->ui->pauseButton,  SIGNAL(clicked()), this, SLOT(pauseButtonClicked()));
    QObject::connect(this->ui->cancelButton, SIGNAL(clicked()), this, SLOT(cancelButtonClicked()));
}

Dialog::~Dialog()
{
    delete ui;
}

// ---------------------------------------------------------------
void Dialog::setPath(QString p1, QString p2)
{
    m_from_file_path = p1;
    m_to_file_path   = p2;
}

// ---------------------------------------------------------------
void Dialog::pauseButtonClicked()
{
    if (!m_is_pause) {
        m_is_pause = true;
        emit sendPauseSignal();
        ui->pauseButton->setText("Resume");
    } else {
        m_is_pause = false;
        emit sendResumeSignal();
        ui->pauseButton->setText("Pause");
    }
}

void Dialog::cancelButtonClicked()
{
    emit stopCopySignal();
}

// ---------------------------------------------------------------
void Dialog::setProgressSlot(float pr)
{
    ui->progressBar->setValue(pr * 100);
}

void Dialog::endCopySlot()
{
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

void Dialog::allCopiedSlot()
{
    m_is_copied = true;
}

void Dialog::reject()
{
    if (m_is_copied)
        QDialog::reject();
    else {
        emit stopCopySignal();
        QDialog::reject();
    }
}

// ---------------------------------------------------------------
void Dialog::start()
{
    // Create "CopyThread"
    CopyThread* copyThread = new CopyThread();

    // Connect "Dialog" and "CopyThread" to send pathes
    QObject::connect(this, SIGNAL(sendPathSignal(QString, QString)),
                         copyThread, SLOT(getPathSlot(QString, QString)));

    // Connect the "Dialog" and "CopyThread" to get progress
    QObject::connect(copyThread, SIGNAL(sendProgressSignal(float)),
                         this, SLOT(setProgressSlot(float)));

    // Connect the "Dialog" and "CopyThread" to send pause
    QObject::connect(this, SIGNAL(sendPauseSignal()),
                         copyThread, SLOT(pauseSlot()));

    // Connect the "Dialog" and "CopyThread" to undo pause
    QObject::connect(this, SIGNAL(sendResumeSignal()),
                         copyThread, SLOT(resumeSlot()));

    // Connect the "Dialog" and "CopyThread" to end copy
    QObject::connect(copyThread, SIGNAL(endCopySignal()),
                         this, SLOT(endCopySlot()));

    // Connect the "Dialog" and "CopyThread" to show dialog if files are opened
    QObject::connect(copyThread, SIGNAL(showDialogSignal()),
                         this, SLOT(show()));

    // Connect the "Dialog" and "CopyThread" to show error message
    QObject::connect(copyThread, SIGNAL(errorSignal()),
                         this, SLOT(errorMessageSlot()));

    // Connect the "Dialog" and "CopyThread" to stop copy
    QObject::connect(this, SIGNAL(stopCopySignal()),
                         copyThread, SLOT(stopCopySlot()));

    QObject::connect(copyThread, SIGNAL(allCopiedSignal()),
                         this, SLOT(allCopiedSlot()));

    // Send path to "CopyThread"
    emit sendPathSignal(m_from_file_path, m_to_file_path);

    // Copy!
    copyThread->start();
}
