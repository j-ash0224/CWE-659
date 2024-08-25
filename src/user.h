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
        return new Base(*this); // cwe-498) RTE_Cloneable_Class : 클래스를 복사하여 반환하는 함수가 virtual 이 아님
    }
};

typedef struct {
    int data;
} dummyStruct;

void process_data(dummyStruct* ptr) {
    printf("Data: %d\n", ptr->data); // cwe-476) RTE_Null_Pointer_Dereference : Null 을 가리킬 수 있는 포인터 변수가 사용됨
}

int nullFunction() {
    dummyStruct* ptr = NULL;
    process_data(ptr);
    return 0;
}

User users[MAX_USER];
int currentUser = 0;
string userFile = "C:\\Users\\sure\\Desktop\\내 문서\\vs\\bank\\user.txt";

ifstream openfile(const string& filePath) {
    ifstream file(filePath);
    if (!file.is_open()) {
        cerr << "파일을 열 수 없습니다: " << filePath << endl;
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

        cout << "이름: ";
        cin >> name;
        cout << "전화번호: ";
        cin >> phonenumber;
        cout << "ID: ";
        cin >> id;
        cout << "Password: ";
        cin >> password;

        if (isIdDuplicate(id)) {
            cout << "아이디 중복. 사용 불가능한 아이디\n";
        }
        else {
            User user;
            user.createUser(name, phonenumber, id, password);
            users[currentUser] = user; 
            currentUser += 1;
            updateUserFile();

            cout << name << " 님이 회원으로 등록 되었습니다.\n";
        }
    }
    else {
        cerr << "사용자 배열의 크기를 초과했습니다." << endl;
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

    cout << "ID " << id << "가 성공적으로 삭제되었습니다." << endl;
}

void updateUserById(const string id) {
    string newName, newPassword, newPhoneNumber;

    cout << "새로운 이름을 입력하세요 :  ";
    cin >> newName;
    cout << "새로운 전화번호를 입력하세요 : ";
    cin >> newPhoneNumber;
    cout << "새로운 Password를 입력하세요 : ";
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
        cout << "ID " << id << "의 정보가 성공적으로 업데이트되었습니다." << endl;
    }
    else {
        cout << "ID " << id << "가 파일에서 발견되지 않았습니다." << endl;
    }
}

bool userLogin(string id, string password) {
    for (int i = 0; i < currentUser; ++i) {
        if (users[i].id == id && users[i].password == password) { // 넘 많음) RTE_Buffer_Overrun : 배열의 최대 범위보다 큰 주소를 참조할 가능성이 있음
            return true;
        }
    }
    return false;
}

// cwe-704) RTE_Incorrect_Numeric_Casting : 잘못된 유형 변환 또는 캐스트
unsigned int rate1() {
    int userRate;
    userRate = 2147483647;
    return userRate; // CWE-195) : 부호가 있는 숫자 타입을 부호 없는 숫자 타입 사이의 변수로 캐스팅
}

int rate2() {
    unsigned int userRate;
    userRate = 1;
    return userRate; // CWE-196) : 부호가 없는 숫자 타입을 부호 있는 숫자 타입 사이의 변수로 캐스팅
}

unsigned int dummyFunction1(int* array, int len, int index) {
    int value;
    if (index < len) {
        value = array[index]; // cwe-839 & cwe-129) RTE_Improper_Validation_of_Array_Index : 배열 인덱스의 부적절한 검증 (최소값 확인 없이 범위 비교)
                            //cwe-119) 만약 index가 음수라면, 이 인덱스는 의도하지 않은 메모리 위치를 참조할 수 있으며, 이는 메모리 버퍼 경계 내에서의 부적절한 제한을 야기할 수 있음
    }
    return 0;
}

void userControlMenu() {
    cout << "1. 회원 정보 등록\n2. 회원 정보 삭제\n3. 회원 정보 수정" << endl;
    int choice;
    string id;
    cin >> choice;
    switch (choice)
    {
    case 1:
        newUser();
        break;
    case 2:
        cout << "삭제할 아이디 입력: ";
        cin >> id;
        deleteUserById(id);
        break;
    case 3:
        cout << "수정할 아이디 입력: ";
        cin >> id;
        updateUserById(id);
        break;
    default:
        cout << "잘못된 선택입니다.\n";
        break;
    }
}
