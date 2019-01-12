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

// // function to find the sucessor of the given function
struct bst_node* find_succesor(struct linkedlist_node* num_head , struct bst_node* tree_head)
{
	// struct linkedlist_node* succ = NULL;
	int compared;
	if(tree_head == NULL)	return NULL;
	else
	{
		while(tree_head != NULL)
		{
			compared = compare(num_head , tree_head->number);
			// if  number given is larger , traverse right
			if(compared == 1)
			{
				if(tree_head->right == NULL)
				{
					while(tree_head->parent != NULL)
					{
						if(tree_head->parent->left == tree_head)	return tree_head->parent;
						else	tree_head = tree_head-> parent;
					}
					return NULL;
				}
				tree_head = tree_head->right;
			}
			else if(compared == -1)
			{
				if(tree_head->left == NULL)
				{
					return tree_head;
				}
				tree_head = tree_head->left;
			}
			else if(compared == 0)
			{// case 1 ; where it has right child
				if(tree_head->right != NULL)
				{
					tree_head = tree_head->right;
					while(tree_head->left != NULL)	tree_head = tree_head->left;
					return tree_head;
				}
				else
				{// case 2 : where there is no right child
					while(tree_head->parent != NULL)
					{
						if(tree_head->parent->left == tree_head)	return tree_head->parent;
						else	tree_head = tree_head-> parent;
					}
					return NULL;
				}
			}
		}
	}
}
// function to delete a specific node which has no children
struct bst_node* delete_node(struct bst_node* tree_head , int parent_right)
{
	struct bst_node* temp = NULL;
	temp = tree_head;
	tree_head = tree_head->parent;
	if(parent_right == 1)	tree_head->right = NULL;	// taking care of null pointers
	else	tree_head->left = NULL;
	while(tree_head->parent != NULL)	tree_head = tree_head->parent;
	free_list(temp->number);
	free(temp);
	return tree_head;
}
// function to splice a given node
struct bst_node* splice_node(struct bst_node* tree_head , int child_right , int parent_right)
{
	struct bst_node* temp = NULL;
	temp = tree_head;
	tree_head = tree_head->parent;

	// taking care of four possible cases which can occur
	if(child_right == 1 && parent_right == 1)	tree_head->right = tree_head->right->right;
	else if(child_right == 1 && parent_right == 0)	tree_head->left = tree_head->left->right;
	else if(child_right == 0 && parent_right == 0)	tree_head->left = tree_head->left->left;
	else if(child_right == 0 && parent_right == 1)	tree_head->right = tree_head->right->left;
	while(tree_head->parent != NULL)	tree_head = tree_head->parent;
	free_list(temp->number);
	free(temp);
	return tree_head;
}
// base function to delete a node in the tree using sucessor
struct bst_node* delete_from_tree(struct linkedlist_node* num_head , struct bst_node* tree_head)
{
	int compared;
	if(tree_head == NULL)	return tree_head;
	else
	{
		while(tree_head != NULL)
		{
			compared = compare(num_head , tree_head->number);
			// if  number given is larger , traverse right
			if(compared == 1)
			{// doing nothing if number doesn't exists
				if(tree_head->right == NULL)
				{
					while(tree_head->parent != NULL)	tree_head = tree_head->parent;
					return	tree_head;
				}
				tree_head = tree_head->right;
			}
			else if(compared == -1)
			{
				if(tree_head->left == NULL)
				{
					while(tree_head->parent != NULL)	tree_head = tree_head->parent;
					return	tree_head;
				}
				tree_head = tree_head->left;
			}
			else if(compared == 0)
			{
				if(tree_head->left == NULL && tree_head->right == NULL)	// case 1: no children
				{
					if(tree_head->parent->right == tree_head)	return delete_node(tree_head , 1);
					else	return delete_node(tree_head , 0);
				}
				else if(tree_head->left == NULL && tree_head->right != NULL)	// case 2: one children and taking care of all posibilities of splicing
				{
					if(tree_head->parent->right == tree_head)
					{
						return	splice_node(tree_head , 1 , 1);
					}
					else	return splice_node(tree_head , 1 , 0);
				}
				else if(tree_head->left != NULL && tree_head->right == NULL)
				{
					if(tree_head->parent->left == tree_head)
					{
						return	splice_node(tree_head , 0 , 0);
					}
					else	return splice_node(tree_head , 0 , 1);
				}
				else if(tree_head->left != NULL && tree_head->right != NULL)	// case 3: if it has both the children
				{
					struct bst_node* sucsr = find_succesor(tree_head->number , tree_head);
					struct linkedlist_node* temp = NULL;
					temp = tree_head->number;
					tree_head->number = sucsr->number;
					sucsr ->number = NULL;

					if(sucsr->left == NULL && sucsr->right == NULL)
					{
						if(sucsr->parent->right == sucsr)	return delete_node(sucsr , 1);
						else	return delete_node(sucsr , 0);
					}

					else if(sucsr->left == NULL && sucsr->right != NULL)
					{
						if(sucsr->parent->right == sucsr)
						{
							return	splice_node(sucsr , 1 , 1);
						}
						else	return splice_node(sucsr , 1 , 0);
					}
					else if(sucsr->left != NULL && sucsr->right == NULL)
					{
						if(sucsr->parent->left == sucsr)
						{
							return	splice_node(sucsr , 0 , 0);
						}
						else	return splice_node(sucsr , 0 , 1);
					}
				}
			}
		}
	}
}

// finding the children and printing them
void find_child(struct linkedlist_node* num_head , struct bst_node* tree_head)
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
				if(tree_head->right == NULL)
				{
					// if number not found, print -1
					printf("-1\n");
					return;
				}
				tree_head = tree_head->right;
			}
			else if(compared == -1)
			{
				if(tree_head->left == NULL)
				{
					// if number not found, and print -1
					printf("-1\n");
					return;
				}
				tree_head = tree_head->left;
			}
			else if(compared == 0)
			{
				if(tree_head->left != NULL)	print_number(tree_head->left->number);	//printing left child
				else	printf("Nil ");
				if(tree_head->right != NULL)	print_number(tree_head->right->number);  //printing right child
				else	printf("Nil");
				printf("\n");
				return;
			}
		}
	}
}

void find_uncle(struct linkedlist_node* num_head , struct bst_node* tree_head)
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
				if(tree_head->right == NULL)
				{
					// if number not found, print -1
					printf("-1\n");
					return;
				}
				tree_head = tree_head->right;
			}
			else if(compared == -1)
			{
				if(tree_head->left == NULL)
				{
					// if number not found, and print -1
					printf("-1\n");
					return;
				}
				tree_head = tree_head->left;
			}
			else if(compared == 0)
			{
				if(tree_head->parent != NULL)
				{
					tree_head = tree_head->parent;
					if(tree_head->parent != NULL)
					{
						if(tree_head->parent->left == tree_head)	//checking which child the node is?
						{
							if(tree_head->parent->right != NULL)
							{
								print_number(tree_head->parent->right->number);	//printing the uncle
								printf("\n");
							}
							else	printf("-1\n");
							return;
						}
						else
						{
							if(tree_head->parent->left != NULL)
							{
								print_number(tree_head->parent->left->number);
								printf("\n");
							}
							else	printf("-1\n");
							return;
						}
					}
					else	printf("-1\n");	//if parent is root
				}
				else	printf("-1\n");	//if number is root
				return;
			}
		}
	}
}

// function to rotate left
struct bst_node* rotate_left(struct linkedlist_node* num_head , struct bst_node* tree_head)
{
	int compared;
	if(tree_head == NULL)	return tree_head;
	else
	{
		while(tree_head != NULL)
		{
			compared = compare(num_head , tree_head->number);
			// if  number given is larger , traverse right
			if(compared == 1)
			{// doing nothing if number doesn't exists
				if(tree_head->right == NULL)
				{
					while(tree_head->parent != NULL)	tree_head = tree_head->parent;
					return	tree_head;
				}
				tree_head = tree_head->right;
			}
			else if(compared == -1)
			{
				if(tree_head->left == NULL)
				{
					while(tree_head->parent != NULL)	tree_head = tree_head->parent;
					return	tree_head;
				}
				tree_head = tree_head->left;
			}
			else if(compared == 0)
			{
				if(tree_head->right != NULL)
				{
					struct bst_node* temp = tree_head->right;
					tree_head->right = temp->left;
					if(temp->left != NULL)	temp->left->parent = tree_head;
					temp->parent = tree_head->parent;	//updating parent pointers
					if(tree_head->parent != NULL && tree_head->parent->left == tree_head)	tree_head->parent->left = temp;
					else if(tree_head->parent != NULL && tree_head->parent->right == tree_head)	tree_head->parent->right = temp;
					temp->left = tree_head;	//updating children pointers
					tree_head->parent = temp;
				}
				while(tree_head->parent != NULL)	tree_head = tree_head->parent;
				return	tree_head;
			}
		}
	}
}

//function to rotate right, similar to the above code
struct bst_node* rotate_right(struct linkedlist_node* num_head , struct bst_node* tree_head)
{
	int compared;
	if(tree_head == NULL)	return tree_head;
	else
	{
		while(tree_head != NULL)
		{
			compared = compare(num_head , tree_head->number);
			// if  number given is larger , traverse right
			if(compared == 1)
			{// doing nothing if number doesn't exists
				if(tree_head->right == NULL)
				{
					while(tree_head->parent != NULL)	tree_head = tree_head->parent;
					return	tree_head;
				}
				tree_head = tree_head->right;
			}
			else if(compared == -1)
			{
				if(tree_head->left == NULL)
				{
					while(tree_head->parent != NULL)	tree_head = tree_head->parent;
					return	tree_head;
				}
				tree_head = tree_head->left;
			}
			else if(compared == 0)
			{
				if(tree_head->left != NULL)
				{
					struct bst_node* temp = tree_head->left;
					tree_head->left = temp->right;
					if(temp->right != NULL)	temp->right->parent = tree_head;
					temp->parent = tree_head->parent;
					if(tree_head->parent != NULL && tree_head->parent->left == tree_head)	tree_head->parent->left = temp;
					else if(tree_head->parent != NULL && tree_head->parent->right == tree_head)	tree_head->parent->right = temp;
					temp->right = tree_head;
					tree_head->parent = temp;
				}
				while(tree_head->parent != NULL)	tree_head = tree_head->parent;
				return	tree_head;
			}
		}
	}
}

//function i=to insert using preorder traversal
struct bst_node* insert_tree_preorder(struct linkedlist_node* num_head , struct bst_node* tree_head)
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
		compared = compare(num_head , tree_head->number);
		if(compared == -1)	//if less than directly insert at the left
		{
			// inserting at the left
			struct bst_node* new_node = (struct bst_node*)malloc(sizeof(struct bst_node));
			new_node->number = num_head;
			new_node->parent = tree_head;
			tree_head->left = new_node;
			new_node->left = NULL;
			new_node->right = NULL;
			tree_head = new_node;
			return tree_head;	//returning the pointer to latest node
		}
		else if(compared = 1)
		{
			struct bst_node* temp = tree_head->parent;
			if(temp != NULL)
			{
				while(compare(num_head , temp->number) == 1)
				{
					if(temp->parent != NULL)	temp = temp->parent;
					else
					{
						tree_head = temp;	//if the node is root
						break;
					}
				}
				if(compare(num_head , temp->number) == -1)	tree_head = temp->left;	//if not root but greater than number
				while(tree_head -> right != NULL)	tree_head = tree_head->right;
				//inserting at right
				struct bst_node* new_temp = (struct bst_node*)malloc(sizeof(struct bst_node));
				new_temp->number = num_head;
				new_temp->parent = tree_head;
				tree_head->right = new_temp;
				new_temp->left = NULL;
				new_temp->right = NULL;
				tree_head = new_temp;
				return	tree_head;
			}
		}
	}
	return tree_head;
}

int main()
{

	struct linkedlist_node* num_head = NULL;
	struct bst_node* tree_head = NULL;

	int digit ,tree_active = 0,tree_active_preorder = 0, search = 0, succesor_find = 0, deletition = 0, child = 0, uncle = 0, left_rotate = 0, right_rotate = 0, lead_zero = 1;
	struct bst_node* succesor = NULL;
	while(((digit = fgetc(stdin)) != EOF))
	{
		if(digit == 'N')
		{
			tree_active = 1;	//identifing if number is being inserted in tree
			lead_zero = 1;
			free_tree(tree_head);
			tree_head = NULL;
			if(num_head != NULL)	num_head = free_list(num_head);
		}
		else if(digit == 'B')
		{
			tree_active_preorder = 1;	//identifing if number is being inserted in tree and also preorder
			lead_zero = 1;
			free_tree(tree_head);
			tree_head = NULL;
			if(num_head != NULL)	num_head = free_list(num_head);
		}
		else if((digit == ' ' || digit == '\n') && tree_active_preorder == 1 && num_head != NULL)
		{

			tree_head = insert_tree_preorder(num_head , tree_head);	//inserting number stored in linked list into tree
			num_head = NULL;
			lead_zero = 1;
			if(digit == '\n')
			{
				tree_active_preorder = 0;
				while(tree_head->parent != NULL)	tree_head = tree_head->parent;	//pointing to root
				print_number(tree_head->number);
			}

		}
		else if(digit == '+')	//inserting an number
		{
			num_head = NULL;
			lead_zero = 1;
			tree_active = 1;
		}
		else if((digit == ' ' || digit == '\n') && tree_active == 1 && num_head != NULL)
		{

			tree_head = insert_tree(num_head , tree_head);	//inserting number stored in linked list into tree
			num_head = NULL;
			lead_zero = 1;
			if(digit == '\n')	tree_active = 0;

		}
		else if(digit == 'P')
		{
			preorder_print_tree(tree_head);		//printing the list in pre order traversal
			printf("\n");
		}
		else if(digit == 'S')
		{
			search = 1;
			num_head = NULL;
			lead_zero = 1;
		}
		else if(search == 1 && digit == '\n')
		{
			search_tree(num_head , tree_head);	//searching in tree
			search = 0;
		}
		else if(digit == '>')
		{
			succesor_find = 1;
			num_head = NULL;
			lead_zero = 1;
		}
		else if(succesor_find == 1 && digit == '\n')
		{
			succesor = find_succesor(num_head , tree_head);	//fnding succesor
			if(succesor != NULL)	print_number(succesor->number);
			else	printf("-1");
			printf("\n");
			succesor_find = 0;
		}
		else if(digit == '-')
		{
			deletition = 1;
			num_head = NULL;
			lead_zero = 1;
		}
		else if(deletition == 1 && digit == '\n')
		{
			tree_head = delete_from_tree(num_head , tree_head);	//to delete using sucessor
			deletition = 0;
		}
		else if(digit == 'C')
		{
			child = 1;
			num_head = NULL;
			lead_zero = 1;
		}
		else if(child == 1 && digit == '\n')
		{
			find_child(num_head , tree_head);	//finding and printing the child
			child = 0;
		}
		else if(digit == 'U')
		{
			uncle = 1;
			num_head = NULL;
			lead_zero = 1;
		}
		else if(uncle == 1 && digit == '\n')
		{
			find_uncle(num_head , tree_head);	//finding and printing the uncle
			uncle = 0;
		}
		else if(digit == 'L')
		{
			left_rotate = 1;
			num_head = NULL;
			lead_zero = 1;
		}
		else if(left_rotate == 1 && digit == '\n')
		{
			tree_head = rotate_left(num_head , tree_head);	//finding and printing the child
			left_rotate = 0;
		}
		else if(digit == 'R')
		{
			right_rotate = 1;
			num_head = NULL;
			lead_zero = 1;
		}
		else if(right_rotate == 1 && digit == '\n')
		{
			tree_head = rotate_right(num_head , tree_head);	//finding and printing the child
			right_rotate = 0;
		}
		else if(!(digit == ' ' || digit == '\n'))
		{
			if(!(digit == '0' && lead_zero == 1))   //taking care of leading zeros in the input
      		{
				num_head = insert_linkedlist(digit-48 , num_head);	//storing number to serach in the tree
				lead_zero = 0;
			}
		}
	}
}
