#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct account{
  	char username[30];
	char password[30];
	int status;
	struct account *next;
};

typedef struct account node;

node *head = NULL;

void insert(char username[30], char password[30], int status){
	node *temp;
	temp=(node *)malloc(sizeof(node));
	strcpy(temp->username, username);
	strcpy(temp->password, password);
	temp->status = status;
	temp->next = head;
	head = temp;
}

node *find(char username[30]){
	node *current = head;

	if(head == NULL){
		return NULL;
	}

	while(strcmp(current->username, username) != 0){
		if(current->next == NULL){
			return NULL;
		}else {
			current = current->next;
		}
	}      
	return current;
}

void openFile(){
	char *username;
	char *password;
	int status;

	username = (char *)malloc(30);
	password = (char *)malloc(30);

    FILE *fptr;
	if((fptr=fopen("account.txt","r+"))==NULL){
		printf("Not found %s\n","account.txt");
		return;
	}
	while(1){
		fscanf(fptr,"%s",username);
		fscanf(fptr,"%s",password);
		fscanf(fptr,"%d",&status);
		if(feof(fptr)) break;
		insert(username, password, status);
	}

	free(username); free(password);		//giai phong pointer		
	fclose(fptr);
}

void writeFile(){
	FILE *fptr;
	node *temp;
    temp = head;
    fptr=fopen("account.txt","w+");
    while(temp){
    	fprintf(fptr, "%s %s %d", temp->username, temp->password, temp->status);
    	fprintf(fptr, "\n");
    	temp=temp->next;
    }
    fclose(fptr);
}

void registerAccount(){
	char username[30];
	char password[30];
	
	printf("-----------Register-----------\n");
	printf("Username: "); 
	scanf("%s", username);
	printf("\n");
	printf("Password: "); 
	scanf("%s", password);
	printf("\n");

	if(find(username) != NULL){
		printf("Account existed");	
	} 
	else{
		printf("Successful registration. Activation required.");
		insert(username, password, 2);
		writeFile();
	}
	printf("\n");
}

void activeAccount(){   /// Check password correct ??
    char username[30];
    char password[30];
    char activationCode[30];
    int activate_count = 0;

	printf("-----------Active account-----------\n");
    printf("Username: ");
    scanf("%s",username);
    printf("Password: ");
    scanf("%s",password);

    node *acc = find(username);
    if(acc!=NULL){
		if(strcmp(acc->password,password) != 0){
			printf("Wrong password!\n");
		}
		else{
			if(acc->status == 2){
            	printf("\nPlease activated your account!\n");
				do{
					printf("Activation code: ");
					scanf("%s",activationCode);
					activate_count++;
					if(strcmp(activationCode,"LTM121216") != 0){
						printf("Account is not activated.\n");
					}
					if(activate_count == 3){
						printf("Activation code is incorrect. Account is blocked\n");
						acc->status = 0;
						writeFile();
						break;
					}
				}while(strcmp(activationCode,"LTM121216") != 0);
				if(activate_count < 3){
					printf("Account is activated\n");
					acc->status = 1;
					writeFile();
				}
			}
		}    
    }
	else{
        printf("Account is not exist!\n");
    }
}

node *signin(){
	char username[30];
	char password[30];
	int login_count = 0;

	printf("-----------Sign in-----------\n");
	printf("Username: "); 
	scanf("%s", username);
	printf("\n");

	node *acc = find(username);
	if(acc != NULL){
		if(acc->status == 0){
			printf("Account is blocked!\n");
			return NULL;
		}
		do{
			printf("Password: "); 
			scanf("%s", password);
			printf("\n");
			if(strcmp(acc->password,password) == 0){
				printf("Hello hust\n");
				return acc; 
			}
			else{
				printf("Password is incorrect\n");
				login_count++;
			}
		}while(login_count < 3);
		if(login_count == 3){
			printf("Password is incorrect. Account is blocked\n");
			acc->status = 0;
			writeFile();
			return NULL;
		}
	}
	else{
		printf("Cannot find account\n");
		return NULL; 
	} 
}

void search(){
	char username[20];
	printf("-----------Search-----------\n");
	printf("Username: "); 
	scanf("%s", username);
	printf("\n");
	node *acc = find(username);
	if(acc != NULL){
		//printf("Username: %s Status: %d\n", acc->username, acc->status);
		if(acc->status == 1) printf("Account is active\n");
		else if(acc->status == 0) printf("Account is blocked\n");
		else printf("Account is not active\n");
	}
	else printf("Cannot find account\n");
}

void changePassword(){
	char username[30];
	char password[30];
	char newPassword1[30];
	char newPassword2[30];
	int countPassword = 0;
	int countNewPassword = 0;

	printf("-----------Change password-----------\n");
	printf("Enter your username: ");
	scanf("%s",username);

	node* acc = find(username);
	if(acc != NULL){
		do{
			printf("Enter current password: ");
			scanf("%s",password);
			countPassword++;
			if(countPassword == 3) break;
			if(strcmp(acc->password,password) != 0) printf("Current password is incorrect. Please try again\n");
		}while(strcmp(acc->password,password) != 0);
		if(countPassword != 3){
			do{
				printf("Enter new password: ");
				scanf("%s",newPassword1);
				printf("Enter new password again:");
				scanf("%s",newPassword2);
				countNewPassword++;
				if(countNewPassword == 3) break;
				if(strcmp(newPassword1,newPassword2) != 0) printf("Error new password! Try again!\n");	//Xac nhan password moi bi sai!
			}while(strcmp(newPassword1,newPassword2) != 0);
			if(countNewPassword == 3){
				printf("3 times wrong password, please try again later!\n");
			}
			else{
				printf("Password is changed\n");
				strcpy(acc->password,newPassword1);
				writeFile();
			}
		}
		else printf("3 times wrong password, please try again later!\n");
	}
	else{
		printf("Account does not exist");
	}
}

node *signout(){
	char username[20];
	printf("-----------Sign out-----------\n");
	printf("Username: "); 
	scanf("%s", username);
	printf("\n");

	node *acc = find(username);
	if(acc == NULL){
		printf("Cannot find account\n");
		return NULL;
	}
	else return acc;
}

int menu(){
	int choice = 0;
	printf("USER MANAGEMENT PROGRAM\n");
	printf("-----------------------------------\n");
	printf("1. Register\n");
    printf("2. Activate\n");
	printf("3. Sign in\n");
	printf("4. Search\n");
    printf("5. Change password\n");
	printf("6. Sign out\n");
	printf("Your choice (1-6, other to quit):\n");
    scanf("%d", &choice);
	return choice;	
}

int main(){
	openFile();

	int choice;
	node *login_acc = NULL;
	node *signout_acc = NULL;
	do{
		choice = menu();
		switch(choice){
		case 1: 
			registerAccount(); 
			break;
        case 2: 
			activeAccount(); 
			break;
		case 3: 
			login_acc = signin(); 
			break;
		case 4: 
			{
				if(login_acc != NULL) {
					search();
				}
				else printf("Account is not sign in\n");
			} 
			break;
        case 5:
			if(login_acc != NULL) {
				changePassword();
			}
			else printf("Account is not sign in\n");
			break;
		case 6: 
			{
				if(login_acc == NULL) printf("Account is not sign in\n"); //check sign in
				else{
					signout_acc = signout();
					if(signout_acc)
					if(strcmp(signout_acc->username,login_acc->username) == 0){
						printf("Goodbye hust\n");
						login_acc = NULL;
					}
					else printf("Cannot find account\n");
				}			
			} 
			break;
		}
	}
	while(choice > 0 && choice < 7);
	return 0;
}
