#ifndef INTERFACE_H
#define INTERFACE_H

#include <QTextStream>
#include <QString>
#include <QVector>

#include "datahandler.h"

enum Table { MAIN, COMPUTER, PERSON, RELATION };

class Interface {
   private:
    const int delimiter_length = 64;
    QTextStream in;
    QTextStream out;
    QString command;

    DataHandler handler;

    void instructions(Table);
    void subMenu(Table);

   public:
    Interface();
    int start();
    void list(Table);
    void separate();
    QString clear();
    void add(Table);
	void addComputer();
	void addPerson();
	void addRelation();
    void remove(Table);
    void specific_instructions(Table table,
                               QString instructions,
                               QVector<QString> fields);
    void display(QVector<Computer> computers);
    void display(QVector<Person> people);
};

#endif  // INTERFACE_H
