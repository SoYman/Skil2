#ifndef DATAHANDLER_H
#define DATAHANDLER_H

#include <QString>
#include <QtSql>
#include <QVector>


struct Computer {
	int id;
    QString name;
    int year;
    QString type;
    bool made;
    int cost;
};

struct Person {
	int id;
	QString name;
	int born;
	int died;
	QString gender;
	QString nationality;
};

class DataHandler {
   private:
    QSqlDatabase database;

   public:
    DataHandler();
	bool add(QString name, int year, QString type, bool made, int cost);
	bool add(QString name, int born, int died, QString gender, QString nationality);
	bool add(int computer_id, int person_id, QString relationship);
	void list(QVector<Computer> & computers, int cmd, QString order);
	void list(QVector<Person> & people, int cmd, QString order);
	int computerCount();
	int peopleCount();
};

#endif  // DATAHANDLER_H
