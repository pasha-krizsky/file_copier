#include "dialog.h"
#include <QApplication>
#include <QString>
#include <QTextStream>
QTextStream cout(stdout);
QTextStream cin(stdin);

int main(int argc, char *argv[])
{
    if (argc != 3) {
        cout << "Wrong format";
        cout.flush();
        return 1;
    }

    QApplication a(argc, argv);
    Dialog d;
    d.setPath(argv[1], argv[2]);
    d.start();
    return a.exec();
}
