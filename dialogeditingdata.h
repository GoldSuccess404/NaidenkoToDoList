#ifndef DIALOGEDITINGDATA_H
#define DIALOGEDITINGDATA_H

#include <QWidget>
#include <QDataWidgetMapper>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTextEdit>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>

class dialogEditingData : public QWidget
{
    Q_OBJECT
public:
    explicit dialogEditingData(QWidget *parent = nullptr);
    QDataWidgetMapper *mapper;
    void lineClear();
private:
    QTextEdit *task;

    QLineEdit *responsible;
    QLineEdit *email;
    QLineEdit *deadline;

    QLabel *labTask;
    QLabel *labResponsible;
    QLabel *labEmail;
    QLabel *labDeadline;

    QPushButton *ok;
    QPushButton *close1;
public slots:
    void addSubmit();
    void closeDialog();
    void setModel(QAbstractItemModel *model);
signals:
    void click_to_close();
    void signal_update_model_add_Row();

};

#endif // DIALOGEDITINGDATA_H
