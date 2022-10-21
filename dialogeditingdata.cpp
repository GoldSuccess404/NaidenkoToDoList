#include "dialogeditingdata.h"

dialogEditingData::dialogEditingData(QWidget *parent) : QWidget(parent)
{
    labTask = new QLabel("Task: ");
    labResponsible = new QLabel("Responsible: ");
    labEmail = new QLabel("Email: ");
    labDeadline = new QLabel("Deadline: ");

    task = new QTextEdit;
    responsible = new QLineEdit;
    email = new QLineEdit;
    deadline = new QLineEdit;

    ok = new QPushButton("Ok");
    close1 = new QPushButton("Close");

    QVBoxLayout *vay = new QVBoxLayout;
    QHBoxLayout *hay = new QHBoxLayout;

    hay->addWidget(ok);
    hay->addWidget(close1);

    vay->addWidget(labTask);
    vay->addWidget(task);
    vay->addWidget(labResponsible);
    vay->addWidget(responsible);
    vay->addWidget(labEmail);
    vay->addWidget(email);
    vay->addWidget(labDeadline);
    vay->addWidget(deadline);
    vay->addLayout(hay);
    setLayout(vay);

    mapper = new QDataWidgetMapper(this);
    mapper->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);

    QObject::connect(ok, SIGNAL(clicked(bool)), this, SLOT(addSubmit()));
    QObject::connect(close1, &QPushButton::clicked, this, &dialogEditingData::closeDialog);
}

/*очистка поля deadline*/
void dialogEditingData::lineClear()
{
    deadline->clear();
}


void dialogEditingData::addSubmit()
{
/*чтобы изменения применились к модели*/
    mapper->submit();
    close();
    emit signal_update_model_add_Row();
}


void dialogEditingData::closeDialog()
{
    close();
    emit click_to_close();
}


void dialogEditingData::setModel(QAbstractItemModel *model)
{
    mapper->setModel(model);
    mapper->addMapping(task, 1, "plainText");
    mapper->addMapping(responsible, 2);
    mapper->addMapping(email, 3);
    mapper->addMapping(deadline, 4);
}
