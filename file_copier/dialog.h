#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include "copythread.h"

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{

private:
    // This member stores current progress of copying
    int m_progress_to_show;
    // Two paths
    QString m_from_file_path;
    QString m_to_file_path;
    // Flags
    bool m_is_pause;
    bool m_is_copied;

Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();

    // Setter for "m_from_file_path" and "m_to_file_path"
    void setPath(QString p1, QString p2);
    // This method starts copy process
    void start();

    void reject();

signals:
    // Send path to "CopyThread"
    void sendPathSignal(QString p1, QString p2);
    // Set pause ("Pause" button clicked)
    void sendPauseSignal();
    // Undo pause ("Pause" button clicked one more time)
    void sendResumeSignal();
    // Stop copying ("Cancel" button clicked)
    void stopCopySignal();

private slots:
    // "Pause" ("Resume") button clicked
    void pauseButtonClicked();
    // "Cancel" button clicked
    void cancelButtonClicked();

public slots:
    // Get current progress from "CopyThread"
    void setProgressSlot(float pr);
    // This slot closes current dialog
    void endCopySlot();
    // This slot shows error message
    void errorMessageSlot();
    void allCopiedSlot();

private:
    Ui::Dialog *ui;
};

#endif
