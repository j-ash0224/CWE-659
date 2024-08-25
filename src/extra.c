#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <limits.h>
#include <string.h>
#include <windows.h>


// 주석 풀면 cpp 파일이 캡쳐되지 않음
// cpp 파일에 넣으면 에러 발생

 //참조 카운트 구조체 정의
typedef struct {
    int value;
    int ref_count;
} RefCountedObject;

RefCountedObject* create_object(int value) {
    RefCountedObject* obj = (RefCountedObject*)malloc(sizeof(RefCountedObject));
    if (obj == NULL) {
        perror("Failed to allocate memory");
        exit(EXIT_FAILURE);
    }
    obj->value = value;
    return obj;
}

int refcountFunc() {
    int a;
    RefCountedObject* obj = create_object(a); //cwe-911) RTE_Uninitialized_Variable : 참조 카운트를 초기화 하지 않음
}

void RTE_Buffer_Overrun() {
	char* buffer = (char*)malloc(10 * sizeof(char)); // cwe-122) Heap-based Buffer Overflow : 작은 크기의 힙 메모리 할당
	strcpy(buffer, "This string is too long for the buffer"); // 버퍼 크기를 초과하는 문자열 복사 (힙 기반 버퍼 오버플로)
	free(buffer);

	char src[10] = "Hello";
	char dst[5];
	memcpy(dst, src, 10); // cwe-805) Buffer Access with Incorrect Length Value : 잘못된 길이 값을 사용한 버퍼 액세스

	char src2[10] = "HelloWorld";
	char dst2[5];
	memcpy(dst2, src2, sizeof(src2)); // cwe-806) Buffer Access Using Size of Source Buffer : 목적지 버퍼 크기보다 큰 소스 버퍼 크기를 사용하여 접근
}

struct MessageBuffer
{
	int msgType;
	union {
		char* name;
		int nameID;
	};
};
void cfun1() {
	struct MessageBuffer buf;
	char* defaultMessage = "Hello World";
	buf.msgType = 1;
	buf.name = defaultMessage;
	buf.nameID = (int)(defaultMessage + 1); // cwe-843) RTE_Access_Using_Incompatible_Type : union 타입을 사용하면 안됨
}

void cfun2(char* filename, char* localfile, char* network, int len, int buf) {
	chroot("/var/ftproot"); // cwe-243) RTE_Creation_chroot_Without_Changing : 함수 chroot' 사용 전 함수 chdir'를 사용했는지 검사
	fgets(filename, sizeof(filename), network); //cwe-467) RTE_Use_Of_sizeof_On_Pointer : sizeof()의 인자로 포인터 변수 filename'이(가) 사용되면 안됨
}

void cfunc3(char pathname, char** argv) {
	char buf[10];
	readlink(pathname, buf, 10);
	int length = strlen(buf); // x) RTE_Improper_Null_Termination : NULL로 종료하지 않을 수 있는 문자열 buf'을 strlen의 인자로 사용하였음

	char* buf1R2 = (char*)malloc(512);
	strncpy(buf1R2, argv[1], 512 - 1); // cwe-123) RTE_Write-what-where_Condition : 수 strncpy'는 사용하면 안됨
	// RTE_Compiler_Removal_of_Code : 컴파일러 최적화로 다시 읽지 않을 경우 코드가 제거될 가능성이 있음.
	// cwe-14) strncpy(buf1R2, argv[1], 512 - 1);를 사용하여 메모리 영역에 문자열을 복사하지만, 버퍼를 지우기 위한 코드가 없는 상황
	// cwe-733) 보안에 중요한 코드, 특히 메모리 관리나 버퍼 처리와 관련된 부분은 컴파일러 최적화가 예상치 못한 동작을 할 수 있음

}


void cfun4() {
	getlogin(); // cwe-558) RTE_Use_Of_getlogin_In_Multithread : getlogin 함수는 사용하면 안됨
	char ch;

	FILE* fp = fopen("test.txt", "r");
	printf("file opened");
	fclose(fp);
	ch = fgetc(fp);
	fprintf(fp, "Trying to write after closing the file\n"); // cwe-910
	
}

void cfun5() {
	unsigned int x = 0;
	x = x - 1; // cwe-191) RTE_Type_Underrun : 해당 위치는 Type-Underrun 이 발생할 가능성이 있음
	return 0;
}

void cfun6() {
	int cleartext_buffer = get_secret();
	cleartext_buffer = realloc(cleartext_buffer, 1024); // cwe244) RTE_Improper_Clearing_of_Heap_Memory : 함수 `realloc'는 사용하면 안됨
	scrub_memory(cleartext_buffer, 1024);
}

HANDLE mutex;
void cfun7(int mutex) {
	pthread_mutex_lock(mutex); // cwe-362 & cwe-366) RTE_Improper_Synchronization : 부적절한 동기화로 공유 자원 동시 실행 금지
	// pthread_mutex_lock은 뮤텍스 객체를 올바르게 동기화하려면 pthread_mutex_t 타입의 변수를 사용해야 함
	// 동기화되지 않은 상태가 되면 결과적으로, 여러 스레드가 동시에 같은 리소스에 접근하고 수정할 수 있음
}

char* logMessage;
void sigint_handler(DWORD sig) {
	if (sig == CTRL_C_EVENT) {
		printf("%s\n", logMessage);
		syslog(10, "Signal %d received", sig); // cwe-479) RTE_Signal_Handler : 시그널 핸들러에서 함수 `syslog'를 사용함
		free(logMessage); // cwe-364) RTE_Signal_Handler : 시그널 핸들러에서 함수 `free'를 사용함

		printf("Ctrl-C 키를 누르셨죠!!\n");
		printf("또 누르시면 종료됩니다.\n");
	}
}
BOOL WINAPI CtrlHandler(DWORD fdwCtrlType) {
	switch (fdwCtrlType) {
	case CTRL_C_EVENT:
		sigint_handler(fdwCtrlType);
		return TRUE;
	default:
		return FALSE;
	}
}
void (*old_fun)(int);
int handlerMain(void)
{
	signal(SIGTERM, sigint_handler);
	old_fun = signal(SIGINT, sigint_handler);
	while (1) {
		printf("badayak.com\n");
		sleep(1);
	}
}
#define SENTINEL -1
void deleteElement(int arr[], int size, int index) {
	if (index >= 0 && index < size) {
		// 요소를 삭제하기 위해 왼쪽으로 시프트
		for (int i = index; i < size - 1; i++) {
			arr[i] = arr[i + 1];
		}
		arr[size - 2] = arr[size];  // arr[size] 접근 (오버런)으로 센티널 덮어쓰기
		//cwe-463) 경계를 벗어난 접근 시도 (버퍼 오버런 발생)
	}
}
void deSen() {
	int arr[6] = { 10, 20, 30, 40, 50, SENTINEL };  // 센티널이 마지막에 위치
	// 요소 삭제 (버퍼 오버런 발생 가능)
	deleteElement(arr, 6, 4);  // 50을 삭제
}


void manipulate_string(char* string) {
	char buf[24];
	strcpy(buf, string); //cwe-676) 데이터의 크기가 로컬 버퍼에 맞는지 확인하지 않고 잠재적으로 위험한 strcpy() 함수로 데이터를 복사
}
int manstring() {
	char long_string[100] = "This is a very long string that exceeds the buffer size.";
	manipulate_string(long_string);
	return 0;
}

//cwe-466
int* fn10(int* array, int index, int size) {
	return &array[index + 100]; //cwe-466) RTE_Buffer_Overrun:  잘못된 위치범위 반환
}

int fn10_1() {
	int array[5] = { 1, 2, 3, 4, 5 };
	int* elem = fn10(array, 4, 5);
	return 0;
}


int mainC(int argc, char* argv[], HANDLE hDevice, LPVOID lpInBuffer, DWORD nInBufferSize, LPVOID lpOutBuffer, DWORD nOutBufferSize, LPDWORD lpBytesReturned, LPOVERLAPPED lpOverlapped) { 
	char buffer[100];
	//cwe-134) RTE_Use_Of_External_Format_String
	// c++ 메인함수 주석 처리하고 이 부분을 main으로 해야 검출 됨
	snprintf(buffer, sizeof(buffer), argv[1]);
	char hello[20];
	const char* src = "Hello, Customer!";
	strncpy(hello, src, 20); // cwe-464) RTE_Addition_of_Data_Structure_Sentinel  : 함수 `strncpy' 호출 시 `1' 번 째 인자의 size보다 `3' 번 째 인자 상수 값이 크거나 같으면 안됨
	printf("%s\n", hello);
}
