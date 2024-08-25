#include "user.h"
#include "account.h"
#include "usingC.h"
#include <vector>
#include <cstring> 
#include <cwchar>  
#include <utility> 
#include <algorithm> 
#include <unordered_map>
#include <windows.h>
#include <map>

class Singleton {
public:
    static Singleton* getInstance() { // cwe-543) RTE_Singleton_Without_Synchronization : ��Ƽ������ ȯ�濡�� ����ȭ ���� �̱��� ������ ���Ǿ���
        if (instance == nullptr) {
            instance = new Singleton();
        }
        return instance;
    }
    void showMessage() {
        cout << "���Ͻô� �޴��� �Է��ϼ���" << endl;
    }

private:
    Singleton() {}
    static Singleton* instance;
};

Singleton* Singleton::instance = nullptr;

void dumyFunction3() throw (int) // cwe-397) RTE_Throw_Generic_Exception : �Լ����� ���� ������� Ÿ���� �ƴ� ���ܰ� �߻��� �� ����
{
    int size = 1000;
    void* data = _alloca(size); // cwe-248) RTE_Uncaught_Exception : ���ܸ� �߻��ϴ� �Լ� `_alloca'�� try ������ ������� ����

    throw float();
}

void dumyFunction4(char* argv[]) {
    try { 
        dumyFunction3(); 
    }
    catch (...) {}// cwe-396) RTE_Generic_Exception_Catch : ��� ������ ���ܸ� ó���ϰ� ����
                  // cwe-460) RTE_Improper_Cleanup_on_Thrown_Exception : catch���� ��������� �ȵ�                         
}

#define IOCTL_METHOD_NEITHER_CODE CTL_CODE(FILE_DEVICE_UNKNOWN, 0x800, METHOD_NEITHER, FILE_ANY_ACCESS)

void dumyFunction5(HANDLE hDevice, LPVOID lpInBuffer, DWORD nInBufferSize, LPVOID lpOutBuffer, DWORD nOutBufferSize, LPDWORD lpBytesReturned, LPOVERLAPPED lpOverlapped) {

    DeviceIoControl(hDevice, METHOD_NEITHER, lpInBuffer, nInBufferSize, lpOutBuffer, nOutBufferSize, lpBytesReturned, lpOverlapped);
    //RTE_Improper_Address_Validation : �Լ� `DeviceIoControl'�� `1'��° ���ڰ� `3'�϶� `2'��° ���� �ּ� ��ȿ�� �˻� �ʿ�
    //cwe-781)  METHOD_NEITHER�� Ŀ�� ��� ����̹��� ���� ����� ���۸� ���� �����ϵ��� �ϴ� ��� (�������� ����ڿ� ���� ������ Ŀ�� �޸𸮿� ������ �� �ִ� ����� ����)
    //cwe-782) �Լ����� ����ϴ� IOCTL �ڵ忡 ���� ������ ���� ��� ��
}

void rule1335(int argc, char** argv) {
    int s = 1 << -2;  // cwe-1335) RTE_Negative_Shift_Amount : ��Ʈ �����ڿ� ���� ���

    unsigned int x = 1;
    unsigned int result = x << 35; // cwe-1335) RTE_Negative_Shift_Amount : ��Ʈ �����ڰ� ���� ���
}

void cweFunc(int num_elements) {

    if (num_elements > 1000) {
        printf("Too many elements requested.\n");
        return;
    }
    // cwe-839) �ּҰ� Ȯ�� ���� �Ҵ�
    int* arr = (int*)malloc(num_elements * sizeof(int));
}

void printMap(const multimap<string, int>& myMap) {
    for (const auto& pair : myMap) {
        cout << pair.first << ": " << pair.second << endl;
    }
}

void alist() {
    multimap<string, int> myMap;
    myMap.insert({ "key1", 100 });
    myMap.insert({ "key1", 1 });
    myMap.emplace("key1", 1);

    printMap(myMap);

    auto range = myMap.equal_range("key1");
    cout << "key1�� ����:" << endl;
    for (auto it = range.first; it != range.second; ++it) { // cwe-462) RTE_Duplicate_Key_in_Associative_List: Associative List�� �ߺ� Ű ��� ���� (Alist)
        cout << it->second << endl;
    }
}

int cwe135() {
    // ���̵� ���ڿ� ����
    wchar_t wideString[] = L"Hello, World!";

    // strlen()�� ����Ͽ� �߸��� ���� ���
    size_t length = strlen(reinterpret_cast<const char*>(wideString));

    // �߸��� ���� ���� ���
    std::wcout << L"�߸��� ���ڿ� ����: " << length << std::endl;

    // ���ڿ��� ���� �۾� (��: �޸� �Ҵ� ��)
    wchar_t* buffer = new wchar_t[length + 1];  // �߸��� ���̷� ���� ������ �߻��� �� ����

    // �޸� ����
    delete[] buffer;

    return 0;
}


int main(char* argv[]) {
    cweFunc(-5);
    size_t size = 1000000 * 1000000;

    // cwe-131: ���� ũ���� �߸��� ��� 
    // cwe-192: ���� ���� ��ȯ ����
    // cwe-789) �����÷ο찡 �߻�. ������ ũ�� ���� ����� �޸� �Ҵ�
    // cwe-1325: �������ϰ� ����� ���� �޸� �Ҵ� (malloc�� ���� �Ҵ��Ϸ��� �޸� ũ�Ⱑ �ſ� Ŀ��, �ý��ۿ��� ���Ǵ� �޸� ������ �ʰ��� �� ����
    int* ptr = (int*)malloc(size * sizeof(int));
    int ap = (int)ptr;

    //cwe-128) �ڷ����� �ִ밪���� ũ�Ƿ� Wrap-around �߻�
    //cwe-192) ���� ���� ��ȯ�� �⺻ ������ ������ �� ��ȯ, Ȯ�� �Ǵ� �߸��� ���õ� �Ϸ��� ������ �ǹ�
    //cwe-197) �ڷ����� �� ���� ũ���� �ڷ������� ĳ�����ϸ� �߸� ������ �߻�
    int a = 2147483647;
    short b;
    b = a;

    readUserFromFile(userFile);
    readAccountFromFile(accountFile);
    int menuchoice;
    Singleton* singletonInstance = Singleton::getInstance();

    typedef int16_t(*fp16) (int16_t n);
    typedef void (*fp32) (int32_t n);
    fp16 fp1 = NULL;
    fp32 fp2 = (fp32)fp1;// cwe-242) RTE_Dangerous_Function_Cast : �Լ� �����͸� ��ȯ Ÿ���� ��ġ���� �ʴ� �Լ� �����ͷ� ����

    
    if (!b) {
        int a = 10;
        int* p = &a;
        char* second_char = (char*)(p + 1); // cwe-468) RTE_Incorrect_Pointer_Scaling : `int' pointer type�� �������� `char' pointer type���� ��ȯ�Ǿ���
    }

    if (a != ap) {
        while (1) {
            singletonInstance->showMessage();
            cout << "1. ȸ�� ���� ����" << endl;
            cout << "2. ���� ����" << endl;
            cout << "0. ����" << endl;
            cin >> menuchoice;
            string id;
            string password;
            bool login;

            // cwe-478) RTE_Missing_Default_Case_in_Switch_State : Switch���� Default case���� ����
            switch (menuchoice) // cwe-484) RTE_Omitted_Break-Statement_in_Switch : case������ Break���� �����ϸ� �ȵ�
            {
            case 1:
                userControlMenu();
                break;
            case 2:
                cout << "ȸ�� ���̵� �Է��ϼ��� : ";
                cin >> id;

                cout << "ȸ�� ��й�ȣ�� �Է��ϼ��� : ";
                cin >> password;
                login = userLogin(id, password);
                if (login == true) { accountControlMenu(id); }
                else { cout << "�������� �ʴ� ȸ�� ���� �Դϴ�\n"; }
                break;
            case 0:
                menuchoice = 'a';
                exit(-1);
            }
        }
    }
    
    return 0;
}
