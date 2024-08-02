#include "header.h"

int main(void)
{
	int acc_counter = 0;
	int fd = open("ac_details.bin", O_CREAT | O_RDWR, 0666);

	if(fd < 0)
	{
		perror("open");
		return -1;
	}

	close(fd);

	fd = open("credentials.bin", O_CREAT | O_RDWR, 0666);
	
	if(fd < 0)
	{
		perror("open");
		return -1;
	}

	lseek(fd, 0, SEEK_SET);

	struct credential admin;
	admin.type = ADMIN;
	strncpy(admin.username, "admin", 6);
	strncpy(admin.password, "pwd", 4);

	write(fd, &admin, sizeof(struct credential));

	struct credential user;
	user.type = USER;
	strncpy(user.username, "user", 5);
	strncpy(user.password, "pwd", 4);

	write(fd, &user, sizeof(struct credential));

	close(fd);
}