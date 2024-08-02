#include "header.h"

struct ac search_account(int sd);
int add_account(int sd);
int modify_account(int sd);
int delete_account(int sd);
int transact(int sd, int mode);
int new_user(int type);

int main(void)
{
	printf("+------------------------------------------------+\n");
	printf("|             Online Banking System              |\n");
	printf("|                                                |\n");
	printf("|                 - Shridhar Sharma (IMT2020065) |\n");
	printf("+------------------------------------------------+\n");
	printf("| 1 -> Login as user                             |\n");
	printf("| 2 -> Login as Joint Acccount User              |\n");
	printf("| 3 -> Login as Administrator                    |\n");
	printf("| 4 -> Join as a new user                        |\n");
	printf("| 5 -> Join as a new joint user                  |\n");
	printf("+------------------------------------------------+\n");

	int login;
	printf("\nEnter your choice: ");
	scanf("%d", &login);

	switch(login)
	{
		case 1:
			user();
			break;
		
		case 2:
			joint_user();
			break;
		
		case 3:
			admin();
			break;

		case 4:
			new_user(USER);
			break;

		case 5:
			new_user(JOINT_USER_PRI);
			break;

		default:
			printf("Invalid input");
	}
}

int new_user(int type)
{
	write(1, "\e[1;1H\e[2J", 10);

	int sd = connect_serv();

	int req = NEW_USER;
	write(sd, &req, sizeof(req));

	int auth;
	read(sd, &auth, sizeof(auth));
	if(auth == 1)
	{
		printf("Connected to server.....\n\n");
	}

	else
	{
		printf("Cannot connect to the server.....\n");
		return -1;
	}

	if(type == USER)
	{
		struct credential cred;
		cred.type = USER;
		printf("Creating a new user.....\n");
		printf("\nEnter Username: ");
		scanf("%s", cred.username);
		printf("Enter password: ");
		scanf("%s", cred.password);

		write(sd, &cred, sizeof(struct credential));
	}

	else
	{
		struct credential primary, secondary;
		primary.type = JOINT_USER_PRI;
		secondary.type = JOINT_USER_SEC;

		printf("Enter new Primary Account Details : \n\n");
		printf("\nEnter Username: ");
		scanf("%s", primary.username);
		printf("Enter password: ");
		scanf("%s", primary.password);

		printf("Enter new Secondary Account Details : \n\n");
		printf("\nEnter Username: ");
		scanf("%s", secondary.username);
		printf("Enter password: ");
		scanf("%s", secondary.password);

		write(sd, &primary, sizeof(struct credential));
		write(sd, &secondary, sizeof(struct credential));
	}

	read(sd, &auth, sizeof(auth));

	if(auth == 0)
	{
		printf("New user created successfully.....\n");
		return 0;
	}

	printf("User creation failed.....\n");
	return -1;
}

int admin()
{
	write(1, "\e[1;1H\e[2J", 10);

	int sd = connect_serv();

	struct credential cred;
	cred.type = ADMIN;
	printf("Enter username: ");
	scanf("%s", cred.username);
	printf("Enter password: ");
	scanf("%s", cred.password);
	
	int auth = AUTH_REQ;
	write(sd, &auth, sizeof(auth));
	read(sd, &auth, sizeof(auth));

	if(auth == 1)
	{
		printf("Connected to server.....\n");
	}

	write(sd, &cred, sizeof(struct credential));
	read(sd, &auth, sizeof(auth));

	if(auth == -1)
	{
		printf("Wrong username/password.....\n");
		return -1;
	}

	printf("Authentication successful.....\n");

	int exit = 0;
	while(exit != 1)
	{
		printf("+------------------------------------------------+\n");
		printf("| 1 -> Add a new account                         |\n");
		printf("| 2 -> Modify an existing account                |\n");
		printf("| 3 -> Delete an account                         |\n");
		printf("| 4 -> Search for an account                     |\n");
		printf("| 5 -> Exit                                      |\n");
		printf("+------------------------------------------------+\n");

		int choice;
		printf("\nEnter your choice: ");
		scanf("%d", &choice);

		switch(choice)
		{
			case 1:
			{
				int oper = ADD_ACC;
				write(sd, &oper, sizeof(oper));
				add_account(sd);
				break;
			}

			case 2:
			{
				int oper = MODIFY_ACC;
				write(sd, &oper, sizeof(oper));
				modify_account(sd);
				break;
			}

			case 3:
			{
				int oper = DELETE_ACC;
				write(sd, &oper, sizeof(oper));
				delete_account(sd);
				break;
			}

			case 4:
			{
				int oper = SEARCH_ACC;
				write(sd, &oper, sizeof(oper));
				struct ac acc = search_account(sd);
				if(acc.num > 0)
					printf("\nAccount Number : %d\nFirst Name : %s\nLast Name : %s\nAccount holder username : %s\nBalance : %lf\n", acc.num, acc.fname, acc.lname, acc.username, acc.balance);
				break;
			}

			case 5:
			{
				exit = 1;
				write(sd, &exit, sizeof(exit));
				break;
			}

			default:
				printf("Invalid input");
		}

		if(exit != 1)
		{
			printf("\nPress any key to return.....\n");
			fflush(stdout);
			getchar();
			getchar();
			write(1, "\e[1;1H\e[2J", 10);
		}
	}

	close(sd);
}

int user()
{
	write(1, "\e[1;1H\e[2J", 10);

	int sd = connect_serv();

	struct credential cred;
	cred.type = USER;
	printf("Enter username: ");
	scanf("%s", cred.username);
	printf("Enter password: ");
	scanf("%s", cred.password);
	
	int auth = AUTH_REQ;
	write(sd, &auth, sizeof(auth));
	read(sd, &auth, sizeof(auth));
	if(auth == 1)
	{
		printf("Connected to server.....\n");
	}

	write(sd, &cred, sizeof(struct credential));
	read(sd, &auth, sizeof(auth));

	if(auth == -1)
	{
		printf("Wrong username/password.....\n");
		return -1;
	}

	printf("Authentication successful.....\n");

	int exit = 0;
	while(exit != 1)
	{
		printf("+------------------------------------------------+\n");
		printf("| 1 -> Deposit                                   |\n");
		printf("| 2 -> Withdraw                                  |\n");
		printf("| 3 -> Balance Enquiry                           |\n");
		printf("| 4 -> View Details                              |\n");
		printf("| 5 -> Exit                                      |\n");
		printf("+------------------------------------------------+\n");

		int choice;
		printf("\nEnter your choice: ");
		scanf("%d", &choice);

		switch(choice)
		{
			case 1:
			{
				int oper = TRANSACT;
				write(sd, &oper, sizeof(oper));
				transact(sd, 1);
				break;
			}

			case 2:
			{
				int oper = TRANSACT;
				write(sd, &oper, sizeof(oper));
				transact(sd, -1);
				break;
			}

			case 3:
			{
				int oper = SEARCH_ACC;
				write(sd, &oper, sizeof(oper));
				struct ac acc = search_account(sd);
				if(acc.num > 0)
					printf("\nBalance : %lf\n", acc.balance);
				else
					printf("You don't own this account...\n");
				break;
			}

			case 4:
			{
				int oper = SEARCH_ACC;
				write(sd, &oper, sizeof(oper));
				struct ac acc = search_account(sd);
				if(acc.num > 0)
					printf("\nAccount Holder : %s %s\nAccount holder username : %s\nAccount type : Normal\n", acc.fname, acc.lname, acc.username);
				else
					printf("You don't own this account...\n");
				break;
			}

			case 5:
			{
				exit = 1;
				write(sd, &exit, sizeof(exit));
				break;
			}

			default:
				printf("Invalid input");
		}

		if(exit != 1)
		{
			printf("\nPress any key to return.....\n");
			fflush(stdout);
			getchar();
			getchar();
			write(1, "\e[1;1H\e[2J", 10);
		}
	}

	close(sd);
}

int joint_user()
{
	write(1, "\e[1;1H\e[2J", 10);

	int sd = connect_serv();

	struct credential cred;

	printf("Primary Joint Account holder? (y/n) : ");
	char c;
	fflush(stdin);
	fflush(stdout);
	scanf("%c", &c);
	scanf("%c", &c);

	if(c == 'y' || c == 'Y')
		cred.type = JOINT_USER_PRI;
	else if(c == 'n' | c == 'N')
		cred.type = JOINT_USER_SEC;
	else
	{
		printf("invalid input.....\n");
		return -1;
	}

	printf("Enter username: ");
	scanf("%s", cred.username);
	printf("Enter password: ");
	scanf("%s", cred.password);
	
	int auth = AUTH_REQ;
	write(sd, &auth, sizeof(auth));
	read(sd, &auth, sizeof(auth));
	if(auth == 1)
	{
		printf("Connected to server.....\n");
	}

	write(sd, &cred, sizeof(struct credential));
	read(sd, &auth, sizeof(auth));

	if(auth == -1)
	{
		printf("Wrong username/password.....\n");
		return -1;
	}

	printf("Authentication successful.....\n");

	int exit = 0;
	while(exit != 1)
	{
		printf("+------------------------------------------------+\n");
		printf("| 1 -> Deposit                                   |\n");
		printf("| 2 -> Withdraw                                  |\n");
		printf("| 3 -> Balance Enquiry                           |\n");
		printf("| 4 -> View Details                              |\n");
		printf("| 5 -> Exit                                      |\n");
		printf("+------------------------------------------------+\n");

		int choice;
		printf("\nEnter your choice: ");
		scanf("%d", &choice);

		switch(choice)
		{
			case 1:
			{
				int oper = TRANSACT;
				write(sd, &oper, sizeof(oper));
				transact(sd, 1);
				break;
			}

			case 2:
			{
				int oper = TRANSACT;
				write(sd, &oper, sizeof(oper));
				transact(sd, -1);
				break;
			}

			case 3:
			{
				int oper = SEARCH_ACC;
				write(sd, &oper, sizeof(oper));
				struct ac acc = search_account(sd);
				if(acc.num > 0)
					printf("\nBalance : %lf\n", acc.balance);
				else
					printf("You don't own this account...\n");
				break;
			}

			case 4:
			{
				int oper = SEARCH_ACC;
				write(sd, &oper, sizeof(oper));
				struct ac acc = search_account(sd);
				if(acc.num > 0)
					printf("\nAccount Holder : %s %s\nAccount holder username : %s\nSecondary Account holder username : %s\nAccount type : Joint\n", acc.fname, acc.lname, acc.username, acc.username_secondary);
				else
					printf("You don't own this account...\n");
				break;
			}

			case 5:
			{
				exit = 1;
				write(sd, &exit, sizeof(exit));
				break;
			}

			default:
				printf("Invalid input");
		}

		if(exit != 1)
		{
			printf("\nPress any key to return.....\n");
			fflush(stdout);
			getchar();
			getchar();
			write(1, "\e[1;1H\e[2J", 10);
		}
	}

	close(sd);
}

int connect_serv()
{
	int sd = socket(AF_UNIX, SOCK_STREAM, 0);
	perror("socket");

	struct sockaddr_in serv;

	serv.sin_family = AF_UNIX;
	serv.sin_addr.s_addr = INADDR_ANY;
	serv.sin_port = htons(PORT);

	connect(sd, (void *) (&serv), sizeof(serv));
	perror("connect");

	return sd;
}

int add_account(int sd)
{
	struct ac acc;
	printf("Enter account number: ");
	scanf("%d", &acc.num);
	printf("Enter first name: ");
	scanf("%s", acc.fname);
	printf("Enter last name: ");
	scanf("%s", acc.lname);
	printf("Enter username: ");
	scanf("%s", acc.username);
	printf("Enter Secondary holder username (-1 if not joint account) : ");
	scanf("%s", acc.username_secondary);
	printf("Enter balance: ");
	scanf("%lf", &acc.balance);

	int auth = -1;
	write(sd, &acc, sizeof(struct ac));
	read(sd, &auth, sizeof(auth));

	if(auth == 1)
	{
		printf("\nAccount added successfully\n");
		return 1;
	}

	return 0;
}

struct ac search_account(int sd)
{
	int num;
	printf("Enter account number: ");
	scanf("%d", &num);

	write(sd, &num, sizeof(num));
	
	struct ac acc;
	read(sd, &acc, sizeof(struct ac));
	
	if(acc.num == -1)
	{
		printf("\nAccount not found.....\n");
		return acc;
	}

	return acc;
}

int modify_account(int sd)
{
	struct ac acc;
	int ac_num, auth;
	printf("Enter A/C number: ");
	scanf("%d", &ac_num);
	acc.num = ac_num;
	printf("\nEnter modified details -> \n\n");
	printf("First Name: ");
	scanf("%s", acc.fname);
	printf("Last Name: ");
	scanf("%s", acc.lname);
	printf("Username: ");
	scanf("%s", acc.username);
	printf("Balance: ");
	scanf("%lf", &acc.balance);

	write(sd, &acc, sizeof(struct ac));
	read(sd, &auth, sizeof(auth));

	if(auth == -1)
	{
		printf("\nAccount does not exist.....\n");
		return -1;
	}

	printf("\nAccount modified successfully.....\n");
	return 1;
}

int delete_account(int sd)
{
	int ac_num, auth;
	printf("Enter A/C number: ");
	scanf("%d", &ac_num);

	write(sd, &ac_num, sizeof(ac_num));
	read(sd, &auth, sizeof(auth));

	if(auth == -1)
	{
		printf("\nAccount does not exist.....\n");
		return -1;
	}

	printf("\nAccount deleted.....\n");
	return 1;
}

int transact(int sd, int mode)
{
	int acnum, amt;

	printf("Enter account number: ");
	scanf("%d", &acnum);

	if(mode == 1)
	{
		printf("Enter amount to be deposited: ");
		scanf("%d", &amt);
	}

	else
	{
		printf("Enter amount to withdraw: ");
		scanf("%d", &amt);
		amt *= mode;
	}

	write(sd, &acnum, sizeof(acnum));
	write(sd, &amt, sizeof(amt));

	int temp;
	read(sd, &temp, sizeof(temp));

	if(temp == ACC_NOT_EXIST)
	{
		printf("Account does not exist / You don't own this account.....\n");
		return -1;
	}

	else if(temp == NOT_ENF_BLN)
	{
		printf("Not enough balance.....\n");
		return -2;
	}

	return 0;
}