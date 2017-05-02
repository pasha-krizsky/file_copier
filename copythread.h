#ifndef COPYTHREAD_H
#define COPYTHREAD_H

#include <QThread>
#include <QString>

// Class to copy file in new thread
class CopyThread: public QThread
{
private:

    // File to read
    FILE *fin;
    // File to write
    FILE *fout;
    // Size of a file
    size_t m_file_size;
    // Current progress
    float m_copy_progress;
    // Path to "fin"
    QString file_from_path;
    // Path to "fout"
    QString file_to_path;
    // Pause ("is_pause" == true) or Resume ("is_pause" == false)
    bool is_pause;
    // Stop (True when cancel button pressed)
    bool is_stop;

Q_OBJECT

public:
    // Constructor
    CopyThread();
    // Destructor
    ~CopyThread();
    // Override "run" method
    void run();

signals:
    // Send current progress of copying to "Dialog"
    void sendProgressSignal(float);
    // Copying was finished
    void endCopySignal();
    // Show "Dialog" if it is acceptable
    void showDialogSignal();
    // Send error message to "Dialog" (file doesn't found)
    void errorSignal();

public slots:
    // Get path from dialog
    void getPathSlot(QString p1, QString p2);
    // Set pause ("is_pause" == true)
    void pauseSlot();
    // Set resume ("is_pause" == false)
    void resumeSlot();
    // Cancel button pressed
    // (Close files, delete file to output, "is_stop" == true)
    void stopCopySlot();
};

#endif
