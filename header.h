#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/stat.h>
#include <string.h>
#include <pthread.h>

#define ADMIN 1
#define USER 2
#define JOINT_USER_PRI 3
#define JOINT_USER_SEC 4

#define NEW_USER 19
#define AUTH_REQ 20
#define ADD_ACC 21
#define SEARCH_ACC 22
#define MODIFY_ACC 23
#define DELETE_ACC 24
#define TRANSACT 25
#define BAL_ENQ 27
#define PASS_CNG 28
#define VIEW_DETAILS 29
#define EXIT 30

#define ACC_NOT_EXIST 100
#define NOT_ENF_BLN 101

#define PORT 7482

struct ac
{
	int num;
	char fname[30];
	char lname[30];
	char username[10];
	char username_secondary[10];
	double balance;
};

struct credential
{
	int type;
	char username[10];
	char password[10];
};

int authenticate();
int admin();
int user();
int joint_user();
int modify_account();
int connect_serv();
