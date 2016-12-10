#include "interface.h"

#include <QProcess>
#include <QString>
#include <QTextStream>
#include <iostream>
#include <QVector>

using namespace std;

Interface::Interface() : in(stdin), out(stdout) {}

int Interface::start() {
    // out.setFieldWidth(4);
    out.setFieldAlignment(QTextStream::AlignRight);
    while (true) {
        // command = clear();
        instructions(MAIN);

        in >> command;
        if (command == "1") {
            subMenu(COMPUTER);
        } else if (command == "2") {
            subMenu(PERSON);
        } else if (command == "3") {
            subMenu(RELATION);
        } else if (command == "4") {
            // TODO: search
        } else if (command == "5") {
            // remoce entry
        } else if (command.startsWith('Q', Qt::CaseInsensitive) ||
                   command.startsWith('X', Qt::CaseInsensitive)) {
            break;
        } else {
            out << "Invalid input, please enter a digit according to the "
                   "following table.\n";
        }
    }
    return 0;
}

void Interface::instructions(Table table) {
    QString title;
    QVector<QString> commands;
    switch (table) {
        case MAIN:
            title = "Choose a submenu or a command";
            commands = {"Computer menu", "Person menu",
                        "Computer - person relation menu", "Exit program"};
            break;
        case COMPUTER:
            title = "Computers menu";
            commands = {"Add a computer entry",
                        "Remove one or more computer entries", "List computers",
                        "Search for computers", "Return to main menu"};
            break;
        case PERSON:
            title = "Person menu";
            commands = {"Add person", "Remove one or more people",
                        "List people", "Search for people",
                        "Return to main menu"};
            break;
        case RELATION:
            title = "Computer, person relations menu";
            commands = {"Add relation", "Remove one or more relations",
                        "List relations", "Search for people",
                        "Return to main menu"};
            break;
    }
    command = clear();
    separate();
    out << title << endl;
    separate();
    for (size_t i = 0; i < commands.size() - 1; ++i) {
        out << qSetFieldWidth(0) << i + 1 << qSetFieldWidth(4) << "-> "
            << commands[i];
        out << qSetFieldWidth(0) << endl;
    }
    out << 'Q' << qSetFieldWidth(4) << "-> " << commands[commands.size() - 1]
        << qSetFieldWidth(0) << endl;
    separate();
    out << "Enter a command: " << flush;
}

void Interface::subMenu(Table table) {
    command = clear();
    instructions(table);
    in >> command;
    if (command == "1") {
        add(table);
    } else if (command == "2") {
        remove(table);
    } else if (command == "3") {
        list(table);
    } else if (command == "4") {
        // TODO: search
    } else if (command == "5") {
        // remoce entry
    } else if (!command.startsWith('Q', Qt::CaseInsensitive) &&
               !command.startsWith('X', Qt::CaseInsensitive) &&
               !command.startsWith('B', Qt::CaseInsensitive)) {
        subMenu(table);
    }
}

void Interface::add(Table table) {
    command = clear();

    if (table == COMPUTER) {
        addComputer();
    } else if (table == PERSON) {
        addPerson();
    } else if (table == RELATION) {
        addRelation();
    }

    out << "Do you want to add more entries? (Y/N) " << flush;
    in >> command;
    if (command.startsWith('Y', Qt::CaseInsensitive)) {
        add(table);
    }
}

void Interface::addComputer() {
    QString name = "";
    QString type = "";
    int year, cost;
    bool made;
    bool finished = true;

    out << "Name: " << flush;
    do {
        name = in.readLine();
        in.flush();
    } while (name == "");

    out << "Year made/designed: " << flush;
    do {
        in >> year;
        in.flush();
    } while (false);

    out << "Type of computer: " << flush;
    do {
        type = in.readLine();
        in.flush();
    } while (type == "");

    out << "Was it constructed? " << flush;
    do {
        in >> command;
        in.flush();
        if (command.startsWith('T', Qt::CaseInsensitive) ||
            command.startsWith('Y', Qt::CaseInsensitive)) {
            made = true;
            finished = true;
        } else if (command.startsWith('F', Qt::CaseInsensitive) ||
                   command.startsWith('N', Qt::CaseInsensitive)) {
            made = false;
            finished = true;
        } else {
            finished = false;
        }
    } while (!finished);

    do {
        out << "How much did it cost to make? " << flush;
        in >> cost;
        in.flush();
    } while (false);
    if (handler.add(name, year, type, made, cost)) {
        out << "Computer successfully added" << endl;
    } else {
        out << "Error, addition unsuccessful" << endl;
    }
}

void Interface::addPerson() {
    QString name, gender, nationality;
    int born, died, cost;
    bool finished = true;

    out << "Name: " << flush;
    do {
        name = in.readLine();
        in.flush();
    } while (name == "");

    do {
        out << "Year of birth: " << flush;
        in >> born;
        in.flush();
    } while (false);

    do {
        out << "(if person is alive input a year lower than the year of "
               "birth)\nYear of death: "
            << flush;
        in >> died;
        in.flush();
        if (died < born) {
            died = 0;
        }
    } while (false);

    do {
        out << "(F)emale / (M)ale / (U)nspecified\nGender: " << flush;
        in >> command;
        in.flush();
        if (command.startsWith('F', Qt::CaseInsensitive)) {
            gender = "Female";
            finished = true;
        } else if (command.startsWith('M', Qt::CaseInsensitive)) {
            gender = "Male";
            finished = true;
        } else if (command.startsWith('U', Qt::CaseInsensitive)) {
            gender = "Unspecified";  // Because it doesn't have to be
            finished = true;
        } else {
            finished = false;
        }
    } while (!finished);

    out << "Nationality: " << flush;
    do {
        nationality = in.readLine();
        in.flush();
    } while (nationality == "");

    if (handler.add(name, born, died, gender, nationality)) {
        out << "Person added successfully" << endl;
    } else {
        out << "Error, addition unsuccessful" << endl;
    }
}

void Interface::addRelation() {
    if (handler.computerCount() == 0) {
        out << "No computers in database, no relation to be made :(" << endl;
    } else if (handler.peopleCount() == 0) {
        out << "No people in database, no relation to be made :(" << endl;
    } else {
        int computer_id, person_id;
        QString relationship;

        out << "Computer id: " << flush;
        do {
            in >> computer_id;
            in.flush();
        } while (computer_id >= 0 && computer_id < handler.computerCount());

        out << "Person id: " << flush;
        do {
            in >> person_id;
            in.flush();
        } while (person_id >= 0 && person_id < handler.peopleCount());

        out << "Relationship: " << flush;
        do {
            relationship = in.readLine();
            in.flush();
        } while (relationship == "");

        if (handler.add(computer_id, person_id, relationship)) {
            out << "Relation added successfully" << endl;
        } else {
            out << "Error, addition unsuccessful" << endl;
        }
    }
}
void Interface::list(Table table) {
    command = clear();
    if (table == COMPUTER) {
        QVector<QString> commands = {"name", "year it was made", "type",
                                     "was constructed", "cost"};

        specific_instructions(table,
                              "Select a field to sort by\n"
                              "unmentioned commands result in default sorting\n"
                              "negative numbers result in ascending output\n",
                              commands);
        out << "Enter a number: " << flush;
        in >> command;
        bool ok;
        int cmd = command.toInt(&ok);

        QVector<Computer> computers;

        if (ok) {
            if (cmd > 0) {
                handler.list(computers, cmd, "DESC");
            } else if (cmd < 0) {
                handler.list(computers, -cmd, "ASC");
            }
        } else {
            handler.list(computers, 0, "ASC");
        }
        display(computers);
    } else if (table == PERSON) {
        QVector<QString> commands = {"name", "born", "died", "gender",
                                     "nationality"};

        specific_instructions(table,
                              "Select a field to sort by\n"
                              "unmentioned commands result in default sorting\n"
                              "negative numbers result in ascending output\n",
                              commands);
        out << "Enter a number: " << flush;
        in >> command;
        bool ok;
        int cmd = command.toInt(&ok);

        QVector<Person> people;

        if (ok) {
            if (cmd > 0) {
                handler.list(people, cmd, "DESC");
            } else if (cmd < 0) {
                handler.list(people, -cmd, "ASC");
            }
        } else {
            handler.list(people, 0, "ASC");
        }
        display(people);
    }
    separate();
    out << "Do you want another list? (y/n) " << flush;
    in >> command;
    if (command.startsWith('Y', Qt::CaseInsensitive)) {
        list(table);
    }
}

QString Interface::clear() {
#if defined(WIN32) || defined(_WIN32) || \
    defined(__WIN32) && !defined(__CYGWIN__)
	out << QString(100, '\n') << flush; // gross
    QProcess::execute("CLS"); // doesn't seem to work
#else
    QProcess::execute("clear"); // works fine
#endif

    return "y";
}

void Interface::separate() {
    out << QString(delimiter_length, '-') << endl;
}

void Interface::remove(Table table) {
    QVector<QString> commands = {"bla"};
    QVector<int> to_remove;

    command = clear();
    specific_instructions(
        table, "Select one or more fields to filter your removal\n", commands);
    in >> command;

    // TODO: finish this
    out << "Do you want to remove more entries? (Y/N) " << flush;
    in >> command;
    if (command.startsWith('Y', Qt::CaseInsensitive)) {
        remove(table);
    }
}

void Interface::specific_instructions(Table table,
                                      QString instructions,
                                      QVector<QString> fields) {
    separate();
    out << instructions;
    separate();
    for (int i = 0; i < fields.size(); ++i) {
        out << qSetFieldWidth(0) << i + 1 << qSetFieldWidth(4) << "-> "
            << fields[i] << qSetFieldWidth(0) << endl;
    }
    separate();
}

void Interface::display(QVector<Computer> computers) {
    clear();

    int longest_name = 5;
    int longest_type = 5;
    for (size_t i = 0; i < computers.size(); ++i) {
        if (longest_name < computers[i].name.length()) {
            longest_name = computers[i].name.length();
        }
        if (longest_type < computers[i].type.length()) {
            longest_type = computers[i].type.length();
        }
    }
    out << qSetFieldWidth(longest_name + 4) << left
        << "Name:" << qSetFieldWidth(9) << left
        << "Year:" << qSetFieldWidth(longest_type + 4) << left
        << "Type:" << qSetFieldWidth(9) << left << "Made:" << qSetFieldWidth(0)
        << left << "Cost:" << endl;
    separate();
    for (size_t i = 0; i < computers.size(); ++i) {
        out << qSetFieldWidth(longest_name + 4) << left << computers[i].name
            << qSetFieldWidth(9) << left << computers[i].year
            << qSetFieldWidth(longest_type + 4) << left << computers[i].type
            << qSetFieldWidth(9) << left << (computers[i].made ? "Yes" : "No")
            << qSetFieldWidth(0) << left << computers[i].cost
            << qSetFieldWidth(0) << endl;
    }
}

void Interface::display(QVector<Person> people) {
    clear();

    int longest_name = 5;
    for (size_t i = 0; i < people.size(); ++i) {
        if (longest_name < people[i].name.length()) {
            longest_name = people[i].name.length();
        }
    }
    out << qSetFieldWidth(longest_name + 4) << left
        << "Name:" << qSetFieldWidth(9) << left << "Born:" << qSetFieldWidth(9)
        << left << "Died:" << qSetFieldWidth(11) << left
        << "Gender:" << qSetFieldWidth(0) << left
        << "Nationality:" << qSetFieldWidth(0) << endl;
    separate();
    for (size_t i = 0; i < people.size(); ++i) {
        out << qSetFieldWidth(longest_name + 4) << left << people[i].name
            << qSetFieldWidth(18) << left << people[i].born
            << qSetFieldWidth(18) << left
            << (people[i].died == 0 ? "" : QString(people[i].died))
            << qSetFieldWidth(11) << left
            << (people[i].gender.startsWith('U') ? "" : people[i].gender)
            << qSetFieldWidth(0) << left << people[i].nationality
            << qSetFieldWidth(0) << endl;
    }
}
