#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "md5.h" //using void md5String(const char* input, char* output);
#include "md4.h" //using void md4String(const char* input, char* output);

#define MD4_HASH 0
#define MD5_HASH 1
#define True 1
#define False 0
typedef unsigned char HashType;
typedef unsigned char Byte;
typedef char Bool;

typedef struct msgWithHash
{
	unsigned int id;
	char* text;
	union
	{
		Byte md4[16];
		Byte md5[16];
	} hashDigest;
	HashType type;
}msgWithHash;

typedef struct msgListNode msgListNode;
struct msgListNode
{
	msgWithHash* data;
	msgListNode* next;
};

void print_hash(const unsigned char* p);
msgWithHash* createMessage(unsigned int id, const char* text, HashType type)
{
	msgWithHash *new_message = (msgWithHash*)malloc(sizeof(msgWithHash));
	new_message->text = (char*)malloc(sizeof(char)*(strlen(text)+1));
	new_message->id = id;
	strcpy(new_message->text, text);
	new_message->type = type;
	if (new_message->type==MD4_HASH)
	{
		md4String(new_message->text, new_message->hashDigest.md4);
	}
	else
	{
		md5String(new_message->text, new_message->hashDigest.md5);
	}
	return new_message;
}

void printMessage(const msgWithHash* message)
{
		printf("Message id: %lu\n", message->id);
		printf("message text: %s\n", message->text);
		if (message->type==MD4_HASH)
		{
			print_hash(message->hashDigest.md4);
		}
		else
		{
			print_hash(message->hashDigest.md5);
		}
	
}
msgListNode* addMessage(msgListNode* head, msgWithHash* data)
{
	msgListNode* new_node = (msgListNode*)malloc(sizeof(msgListNode));
	if (!new_node)
	{
		printf("allocation failed");
		exit(1);
	}
	if (head == NULL)
	{
		new_node->next = NULL;
		return new_node;
	}
	msgListNode* curr = head;
	msgListNode* ptr = NULL;
	while (curr != NULL && curr->data->id < data->id)
	{
		ptr = curr;
		curr = curr->next;
	}
	new_node->next = curr;

	if (ptr)
	{
		ptr->next = new_node;
	}
	else
	{
		head = new_node;
	}
	return head;

}
int findMsgByHashIterative(const msgListNode* head, Byte hash[16])
{
	int location = 1;
	while (head->next != NULL)
	{
		if (head->data->type == MD4_HASH)
		{
			for (int i = 0; i < 16; i++)
			{
				if (head->data->hashDigest.md4[i] != hash[i])
				{
					break;
				}
				else
				{
					return location;
				}

			}
		}
		else
		{
			for (int i = 0; i < 16; i++)
			{
				if (head->data->hashDigest.md5[i] != hash[i])
				{
					break;
				}
				else
				{
					return location;
				}

			}
		}
		location++;
		head = head->next;
	}

	return -1;

}
int findMsgByHashRecursive(const msgListNode* head, Byte hash[16]);
msgListNode* deleteMessage(msgListNode* head, unsigned int id)
{
	if (head == NULL)
	{
		return head;
	}
	msgListNode* curr = head;
	if (curr->data->id == id && head->next==NULL)
	{
		head = head->next;
		free(curr);
		return head;
	}
	msgListNode* prev = NULL;
	while (curr != NULL && curr->data->id != id)
	{
		prev = curr;
		curr = curr->next;
	}
	if (curr != NULL)
	{
		prev->next = curr->next;
		free(curr);
	}
	return head;
}
Bool verifyIntegrity(msgListNode* head, unsigned int id, Byte compareHash[16])
{
	while (head->next != NULL)
	{
		if (head->data->id == id)
		{
			if (head->data->type == MD4_HASH)
			{
				for (int i = 0; i < 16; i++)
				{
					if (head->data->hashDigest.md4[i] != compareHash[i])
					{
						return 1;
					}
					else
					{
						return -1;
					}

				}
			}
			else
			{
				for (int i = 0; i < 16; i++)
				{
					if (head->data->hashDigest.md5[i] != compareHash[i])
					{
						return 1;
					}
					else
					{
						return -1;
					}

				}
			}

			head = head->next;
		}
	}

		return 0;
}
msgListNode* reverseMsgList(msgListNode* head)
{
	msgListNode* prev = NULL;
	msgListNode* cur = head;
	msgListNode* temp;
	while (cur)
	{
		temp = cur->next;
		cur->next = prev;
		prev = cur;
		cur = temp;
	}
	return prev;
}

void freeMsgList(msgListNode* head)
{
	msgListNode* curr;
	curr = head;
	while (head->next != NULL)
	{
		free(curr);
		head = head->next;
		curr = head;
	}
}



typedef struct Item
{
	Point point;
	struct Item* next;
}Item;

typedef struct
{
	int x;
	int y;
}Point;

void mystery(Item** head)
{
	Item* p, * q, * prev, * temp;
	p = q = prev = *head;
	for (; p != NULL; p = p->next)
	{
		q = p->next;
		while (q != NULL)
		{
			if (q->point.x == p->point.x && q->point.y == p->point.y)
			{
				prev->next = q->next;
				temp = q;
				free(temp);
				q = prev;
			}
			else prev = q;
			q = q->next;
		}
	}
}


int main() {
	
	Item* new;
	new->point.x = 2;
	new->point.y = 3;
	mystery(new);

	/******************** Example 1 from PDF ********************************************/
	
	/*const char* sentence = "this is some text that I want to hash.";
	printf("text: \"%s\"\n", sentence);
	unsigned char hash[16] = { 0 };
	md5String(sentence, hash);
	printf("md5: ");
	print_hash(hash);

	printf("md4: ");
	md4String(sentence, hash);
	print_hash(hash);*/
	
	/******************** Example 2 from PDF ********************************************/

	//const msgWithHash* msg1 = createMessage(87651233, "", MD4_HASH);
	//const msgWithHash* msg2 = createMessage(3, "banana1", MD5_HASH);
	//const msgWithHash* msg3 = createMessage(275, "banana2!", MD4_HASH);
	//const msgWithHash* msg4 = createMessage(342234, "Hello World", MD4_HASH);

	//printMessage(msg1);
	//printMessage(msg2);
	//printMessage(msg3);
	//printMessage(msg4);

}

void print_hash(const unsigned char* p) {
	for (unsigned int i = 0; i < 16; ++i) {
		printf("%02x", p[i]);
	}
	printf("\n");
}