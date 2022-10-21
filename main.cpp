#include "todolistwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ToDoListWindow w;
    w.show();
    return a.exec();
}
