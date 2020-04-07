#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <process.h>

typedef struct test_struct {
	char plaintext;
	char key;
	char result;
}tstruct;

unsigned __stdcall test(void* a) {
	tstruct *tmp = a;

	tmp->result = tmp->plaintext ^ tmp->key;

	_endthreadex(0);
	return 0;
}

int main() {
	unsigned int id;
	HANDLE *handler = NULL;
	tstruct t;
	t.plaintext = 'A';
	t.key = '\x01';
	t.result = '\00';

	handler = (HANDLE*)malloc(sizeof(HANDLE)*2);

	handler[0] = (HANDLE)_beginthreadex(NULL, 0, test, &t, 0, &id);
	WaitForSingleObject(handler[0], INFINITE);

	printf("%02x\n", t.result);
	t.plaintext = t.result;

	handler[1] = (HANDLE)_beginthreadex(NULL, 0, test, &t, 0, &id);
	WaitForSingleObject(handler[1], INFINITE);

	printf("%02x\n", t.result);

	CloseHandle(handler[0]);
	CloseHandle(handler[1]);

	return 0;
}