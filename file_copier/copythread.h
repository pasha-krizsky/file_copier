#ifndef COPYTHREAD_H
#define COPYTHREAD_H

#include <QThread>
#include <QString>
#include <fstream>

class CopyThread: public QThread
{
private:
    // Files
    FILE *m_fin;
    FILE *m_fout;
    // This member stores size of a file (max size = 2^64 bytes)
     long long  m_file_size;
    // This member stores current progress of copying
    float m_copy_progress;
    // Pathes
    QString m_file_from_path;
    QString m_file_to_path;
    // Flags
    bool m_is_pause;
    bool m_is_stop;

Q_OBJECT

public:
    CopyThread();
    ~CopyThread();
    // This method is performed in a separate thread
    void run();

signals:
    // Send current progress of copying to "Dialog"
    void sendProgressSignal(float);
    // Copying was finished
    void endCopySignal();
    // Show "Dialog" if it is acceptable
    void showDialogSignal();
    // Send error message to "Dialog" (can't open file)
    void errorSignal();
    void allCopiedSignal();

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
