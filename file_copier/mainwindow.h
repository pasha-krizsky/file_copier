#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "dialog.h"
#include "copythread.h"
#include <QString>

namespace Ui {
class MainWindow;
}

// Main window
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    // Constructor
    explicit MainWindow(QWidget *parent = 0);
    // Destructor
    ~MainWindow();

signals:
    // Send path to "Dialog"
    void sendPathSignal(QString p1, QString p2);
    // Send copy request to "Dialog"
    void startCopySignal();

private slots:
    // Start button clicked
    void on_startButton_clicked();

private:
    // Pointer to "Dialog"
    Dialog* dialog;
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
