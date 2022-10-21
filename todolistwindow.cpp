#include "todolistwindow.h"
#include <QDebug>
ToDoListWindow::ToDoListWindow(QWidget *parent)
    : QMainWindow(parent)
{
    /*формирование текущей даты*/
    int year = QDate::currentDate().year();
    int month = QDate::currentDate().month();
    int day = QDate::currentDate().day();
    date = QString::number(year) + "/" + QString::number(month) + "/" + QString::number(day);


    tv = new QTableView;
    tv1 = new QTableView;
    tv1->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tv2 = new QTableView;
    tv2->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tv3 = new QTableView;
    tv3->setEditTriggers(QAbstractItemView::NoEditTriggers);

    tabWid = new QTabWidget(this);
    this->setMinimumSize(700, 400);
    setWindowIcon(QIcon("://images/mainIcon.png"));

/*подключение базы данных и в целом настройка работы программы*/

    nameTable = "list";           /*тут нужно вставить название вашей таблицы*/
    db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("localhost");
    db.setUserName("noname");      /*изменить на ваше имя пользователя*/
    db.setPassword("1111");          /*изменить на ваш пароль*/
    db.setDatabaseName("ToDoList");  /*изменить на название вашей базы данных*/
                                     /*последние изменения нужно внести в строках: 156 и 283*/
    if (!db.open()){
       qDebug() << db.lastError().text();
       return;
     }

/*инициализация модели All Tasks*/
    allTasks = new QWidget(this);
    model = new QSqlTableModel(this, db);
    model->setTable(nameTable);
    model->select();

/*установка модели All Tasks в таблицу*/
    di.setModel(model);
    tv->setModel(model);
    tv->setColumnHidden(0, true);
    tv->setColumnHidden(5, true);

/*инициализация модели Done Tasks*/
    doneTasks = new QWidget(this);
    model1 = new QSqlTableModel(this, db);
    model1->setTable(nameTable);
    model1->setFilter("status = '1'");
    model1->select();

/*установка модели Done Tasks в таблицу*/
    tv1->setModel(model1);
    tv1->setColumnHidden(0, true);
    tv1->setColumnHidden(5, true);

/*инициализация модели Failed Tasks*/
    failedTasks = new QWidget(this);
    model2 = new QSqlTableModel(this, db);
    model2->setTable(nameTable);
    change_to_failed();
    model2->setFilter("status = 2");
    model2->select();

/*установка модели Failed Tasks*/
    tv2->setModel(model2);
    tv2->setColumnHidden(0, true);
    tv2->setColumnHidden(5, true);

/*инициализация модели Pending Tasks*/
    pendingTasks = new QWidget(this);
    model3 = new QSqlTableModel(this, db);
    model3->setTable(nameTable);
    model3->setFilter("status = 0");
    model3->select();

/*установка модели Pending Tasks*/
    tv3->setModel(model3);
    tv3->setColumnHidden(0, true);
    tv3->setColumnHidden(5, true);

/*компановка All Tasks*/
    QVBoxLayout *vay = new QVBoxLayout;
    vay->addWidget(tv);
    allTasks->setLayout(vay);

/*компановка Done Tasks*/
    QVBoxLayout *vay1 = new QVBoxLayout;
    vay1->addWidget(tv1);
    doneTasks->setLayout(vay1);

/*компановка Failed Tasks*/
    QVBoxLayout *vay2 = new QVBoxLayout;
    vay2->addWidget(tv2);
    failedTasks->setLayout(vay2);

/*компановка Pending Tasks*/
    QVBoxLayout *vay3 = new QVBoxLayout;
    vay3->addWidget(tv3);
    pendingTasks->setLayout(vay3);

/*вкладки*/
    setCentralWidget(tabWid);
    tabWid->addTab(allTasks, "All tasks");
    tabWid->addTab(doneTasks, "Done Tasks");
    tabWid->addTab(failedTasks, "Failed Tasks");
    tabWid->addTab(pendingTasks, "Pending Tasks");
    tabWid->setCurrentIndex(0);

/*экшены на tool bar*/
    QAction *add = new QAction(QIcon("://images/add.png"), "add", this);
    QAction *remove = new QAction(QIcon("://images/remove.png"), "remove", this);
    QAction *edit = new QAction(QIcon("://images/edit.png"), "edit", this);
    QAction *done = new QAction(QIcon("://images/done.png"), "done", this);

/*tool bar*/
    toolBar = addToolBar("main");
    toolBar->addAction(add);
    toolBar->addAction(remove);
    toolBar->addAction(edit);
    toolBar->addAction(done);


    /*добавление строки через виджет*/
    QObject::connect(add, &QAction::triggered, this, &ToDoListWindow::addRow);
    /*удаление строки*/
    QObject::connect(remove, &QAction::triggered, this, &ToDoListWindow::removeRow);
    /*обновление модели, если мы закрыли диал. окно, чтобы не было лишней строки*/
    QObject::connect(&di, SIGNAL(click_to_close()), this, SLOT(updateModel()));
    /*редактирование через виджет*/
    QObject::connect(edit, &QAction::triggered, this, &ToDoListWindow::editingData);
    /*редактирование по двойному клику через виджет (All Tasks)*/
    QObject::connect(tv, &QTableView::doubleClicked, this, &ToDoListWindow::doubleClicked);
    /*добавление 1 в статус при нажатии на кнопку tool bara*/
    QObject::connect(done, &QAction::triggered, this, &ToDoListWindow::addDoneTask);
    /*обновление моделей при добавлении строки*/
    QObject::connect(&di, &dialogEditingData::signal_update_model_add_Row, this, &ToDoListWindow::slot_update_model_add_Row);
}


ToDoListWindow::~ToDoListWindow()
{
    delete toolBar;
}



void ToDoListWindow::change_to_failed()
{
    QString str = "update list set status = '%1'"          /*вместо list название вашей таблицы*/
            "where deadline < '%2' and status != '%3'";

    QSqlQuery(str.arg(2).arg(date).arg(1));
}



/*добавление строки через виджет*/
void ToDoListWindow::addRow()
{
    di.lineClear();
    model->insertRow(model->rowCount());
    di.setModel(model);
    di.mapper->toLast();
    di.show();
}




/*удаление строки*/
void ToDoListWindow::removeRow()
{
    /*если удаляем с вкладки All Tasks*/
   if(tabWid->currentIndex() == 0){
    model->removeRow(tv->currentIndex().row());
/*обновление всех моделей*/
    model->select();
    model1->select();
    model2->select();
    model3->select();
   }

   /*если удаляем с вкладки Done Tasks*/
   else
       if(tabWid->currentIndex() == 1){
           model1->removeRow(tv->currentIndex().row());

           model->select();
           model1->select();
       }

   /*если удаляем с вкладки Failed Tasks*/
   else
       if(tabWid->currentIndex() == 2){
           model2->removeRow(tv->currentIndex().row());

           model->select();
           model2->select();
       }

   /*если удаляем с вкладки Pending Tasks*/
   else
       if(tabWid->currentIndex() == 3){
           model3->removeRow(tv->currentIndex().row());

           model->select();
           model3->select();
       }
}




/*обновление модели при закрытии диал. окна*/
void ToDoListWindow::updateModel()
{
    model->select();
}




/*редактирование через виджет*/
void ToDoListWindow::editingData()
{
    /*если редактирование во вкладке All Tasks*/
    if(tabWid->currentIndex() == 0){
    di.setModel(model);
    di.mapper->setCurrentModelIndex(tv->currentIndex());
    di.show();
    }

    /*если редактирование во вкладке Done Tasks*/
    else
        if (tabWid->currentIndex() == 1){
            di.setModel(model1);
            di.mapper->setCurrentModelIndex(tv1->currentIndex());
            di.show();
        }

    /*если редактирование во вкладке Failed Tasks*/
    else
        if (tabWid->currentIndex() == 2){
            di.setModel(model2);
            di.mapper->setCurrentModelIndex(tv2->currentIndex());
            di.show();
        }

    /*если редактирование во вкладке Pending Tasks*/
    else
        if (tabWid->currentIndex() == 3){
           di.setModel(model3);
           di.mapper->setCurrentModelIndex(tv3->currentIndex());
           di.show();
        }
}




/*редактирование по двойному клику через виджет (только All Tasks)*/
void ToDoListWindow::doubleClicked(QModelIndex index)
{
    di.mapper->setModel(model);
    di.mapper->setCurrentModelIndex(index);
    di.show();
}




/*добавление выполненного задания во вкладку Done Tasks*/
void ToDoListWindow::addDoneTask()
{
    QVariant var = model->data(model->index(tv->currentIndex().row(), 0));
    QString str = "update list set status = '%1'"             /*и здесь вместо list название вашей таблицы*/
            "where id = '%2'";

    /*ищем по айди и в статус добавляем 1*/
    QSqlQuery(str.arg(1).arg(var.toString()));


    /*обновление данных модели Done Tasks*/
    model1->select();
    model3->select();
}




/*обновление моделей при добавлении строки*/
void ToDoListWindow::slot_update_model_add_Row()
{
    model->select();

    model1->setFilter("status = 1");
    model1->select();

    change_to_failed();
    model2->setFilter("status = 2");
    model2->select();

    model3->setFilter("status = 0");
    model3->select();
}

