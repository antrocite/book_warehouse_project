#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>

#include "HW2_Mark_Baskov_185023MVEB.h"

int main(void)
{			
	int option;
	
	struct tm *local;
	time_t t;
	t = time(NULL);
	local = localtime(&t);

	FILE *fl = fopen(LogFile, "a");
	if (fl == NULL){
		fprintf(stderr, "Opening log file failed\n");
		exit(1);
	} 
	fprintf(fl, "Opened log file\n");
	fprintf(fl, "\n\t\t*** local : %s:\n", asctime(local)); 
		
	fprintf(fl, "Declaring Books and Inv structs pointers\n");				
	Books *headB = NULL;
	Inv *headI = NULL;
	
	ReadFile(fl, &headB, &headI);
	LinkStructs(fl, headB, headI);
	
	do
	{
		puts("");
		fprintf(fl, "\n");
		puts("--------------------------------------------------");
		fprintf(fl, "--------------------------------------------------\n");
		puts("Choose Your hero:");
		fprintf(fl, "Visible : Choose Your hero:\n");
		puts("1.Display all data");
		fprintf(fl, "Visible : 1.Display all data\n");
		puts("2.Edit fields");
		fprintf(fl, "Visible : 2.Edit\n");
		puts("3.Find book by author");
		fprintf(fl, "Visible : 3.Find book by author\n");
		puts("4.Add book");
		fprintf(fl, "Visible : 4.Add book\n");
		puts("5.Add inventory position");
		fprintf(fl, "Visible : 5.Add inventory position\n");
		puts("6.Remove book");
		fprintf(fl, "Visible : 6.Remove book\n");
		puts("7.Remove inventory position");
		fprintf(fl, "Visible : 6.Remove inventory position\n");
		puts("0.Exit");
		fprintf(fl, "Visible : 0.Exit\n");
		puts("--------------------------------------------------");
		fprintf(fl, "--------------------------------------------------\n");
		fprintf(fl, "Asking for integer\n");
		option = GetInt(fl);

		switch(option)
		{
			case 1:
				PrintList(fl, headB, headI);
				break;
			case 2:
				EditFunc(fl, headB, headI);
				break;
			case 3:
				FindFunc(fl, headB, headI);
				break;
			case 4:
				AddBook(fl, headB);
				break;
			case 5:
				AddInv(fl, headB, headI);
				break;
			case 6:
				RemoveBook(fl, headB);
				break;
			case 7:
				RemoveInv(fl, headB, headI);
				break;
		}	
	}while(option != 0);
	
	PrintFile(fl, headB, headI);
	Free(fl, headB, headI);
	
	fprintf(fl, "\n\n\n\t\t***\nPROGRAM ENDS SUCCESSFULLY\n");
	fprintf(fl, "EXITING PROGRAM\n\t\t***\n\n\n\n\n");
	fclose(fl);
	return 0;
}

void GetStr(FILE *fl, char *str)
{
	int i;
	
	struct tm *local;
	time_t t;
	t = time(NULL);
	local = localtime(&t);
	
	fprintf(fl, "Removing null terminator from buffer\n");
	scanf("%*c");      //I don't know why! I've really tried to figure it out
					   //I know valgrind doesn't like it too. But i give up on this
	do
	{
		fprintf(fl, "Getting input from user\n");
		scanf("%[^\n]s", str);
		fprintf(fl, "\n\t\t***  local : %s\n", asctime(local)); 
		fprintf(fl, "User inputs: %s\n", str);
		if(strlen(str) > MAX_LENGTH){
			puts("Input is too big");
			fprintf(fl, "Visible : Input is too big\n");
		}
	}while(strlen(str) > MAX_LENGTH);
	
	str[0] = toupper(str[0]);

	for (i = 1; i < strlen(str); i++)
	{
		str[i] = tolower(str[i]);
		if(str[i - 1] == ' ')
		{
			str[i] = toupper(str[i]);
		}
	}
	
	fprintf(fl, "Converted string is : %s\n", str);
	
}

int GetInt(FILE *fl)
{
	int i, error;
	char str[MAX_LENGTH];
	
	do
	{
		error = 0;
		GetStr(fl, str);
		for (i = 0; i < strlen(str); i++)
		{
			if(isdigit(str[i]) == 0)
			{
				error = 1;
				puts("Not an integer value, try again");
				break;
			}
		}
	}while(error == 1);
	
	sscanf(str, "%d", &i);
	fprintf(fl, "Integer obtained : %d\n", i);
	return i;
}

	
double GetNum(FILE *fl)
{
	int i;
	double returnVal;
	int check;
	int dotIs;
	char str[MAX_LENGTH];
	
	do
	{
		check = 0;
		dotIs = 0;
		GetStr(fl, str);
		for (i = 0; i < strlen(str); i++)
		{
			if(isdigit(str[i]) == 0)
			{
				if(str[i] == '.' && dotIs == 0)
				{
					dotIs = 1;
				}else{
					puts("Not a value and/or too many values, try again");
					fprintf(fl, "Visible : Not a value and/or too many values, try again\n");
					check = 1;
					break;
				}	
			}
		}	
	}while(check == 1);
	
	sscanf(str, "%lf", &returnVal);
	fprintf(fl, "Double value obtained: %lf\n", returnVal);
	return returnVal;
}

void Parse(FILE *fl, char *line, Books *nodeB, Inv *nodeI, int whichOne)
{
	int i = 0, j = 0, fieldNum = 0, quote = 0;
	char field[FIELDS][MAX_LENGTH];
	
	switch(whichOne)
	{
		case 1:
			fprintf(fl, "Parsing the line of Books struct: \n");
			while(line[i] != '\0')
			{
				if(line[i] == '"')
				{
					if(quote == 0)
					{
						quote = 1;
						i++;
					}else if(quote == 1)
					{
						quote = 0;
						field[fieldNum][j] = '\0';
						fprintf(fl, "field got : %s\n", field[fieldNum]);
						switch(fieldNum)
						{
							case 0:		
								Flush(fl, &(nodeB)->code, field[fieldNum]);
								break;
							case 1:
								Flush(fl, &(nodeB)->title, field[fieldNum]);
								break;
							case 2:
								Flush(fl, &(nodeB)->author, field[fieldNum]);
								break;
							case 3:
								sscanf(field[fieldNum], "%d", &nodeB->year);
								break;
							case 4:
								sscanf(field[fieldNum], "%lf", &nodeB->cost);
								break;
							case 5:
								sscanf(field[fieldNum], "%lf", &nodeB->price);
								break;
						}
						i = i + 2;
						j = 0;
						fieldNum++;
					}	
				}else if(quote == 1)
				{
					field[fieldNum][j] = line[i];
					j++;
					i++;
				}
			}
			break;
		case 2:
			while(line[i] != '\0')
			{
				if(line[i] == '"')
				{
					if(quote == 0)
					{
						quote = 1;
						i++;
					}else if(quote == 1)
					{
						quote = 0;
						field[fieldNum][j] = '\0';
						fprintf(fl, "field got : %s\n", field[fieldNum]);
						switch(fieldNum)
						{
							case 0:		
								sscanf(field[fieldNum], "%d", &nodeI->ID);
								break;
							case 1:
								Flush(fl, &(nodeI)->code, field[fieldNum]);
								break;
							case 2:
								sscanf(field[fieldNum], "%d", &nodeI->shelf);
								break;
							case 3:
								sscanf(field[fieldNum], "%d", &nodeI->stock);
								break;
						}
						i = i + 2;
						j = 0;
						fieldNum++;
					}	
				}else if(quote == 1)
				{
					field[fieldNum][j] = line[i];
					j++;
					i++;
				}
			}
	}
	
}	
	
void Flush(FILE *fl, char **inStr, char *outStr)
{	
	fprintf(fl, "Allocating memory for %s\n", outStr);
	*inStr = (char*)malloc(sizeof(char) * (strlen(outStr) + 1));
	if (inStr == NULL) 
		{
			fprintf(stderr, "Failed\n");
			fprintf(fl, "Malloc for the string(%s) failed\n", outStr); 
			exit(1);
		} 
	fprintf(fl, "Inserting string");
	strcpy(*inStr, outStr);
}

void LinkStructs(FILE *fl, Books *nodeB, Inv *nodeI)
{
	int n = 0;
	fprintf(fl, "Declaring temp pointer for linking\n");
	Inv **pTemp = NULL;
	Inv *headI = nodeI;
	while(nodeB != NULL)
	{
		nodeB->count = 0;
		nodeB->pInv = NULL;
		while(nodeI != NULL)
		{
			if(strcmp(nodeB->code, nodeI->code) == 0)
			{
				fprintf(fl, "Reallocating array of pointers in struct books to Inv structs\n");
				pTemp = (Inv**)realloc(nodeB->pInv, (sizeof(Inv) * (n + 1)));
				if(pTemp == NULL)
				{
					puts("not reallocated");
					exit(1);	
				}		
				nodeB->pInv = pTemp;
				
				fprintf(fl, "Assigning related Inv nodes to array in Books\n");
				*(nodeB->pInv + nodeB->count) = nodeI;
				
				nodeB->count++;
			}
			nodeI = nodeI->next;
		}
		nodeI = headI;
		nodeB = nodeB->next;
	}
}
		
void Free(FILE *fl, Books *headB, Inv *headI)
{
	fprintf(fl, "Declaring temp pointers to Books and Inv head nodes\n");
	Books *pTempB = headB;
	Inv *pTempI = headI;
	
	while(headB != NULL)
	{
		
		pTempB = headB->next;
		fprintf(fl, "Freeing strings in Books node\n");
		free(headB->code);
		free(headB->title);
		free(headB->author);
		free(headB->pInv);
		fprintf(fl, "Freeing node Books\n");
		free(headB);
		headB = pTempB;
	}
	
	while(headI != NULL)
	{
		pTempI = headI->next;
		fprintf(fl, "Freeing string in node Inv\n");
		free(headI->code);
		fprintf(fl, "Freeing node Inv\n");
		free(headI);
		headI = pTempI;
	}
}
	
		
void ReadFile(FILE *fl, Books **headB, Inv **headI)
{
	char line[MAX_LENGTH];
	
	fprintf(fl, "Opening list_of_books.txt and warehouse_info.txt for reading\n");
	FILE *fb = fopen(BooksFile, "r");
	FILE *fi = fopen(InvFile, "r");
	if (fb == NULL) {
		fprintf(fl, "list_of_books opening failed\n");
		fprintf(stderr, "Opening failed");
		exit(1);
	}
	if (fi == NULL) {
		fprintf(fl, "warehouse_info opening failed\n");
		fprintf(stderr, "Opening failed");
		exit(1);
	} 
	
	while(fgets(line, LINE_LENGTH, fb) != NULL)
	{
		fprintf(fl, "Declaring node Books\n");
		Books *nodeB = (Books*)malloc(sizeof(Books)); 
		Books *lastB = *headB;
		
		Parse(fl, line, nodeB, *headI, 1);
	
		nodeB->next = NULL; 
		if (*headB == NULL) 
		{ 
			nodeB->prev = NULL; 
			*headB = nodeB; 
		}else
		{
			while (lastB->next != NULL)
			{
				lastB = lastB->next; 
			}
			lastB->next = nodeB; 
			nodeB->prev = lastB; 
		}
	}
	while(fgets(line, LINE_LENGTH, fi) != NULL)
	{
		fprintf(fl, "Declaring node Inv\n");
		Inv *nodeI = (Inv*)malloc(sizeof(Inv)); 
		Inv *lastI = *headI;
		
		Parse(fl, line, *headB, nodeI, 2);
		
		nodeI->next = NULL; 
		if (*headI == NULL) 
		{ 
			nodeI->prev = NULL; 
			*headI = nodeI; 
		}else
		{
			while (lastI->next != NULL)
			{
				lastI = lastI->next; 
			}
			lastI->next = nodeI; 
			nodeI->prev = lastI; 
		}
	}
	
	fprintf(fl, "Closing list_of_books.txt and warehouse_info.txt\n");
	fclose(fb);
	fclose(fi);
}

void PrintFile(FILE *fl, Books *nodeB, Inv *nodeI)
{
	fprintf(fl, "Opening BooksFile and InvFile for writing\n");
	FILE *fb = fopen(BooksFile, "w");
	if (fb == NULL){
		fprintf(stderr, "Opening Books file failed\n");
		exit(1);
	} 
	FILE *fi = fopen(InvFile, "w");
	if (fi == NULL){
		fprintf(stderr, "Opening Inventory file failed\n");
		exit(1);
	} 

	while(nodeB != NULL)
	{
		fprintf(fl, "Writing node Books\n");
		fprintf(fb, "\"%s\",\"%s\",\"%s\",\"%d\",\"%.2lf\",\"%.2lf\"\n",
						 nodeB->code, nodeB->title, nodeB->author, 
						 nodeB->year, nodeB->cost, nodeB->price);
		nodeB = nodeB->next;
	}
	while(nodeI != NULL)
	{
		fprintf(fl, "Writing node Inv\n");
		fprintf(fi, "\"%d\",\"%s\",\"%d\",\"%d\"\n",
								nodeI->ID, nodeI->code, 
								nodeI->shelf, nodeI->stock);
		nodeI = nodeI->next;
	}
	
	fprintf(fl, "Closing Books file and Inv file\n");
	fclose(fb);
	fclose(fi);
}
	
	
	
void PrintList(FILE *fl, Books *nodeB, Inv *nodeI)  
{  
	int i;
	int amount = 0;
	
    while (nodeB != NULL)
    {  
		fprintf(fl, "Displaying node Books\n");
		puts("\t\t  ***");
		printf("%s, %s, %s, %d, %.2lf, %.2lf\n", (nodeB)->code, (nodeB)->title, 
										(nodeB)->author, (nodeB)->year, 
										(nodeB)->cost, (nodeB)->price);	
		fprintf(fl, "%s, %s, %s, %d, %.2lf, %.2lf\n", (nodeB)->code, (nodeB)->title, 
									            	(nodeB)->author, (nodeB)->year, 
										            (nodeB)->cost, (nodeB)->price);	
										
		puts("Related items on warehouse:");
		for (i = 0; i < (nodeB->count); i++)
		{
			fprintf(fl, "Displaying related Inv nodes\n");
			printf("%d, %s, %d, %d\n", (*(nodeB->pInv + i))->ID, (*(nodeB->pInv + i))->code, 
								   (*(nodeB->pInv + i))->shelf, (*(nodeB->pInv + i))->stock);
			fprintf(fl, "%d, %s, %d, %d\n", (*(nodeB->pInv + i))->ID, (*(nodeB->pInv + i))->code, 
								      (*(nodeB->pInv + i))->shelf, (*(nodeB->pInv + i))->stock);
			
			amount += (*(nodeB->pInv + i))->stock;
		}
		if(nodeB->count == 0) puts("No related positions exist");
		else printf("Total amount of copies on a warehouse : %d\n", amount);
		puts("\t\t  ***\n");
		amount = 0;
        nodeB = nodeB->next;  
    }  
    
}  

void EditFunc(FILE *fl, Books *nodeB, Inv *nodeI)
{
	int option, checkSuccess = 0;
	int ID;

	char book_code[MAX_LENGTH];
	char new_author[MAX_LENGTH];

	char *pTemp;
	
	puts("Would You like to edit  1.Book author\n\t\t\tor");
	fprintf(fl, "Visible : Would You like to edit  1.Book author\n\t\t\tor\n");
	puts("\t\t\t2.Inventory stock");
	fprintf(fl, "\t\t\tVisible : 2.Inventory stock\n");
	option = GetInt(fl);
	
	switch(option)
	{
		case 1:			
			printf("Enter the code of the book You would like to edit:");
			fprintf(fl, "Enter the code of the book You would like to edit:\n");
			GetStr(fl, book_code);
			
			while(nodeB != NULL)
			{
				if(strcmp(book_code, nodeB->code) == 0)
				{
					printf("Current book(%s) author's name is : %s\n",
						nodeB->code, nodeB->author);
					fprintf(fl, "Visible : Current book(%s) author's name is : %s\n", 
						nodeB->code, nodeB->author);
						
					puts("Enter new author:");
					fprintf(fl, "Visible : Enter new author:\n");
					GetStr(fl, new_author);
					
					fprintf(fl, "Reallocating for new author\n");
					pTemp = (char*)realloc(nodeB->author, sizeof(char) * (strlen(new_author) + 1));
					if (pTemp == NULL) 
					{
						fprintf(stderr, "Failed\n");
						exit(1);
					} 
					nodeB->author = pTemp;
					
					strcpy(nodeB->author, new_author);
					
					checkSuccess = 1;
					break;
				}
				nodeB = nodeB->next;
			}
			if(checkSuccess == 0)
			{
					puts("Error : No such book code was found");
					fprintf(fl, "Error : No such book code was found\n");
			}
			break;
			
		case 2:
			puts("Enter the inventory ID:");
			fprintf(fl, "Visible : Enter the inventory ID:\n");
			ID = GetInt(fl);
			
			while(nodeI != NULL)
			{
				if(nodeI->ID == ID)
				{
					printf("Current inventory(ID is : %d) stock is : %d\n", 
						nodeI->ID, nodeI->stock);
					fprintf(fl, "Visible : Current inventory(ID is : %d) stock is : %d\n",
						nodeI->ID, nodeI->stock);
						
					puts("Enter new stock value:");
					fprintf(fl, "Visible : Enter new stock value:\n");
					nodeI->stock = GetNum(fl);
		
					checkSuccess = 1;
					break;
				}	
				
				nodeI = nodeI->next;			
			}
			if(checkSuccess == 0)
			{
				puts("Error : No such inventory ID was found");
				fprintf(fl, "Error : No such inventory ID was found\n");
			}
			break;
	}
}

void FindFunc(FILE *fl, Books *nodeB, Inv *nodeI)
{
	int checkSuccessInv;
	int checkSuccessBook;
	int totalNum;
	char author[MAX_LENGTH];
	
	puts("Enter the author :");
	fprintf(fl, "Visible : Enter the author :\n");
	GetStr(fl, author);
	puts("\nBooks by this author:\n");
	fprintf(fl, "\nVisible : Books by this author:\n");
	checkSuccessBook = 0;
	while(nodeB != NULL)
	{
		totalNum = 0;
		checkSuccessInv = 0;
		
		if(strncmp(nodeB->author, author, strlen(author)) == 0)
		{
			puts("\n\t\t***");
			fprintf(fl, "\n\t\t***\n");
			printf("%s, %s, %s, %d, %.2lf, %.2lf\n", nodeB->code, nodeB->title, 
											    nodeB->author, nodeB->year, 
											    nodeB->cost, nodeB->price);
			
			fprintf(fl, "Visible : %s, %s, %s, %d, %.2lf, %.2lf\n", 
									nodeB->code, nodeB->title, 
									nodeB->author, nodeB->year, 
									nodeB->cost, nodeB->price);
			puts("\nInventory associated with this book:\n");
			fprintf(fl, "\nVisible : Inventory associated with this book:\n");
			
			while(nodeI != NULL)
			{
				if((strcmp(nodeB->code, nodeI->code) == 0) && (nodeI->stock != 0))
				{
					printf("%d, %s, %d, %d\n", nodeI->ID, nodeI->code, 
											   nodeI->shelf, nodeI->stock);
											
					fprintf(fl, "Visible : %d, %s, %d, %d\n", nodeI->ID, nodeI->code, 
															  nodeI->shelf, nodeI->stock);
					
				totalNum += nodeI->stock;
				checkSuccessInv = 1;
				}
				nodeI = nodeI->next;
			}
			if(checkSuccessInv == 0)
			{
				puts("No such books was found in the inventory");
				fprintf(fl, "Visible : No such books was found in the inventory\n");
			}else{
				printf("\nInventory contains %d pieces of this book\n", totalNum);
				fprintf(fl, "\nVisible : Inventory contains %d pieces of this book\n", totalNum);
			}
			
			checkSuccessBook = 1;
		}
		
		nodeB = nodeB->next;
	}
	if(checkSuccessBook == 0)
	{
		puts("No books associated with the author");
		fprintf(fl, "Visible : No books associated with the author\n");
	}else{
		puts("\t\t***");
		fprintf(fl, "\t\t***\n");
	}
}

void AddBook(FILE *fl, Books *nodeB)
{
	int i;
	int codeNum = 0;
	
	char field[MAX_LENGTH];
	
	fprintf(fl, "Declaring and allocating new node\n");
	Books *new_node = (Books*)malloc(sizeof(Books));
	if(new_node == NULL){
		fprintf(fl, "Declaring was not successfull\n");
		exit(1);
	}
	new_node->next = NULL;
	
	while(nodeB->next != NULL)
	{
		fprintf(fl, "Transfering to last node\n");
		nodeB = nodeB->next;
	}
	
	fprintf(fl, "Attaching new node to list\n");
	nodeB->next = new_node;
	new_node->prev = nodeB;
	
	nodeB = nodeB->next;
	
	fprintf(fl, "Filling the node\n");
	for (i = 0; i < strlen((nodeB->prev)->code); i++)
	{
		if(isdigit((nodeB->prev)->code[i]) != 0)
		{
			codeNum = codeNum * 10 + ((nodeB->prev)->code[i] - '0');
		}
	}
	codeNum++;
	strcpy(field, "Book Code");
	sprintf(field, "%s %d", field, codeNum);
	Flush(fl, &(nodeB)->code, field);
	
	puts("Enter title for the new book:");
	GetStr(fl, field);
	Flush(fl, &(nodeB)->title, field);
	
	puts("Enter author of the new book:");
	GetStr(fl, field);
	Flush(fl, &(nodeB)->author, field);
	
	puts("Enter the year of the new book:");
	nodeB->year = GetInt(fl);
	
	puts("Enter self-cost of the new book:");
	nodeB->cost = GetNum(fl);

	
	puts("Enter selling price of the new book:");
	nodeB->price = GetNum(fl);
	
}
	
void AddInv(FILE *fl, Books *nodeB, Inv *nodeI)
{
	char field[MAX_LENGTH];
	int checkSucess;
	
	fprintf(fl, "Declaring and allocating new node\n");
	Books *headB = nodeB;
	Inv *headI = nodeI;
	
	Inv *new_node = (Inv*)malloc(sizeof(Inv));
	if(new_node == NULL) exit(1);
	
	new_node->next = NULL;
	
	while(nodeI->next != NULL) nodeI = nodeI->next;
	
	fprintf(fl, "Attaching new node to end of the list\n");
	nodeI->next = new_node;
	new_node->prev = nodeI;
	
	nodeI = nodeI->next;
	
	nodeI->ID = (nodeI->prev->ID + 1);
	
	do
	{
		checkSucess = 0;
		puts("Enter book code for the new position:");
		fprintf(fl, "Visible : Enter book code for the new position:\n");
		GetStr(fl, field);
		while(nodeB != NULL)
		{
			if(strcmp(nodeB->code, field) == 0)
			{
				checkSucess = 1;
			}
			nodeB = nodeB->next;
		}
		nodeB = headB;
		if(checkSucess == 0){
			puts("There are no books with such code, try another one");
			fprintf(fl, "Visible : There are no books with such code, try another one\n");
		}
	}while(checkSucess == 0);
	
	Flush(fl, &(nodeI)->code, field);
	
	fprintf(fl, "Visible : Enter shelf number of the position\n");
	puts("Enter shelf number of the position:");
	nodeI->shelf = GetInt(fl);
	
	fprintf(fl, "Visible : Enter stock value of the position\n");
	puts("Enter stock value of the position:");
	nodeI->stock = GetInt(fl);
	
	LinkStructs(fl, headB, headI);
}
	
void RemoveBook(FILE *fl, Books *nodeB)
{
	int i;
	char book_code[MAX_LENGTH];
	
	puts("Enter the code of the book You would like to remove:");
	fprintf(fl, "Visible : Enter the code of the book You would like to remove:\n");
	GetStr(fl, book_code);
	while(nodeB != NULL)
	{
		fprintf(fl, "Searching for related book\n");
		if(strcmp(nodeB->code, book_code) == 0)
		{
			fprintf(fl, "Related book found\n");
			fprintf(fl, "Excluding the related node from list\n");
			(nodeB->prev)->next = nodeB->next;
			(nodeB->next)->prev = nodeB->prev;
			for (i = 0; i < nodeB->count; i++)
			{
				fprintf(fl, "Freeing array of pointers to Inv node in Books node\n");
				if((nodeB->pInv[i])->next != NULL)
				{
					((nodeB->pInv[i])->prev)->next = (nodeB->pInv[i])->next;
					((nodeB->pInv[i])->next)->prev = (nodeB->pInv[i])->prev;
				}else
				{
					((nodeB->pInv[i])->prev)->next = NULL;
				}
				free((nodeB->pInv[i])->code);
				free(nodeB->pInv[i]);
			}
			
			fprintf(fl, "Freeing Books node\n");
			free(nodeB->pInv);
			free(nodeB->code);
			free(nodeB->title);
			free(nodeB->author);
			free(nodeB);
			break;
		}
		nodeB = nodeB->next;
	}
}
	
void RemoveInv(FILE *fl, Books *nodeB, Inv *nodeI)
{
	int i;
	int ID;
	
	fprintf(fl, "Visible : Enter the ID of the position You would like to remove:\n");
	puts("Enter the ID of the position You would like to remove:");
	ID = GetInt(fl);
	
	while(nodeI != NULL)
	{
		fprintf(fl, "Searching for related Inv node:\n");
		if(ID == nodeI->ID)
		{
			fprintf(fl, "Excluding found node\n");
			(nodeI->prev)->next = nodeI->next;
			(nodeI->next)->prev = nodeI->prev;
			
			while(nodeB != NULL)
			{
				if(strcmp(nodeB->code, nodeI->code) == 0)
				{
					for (i = 0; i < nodeB->count; i++)
					{
						if(nodeB->pInv[i] == nodeI)
						{
							nodeB->count = nodeB->count - 1;
							while(i < (nodeB->count))
							{
								fprintf(fl, "Reforming the order of pointers in array in books node\n");
								nodeB->pInv[i] = nodeB->pInv[i+1];
								i++;
							}
						}
					}
				}
				nodeB = nodeB->next;
			}
			
			fprintf(fl, "Freeing code and the node\n");
			free(nodeI->code);
			free(nodeI);
			break;
		}
		nodeI = nodeI->next;
	}
}
		
		
		
		
		
		
