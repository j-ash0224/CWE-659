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
    static Singleton* getInstance() { // cwe-543) RTE_Singleton_Without_Synchronization : 멀티쓰레드 환경에서 동기화 없이 싱글톤 패턴이 사용되었음
        if (instance == nullptr) {
            instance = new Singleton();
        }
        return instance;
    }
    void showMessage() {
        cout << "원하시는 메뉴를 입력하세요" << endl;
    }

private:
    Singleton() {}
    static Singleton* instance;
};

Singleton* Singleton::instance = nullptr;

void dumyFunction3() throw (int) // cwe-397) RTE_Throw_Generic_Exception : 함수에서 예외 기술절의 타입이 아닌 예외가 발생할 수 있음
{
    int size = 1000;
    void* data = _alloca(size); // cwe-248) RTE_Uncaught_Exception : 예외를 발생하는 함수 `_alloca'를 try 문에서 사용하지 않음

    throw float();
}

void dumyFunction4(char* argv[]) {
    try { 
        dumyFunction3(); 
    }
    catch (...) {}// cwe-396) RTE_Generic_Exception_Catch : 모든 형식의 예외를 처리하고 있음
                  // cwe-460) RTE_Improper_Cleanup_on_Thrown_Exception : catch문이 비어있으면 안됨                         
}

#define IOCTL_METHOD_NEITHER_CODE CTL_CODE(FILE_DEVICE_UNKNOWN, 0x800, METHOD_NEITHER, FILE_ANY_ACCESS)

void dumyFunction5(HANDLE hDevice, LPVOID lpInBuffer, DWORD nInBufferSize, LPVOID lpOutBuffer, DWORD nOutBufferSize, LPDWORD lpBytesReturned, LPOVERLAPPED lpOverlapped) {

    DeviceIoControl(hDevice, METHOD_NEITHER, lpInBuffer, nInBufferSize, lpOutBuffer, nOutBufferSize, lpBytesReturned, lpOverlapped);
    //RTE_Improper_Address_Validation : 함수 `DeviceIoControl'의 `1'번째 인자가 `3'일때 `2'번째 인자 주소 유효성 검사 필요
    //cwe-781)  METHOD_NEITHER는 커널 모드 드라이버가 유저 모드의 버퍼를 직접 참조하도록 하는 방식 (악의적인 사용자에 의해 임의의 커널 메모리에 접근할 수 있는 취약점 존재)
    //cwe-782) 함수에서 사용하는 IOCTL 코드에 대한 적절한 접근 제어가 없
}

void rule1335(int argc, char** argv) {
    int s = 1 << -2;  // cwe-1335) RTE_Negative_Shift_Amount : 비트 연산자에 음수 사용

    unsigned int x = 1;
    unsigned int result = x << 35; // cwe-1335) RTE_Negative_Shift_Amount : 비트 연산자가 범위 벗어남
}

void cweFunc(int num_elements) {

    if (num_elements > 1000) {
        printf("Too many elements requested.\n");
        return;
    }
    // cwe-839) 최소값 확인 없이 할당
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
    cout << "key1의 값들:" << endl;
    for (auto it = range.first; it != range.second; ++it) { // cwe-462) RTE_Duplicate_Key_in_Associative_List: Associative List에 중복 키 사용 금지 (Alist)
        cout << it->second << endl;
    }
}

int cwe135() {
    // 와이드 문자열 선언
    wchar_t wideString[] = L"Hello, World!";

    // strlen()을 사용하여 잘못된 길이 계산
    size_t length = strlen(reinterpret_cast<const char*>(wideString));

    // 잘못된 길이 값을 출력
    std::wcout << L"잘못된 문자열 길이: " << length << std::endl;

    // 문자열에 대한 작업 (예: 메모리 할당 등)
    wchar_t* buffer = new wchar_t[length + 1];  // 잘못된 길이로 인해 문제가 발생할 수 있음

    // 메모리 해제
    delete[] buffer;

    return 0;
}


int main(char* argv[]) {
    cweFunc(-5);
    size_t size = 1000000 * 1000000;

    // cwe-131: 버퍼 크기의 잘못된 계산 
    // cwe-192: 정수 강제 변환 오류
    // cwe-789) 오버플로우가 발생. 과도한 크기 값을 사용한 메모리 할당
    // cwe-1325: 부적절하게 제어된 순차 메모리 할당 (malloc을 통해 할당하려는 메모리 크기가 매우 커서, 시스템에서 허용되는 메모리 제한을 초과할 수 있음
    int* ptr = (int*)malloc(size * sizeof(int));
    int ap = (int)ptr;

    //cwe-128) 자료형의 최대값보다 크므로 Wrap-around 발생
    //cwe-192) 정수 강제 변환은 기본 데이터 유형의 형 변환, 확장 또는 잘림과 관련된 일련의 결함을 의미
    //cwe-197) 자료형을 더 작은 크기의 자료형으로 캐스팅하면 잘림 오류가 발생
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
    fp32 fp2 = (fp32)fp1;// cwe-242) RTE_Dangerous_Function_Cast : 함수 포인터를 반환 타입이 일치하지 않는 함수 포인터로 변함

    
    if (!b) {
        int a = 10;
        int* p = &a;
        char* second_char = (char*)(p + 1); // cwe-468) RTE_Incorrect_Pointer_Scaling : `int' pointer type의 연산결과가 `char' pointer type으로 변환되었음
    }

    if (a != ap) {
        while (1) {
            singletonInstance->showMessage();
            cout << "1. 회원 정보 관리" << endl;
            cout << "2. 계좌 관리" << endl;
            cout << "0. 종료" << endl;
            cin >> menuchoice;
            string id;
            string password;
            bool login;

            // cwe-478) RTE_Missing_Default_Case_in_Switch_State : Switch문에 Default case문이 없음
            switch (menuchoice) // cwe-484) RTE_Omitted_Break-Statement_in_Switch : case문에서 Break문을 생략하면 안됨
            {
            case 1:
                userControlMenu();
                break;
            case 2:
                cout << "회원 아이디를 입력하세요 : ";
                cin >> id;

                cout << "회원 비밀번호를 입력하세요 : ";
                cin >> password;
                login = userLogin(id, password);
                if (login == true) { accountControlMenu(id); }
                else { cout << "존재하지 않는 회원 정보 입니다\n"; }
                break;
            case 0:
                menuchoice = 'a';
                exit(-1);
            }
        }
    }
    
    return 0;
}
