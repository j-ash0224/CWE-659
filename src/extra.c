#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <limits.h>
#include <string.h>
#include <windows.h>


// �ּ� Ǯ�� cpp ������ ĸ�ĵ��� ����
// cpp ���Ͽ� ������ ���� �߻�

 //���� ī��Ʈ ����ü ����
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
    RefCountedObject* obj = create_object(a); //cwe-911) RTE_Uninitialized_Variable : ���� ī��Ʈ�� �ʱ�ȭ ���� ����
}

void RTE_Buffer_Overrun() {
	char* buffer = (char*)malloc(10 * sizeof(char)); // cwe-122) Heap-based Buffer Overflow : ���� ũ���� �� �޸� �Ҵ�
	strcpy(buffer, "This string is too long for the buffer"); // ���� ũ�⸦ �ʰ��ϴ� ���ڿ� ���� (�� ��� ���� �����÷�)
	free(buffer);

	char src[10] = "Hello";
	char dst[5];
	memcpy(dst, src, 10); // cwe-805) Buffer Access with Incorrect Length Value : �߸��� ���� ���� ����� ���� �׼���

	char src2[10] = "HelloWorld";
	char dst2[5];
	memcpy(dst2, src2, sizeof(src2)); // cwe-806) Buffer Access Using Size of Source Buffer : ������ ���� ũ�⺸�� ū �ҽ� ���� ũ�⸦ ����Ͽ� ����
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
	buf.nameID = (int)(defaultMessage + 1); // cwe-843) RTE_Access_Using_Incompatible_Type : union Ÿ���� ����ϸ� �ȵ�
}

void cfun2(char* filename, char* localfile, char* network, int len, int buf) {
	chroot("/var/ftproot"); // cwe-243) RTE_Creation_chroot_Without_Changing : �Լ� chroot' ��� �� �Լ� chdir'�� ����ߴ��� �˻�
	fgets(filename, sizeof(filename), network); //cwe-467) RTE_Use_Of_sizeof_On_Pointer : sizeof()�� ���ڷ� ������ ���� filename'��(��) ���Ǹ� �ȵ�
}

void cfunc3(char pathname, char** argv) {
	char buf[10];
	readlink(pathname, buf, 10);
	int length = strlen(buf); // x) RTE_Improper_Null_Termination : NULL�� �������� ���� �� �ִ� ���ڿ� buf'�� strlen�� ���ڷ� ����Ͽ���

	char* buf1R2 = (char*)malloc(512);
	strncpy(buf1R2, argv[1], 512 - 1); // cwe-123) RTE_Write-what-where_Condition : �� strncpy'�� ����ϸ� �ȵ�
	// RTE_Compiler_Removal_of_Code : �����Ϸ� ����ȭ�� �ٽ� ���� ���� ��� �ڵ尡 ���ŵ� ���ɼ��� ����.
	// cwe-14) strncpy(buf1R2, argv[1], 512 - 1);�� ����Ͽ� �޸� ������ ���ڿ��� ����������, ���۸� ����� ���� �ڵ尡 ���� ��Ȳ
	// cwe-733) ���ȿ� �߿��� �ڵ�, Ư�� �޸� ������ ���� ó���� ���õ� �κ��� �����Ϸ� ����ȭ�� ����ġ ���� ������ �� �� ����

}


void cfun4() {
	getlogin(); // cwe-558) RTE_Use_Of_getlogin_In_Multithread : getlogin �Լ��� ����ϸ� �ȵ�
	char ch;

	FILE* fp = fopen("test.txt", "r");
	printf("file opened");
	fclose(fp);
	ch = fgetc(fp);
	fprintf(fp, "Trying to write after closing the file\n"); // cwe-910
	
}

void cfun5() {
	unsigned int x = 0;
	x = x - 1; // cwe-191) RTE_Type_Underrun : �ش� ��ġ�� Type-Underrun �� �߻��� ���ɼ��� ����
	return 0;
}

void cfun6() {
	int cleartext_buffer = get_secret();
	cleartext_buffer = realloc(cleartext_buffer, 1024); // cwe244) RTE_Improper_Clearing_of_Heap_Memory : �Լ� `realloc'�� ����ϸ� �ȵ�
	scrub_memory(cleartext_buffer, 1024);
}

HANDLE mutex;
void cfun7(int mutex) {
	pthread_mutex_lock(mutex); // cwe-362 & cwe-366) RTE_Improper_Synchronization : �������� ����ȭ�� ���� �ڿ� ���� ���� ����
	// pthread_mutex_lock�� ���ؽ� ��ü�� �ùٸ��� ����ȭ�Ϸ��� pthread_mutex_t Ÿ���� ������ ����ؾ� ��
	// ����ȭ���� ���� ���°� �Ǹ� ���������, ���� �����尡 ���ÿ� ���� ���ҽ��� �����ϰ� ������ �� ����
}

char* logMessage;
void sigint_handler(DWORD sig) {
	if (sig == CTRL_C_EVENT) {
		printf("%s\n", logMessage);
		syslog(10, "Signal %d received", sig); // cwe-479) RTE_Signal_Handler : �ñ׳� �ڵ鷯���� �Լ� `syslog'�� �����
		free(logMessage); // cwe-364) RTE_Signal_Handler : �ñ׳� �ڵ鷯���� �Լ� `free'�� �����

		printf("Ctrl-C Ű�� ��������!!\n");
		printf("�� �����ø� ����˴ϴ�.\n");
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
		// ��Ҹ� �����ϱ� ���� �������� ����Ʈ
		for (int i = index; i < size - 1; i++) {
			arr[i] = arr[i + 1];
		}
		arr[size - 2] = arr[size];  // arr[size] ���� (������)���� ��Ƽ�� �����
		//cwe-463) ��踦 ��� ���� �õ� (���� ������ �߻�)
	}
}
void deSen() {
	int arr[6] = { 10, 20, 30, 40, 50, SENTINEL };  // ��Ƽ���� �������� ��ġ
	// ��� ���� (���� ������ �߻� ����)
	deleteElement(arr, 6, 4);  // 50�� ����
}


void manipulate_string(char* string) {
	char buf[24];
	strcpy(buf, string); //cwe-676) �������� ũ�Ⱑ ���� ���ۿ� �´��� Ȯ������ �ʰ� ���������� ������ strcpy() �Լ��� �����͸� ����
}
int manstring() {
	char long_string[100] = "This is a very long string that exceeds the buffer size.";
	manipulate_string(long_string);
	return 0;
}

//cwe-466
int* fn10(int* array, int index, int size) {
	return &array[index + 100]; //cwe-466) RTE_Buffer_Overrun:  �߸��� ��ġ���� ��ȯ
}

int fn10_1() {
	int array[5] = { 1, 2, 3, 4, 5 };
	int* elem = fn10(array, 4, 5);
	return 0;
}


int mainC(int argc, char* argv[], HANDLE hDevice, LPVOID lpInBuffer, DWORD nInBufferSize, LPVOID lpOutBuffer, DWORD nOutBufferSize, LPDWORD lpBytesReturned, LPOVERLAPPED lpOverlapped) { 
	char buffer[100];
	//cwe-134) RTE_Use_Of_External_Format_String
	// c++ �����Լ� �ּ� ó���ϰ� �� �κ��� main���� �ؾ� ���� ��
	snprintf(buffer, sizeof(buffer), argv[1]);
	char hello[20];
	const char* src = "Hello, Customer!";
	strncpy(hello, src, 20); // cwe-464) RTE_Addition_of_Data_Structure_Sentinel  : �Լ� `strncpy' ȣ�� �� `1' �� ° ������ size���� `3' �� ° ���� ��� ���� ũ�ų� ������ �ȵ�
	printf("%s\n", hello);
}
