#define _CRT_SECURE_NO_WARNINGS
#define MAX_ACCOUNT 100

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

using namespace std;

class Account{
public:
    string id;
    string accountNumber;
    string password; // cwe-766) RTE_Public_Field  : 민간함 정보를 담고 있을 수 있는 멤버 변수 `password'가 private으로 선언되지 않음
    int balance = 0;

public:
    void createAccount(string id, string accountNumber, string password) {
        this->id = id;
        this->accountNumber = accountNumber;
        this->password = password;
    }
};

class Bank {
public:
    static int bankPassword; // cwe-500) RTE_Public_Static_Field : public static 필드는 const로 선언해야 함
                        // cwe-493) Critical Public Variable 인 bankPassword는 const로 선언해야 함
    void otherCall(int mem, char* ch);

private:
    int day;
    char* bankName;
    char location[10];
    float bankPrice;

public:
    void newFree() {
        Bank* ptr = new Bank();
        free(ptr); // cwe-762) RTE_Mismatched_Memory_Management : new 로 할당된 오브젝트를 free 로 해제할 수 없음
    }

    char* memoryLeak(char* bankName) {
        Bank* b = new Bank();
        b->otherCall(day, location);    // cwe-374) RTE_Pass_Field_To_Untrusted_Method : 변경할 수 있는 private 필드를 외부 클래스의 메서드 인자로 전달함
                                        //cwe-401) RTE_Memory_Leak : 프로그램 흐름상에 메모리 누수가 발생할 가능성이 있음
        this->bankName = bankName; // cwe-496) RTE_Assign_Public_Data_To_Private_Field : 변경할 수 있는 private 필드에 public 데이터를 할당함    
    }

    char* badretrun() {
        return location;//RTE_Return_Mutable_Private_Field : public 메서드에서 변경할 수 있는 private 필드를 반환 함
        // cwe-375) 신뢰할 수 없는 호출자에게 변경 가능한 개체 반환
        // cwe-495) 배열 형식 데이터 구조 반환
    }

    void changePrice(float newPrice) {
        bankPrice = newPrice; // cwe-767) RTE_Assign_Public_Data_To_Private_Field : 변경할 수 있는 private 필드에 public 데이터를 할당함
        
    }
};

Account accounts[MAX_ACCOUNT];
int currentAccount = 0;
string accountFile = "C:\\Users\\sure\\Desktop\\내 문서\\vs\\bank\\account.txt";
string userId;

void trim2(string& str) {
    str.erase(str.begin(), find_if(str.begin(), str.end(), [](unsigned char ch) {
        return !isspace(ch);
        }));

    str.erase(find_if(str.rbegin(), str.rend(), [](unsigned char ch) {
        return !isspace(ch);
        }).base(), str.end());
}

void readAccountFromFile(const string& filePath) {
    ifstream file = openfile(filePath);

    string line;
    bool firstLine = true; 

    while (getline(file, line)) {
        if (firstLine) {
            firstLine = false;
            continue;
        }
        stringstream lineStream(line);
        string id;
        string accountNumber;
        string password;
        string balance;

        getline(lineStream, id, ',');
        getline(lineStream, accountNumber, ',');
        getline(lineStream, password, ',');
        getline(lineStream, balance, ',');

        trim2(id);
        trim2(accountNumber);
        trim2(password);
        trim2(balance);
        
        Account account;
        account.balance = stoi(balance);
        account.createAccount(id, accountNumber, password);

        accounts[currentAccount] = account;
        currentAccount += 1;
    }

    file.close();
}

void updateAccountFile() {
    ofstream file(accountFile);

    file << "id,accountNumber,password,balance\n";
    for (int i = 0; i < currentAccount; i++) {
        file << accounts[i].id << ","
            << accounts[i].accountNumber << ","
            << accounts[i].password << ","
            << accounts[i].balance << "\n";
    }

    file.close();
}

string generateRandomAccountNumber() {
    srand((unsigned int)time(0));
    string accountNumber;

    while (true) {
        accountNumber = to_string(rand() % 100000000); 

        while (accountNumber.length() < 8) {
            accountNumber = "0" + accountNumber;
        }

        bool duplicate = false;
        for (int i = 0; i < currentAccount; i++) {
            if (accounts[i].accountNumber == accountNumber) {
                duplicate = true;
                break;
            }
        }

        if (!duplicate) {
            break;
        }
    }

    return accountNumber;
}

void createNewAccount() {
    string accountNumber = generateRandomAccountNumber();
    string password;

    cout << "생성된 계좌 번호는 " << accountNumber << " 입니다. 잘 기억해두세요!" << endl;
    cout << "비밀번호를 설정하세요: ";
    cin >> password;
    Account newAccount;
    newAccount.createAccount(userId, accountNumber, password);
    newAccount.balance = 0;

    accounts[currentAccount] = newAccount;
    currentAccount++;

    updateAccountFile();


    cout << "계좌가 성공적으로 생성되었습니다." << endl;
}

int findAccount(string accountNum, string password) {
    for (int i = 0; i < currentAccount; i++) {
        if (accounts[i].accountNumber == accountNum && accounts[i].password == password) {
            return i;
        }
    }
    return -1;
}

void checkBalance() {
    string accountNum, password;
    cout << "계좌 번호: ";
    cin >> accountNum;
    cout << "비밀번호: ";
    cin >> password;

    int accountIndex = findAccount(accountNum, password);
    if (accountIndex != -1) {
        cout << "잔액: " << accounts[accountIndex].balance << "원\n";
    }
    else {
        cout << "계좌 번호 또는 비밀번호가 잘못되었습니다.\n";
    }
}

void deposit() {
    string accountNum, password;
    int amount;
    cout << "계좌 번호: ";
    cin >> accountNum;
    cout << "비밀번호: ";
    cin >> password;

    int accountIndex = findAccount(accountNum, password);
    if (accountIndex != -1) {
        cout << "입금할 금액: ";
        cin >> amount;
        if (0 > 1) {
            printf("%s", amount); // cwe-134) RTE_Format_String : 함수 `printf' 의 `2' 번 째 인자와 맞지 않는 포맷 스트링을 사용함
        }
        accounts[accountIndex].balance += amount;
        updateAccountFile(); 
        cout << "입금이 완료되었습니다. 현재 잔액: " << accounts[accountIndex].balance << "원\n";
    }
    else {
        cout << "계좌 번호 또는 비밀번호가 잘못되었습니다.\n";
    }
}

void withdraw() {
    string accountNum, password;
    int amount;
    cout << "계좌 번호: ";
    cin >> accountNum;
    cout << "비밀번호: ";
    cin >> password;

    int accountIndex = findAccount(accountNum, password);
    if (accountIndex != -1) {
        cout << "출금할 금액: ";
        cin >> amount;
        if (amount <= accounts[accountIndex].balance) {
            accounts[accountIndex].balance -= amount;
            updateAccountFile(); 
            cout << "출금이 완료되었습니다. 현재 잔액: " << accounts[accountIndex].balance << "원\n";
        }
        else {
            cout << "잔액이 부족합니다.\n";
        }
    }
    else {
        cout << "계좌 번호 또는 비밀번호가 잘못되었습니다.\n";
    }
}

void deleteAccount() {
    string accountNum, password;
    cout << "삭제할 계좌 번호: ";
    cin >> accountNum;
    cout << "비밀번호: ";
    cin >> password;

    int accountIndex = findAccount(accountNum, password);
    if (accountIndex != -1) {
        for (int i = accountIndex; i < currentAccount - 1; i++) {
            accounts[i] = accounts[i + 1];
        }
        currentAccount--; 

        updateAccountFile();

        cout << "계좌가 성공적으로 삭제되었습니다.\n";
    }
    else {
        cout << "계좌 번호 또는 비밀번호가 잘못되었습니다.\n";
    }
}

char* dumyFunction2(const char* input) {
    try {
        char arr[1] = { 'a' };
        // 배열은 데이터 구조를 의미하며, 버퍼는 일반적으로 배열로 구현될 수 있지만, 특정 목적(데이터 임시 저장)에 따라 사용되는 메모리 공간을 의미
        //RTE_Buffer_Overrun
        
        // cwe-125: Out-of-bounds Read: 배열의 범위를 지나서 읽을 가능성이 있음 (범위를 벗어난 읽기)
        // cwe--126: Buffer Over-read : 버퍼의 경계를 벗어나 읽을 가능성이 존재 (버퍼 오버리드)
        for (int i = 0; i < 3; i++) {
            printf("%d", arr[i]); // cwe-457) RTE_Uninitialized_Variable : "`array[index]'" 변수가 초기화되지 않고 사용될 수 있음
            arr[i] = 1; // cwe-787: Out-of-bounds Write : 배열의 범위를 지나서 쓸 가능성이 있음 (범위를 벗어난 쓰기)
        }

        int length = strlen(input); 
        char buffer[10];
        if (length < 10) {
            strncpy(buffer, input, length + 5); // cwe-130) Improper Handling of Length Parameter Inconsistency : 길이를 초과하여 복사 (길이 매개변수 불일치의 부적절한 처리)
        }
        
        for (int i = 0; i > -4; i--) {
            printf("%d", arr[-1]); // cwe-127) RTE_Buffer_Underrun : 배열의 시작 주소보다 작은 주소를 참조할 가능성이 있음
            arr[i] = 0; // cwe-124) RTE_Buffer_Underrun : 배열의 시작 주소보다 작은 주소를 참조할 가능성이 있음
        }

        char* name = (char*)malloc(10);
        free(name);
        free(name); // cwe-415) RTE_Double_Free : 이미 해제된 메모리를 중복 해제함
                    // cwe-1341) 성공적인 열기 없이 리소스를 두번 이상 닫으려 시도
        for (int i = 0; i < 10; ++i) {
            name[i] = i; // cwe-416) RTE_Use_After_Free  : 해제한 메모리 오브젝트를 사용함
        }

        return arr; // cwe-562) RTE_Return_Pointer_To_Local : 지역 변수의 포인터를 반환
    }
    catch (exception& e) { 
    }
}

void accountControlMenu(string id) {
    cout << "1. 계좌 생성\n2. 잔액 조회\n3. 입금\n4. 출금\n5. 계좌 삭제" << endl;
    int choice;
    userId = id;

    cin >> choice;
    switch (choice)
    {
    case 1:
        createNewAccount();
        break;
    case 2:
        checkBalance();
        break;
    case 3:
        deposit();
        break;
    case 4:
        withdraw();
        break;
    case 5:
        deleteAccount();
        break;
    }

    //cwe-480) 아래의 481번과 482번 규칙에서 적절하지 않은 연산자를 사용하서 조건 연산을 수행
    // cwe-481) RTE_Assigning_instead_of_Comparing : condition에 직접적인 assign 연산자가 사용되면 안됨
    if (choice = 6 == 0)  // cwe-783) RTE_Operator_Precedence_Logic_Error : 괄호를 사용하여 우선순위를 명확히 해야 함
        *(&choice + 1); // cwe-188) RTE_Numeric_Operation_On_Address_Of_Var : 변수의 주소값에 산술 연산이 사용되었음
        choice == 6; // cwe-482) RTE_Comparing_instead_of_Assigning : 할당문 대신 비교문을 사용함
        //cwe-483) RTE_Incorrect_Block_Delimitation : 잘못된 블록 구분으로 로직에러의 가능성이 있음
   
}