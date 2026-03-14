#include <cstddef>
#include <cstdlib>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define test_length 8
#define string_size 20

typedef struct Node {
	char data[string_size];
	struct Node* next_ptr;
	struct Node* prev_ptr;
} Node;

Node* make_dl_list(int length);
Node* peek_dl_list(Node* pos, const char direction[]="forward");
Node* walk_dl_list(Node* walking_pointer, int index, const char direction[]="forward");
Node* dl_list_find(Node* walking_pointer, const char string[]);

void dl_list_insert(Node* walking_pointer, int index, const char string[]);
void dl_list_remove(Node* walking_pointer, const char string[]);

void free_dl_list(Node* start);

char* dl_list2str(Node* start);
void show_linked_list(Node* start);


int main()
{
	Node* test_list = make_dl_list(test_length);
	show_linked_list(test_list);

	dl_list_insert(test_list, 4, "in");
	dl_list_remove(test_list, "3");

	char* string = dl_list2str(test_list);
	printf("\nfinal string: %s\n", string);

	char item[] = "2";
	Node* item_loc = dl_list_find(test_list, item);
	printf("\nfound item: '%s' pointer: %p\n", item, item_loc);

	free(string);
	free_dl_list(test_list);
	getchar();
	return 0;
}

Node* make_dl_list(int length) {
	Node* head = NULL;
	Node* tail = NULL;
	for (size_t i = 0; i < length; i++)
	{
		
		Node* walking_pointer = (Node*)calloc(1, sizeof *walking_pointer);

		if (!walking_pointer) return NULL;
		if (!head) tail = walking_pointer;

		printf("\nEnter data for node %zu: ", i);
		fgets(walking_pointer->data, sizeof walking_pointer->data, stdin);
		walking_pointer->data[strcspn(walking_pointer->data, "\n")] = '\0';
		
		walking_pointer->prev_ptr = head;
		if (head)
			walking_pointer->prev_ptr->next_ptr = walking_pointer;
		
		head = walking_pointer;
	}
	return tail;
}
Node* peek_dl_list(Node* pos, const char direction[]) {
	printf("\nwalking pointer: %p | value: %s | previous pointer %p | next pointer: %p\n",
			(void*)pos, pos->data, pos->prev_ptr, pos->next_ptr);
	
	if (strcmp(direction,"forward") == 0)
	{
		pos = pos->next_ptr;
	}
	else if (strcmp(direction,"backward") == 0)
	{
		pos = pos->prev_ptr;
	}	
	printf("pointer '%p' walked %s\n", pos, direction);
	return pos;
}
Node* walk_dl_list(Node* walking_pointer, int index, const char direction[]) {
	for (size_t i = 0; i<index; i++) {
		walking_pointer = peek_dl_list(walking_pointer, direction);
	}
	return walking_pointer;
}
Node* dl_list_find(Node* walking_pointer, const char string[]) {
	/*returns nullpointer if not found*/
	char direction[] = "forward";
	printf("\nexecuting %s search\n", direction);
	if (walking_pointer->prev_ptr==NULL) {
		/*execute search in forward direction*/
		while (walking_pointer!=NULL && strcmp((const char*)walking_pointer->data, string) != 0)
		{
			walking_pointer = peek_dl_list(walking_pointer, "forward");
		}
	}
	else {
		/*execute search in both directions*/
		Node* start_pointer = walking_pointer;
		while (walking_pointer!=NULL && strcmp((const char*)walking_pointer->data, string) != 0)
		{	
			if (walking_pointer->next_ptr==NULL)
			{
				strcpy_s(direction, "backward");
				printf("\nexecuting %s search\n", direction);
				walking_pointer = start_pointer;
			}
			walking_pointer = peek_dl_list(walking_pointer, direction);
		}
	}
	return walking_pointer;
}
void dl_list_insert(Node* walking_pointer, int index, const char string[]) {
	// loc == [x, x+1, x+2]
	walking_pointer = walk_dl_list(walking_pointer, index);

	// set loc[x+1]->prev/next_ptr/data and allocate memory
	Node* inserted_item = (Node*)calloc(1, sizeof *walking_pointer);
	strcpy_s(inserted_item->data, string);
	inserted_item->prev_ptr = walking_pointer;
	inserted_item->next_ptr = walking_pointer->next_ptr;
	
	// set loc[x+2]->prev_ptr
	walking_pointer->next_ptr->prev_ptr = inserted_item;

	//set loc[x]->_next_ptr
	walking_pointer->next_ptr = inserted_item;
}
void dl_list_remove(Node* walking_pointer, const char string[]) {
	// loc == [x, x+1, x+2]
	walking_pointer = dl_list_find(walking_pointer, string);
	// set loc[x]->next_ptr
	walking_pointer->prev_ptr->next_ptr = walking_pointer->next_ptr;
	// set loc[x]->next_ptr
	walking_pointer->next_ptr->prev_ptr = walking_pointer->prev_ptr;

	free(walking_pointer);
}
void free_dl_list(Node* start) {
    Node* tmp;
    while (start) {
        tmp = start->next_ptr;
        free(start);
        start = tmp;
    }
	free(tmp);
}
void show_linked_list(Node* start) {
	int i = 0;
	while(start)
	{
		printf("\nslot: %d", i++);
		printf("\npointer: %p | value: %s | previous pointer %p | next pointer: %p\n",
				 (void*)start, start->data, start->prev_ptr, start->next_ptr);
		start = start->next_ptr;
	}
}
char* dl_list2str(Node* start) {
	if (!start) return NULL;
	size_t str_length = strlen(start->data);
	char *str = (char*)malloc(str_length + 1);
	if (!str) return NULL;

	strcpy_s(str, str_length + 1, start->data);

	start = start->next_ptr;
	while (start != NULL) {
		
		str_length += strlen(start->data);
		char* buffer = (char*)realloc(str, str_length + 1);
		if (!buffer) {
			free(str);
			return NULL;
		}
		str = buffer;

		strcat(str, start->data);
		start = start->next_ptr;
	}
	return str;
}