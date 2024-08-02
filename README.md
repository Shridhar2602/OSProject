Operating Systems - Miniproject (IMT2020065)

===================================================

INSTRUCTIONS TO RUN THE CODE :
	
	gcc init.c -o init
	gcc server.c -lpthread -o server
	gcc client.c -o client

	./init
	./server
	./client

==================================================

PROJECT DETAILS :

	- Use the following admin credentials to login as admin
		username : admin
		password : pwd

	- Only admin can create new bank accounts.
	- Anyone can create new users. The admin must enter username when making new accounts.
	- Make sure to change the port number in header.h when running the code multiple times.

	- One user is already created :
		username : user
		password : pwd