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
    string password; // cwe-766) RTE_Public_Field  : �ΰ��� ������ ��� ���� �� �ִ� ��� ���� `password'�� private���� ������� ����
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
    static int bankPassword; // cwe-500) RTE_Public_Static_Field : public static �ʵ�� const�� �����ؾ� ��
                        // cwe-493) Critical Public Variable �� bankPassword�� const�� �����ؾ� ��
    void otherCall(int mem, char* ch);

private:
    int day;
    char* bankName;
    char location[10];
    float bankPrice;

public:
    void newFree() {
        Bank* ptr = new Bank();
        free(ptr); // cwe-762) RTE_Mismatched_Memory_Management : new �� �Ҵ�� ������Ʈ�� free �� ������ �� ����
    }

    char* memoryLeak(char* bankName) {
        Bank* b = new Bank();
        b->otherCall(day, location);    // cwe-374) RTE_Pass_Field_To_Untrusted_Method : ������ �� �ִ� private �ʵ带 �ܺ� Ŭ������ �޼��� ���ڷ� ������
                                        //cwe-401) RTE_Memory_Leak : ���α׷� �帧�� �޸� ������ �߻��� ���ɼ��� ����
        this->bankName = bankName; // cwe-496) RTE_Assign_Public_Data_To_Private_Field : ������ �� �ִ� private �ʵ忡 public �����͸� �Ҵ���    
    }

    char* badretrun() {
        return location;//RTE_Return_Mutable_Private_Field : public �޼��忡�� ������ �� �ִ� private �ʵ带 ��ȯ ��
        // cwe-375) �ŷ��� �� ���� ȣ���ڿ��� ���� ������ ��ü ��ȯ
        // cwe-495) �迭 ���� ������ ���� ��ȯ
    }

    void changePrice(float newPrice) {
        bankPrice = newPrice; // cwe-767) RTE_Assign_Public_Data_To_Private_Field : ������ �� �ִ� private �ʵ忡 public �����͸� �Ҵ���
        
    }
};

Account accounts[MAX_ACCOUNT];
int currentAccount = 0;
string accountFile = "C:\\Users\\sure\\Desktop\\�� ����\\vs\\bank\\account.txt";
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

    cout << "������ ���� ��ȣ�� " << accountNumber << " �Դϴ�. �� ����صμ���!" << endl;
    cout << "��й�ȣ�� �����ϼ���: ";
    cin >> password;
    Account newAccount;
    newAccount.createAccount(userId, accountNumber, password);
    newAccount.balance = 0;

    accounts[currentAccount] = newAccount;
    currentAccount++;

    updateAccountFile();


    cout << "���°� ���������� �����Ǿ����ϴ�." << endl;
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
    cout << "���� ��ȣ: ";
    cin >> accountNum;
    cout << "��й�ȣ: ";
    cin >> password;

    int accountIndex = findAccount(accountNum, password);
    if (accountIndex != -1) {
        cout << "�ܾ�: " << accounts[accountIndex].balance << "��\n";
    }
    else {
        cout << "���� ��ȣ �Ǵ� ��й�ȣ�� �߸��Ǿ����ϴ�.\n";
    }
}

void deposit() {
    string accountNum, password;
    int amount;
    cout << "���� ��ȣ: ";
    cin >> accountNum;
    cout << "��й�ȣ: ";
    cin >> password;

    int accountIndex = findAccount(accountNum, password);
    if (accountIndex != -1) {
        cout << "�Ա��� �ݾ�: ";
        cin >> amount;
        if (0 > 1) {
            printf("%s", amount); // cwe-134) RTE_Format_String : �Լ� `printf' �� `2' �� ° ���ڿ� ���� �ʴ� ���� ��Ʈ���� �����
        }
        accounts[accountIndex].balance += amount;
        updateAccountFile(); 
        cout << "�Ա��� �Ϸ�Ǿ����ϴ�. ���� �ܾ�: " << accounts[accountIndex].balance << "��\n";
    }
    else {
        cout << "���� ��ȣ �Ǵ� ��й�ȣ�� �߸��Ǿ����ϴ�.\n";
    }
}

void withdraw() {
    string accountNum, password;
    int amount;
    cout << "���� ��ȣ: ";
    cin >> accountNum;
    cout << "��й�ȣ: ";
    cin >> password;

    int accountIndex = findAccount(accountNum, password);
    if (accountIndex != -1) {
        cout << "����� �ݾ�: ";
        cin >> amount;
        if (amount <= accounts[accountIndex].balance) {
            accounts[accountIndex].balance -= amount;
            updateAccountFile(); 
            cout << "����� �Ϸ�Ǿ����ϴ�. ���� �ܾ�: " << accounts[accountIndex].balance << "��\n";
        }
        else {
            cout << "�ܾ��� �����մϴ�.\n";
        }
    }
    else {
        cout << "���� ��ȣ �Ǵ� ��й�ȣ�� �߸��Ǿ����ϴ�.\n";
    }
}

void deleteAccount() {
    string accountNum, password;
    cout << "������ ���� ��ȣ: ";
    cin >> accountNum;
    cout << "��й�ȣ: ";
    cin >> password;

    int accountIndex = findAccount(accountNum, password);
    if (accountIndex != -1) {
        for (int i = accountIndex; i < currentAccount - 1; i++) {
            accounts[i] = accounts[i + 1];
        }
        currentAccount--; 

        updateAccountFile();

        cout << "���°� ���������� �����Ǿ����ϴ�.\n";
    }
    else {
        cout << "���� ��ȣ �Ǵ� ��й�ȣ�� �߸��Ǿ����ϴ�.\n";
    }
}

char* dumyFunction2(const char* input) {
    try {
        char arr[1] = { 'a' };
        // �迭�� ������ ������ �ǹ��ϸ�, ���۴� �Ϲ������� �迭�� ������ �� ������, Ư�� ����(������ �ӽ� ����)�� ���� ���Ǵ� �޸� ������ �ǹ�
        //RTE_Buffer_Overrun
        
        // cwe-125: Out-of-bounds Read: �迭�� ������ ������ ���� ���ɼ��� ���� (������ ��� �б�)
        // cwe--126: Buffer Over-read : ������ ��踦 ��� ���� ���ɼ��� ���� (���� ��������)
        for (int i = 0; i < 3; i++) {
            printf("%d", arr[i]); // cwe-457) RTE_Uninitialized_Variable : "`array[index]'" ������ �ʱ�ȭ���� �ʰ� ���� �� ����
            arr[i] = 1; // cwe-787: Out-of-bounds Write : �迭�� ������ ������ �� ���ɼ��� ���� (������ ��� ����)
        }

        int length = strlen(input); 
        char buffer[10];
        if (length < 10) {
            strncpy(buffer, input, length + 5); // cwe-130) Improper Handling of Length Parameter Inconsistency : ���̸� �ʰ��Ͽ� ���� (���� �Ű����� ����ġ�� �������� ó��)
        }
        
        for (int i = 0; i > -4; i--) {
            printf("%d", arr[-1]); // cwe-127) RTE_Buffer_Underrun : �迭�� ���� �ּҺ��� ���� �ּҸ� ������ ���ɼ��� ����
            arr[i] = 0; // cwe-124) RTE_Buffer_Underrun : �迭�� ���� �ּҺ��� ���� �ּҸ� ������ ���ɼ��� ����
        }

        char* name = (char*)malloc(10);
        free(name);
        free(name); // cwe-415) RTE_Double_Free : �̹� ������ �޸𸮸� �ߺ� ������
                    // cwe-1341) �������� ���� ���� ���ҽ��� �ι� �̻� ������ �õ�
        for (int i = 0; i < 10; ++i) {
            name[i] = i; // cwe-416) RTE_Use_After_Free  : ������ �޸� ������Ʈ�� �����
        }

        return arr; // cwe-562) RTE_Return_Pointer_To_Local : ���� ������ �����͸� ��ȯ
    }
    catch (exception& e) { 
    }
}

void accountControlMenu(string id) {
    cout << "1. ���� ����\n2. �ܾ� ��ȸ\n3. �Ա�\n4. ���\n5. ���� ����" << endl;
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

    //cwe-480) �Ʒ��� 481���� 482�� ��Ģ���� �������� ���� �����ڸ� ����ϼ� ���� ������ ����
    // cwe-481) RTE_Assigning_instead_of_Comparing : condition�� �������� assign �����ڰ� ���Ǹ� �ȵ�
    if (choice = 6 == 0)  // cwe-783) RTE_Operator_Precedence_Logic_Error : ��ȣ�� ����Ͽ� �켱������ ��Ȯ�� �ؾ� ��
        *(&choice + 1); // cwe-188) RTE_Numeric_Operation_On_Address_Of_Var : ������ �ּҰ��� ��� ������ ���Ǿ���
        choice == 6; // cwe-482) RTE_Comparing_instead_of_Assigning : �Ҵ繮 ��� �񱳹��� �����
        //cwe-483) RTE_Incorrect_Block_Delimitation : �߸��� ��� �������� ���������� ���ɼ��� ����
   
}