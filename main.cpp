#include <iostream>
#include <mysql_driver.h>
#include <mysql_connection.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <stdexcept>

using namespace std;

class StudentManagementSystem {
private:
    sql::mysql::MySQL_Driver *driver;
    sql::Connection *con;

public:
    StudentManagementSystem(const string &host, const string &user, const string &password, const string &database) {
        driver = sql::mysql::get_mysql_driver_instance();
        con = driver->connect(host, user, password);
        con->setSchema(database);
    }

    ~StudentManagementSystem() {
        delete con;
    }

    void addStudent(const string &name, int age, const string &gender, const string &course, const string &email) {
        try {
            sql::PreparedStatement *prep_stmt = con->prepareStatement(
                "INSERT INTO students (name, age, gender, course, email) VALUES (?, ?, ?, ?, ?)");
            prep_stmt->setString(1, name);
            prep_stmt->setInt(2, age);
            prep_stmt->setString(3, gender);
            prep_stmt->setString(4, course);
            prep_stmt->setString(5, email);
            prep_stmt->executeUpdate();
            delete prep_stmt;
            cout << "Student added successfully!" << endl;
        } catch (sql::SQLException &e) {
            cerr << "Error: " << e.what() << endl;
        }
    }

    void viewStudents() {
        try {
            sql::Statement *stmt = con->createStatement();
            sql::ResultSet *res = stmt->executeQuery("SELECT * FROM students");

            cout << "ID | Name       | Age | Gender | Course            | Email" << endl;
            cout << "--------------------------------------------------------------" << endl;

            while (res->next()) {
                cout << res->getInt("id") << " | "
                     << res->getString("name") << " | "
                     << res->getInt("age") << " | "
                     << res->getString("gender") << " | "
                     << res->getString("course") << " | "
                     << res->getString("email") << endl;
            }

            delete res;
            delete stmt;
        } catch (sql::SQLException &e) {
            cerr << "Error: " << e.what() << endl;
        }
    }

    void updateStudent(int id, const string &name, int age, const string &gender, const string &course, const string &email) {
        try {
            sql::PreparedStatement *prep_stmt = con->prepareStatement(
                "UPDATE students SET name = ?, age = ?, gender = ?, course = ?, email = ? WHERE id = ?");
            prep_stmt->setString(1, name);
            prep_stmt->setInt(2, age);
            prep_stmt->setString(3, gender);
            prep_stmt->setString(4, course);
            prep_stmt->setString(5, email);
            prep_stmt->setInt(6, id);
            prep_stmt->executeUpdate();
            delete prep_stmt;
            cout << "Student updated successfully!" << endl;
        } catch (sql::SQLException &e) {
            cerr << "Error: " << e.what() << endl;
        }
    }

    void deleteStudent(int id) {
        try {
            sql::PreparedStatement *prep_stmt = con->prepareStatement("DELETE FROM students WHERE id = ?");
            prep_stmt->setInt(1, id);
            prep_stmt->executeUpdate();
            delete prep_stmt;
            cout << "Student deleted successfully!" << endl;
        } catch (sql::SQLException &e) {
            cerr << "Error: " << e.what() << endl;
        }
    }
};

int main() {
    StudentManagementSystem sms("tcp://127.0.0.1:3306", "root", "password", "student_management");

    int choice;
    while (true) {
        cout << "\nStudent Management System\n";
        cout << "1. Add Student\n";
        cout << "2. View Students\n";
        cout << "3. Update Student\n";
        cout << "4. Delete Student\n";
        cout << "5. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1: {
                string name, gender, course, email;
                int age;

                cout << "Enter student name: ";
                cin.ignore();
                getline(cin, name);

                cout << "Enter age: ";
                cin >> age;

                cout << "Enter gender (Male/Female): ";
                cin.ignore();
                getline(cin, gender);

                cout << "Enter course: ";
                getline(cin, course);

                cout << "Enter email: ";
                getline(cin, email);

                sms.addStudent(name, age, gender, course, email);
                break;
            }
            case 2:
                sms.viewStudents();
                break;
            case 3: {
                int id;
                string name, gender, course, email;
                int age;

                cout << "Enter student ID to update: ";
                cin >> id;

                cout << "Enter new name: ";
                cin.ignore();
                getline(cin, name);

                cout << "Enter new age: ";
                cin >> age;

                cout << "Enter new gender: ";
                cin.ignore();
                getline(cin, gender);

                cout << "Enter new course: ";
                getline(cin, course);

                cout << "Enter new email: ";
                getline(cin, email);

                sms.updateStudent(id, name, age, gender, course, email);
                break;
            }
            case 4: {
                int id;
                cout << "Enter student ID to delete: ";
                cin >> id;
                sms.deleteStudent(id);
                break;
            }
            case 5:
                cout << "Exiting program." << endl;
                return 0;
            default:
                cout << "Invalid choice. Please try again." << endl;
        }
    }

    return 0;
}
