#include "dialogeditingdata.h"
#include "src/SmtpMime"

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

    setWindowIcon(QIcon("://images/mainicon.png"));

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

/*рассылка на почту*/
if (email->text().contains("@gmail.com")){
    /*изменений для gmail почты*/
    MimeMessage message;

    //почта отправителя
    EmailAddress sender("здесь пишите свою почту, с которой будете отправлять", "тут имя от которого будете отправлять");
    message.setSender(sender);

    //почта кому отправить
    EmailAddress to(email->text(), responsible->text());
    message.addRecipient(to);

    //тема письма
    message.setSubject("Letter from NaidenkoToDoList");

    MimeText text;
    //объект для добавления текста
    text.setText(task->toPlainText());
    //теперь добавляем текст в почту
    message.addPart(&text);

    //теперь отправляем письмо
    SmtpClient smtp("smtp.gmail.com", 465, SmtpClient::SslConnection);

    smtp.connectToHost();
    if (!smtp.waitForReadyConnected()) {
        qDebug() << "Ошибка подключения к хосту";
        return;
    }

    //здесь нужно указать ваш пароль для внешних приложений
    smtp.login("здесь указываем опять вашу почту, с которой будет отправка", "а здесь пароль для внешних приложений");
    if (!smtp.waitForAuthenticated()) {
        qDebug() << "Ошибка, неправильный логин!!!!";
        return;
    }

    smtp.sendMail(message);
    if (!smtp.waitForMailSent()) {
        qDebug() << "Ошибка отправки сообщения!";
        return;
    }

    smtp.quit();
  }

  else {
    /*изменения для mail почты*/
    MimeMessage message;

    //почта отправителя
    EmailAddress sender("здесь пишите свою почту, с которой будете отправлять", "тут имя от которого будете отправлять");
    message.setSender(sender);

    //почта кому отправить
    EmailAddress to(email->text(), responsible->text());
    message.addRecipient(to);

    //тема письма
    message.setSubject("Letter from NaidenkoToDoList");

    MimeText text;
    //объект для добавления текста
    text.setText(task->toPlainText());
    //теперь добавляем текст в почту
    message.addPart(&text);

    //теперь отправляем письмо
    SmtpClient smtp("smtp.mail.ru", 465, SmtpClient::SslConnection);

    smtp.connectToHost();
    if (!smtp.waitForReadyConnected()) {
        qDebug() << "Ошибка подключения к хосту";
        return;
    }

    //здесь нужно указать ваш пароль для внешних приложений
    smtp.login("здесь указываем опять вашу почту, с которой будет отправка", "а здесь пароль для внешних приложений");
    if (!smtp.waitForAuthenticated()) {
        qDebug() << "Ошибка, неправильный логин!!!!";
        return;
    }

    smtp.sendMail(message);
    if (!smtp.waitForMailSent()) {
        qDebug() << "Ошибка отправки сообщения!";
        return;
    }

    smtp.quit();
  }

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
