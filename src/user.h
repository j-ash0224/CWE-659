#define _CRT_SECURE_NO_WARNINGS
#define MAX_USER 100

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

using namespace std;

class User {
public:
    string name;
    string password; 
    string phonenumber;
    string id;

public:
    void createUser(string name, string phonenumber, string id, string password) {
        this->name = name;
        this->phonenumber = phonenumber;
        this->id = id;
        this->password = password;
    }
};

class Clonable {
    virtual Clonable* clone() const = 0;
};

class Base : public Clonable {
public:
    Base* clone() const
    {
        return new Base(*this); // cwe-498) RTE_Cloneable_Class : Ŭ������ �����Ͽ� ��ȯ�ϴ� �Լ��� virtual �� �ƴ�
    }
};

typedef struct {
    int data;
} dummyStruct;

void process_data(dummyStruct* ptr) {
    printf("Data: %d\n", ptr->data); // cwe-476) RTE_Null_Pointer_Dereference : Null �� ����ų �� �ִ� ������ ������ ����
}

int nullFunction() {
    dummyStruct* ptr = NULL;
    process_data(ptr);
    return 0;
}

User users[MAX_USER];
int currentUser = 0;
string userFile = "C:\\Users\\sure\\Desktop\\�� ����\\vs\\bank\\user.txt";

ifstream openfile(const string& filePath) {
    ifstream file(filePath);
    if (!file.is_open()) {
        cerr << "������ �� �� �����ϴ�: " << filePath << endl;
        exit(-1);
    }
    return file;
}

void trim(string& str) {
    str.erase(str.begin(), find_if(str.begin(), str.end(), [](unsigned char ch) {
        return !isspace(ch);
        }));

    str.erase(find_if(str.rbegin(), str.rend(), [](unsigned char ch) {
        return !isspace(ch);
        }).base(), str.end());
}

void readUserFromFile(const string& filePath) {
    ifstream file = openfile(filePath);

    string line;
    bool firstLine = true; 

    while (getline(file, line)) {
        if (firstLine) {
            firstLine = false; 
            continue;
        }
        stringstream lineStream(line);
        string name;
        string phonenumber;
        string id;
        string password;

        getline(lineStream, name, ',');
        getline(lineStream, phonenumber, ',');
        getline(lineStream, id, ',');
        getline(lineStream, password, ',');

        trim(name);
        trim(phonenumber);
        trim(id);
        trim(password);

        User user;
        user.createUser(name, phonenumber, id, password);

        users[currentUser] = user;
        currentUser += 1;

    }

    file.close();
}

void updateUserFile() {
    ofstream file(userFile);

    file << "name,phonenumber,id,password\n";
    for (int i = 0; i < currentUser; i++) {
        file << users[i].name << ","
            << users[i].phonenumber << ","
            << users[i].id << ","
            << users[i].password << "\n";
    }

    file.close();
}

bool isIdDuplicate(string id) {
    for (int i = 0; i < currentUser; ++i) {
        if (users[i].id == id) {
            return true;
        }
    }
    return false;
}

void newUser() {
    if (currentUser < MAX_USER) {
        string name;
        string password;
        string phonenumber;
        string id;

        cout << "�̸�: ";
        cin >> name;
        cout << "��ȭ��ȣ: ";
        cin >> phonenumber;
        cout << "ID: ";
        cin >> id;
        cout << "Password: ";
        cin >> password;

        if (isIdDuplicate(id)) {
            cout << "���̵� �ߺ�. ��� �Ұ����� ���̵�\n";
        }
        else {
            User user;
            user.createUser(name, phonenumber, id, password);
            users[currentUser] = user; 
            currentUser += 1;
            updateUserFile();

            cout << name << " ���� ȸ������ ��� �Ǿ����ϴ�.\n";
        }
    }
    else {
        cerr << "����� �迭�� ũ�⸦ �ʰ��߽��ϴ�." << endl;
    }
}

void deleteUserById(const string id) {
    int i;
    for (i = 0; i < currentUser; ++i) {
        if (users[i].id == id) {
            break;
        }
    }

    for (int j = i; j < currentUser - 1; ++j) {
        users[j] = users[j + 1];
    }

    --currentUser;

    updateUserFile(); 

    cout << "ID " << id << "�� ���������� �����Ǿ����ϴ�." << endl;
}

void updateUserById(const string id) {
    string newName, newPassword, newPhoneNumber;

    cout << "���ο� �̸��� �Է��ϼ��� :  ";
    cin >> newName;
    cout << "���ο� ��ȭ��ȣ�� �Է��ϼ��� : ";
    cin >> newPhoneNumber;
    cout << "���ο� Password�� �Է��ϼ��� : ";
    cin >> newPassword;

    bool found = false;

    for (int i = 0; i < currentUser; ++i) {
        if (users[i].id == id) {
            found = true;
            if (!newName.empty()) {
                users[i].name = newName;
            }
            if (!newPhoneNumber.empty()) {
                users[i].phonenumber = newPhoneNumber;
            }
            if (!newPassword.empty()) {
                users[i].password = newPassword;
            }
            break;
        }
    }

    if (found) {
        updateUserFile(); 
        cout << "ID " << id << "�� ������ ���������� ������Ʈ�Ǿ����ϴ�." << endl;
    }
    else {
        cout << "ID " << id << "�� ���Ͽ��� �߰ߵ��� �ʾҽ��ϴ�." << endl;
    }
}

bool userLogin(string id, string password) {
    for (int i = 0; i < currentUser; ++i) {
        if (users[i].id == id && users[i].password == password) { // �� ����) RTE_Buffer_Overrun : �迭�� �ִ� �������� ū �ּҸ� ������ ���ɼ��� ����
            return true;
        }
    }
    return false;
}

// cwe-704) RTE_Incorrect_Numeric_Casting : �߸��� ���� ��ȯ �Ǵ� ĳ��Ʈ
unsigned int rate1() {
    int userRate;
    userRate = 2147483647;
    return userRate; // CWE-195) : ��ȣ�� �ִ� ���� Ÿ���� ��ȣ ���� ���� Ÿ�� ������ ������ ĳ����
}

int rate2() {
    unsigned int userRate;
    userRate = 1;
    return userRate; // CWE-196) : ��ȣ�� ���� ���� Ÿ���� ��ȣ �ִ� ���� Ÿ�� ������ ������ ĳ����
}

unsigned int dummyFunction1(int* array, int len, int index) {
    int value;
    if (index < len) {
        value = array[index]; // cwe-839 & cwe-129) RTE_Improper_Validation_of_Array_Index : �迭 �ε����� �������� ���� (�ּҰ� Ȯ�� ���� ���� ��)
                            //cwe-119) ���� index�� �������, �� �ε����� �ǵ����� ���� �޸� ��ġ�� ������ �� ������, �̴� �޸� ���� ��� �������� �������� ������ �߱��� �� ����
    }
    return 0;
}

void userControlMenu() {
    cout << "1. ȸ�� ���� ���\n2. ȸ�� ���� ����\n3. ȸ�� ���� ����" << endl;
    int choice;
    string id;
    cin >> choice;
    switch (choice)
    {
    case 1:
        newUser();
        break;
    case 2:
        cout << "������ ���̵� �Է�: ";
        cin >> id;
        deleteUserById(id);
        break;
    case 3:
        cout << "������ ���̵� �Է�: ";
        cin >> id;
        updateUserById(id);
        break;
    default:
        cout << "�߸��� �����Դϴ�.\n";
        break;
    }
}
