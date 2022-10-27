# Введение
__NaidenkoToDoList__ - это простой список дел, написанный на С++ (FrameWork QT), но к нему подключена база данных _MySql_.
## Руководство по установке
1) В самом начале вы должны загрузить данный репозиторий, используя команду:
~~~
git clone https://github.com/GoldSuccess404/NaidenkoToDoList.git
~~~
2) Затем откройте свою оболочку SQL (MySQL) и создайте новую базу данных:
~~~
create database <название вашей базы данных>;
~~~
3) После этого вам нужно восстановить базу данных, используя файл дампа в репозитории, который вы загрузили (находится в папке _dump_). Можете открыть командную
строку и перейти по пути _C:\MySql\bin_ (у вас он может немного отличатся, поэтому корректируйте уже под себя) и ввести команду:
~~~
mysql -u <имя вашего пользователя> -p <название вашей новосозданной базы данных> < C:\qtProjects\NaidenkoToDoList\NaidenkoToDoList\dump\dump.sql (и полный путь
к файлу дампа, у вас он тоже будет оличаться)
~~~
4) Откройте исходный файл __todolistwindow.cpp__ и измените аргументы функций: _setHostName_, _setUserName_, _SetPassword_, _setDataBaseName_ и для переменной _nameTable_
(если название таблицы осталось по умолчанию __list__, то менять не нужно) в соответсвии с вашими настройками:
~~~
27     nameTable = "list";
29    db.setHostName("localhost");
30    db.setUserName("noname");
31    db.setPassword("1111");
32    db.setDatabaseName("ToDoList");
~~~
5) _Все, теперь вы можете пользоваться программой!_
