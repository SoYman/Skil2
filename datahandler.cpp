#include "datahandler.h"
#include <QtSql>

// DEBUG
#include <QDebug>
// DEBUG

DataHandler::DataHandler() {
    database = QSqlDatabase::addDatabase("QSQLITE");

    database.setDatabaseName("database");
    database.open();

    QSqlQuery query;

    query.exec(
        "create table if not exists Computers ("
        "id INTEGER primary key autoincrement,"
        "name TEXT,"
        "year INTEGER,"
        "type TEXT,"
        "made INTEGER,"
        "cost INTEGER)");

    query.exec(
        "create table if not exists People ("
        "id INTEGER primary key autoincrement,"
        "name TEXT,"
        "born INTEGER,"
        "died TEXT,"
        "gender TEXT,"
        "nationality TEXT)");

    query.exec(
        "create table if not exists Relations ("
        "id INTEGER primary key autoincrement"
        "computer_id INTEGER"
        "person_id INTEGER"
        "relationship TEXT)");
}

// Add computer
bool DataHandler::add(QString name,
                      int year,
                      QString type,
                      bool made,
                      int cost) {
    QSqlQuery query;

    query.prepare(
        "INSERT INTO Computers (name, year, type, made, cost) "
        "values (:name, :year, :type, :made, :cost)");
    query.bindValue(":name", name);
    query.bindValue(":year", year);
    query.bindValue(":type", type);
    query.bindValue(":made", made);
    query.bindValue(":cost", cost);

    return query.exec();
}

// Add person
bool DataHandler::add(QString name,
                      int born,
                      int died,
                      QString gender,
                      QString nationality) {
    QSqlQuery query;

    query.prepare(
        "INSERT INTO People (name, born, died, gender, "
        "nationality) values (:name, :born, :died, "
        ":gender, :nationality)");
    query.bindValue(":name", name);
    query.bindValue(":born", born);
    query.bindValue(":died", died);
    query.bindValue(":gender", gender);
    query.bindValue(":nationality", nationality);

    return query.exec();
}

// Add relationship
bool DataHandler::add(int computer_id, int person_id, QString relationship) {
    QSqlQuery query;
    query.prepare(
        "INSERT INTO Relations (computer_id, person_id, relationship) values "
        "(:computer_id, :person_id, :relationship)");
    query.bindValue(":computer_id", computer_id);
    query.bindValue(":person_id", person_id);
    query.bindValue(":relationship", relationship);

    return query.exec();
}

void DataHandler::list(QVector<Computer>& computers, int cmd, QString order) {
    QSqlQuery query;
    QVector<QString> commands = {"id", "name", "year", "type", "made", "cost"};

    // QSqlQuery didn't behave when constructing
    query.exec("SELECT * FROM Computers ORDER BY " + commands[cmd] + " " +
               order);

    while (query.next()) {
		int id = query.value("id").toInt();
        QString name = query.value("name").toString();
        int year = query.value("year").toInt();
        QString type = query.value("type").toString();
        bool made = query.value("made").toBool();
        int cost = query.value("cost").toInt();
        computers.append(Computer{id, name, year, type, made, cost});
    }
}

void DataHandler::list(QVector<Person>& people, int cmd, QString order) {
    QSqlQuery query;
    QVector<QString> commands = {"id",   "name",   "born",
                                 "died", "gender", "nationality"};

    query.exec("SELECT * FROM People ORDER BY " + commands[cmd] + " " + order);

    while (query.next()) {
		int id = query.value("id").toInt();
        QString name = query.value("name").toString();
        int born = query.value("born").toInt();
        int died = query.value("died").toInt();
        QString gender = query.value("gender").toString();
        QString nationality = query.value("nationality").toString();
        people.append(Person{id, name, born, died, gender, nationality});
    }
}

int DataHandler::computerCount() {
    QSqlQuery query;
    query.exec("SELECT COUNT(*) FROM Computers");
    query.next();
	qDebug() << query.value(0).toInt();
    return query.value(0).toInt();
}

int DataHandler::peopleCount() {
    QSqlQuery query;
    query.exec("SELECT COUNT(*) FROM People");
    query.next();
	qDebug() << query.value(0).toInt();
    return query.value(0).toInt();
}
