#include "header.h"

struct ac search_account(int acnum);
int add_account(struct ac acc);
int modify_account(struct ac acc);
int delete_account(int acnum);
int transact(int acnum, int amt);
int new_user(struct credential cred);

char username[10];

int main(void)
{
	int sd = socket(AF_UNIX, SOCK_STREAM, 0);
	perror("socket");

	struct sockaddr_in serv, cli;

	serv.sin_family = AF_UNIX;
	serv.sin_addr.s_addr = INADDR_ANY;
	serv.sin_port = htons(PORT);

	// int temp = 1;
	// setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, &temp, sizeof(temp));
	// perror("setsockopt");

	bind(sd, (void *) (&serv), sizeof(serv));
	perror("bind");

	listen(sd, 10);
	perror("listen");

	while(1)
	{
		int size = sizeof(cli);
		int nsd = accept(sd, (void *) (&cli), &size);
		perror("accept");

		if(!fork())
		{
			int req;
			read(nsd, &req, sizeof(req));

			int exit = 0;
			while(exit != 1)
			{
				switch(req)
				{
					case NEW_USER:
					{
						int auth = 1;
						write(nsd, &auth, sizeof(auth));
						struct credential user;
						read(nsd, &user, sizeof(struct credential));
						int temp;
						temp += new_user(user);

						if(user.type == JOINT_USER_PRI)
						{
							read(nsd, &user, sizeof(user));
							temp += new_user(user);
						}

						write(nsd, &temp, sizeof(temp));
						break;
					}

					case AUTH_REQ:
					{
						int auth = 1;
						write(nsd, &auth, sizeof(auth));
						struct credential user;
						read(nsd, &user, sizeof(struct credential));
						auth = authenticate(user);
						write(nsd, &auth, sizeof(auth));
						break;
					}

					case ADD_ACC:
					{
						struct ac acc;
						read(nsd, &acc, sizeof(struct ac));
						int auth = add_account(acc);
						write(nsd, &auth, sizeof(auth));
						break;
					}

					case SEARCH_ACC:
					{
						int ac_num = 0;
						read(nsd, &ac_num, sizeof(ac_num));
						struct ac acc = search_account(ac_num);
						write(nsd, &acc, sizeof(struct ac));
						break;
					}

					case MODIFY_ACC:
					{
						struct ac acc;
						read(nsd, &acc, sizeof(struct ac));
						int auth = modify_account(acc);
						write(nsd, &auth, sizeof(auth));
						break;
					}

					case DELETE_ACC:
					{
						int ac_num = 0;
						read(nsd, &ac_num, sizeof(ac_num));
						int auth = delete_account(ac_num);
						write(nsd, &auth, sizeof(auth));
						break;
					}

					case TRANSACT:
					{
						int acnum, amt;
						read(nsd, &acnum, sizeof(acnum));
						read(nsd, &amt, sizeof(amt));
						int temp = transact(acnum, amt);
						write(nsd, &temp, sizeof(temp));
						break;
					}

					case EXIT:
						exit = 1;
						break;
				}

				read(nsd, &req, sizeof(req));
			}

			close(nsd);
		}

		else
		{
			close(nsd);
		}
	}
	
	getchar();
	close(sd);
}

int authenticate(struct credential user)
{
	int fd = open("credentials.bin", O_RDONLY);

	lseek(fd, 0, SEEK_SET);
	struct credential cred;

	while(read(fd, &cred, sizeof(struct credential)))
	{
		if(cred.type == user.type && !strcmp(cred.username, user.username) && !strcmp(cred.password, user.password))
		{
			close(fd);
			strncpy(username, user.username, sizeof(username));
			return 1;
		}
	}

	close(fd);
	return -1;
}

int new_user(struct credential cred)
{
	int fd = open("credentials.bin", O_RDWR);

	lseek(fd, 0, SEEK_END);

	if(write(fd, &cred, sizeof(struct credential)) > 0)
	{
		close(fd);
		return 0;
	}

	return -1;
}

int add_account(struct ac acc)
{
	int fd = open("ac_details.bin", O_RDWR);

	if(fd < 0)
	{
		perror("open");
		return -1;
	}

	lseek(fd, 0, SEEK_END);
	if(write(fd, &acc, sizeof(struct ac)) > 0)
	{
		close(fd);
		return 1;
	}

	close(fd);
	return -1;
}

struct ac search_account(int acnum)
{
	struct ac acc;
	acc.num = -1;

	int fd = open("ac_details.bin", O_RDONLY);

	if(fd < 0)
	{
		perror("open");
		return acc;
	}

	struct flock lock;

	lock.l_type = F_RDLCK;
	lock.l_whence = SEEK_SET;
	lock.l_start = 0;
	lock.l_len = 0;

	// Locking record
    lock.l_type = F_RDLCK;
    fcntl(fd, F_SETLKW, &lock);

	lseek(fd, 0, SEEK_SET);
	while(read(fd, &acc, sizeof(struct ac)))
	{
		if(acnum == acc.num && (!strcmp(username, acc.username) || !strcmp(username, acc.username_secondary) || !strcmp(username, "admin")))
		{
			lock.l_type = F_UNLCK;
   			fcntl(fd, F_SETLKW, &lock);
			close(fd);
			return acc;
		}
	}

	acc.num = -1;

	lock.l_type = F_UNLCK;
   	fcntl(fd, F_SETLKW, &lock);
	close(fd);
	return acc;
}

int modify_account(struct ac new)
{
	int fd = open("ac_details.bin", O_RDWR);

	if(fd < 0)
	{
		perror("open");
		return -1;
	}
	
	struct flock lock;

	lock.l_type = F_WRLCK;
	lock.l_whence = SEEK_SET;
	lock.l_start = 0;
	lock.l_len = 0;

	// Locking record
    lock.l_type = F_WRLCK;

    fcntl(fd, F_SETLKW, &lock);

	lseek(fd, 0, SEEK_SET);

	int temp = -1;
	int count = 0;
	struct ac acc;
	while(read(fd, &acc, sizeof(struct ac)))
	{
		if(acc.num == new.num && (!strcmp(username, acc.username) || !strcmp(username, acc.username_secondary) || !strcmp(username, "admin")))
		{
			temp = 1;
			break;
		}

		count++;
	}

	if(temp == -1)
	{
		lock.l_type = F_UNLCK;
    	fcntl(fd, F_SETLKW, &lock);
		return -1;
	}

	lseek(fd, count * sizeof(struct ac), SEEK_SET);
	if(write(fd, &new, sizeof(struct ac)) > 0)
	{
		lock.l_type = F_UNLCK;
   		fcntl(fd, F_SETLKW, &lock);
		close(fd);
		return 1;
	}

	lock.l_type = F_UNLCK;
    fcntl(fd, F_SETLKW, &lock);

	close(fd);
	return -1;
}

int delete_account(int acnum)
{
	int fd = open("ac_details.bin", O_RDWR);

	if(fd < 0)
	{
		perror("open");
		return -1;
	}

	int temp = -1;
	int count = 0;
	struct ac acc;
	while(read(fd, &acc, sizeof(struct ac)))
	{
		if(acc.num == acnum)
		{
			acc.num = -1;
			temp = 1;
			break;
		}

		count++;
	}

	if(temp == -1)
	{
		return -1;
	}

	lseek(fd, count * sizeof(struct ac), SEEK_SET);
	if(write(fd, &acc, sizeof(struct ac)) > 0)
	{
		close(fd);
		return 1;
	}

	close(fd);
	return -1;
}

int transact(int acnum, int amt)
{
	struct ac acc = search_account(acnum);

	if(acc.num == -1)
		return ACC_NOT_EXIST;

	acc.balance += amt;

	if(acc.balance < 0)
		return NOT_ENF_BLN;

	if(modify_account(acc) == 1)
	{
		return 0;
	}

	return -1;
}