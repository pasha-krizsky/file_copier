#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include "copythread.h"

namespace Ui {
class Dialog;
}

// Dialog with progress bar and two buttons
class Dialog : public QDialog
{
private:
    // Current progress (get from "CopyThread")
    int progress_to_show;
    // First file path
    QString file_from_path;
    // Second file path
    QString file_to_path;
    // Pause ("is_pause" == true) or Resume ("is_pause" == false)
    bool is_pause;

Q_OBJECT

public:
    // Constructor
    explicit Dialog(QWidget *parent = 0);
    // Destructor
    ~Dialog();

signals:
    // Send path to "CopyThread"
    void sendPathSignal(QString p1, QString p2);
    // Set pause (pause button clicked)
    void sendPauseSignal();
    // Undo pause (pause button clicked one more time)
    void sendResumeSignal();
    // Stop copying (cancel button clicked)
    void stopCopySignal();

private slots:
    // Pause button clicked
    void on_pauseButton_clicked();
    // Cancel button clicked
    void on_cancelButton_clicked();

public slots:
    // Get current progress from thread
    void getProgressSlot(float pr);
    // Get the names of files from main thread
    void getPathSlot(QString p1, QString p2);
    // Handle copy request
    void copySlot();
    // End copy
    void endCopySlot();
    // Show error message
    void errorMessageSlot();

private:
    Ui::Dialog *ui;
};

#endif
