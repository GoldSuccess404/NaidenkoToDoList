#ifndef TODOLISTWINDOW_H
#define TODOLISTWINDOW_H

#include <QMainWindow>
#include <QSqlDatabase>
#include <QSqlTableModel>
#include <QSqlError>
#include <QSqlQuery>
#include <QTableView>
#include <QVBoxLayout>
#include <QToolBar>
#include <QAction>
#include <QTabWidget>
#include <QDate>
#include "dialogeditingdata.h"


class ToDoListWindow : public QMainWindow
{
    Q_OBJECT

public:
    ToDoListWindow(QWidget *parent = nullptr);
    ~ToDoListWindow();
    void change_to_failed();
private:
    QSqlDatabase db;
    QString date;
    QString nameTable;

    QWidget *allTasks;
    QWidget *doneTasks;
    QWidget *failedTasks;
    QWidget *pendingTasks;

    QTableView *tv;
    QTableView *tv1;
    QTableView *tv2;
    QTableView *tv3;

    QSqlTableModel *model;
    QSqlTableModel *model1;
    QSqlTableModel *model2;
    QSqlTableModel *model3;

    dialogEditingData di;

    QToolBar *toolBar;
    QTabWidget *tabWid;
private slots:
    void addRow();
    void removeRow();
    void updateModel();
    void editingData();
    void doubleClicked(QModelIndex index);
    void addDoneTask();
    void slot_update_model_add_Row();

};
#endif // TODOLISTWINDOW_H
