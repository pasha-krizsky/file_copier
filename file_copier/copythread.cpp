#include "copythread.h"
#include <fstream>
#include <string>
#include <QMessageBox>
#include <QWaitCondition>

// -------------------------------------------------------------
CopyThread::CopyThread()
{
    m_is_pause = false;
    m_is_stop  = false;
    m_fin  = NULL;
    m_fout = NULL;
    m_file_size = 0;
}

CopyThread::~CopyThread()
{
    delete m_fin;
    delete m_fout;
}

// -------------------------------------------------------------
void CopyThread::getPathSlot(QString p1, QString p2)
{
    this->m_file_from_path = p1;
    this->m_file_to_path = p2;
}

void CopyThread::pauseSlot()
{
    m_is_pause = true;
}

void CopyThread::resumeSlot()
{
    m_is_pause = false;
}

void CopyThread::stopCopySlot()
{
    if (m_fin != NULL) {
        fclose(m_fin);
    }
    if (m_fout != NULL) {
        fclose(m_fout);
        remove(m_file_to_path.toStdString().c_str());
    }
    m_is_stop = true;
    emit endCopySignal();
}

// -------------------------------------------------------------
void CopyThread::run() {

    // Number of bytes to read and write by one step
    const int BLOCK_SIZE = 256;
    // Temp storage
    unsigned char buffer[BLOCK_SIZE];

    // Try to open two files
    if(!(m_fin = fopen(m_file_from_path.toStdString().c_str(), "rb")) == NULL &&
            !(m_fout = fopen(m_file_to_path.toStdString().c_str(), "wb")) == NULL) {

        // Show "Dialog" if files are opened
        emit showDialogSignal();

        // The end of the file
        _fseeki64(m_fin, 0, SEEK_END);
        // Get size of the file
        m_file_size = _ftelli64(m_fin);
        // Go to start
        _fseeki64(m_fin, 0, SEEK_SET);

        // Current progress
        m_copy_progress = 0.0;

        // Read and write by "BLOCK_SIZE" bytes
        for(size_t i = 0; i <= m_file_size / BLOCK_SIZE; ++i) {

            // Check flag
            if (m_is_stop)
                return;

            // Sleep 0.2 sec. if "is_pause" and check "is_pause" again
            while (m_is_pause)
                sleep(0.2);

            // Read "BLOCK_SIZE" bytes to buffer
            int readed = fread(buffer, sizeof(unsigned char), BLOCK_SIZE, m_fin);
            // Write "BLOCK_SIZE" bytes
            fwrite(buffer, sizeof(unsigned char), readed, m_fout);
            // Increase current progress by "BLOCK_SIZE"
            m_copy_progress += BLOCK_SIZE;
            // Send current progress to "Dialog" (value from 0 to 1)
            emit sendProgressSignal(m_copy_progress / m_file_size);
        }

        emit sendProgressSignal(1);

        // Close files
        fclose(m_fin);
        fclose(m_fout);

        emit allCopiedSignal();
        emit endCopySignal();

    } else {
        // Can't open file
        emit errorSignal();
    }
}
