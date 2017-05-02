#include "copythread.h"
#include <fstream>
#include <string>
#include <QMessageBox>
#include <QWaitCondition>


// -------------------------------------------------------------
CopyThread::CopyThread()
{
    is_pause = false;
    is_stop  = false;

    fin  = NULL;
    fout = NULL;

    m_file_size = 0;
}

CopyThread::~CopyThread()
{
    delete fin;
    delete fout;
}
// -------------------------------------------------------------


void CopyThread::getPathSlot(QString p1, QString p2)
{
    this->file_from_path = p1;
    this->file_to_path = p2;
}

void CopyThread::pauseSlot()
{
    is_pause = true;
}

void CopyThread::resumeSlot()
{
    is_pause = false;
}

void CopyThread::stopCopySlot()
{
    if (fin != NULL) {
        fclose(fin);
    }
    if (fout != NULL) {
        fclose(fout);
        remove(file_to_path.toStdString().c_str());
    }
    emit endCopySignal();
    is_stop = true;
}
// -------------------------------------------------------------


void CopyThread::run() {

    // Number of bytes to read and write by one step
    const int BLOCK_SIZE = 256;
    // Number of bytes to read and write by one step
    // (If last part of the file or whole file
    // is smaller than "BLOCK_SIZE" bytes)
    const int SMALL_BLOCK_SIZE = 32;
    // Temp storage
    unsigned char buffer[BLOCK_SIZE];
    // Temp storage 2
    unsigned char buffer_small[SMALL_BLOCK_SIZE];

    // Try to open two files
    if(!(fin = fopen(file_from_path.toStdString().c_str(), "rb")) == NULL &&
            !(fout = fopen(file_to_path.toStdString().c_str(), "wb")) == NULL) {

        // Show "Dialog" if files are opened
        emit showDialogSignal();

        // The end of the file
        fseek(fin, 0, SEEK_END);
        // Get size of the file
        m_file_size = ftell(fin);
        // Go to start
        rewind(fin);

        // Current progress
        m_copy_progress = 0.0;

        // Read and write by "BLOCK_SIZE" bytes
        for(size_t i = 0; i < m_file_size / BLOCK_SIZE; ++i) {

            // Check flag
            if (is_stop)
                return;

            // Sleep 0.2 sec. if "is_pause" and check "is_pause" again
            while (is_pause) {
                sleep(0.2);
            }

            // Read "BLOCK_SIZE" bytes to buffer
            fread(buffer, sizeof(unsigned char), BLOCK_SIZE, fin);
            // Write "BLOCK_SIZE" bytes
            fwrite(buffer, sizeof(unsigned char), BLOCK_SIZE, fout);
            // Increase current progress by "BLOCK_SIZE"
            m_copy_progress += BLOCK_SIZE;
            // Send current progress to "Dialog" (value from 0 to 1)
            emit sendProgressSignal(m_copy_progress / m_file_size);
        }

        // Read last part of the file or whole file
        // (if its size is smaller than "BLOCK_SIZE")
        if(m_file_size %= BLOCK_SIZE) {

            // Check flag
            if (is_stop)
                return;

            // Read and write by "SMALL_BLOCK_SIZE" bytes
            if (m_file_size > SMALL_BLOCK_SIZE)
                for (size_t i = 0; i < m_file_size / SMALL_BLOCK_SIZE; ++i) {

                    // Check flag
                    if (is_stop)
                        return;

                    // Read "SMALL_BLOCK_SIZE" bytes to buffer
                    fread(buffer_small, sizeof(unsigned char), SMALL_BLOCK_SIZE, fin);
                    // Write "SMALL_BLOCK_SIZE" bytes
                    fwrite(buffer_small, sizeof(unsigned char), SMALL_BLOCK_SIZE, fout);

                    // Increase current progress by "SMALL_BLOCK_SIZE"
                    m_copy_progress += SMALL_BLOCK_SIZE;
                    // Send current progress to "Dialog"
                    emit sendProgressSignal(m_copy_progress / m_file_size);

                    // Read and write last part
                    if (m_file_size %= SMALL_BLOCK_SIZE) {
                        fread(buffer_small, sizeof(unsigned char), m_file_size, fin);
                        fwrite(buffer_small, sizeof(unsigned char), m_file_size, fout);
                        m_copy_progress += m_file_size;
                        emit sendProgressSignal(m_copy_progress / m_file_size);
                    }
            } else {
                // Check flag
                if (is_stop)
                    return;

                fread(buffer_small, sizeof(unsigned char), m_file_size, fin);
                fwrite(buffer_small, sizeof(unsigned char), m_file_size, fout);
                m_copy_progress += m_file_size;
                emit sendProgressSignal(m_copy_progress / m_file_size);
            }

            emit sendProgressSignal(1);
        }

        // Close files
        fclose(fin);
        fclose(fout);
        emit endCopySignal();

    } else {
        // Can't open file
        emit errorSignal();
    }
}
