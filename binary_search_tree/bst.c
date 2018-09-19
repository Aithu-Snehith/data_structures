// WORKS FOR ARBITARY LENGTH NUMBER
#include <stdio.h>
#include <stdlib.h>

struct linkedlist_node
{
  int value;
  struct linkedlist_node* next ;
  struct linkedlist_node* prev ;
};

struct bst_node
{
	struct bst_node* left;
	struct bst_node* right;
	struct bst_node* parent;
	struct linkedlist_node* number;
};

struct linkedlist_node* num_head = NULL;
struct bst_node* tree_head = NULL;

// making the linked list free of memory
struct linkedlist_node* free_list(struct linkedlist_node* head)
{
  struct linkedlist_node* temp = NULL;
  while(head != NULL)
  {
    temp = head->prev;
    free(head);
    head = temp;
  }
  return head;
}

// freeing the memory of the tree using post order traversal
void free_tree(struct bst_node* tree_head)
{
	if(tree_head == NULL)	return;
	free_tree(tree_head->left);
	free_tree(tree_head->right);
	free_list(tree_head->number);
	free(tree_head);
}

// printing the number stored in a linked list
void print_number(struct linkedlist_node* num)
{
	while(num->prev != NULL)	num = num->prev;
	while(num != NULL)
	{
		printf("%d",num->value);
		num = num->next;
	}
	printf(" ");
}

// printing tree using pre-order traversal
void preorder_print_tree(struct bst_node* tree_head)
{
	if(tree_head == NULL)	return;
	print_number(tree_head->number);
	preorder_print_tree(tree_head->left);
	preorder_print_tree(tree_head->right);
}

// inserting a digit in a linked list to store a number
struct linkedlist_node* insert_linkedlist(int digit , struct linkedlist_node* head)
{
  struct linkedlist_node* temp = (struct linkedlist_node*)malloc(sizeof(struct linkedlist_node));
  // For initializing the linked list
  if(head == NULL)
  {
    temp->value = digit;
    temp->next = NULL;
    temp->prev = NULL;
    head = temp;
  } 
  // For adding a node to the list
  else
  {
    temp->value = digit;
    head->next = temp;
    temp->prev = head;
    temp->next = NULL;
    head = temp;
  }
  return head;
}

// comparing the two numbers stored in linked list , return 0 for equal , 1 for greater than , -1 for less than
int compare(struct linkedlist_node* head_1 , struct linkedlist_node* head_2)
{
  while(!(head_1 ==NULL && head_2 == NULL))
  {
  	// comparing the length of each number
    if(head_1 == NULL && head_2 != NULL)  return -1;
    else if(head_1 != NULL && head_2 == NULL) return 1;
    else if(head_1->prev == NULL && head_2->prev == NULL)
    {
      while(head_2 != NULL)
      {
        // comparing the digits if the lengths are equal
        if(head_1->value > head_2->value) return 1;
        else if(head_1->value < head_2->value)  return -1;
        else
        {
          head_1 = head_1->next;
          head_2 = head_2->next;  
        }
      }
    }
    else
    {
      head_1 = head_1->prev;
      head_2 = head_2->prev;
    }
  }
  return 0;
}

// inserting a number in a binary search tree
struct bst_node* insert_tree(struct linkedlist_node* num_head , struct bst_node* tree_head)
{
	struct bst_node* temp = (struct bst_node*)malloc(sizeof(struct bst_node));
	int compared , inserted = 0;
	if(tree_head == NULL)
	{
		temp->number = num_head;
		temp->parent = NULL;
		temp->left = NULL;
		temp->right = NULL;
		tree_head = temp;
		// printf("inserting to root\n");
	}
	else
	{
		// looping while it is inserted
		while(inserted == 0)
		{
			compared = compare(num_head , tree_head->number);
			if(compared == 1)
			{
				// if number to be inserted is greater , traverse right
				if(tree_head->right != NULL)
				{
					tree_head = tree_head->right;	// moving right
				}
				else
				{
					// inserting to right 
					struct bst_node* new_temp = (struct bst_node*)malloc(sizeof(struct bst_node));
					new_temp->number = num_head;
					new_temp->parent = tree_head;
					tree_head->right = new_temp;
					new_temp->left = NULL;
					new_temp->right = NULL;
					tree_head = new_temp;
					inserted =1;
				}
			}
			else if(compared == -1)
			{
				// if number to be inserted is greater , traverse right
				if(tree_head->left != NULL)
				{
					tree_head = tree_head->left;	// moving left
				}
				else
				{
					// inserting at the left
					struct bst_node* new_node = (struct bst_node*)malloc(sizeof(struct bst_node));
					new_node->number = num_head;
					new_node->parent = tree_head;
					tree_head->left = new_node;
					new_node->left = NULL;
					new_node->right = NULL;
					tree_head = new_node;
					inserted = 1;
				}
			}
			else
			{
				inserted = 1;	// if number already there in list , do nothing
			}
		}
	}
	while(tree_head->parent != NULL)	tree_head = tree_head->parent;	//point head to root
	return tree_head;
}

// searching in the tree
void search_tree(struct linkedlist_node* num_head , struct bst_node* tree_head)
{
	int compared;
	if(tree_head == NULL)	printf("-1\n");	
	else
	{
		while(tree_head != NULL)
		{
			compared = compare(num_head , tree_head->number);
			// if  number given is larger , traverse right
			if(compared == 1)
			{
				if(tree_head->right != NULL)	printf("1");
				else
				{
					// if number not found, erase the path(did using escape sequences) printed, and print -1
					printf("\33[2K\r-1\n");
					return;
				}
				tree_head = tree_head->right;
			}
			else if(compared == -1)
			{
				if(tree_head->left != NULL)	printf("0");
				else
				{
					// if number not found, erase the path (did using escape sequences) printed, and print -1
					printf("\33[2K\r-1\n");
					return;
				}
				tree_head = tree_head->left;
			}
			else if(compared == 0)
			{
				printf("\n");
				return;
			}
		}
	}
}

int main()
{
	int digit ,tree_active = 0, search = 0 , lead_zero = 1;
	while(((digit = fgetc(stdin)) != EOF))
	{
		if(digit == 'N')
		{
			tree_active = 1;	//identifing if number is being inserted in tree
			search = 0;
			lead_zero = 1;
			free_tree(tree_head);
			tree_head = NULL;
			if(num_head != NULL)	num_head = free_list(num_head);
		}
		else if((digit == ' ' || digit == '\n') && tree_active == 1 && num_head != NULL)
		{
			
			tree_head = insert_tree(num_head , tree_head);	//inserting number stored in linked list into tree
			num_head = NULL;
			lead_zero = 1;
			if(digit == '\n')	tree_active = 0;

		}
		else if(tree_active == 1 && !(digit == ' ' || digit == '\n'))
		{
			if(!(digit == '0' && lead_zero == 1))   //taking care of leading zeros in the input
      		{
				num_head = insert_linkedlist(digit-48 , num_head);	//iserting digits into linked list
				lead_zero = 0;
			}
		}
		else if(digit == 'P')
		{
			preorder_print_tree(tree_head);		//printing the list in pre order traversal
			printf("\n");
		}
		else if(digit == 'S')	
		{
			search = 1;
			tree_active = 0;
			num_head = NULL;
			lead_zero = 1;
		}
		else if(search == 1 && !(digit == ' ' || digit == '\n'))
		{
			if(!(digit == '0' && lead_zero == 1))   //taking care of leading zeros in the input
      		{
				num_head = insert_linkedlist(digit-48 , num_head);	//storing number to serach in the tree
				lead_zero = 0;
			}
		}
		else if(search == 1 && digit == '\n')
		{
			search_tree(num_head , tree_head);	//searching in tree
			search = 0;
		}
	}
}