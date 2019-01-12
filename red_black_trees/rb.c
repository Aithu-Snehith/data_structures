// WORKS FOR ARBITARY LENGTH NUMBER
#include <stdio.h>
#include <stdlib.h>

// to store each digit of a number
struct linkedlist_node
{
  int value;
  struct linkedlist_node* next ;
  struct linkedlist_node* prev ;
};

// node of a red black tree
struct rb_node
{
	struct rb_node* left;
	struct rb_node* right;
	struct rb_node* parent;
  _Bool color;
	struct linkedlist_node* number;
};

// tree defining
struct rb_tree{
  struct rb_node* root;
  struct rb_node* centinal;
};

// making the linked list free of memory
struct linkedlist_node* free_list(struct linkedlist_node* head){
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
void free_tree(struct rb_node* tree_head){
	if(tree_head->number == NULL)	return;  //checking if it is centinal node
	free_tree(tree_head->left);
	free_tree(tree_head->right);
	free_list(tree_head->number);
	free(tree_head);
}

// printing the number stored in a linked list
void print_number(struct linkedlist_node* num){
	while(num->prev != NULL)	num = num->prev;
	while(num != NULL)
	{
		printf("%d",num->value);
		num = num->next;
	}
	printf(" ");
}

// printing tree using pre-order traversal
void preorder_print_tree(struct rb_node* tree_head){
  if(tree_head == NULL || tree_head->number == NULL)	return;  //taking care of empty trees
  printf("%d@",tree_head->color);
  print_number(tree_head->number);
	preorder_print_tree(tree_head->left);
	preorder_print_tree(tree_head->right);
}

// inserting a digit in a linked list to store a number
struct linkedlist_node* insert_linkedlist(int digit , struct linkedlist_node* head){
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
int compare(struct linkedlist_node* head_1 , struct linkedlist_node* head_2){
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

// function to find uncle
struct rb_node* find_uncle(struct rb_node* tree_head){
  tree_head = tree_head->parent;
  if(tree_head->parent->right == tree_head) return tree_head->parent->left;
  else if(tree_head->parent->left == tree_head) return tree_head->parent->right;
}

// function to rotate left
struct rb_node* rotate_left(struct rb_node* tree_head){
  struct rb_node* temp = tree_head->right;
  tree_head->right = temp->left;
  if(temp->left != NULL)	temp->left->parent = tree_head;
  temp->parent = tree_head->parent;	//updating parent pointers
  if(tree_head->parent != NULL && tree_head->parent->left == tree_head)	tree_head->parent->left = temp;
  else if(tree_head->parent != NULL && tree_head->parent->right == tree_head)	tree_head->parent->right = temp;
  temp->left = tree_head;	//updating children pointers
  tree_head->parent = temp;
  return tree_head;
}

// function to rotate right
struct rb_node* rotate_right(struct rb_node* tree_head){
  struct rb_node* temp = tree_head->left;
  tree_head->left = temp->right;
  if(temp->right != NULL)	temp->right->parent = tree_head;
  temp->parent = tree_head->parent;
  if(tree_head->parent != NULL && tree_head->parent->left == tree_head)	tree_head->parent->left = temp;
  else if(tree_head->parent != NULL && tree_head->parent->right == tree_head)	tree_head->parent->right = temp;
  temp->right = tree_head;
  tree_head->parent = temp;
  return tree_head;
}

// function to fix the voilation occured by insertion
struct rb_tree fix_insert_rb(struct rb_tree tree){
  while(tree.root->parent->color == 0)
  {
    struct rb_node* uncle = find_uncle(tree.root);
    if(tree.root->parent->parent->left == tree.root->parent)  //case 1
    {
      if(uncle->color == 0)
      {
        tree.root->parent->color = !(tree.root->parent->color);
        uncle->color = !(uncle->color);
        tree.root->parent->parent->color = !(tree.root->parent->parent->color);
        tree.root = tree.root->parent->parent;
      }
      else
      {
        if(tree.root->parent->right == tree.root) //case 2
        {
          tree.root = tree.root->parent;
          tree.root = rotate_left(tree.root);
        }
        tree.root->parent->color = !(tree.root->parent->color);   //case 3
        tree.root->parent->parent->color = !(tree.root->parent->parent->color);
        rotate_right(tree.root->parent->parent);
      }
    }
    else  //symetric cases..
    {
      if(uncle->color == 0)
      {
        tree.root->parent->color = !(tree.root->parent->color);
        uncle->color = !(uncle->color);
        tree.root->parent->parent->color = !(tree.root->parent->parent->color);
        tree.root = tree.root->parent->parent;
      }
      else
      {
        if(tree.root->parent->left == tree.root)
        {
          tree.root = tree.root->parent;
          tree.root = rotate_right(tree.root);
        }
        // printf("gp: %d\n",tree.root->parent->parent->color);
        tree.root->parent->parent->color = !(tree.root->parent->parent->color);
        // printf("gp: %d\n",tree.root->parent->parent->color);
        // printf("p: %d\n",tree.root->parent->color);
        tree.root->parent->color = !(tree.root->parent->color);
        // printf("p: %d\n",tree.root->parent->color);
        rotate_left(tree.root->parent->parent);
      }
    }
  }
  while(tree.root->parent != tree.centinal) tree.root = tree.root->parent;
  tree.root->color = 1;
  // printf("done fixing\n" );
  return tree;
}

// inserting a number in a binary search tree
struct rb_tree insert_tree(struct linkedlist_node* num_head , struct rb_tree tree){
  struct rb_node* temp = (struct rb_node*)malloc(sizeof(struct rb_node));
	int compared , inserted = 0;
	if(tree.root == NULL)  //if tree is empty
	{
		temp->number = num_head;
    temp->color = 0;  // 0 is red , 1 is black
		temp->parent = tree.centinal;
		temp->left = tree.centinal;
		temp->right = tree.centinal;
		tree.root = temp;
	}
	else
	{
		// looping while it is inserted
		while(inserted == 0)
		{
      compared = compare(num_head , tree.root->number);
			if(compared == 1)
			{
				// if number to be inserted is greater , traverse right
				if(tree.root->right != tree.centinal)
				{
					tree.root = tree.root->right;	// moving right
				}
				else
				{
					// inserting to right
					struct rb_node* new_temp = (struct rb_node*)malloc(sizeof(struct rb_node));
					new_temp->number = num_head;
          new_temp->color = 0;
					new_temp->parent = tree.root;
					tree.root->right = new_temp;
					new_temp->left = tree.centinal;
					new_temp->right = tree.centinal;
					tree.root = new_temp;
					inserted =1;
				}
			}
			else if(compared == -1)
			{
				// if number to be inserted is greater , traverse right
				if(tree.root->left != tree.centinal)
				{
					tree.root = tree.root->left;	// moving left
				}
				else
				{
					// inserting at the left
					struct rb_node* new_node = (struct rb_node*)malloc(sizeof(struct rb_node));
					new_node->number = num_head;
          new_node->color = 0;
					new_node->parent = tree.root;
					tree.root->left = new_node;
					new_node->left = tree.centinal;
					new_node->right = tree.centinal;
					tree.root = new_node;
					inserted = 1;
				}
			}
			else
			{
				inserted = 1;	// if number already there in list , do nothing
			}
		}
	}
  tree = fix_insert_rb(tree); //to fix the rules voilated
  return tree;
}


// searching in the tree
void search_tree(struct linkedlist_node* num_head , struct rb_node* tree_head){
	int compared;
	if(tree_head->number == NULL)	printf("-1\n");
	else
	{
		while(tree_head->number != NULL)
		{
			compared = compare(num_head , tree_head->number);
			// if  number given is larger , traverse right
			if(compared == 1)
			{
				if(tree_head->right->number != NULL)	printf("1");
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
				if(tree_head->left->number != NULL)	printf("0");
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
				if(tree_head->color == 0) printf(" R\n"); //printing the color
        if(tree_head->color == 1) printf(" B\n");
				return;
			}
		}
	}
}

// finding the children and printing them
void find_child(struct linkedlist_node* num_head , struct rb_node* tree_head){
	int compared;
	if(tree_head->number == NULL)	printf("-1\n");
	else
	{
		while(tree_head->number != NULL)
		{
			compared = compare(num_head , tree_head->number);
			// if  number given is larger , traverse right
			if(compared == 1)
			{
				if(tree_head->right->number == NULL)
				{
					// if number not found, print -1
					printf("-1\n");
					return;
				}
				tree_head = tree_head->right;
			}
			else if(compared == -1)
			{
				if(tree_head->left->number == NULL)
				{
					// if number not found, and print -1
					printf("-1\n");
					return;
				}
				tree_head = tree_head->left;
			}
			else if(compared == 0)
			{
				if(tree_head->left->number != NULL)
        {
          print_number(tree_head->left->number);	//printing left child
          if(tree_head->left->color == 0) printf("R ");
          else if(tree_head->left->color == 1) printf("B ");
        }
        else	printf("L B ");  //taking care of centinal node
				if(tree_head->right->number != NULL)
        {
          print_number(tree_head->right->number);  //printing right child
          if(tree_head->right->color == 0) printf("R ");
          else if(tree_head->right->color == 1) printf("B ");
        }
        else	printf("L B");
				printf("\n");
				return;
			}
		}
	}
}

// // function to find the sucessor of the given function
struct rb_node* find_succesor(struct linkedlist_node* num_head , struct rb_node* tree_head){
	int compared;
	if(tree_head == NULL)	return NULL;
	else
	{
		while(tree_head->number != NULL)
		{
			compared = compare(num_head , tree_head->number);
			// if  number given is larger , traverse right
			if(compared == 1)
			{
				if(tree_head->right->number == NULL)
				{
					while(tree_head->parent->number != NULL)
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
				if(tree_head->left->number == NULL)
				{
					return tree_head;
				}
				tree_head = tree_head->left;
			}
			else if(compared == 0)
			{// case 1 ; where it has right child
				if(tree_head->right->number != NULL)
				{
					tree_head = tree_head->right;
					while(tree_head->left->number != NULL)	tree_head = tree_head->left;
					return tree_head;
				}
				else
				{// case 2 : where there is no right child
					while(tree_head->parent->number != NULL)
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

// function to splice a node
struct rb_tree rb_splice_node(struct rb_tree tree , struct rb_node* x , struct rb_node* y){
  if(x->parent == tree.centinal) tree.root = y; //taking care of root node
  else if(x->parent->left == x) x->parent->left = y;
  else  x->parent->right = y;
  y->parent = x->parent;
  return tree;
}

// function to fix tree after deleting a node
struct rb_tree rb_fix_delete(struct rb_tree tree, struct rb_node* temp){
  while(temp != tree.root && temp->color ==1)
  {
    if(temp->parent->left == temp)
    {
      struct rb_node* t = temp->parent->right;
      if(t->color == 0) //case 1
      {
        t->color =1;
        temp->parent->color = 0;
        // printf("brfore rotate adrr: %p\n", &temp->parent);
        if(temp->parent == tree.root) tree.root = tree.root->right;
        rotate_left(temp->parent);
        // printf("brfore rotate adrr: %p\n", &temp->parent);
        t = temp->parent->right;
      }
      if(t->left->color == 1 && t->right->color == 1)   //case 2
      {
        t->color = 0;
        temp = temp->parent;
      }
      else
      {
        if(t->right->color == 1)    //case 3
        {
          t->left->color = 1;
          t->color = 0;
          if(t == tree.root) tree.root = tree.root->left;
          rotate_right(t);
          t = temp->parent->right;
        }
        t->color = temp->parent->color;   //case 4
        temp->parent->color = 1;
        t->right->color = 1;
        if(temp->parent == tree.root) tree.root = temp->parent->right;
        rotate_left(temp->parent);
        temp = tree.root;
      }
    }
    else  //symetric cases
    {
      struct rb_node* t = temp->parent->left;
      if(t->color == 0)
      {
        t->color =1;
        temp->parent->color = 0;
        if(temp->parent == tree.root) tree.root = tree.root->left;
        rotate_right(temp->parent);
        t = temp->parent->left;
      }
      if(t->left->color == 1 && t->right->color == 1)
      {
        t->color = 0;
        temp = temp->parent;
      }
      else
      {
        if(t->left->color == 1)
        {
          t->right->color = 1;
          t->color = 0;
          if(t == tree.root) tree.root = tree.root->right;
          rotate_left(t);
          t = temp->parent->left;
        }
        t->color = temp->parent->color;
        temp->parent->color = 1;
        t->left->color = 1;
        if(temp->parent == tree.root) tree.root = tree.root->left;
        rotate_right(temp->parent);
        temp = tree.root;
      }
    }
  }
  temp->color = 1;
  return tree;
}

// function to deletea node from tree
struct rb_tree delete_from_tree(struct linkedlist_node* num_head , struct rb_tree tree){
	int compared;
  struct rb_node* temp_root = tree.root;
	if(temp_root == NULL)	return tree;
	else
	{
		while(temp_root->number != NULL)
		{
			compared = compare(num_head , temp_root->number);
			// if  number given is larger , traverse right
			if(compared == 1)
			{// doing nothing if number doesn't exists
				if(temp_root->right->number == NULL)
				{
					while(temp_root->parent->number != NULL)	temp_root = temp_root->parent;
					return	tree;
				}
				temp_root = temp_root->right;
			}
			else if(compared == -1)
			{
				if(temp_root->left->number == NULL)
				{
					while(temp_root->parent->number != NULL)	temp_root = temp_root->parent;
					return	tree;
				}
				temp_root = temp_root->left;
			}
			else if(compared == 0)
			{
        struct rb_node* temp_1 = temp_root;
        struct rb_node* temp_2 = NULL;
        _Bool temp1_act_color = temp_1->color;  //similar to token

        if(temp_root->left == tree.centinal)  //taking care if childeren are leaves
        {
          temp_2 = temp_root->right;
          tree = rb_splice_node(tree , temp_root, temp_root->right);
        }
        else if(temp_root->right == tree.centinal)
        {
          temp_2 = temp_root->left;
          tree = rb_splice_node(tree , temp_root, temp_root->left);
        }
        else
        {
          temp_1  = temp_1->right;
          while(temp_1->left != tree.centinal)  temp_1 = temp_1->left;    //finding the sucessor
          temp1_act_color = temp_1->color;
          temp_2 = temp_1->right;

          if(temp_1->parent == temp_root) temp_2->parent = temp_1;
          else
          {
            tree = rb_splice_node(tree, temp_1 , temp_1->right);
            temp_1->right = temp_root->right;
            temp_1->right->parent = temp_1;
          }
          tree = rb_splice_node(tree , temp_root , temp_1);   //splicing the node to be deleted..
          temp_1->left = temp_root->left;
          temp_1->left->parent = temp_1;
          temp_1->color = temp_root->color;
        }
        if(temp1_act_color == 1)  tree = rb_fix_delete(tree, temp_2); //to fix the voilated done by deleting the node
        return tree;
			}
		}
	}
}


int main()
{
  struct linkedlist_node* num_head = NULL;
  // initializing the tree
  struct rb_tree tree;
	tree.root = NULL;
  tree.centinal = (struct rb_node*)malloc(sizeof(struct rb_node));
  tree.centinal->number = NULL;
  tree.centinal->color = 1;
  tree.centinal->left = tree.centinal;
  tree.centinal->right = tree.centinal;
  tree.centinal->parent = tree.centinal;

	int digit ,tree_active = 0,tree_active_preorder = 0, search = 0, succesor_find = 0, deletition = 0, child = 0, uncle = 0, left_rotate = 0, right_rotate = 0, lead_zero = 1;
	struct rb_node* succesor = NULL;
	while(((digit = fgetc(stdin)) != EOF))
	{
		if(digit == 'N')
		{
			tree_active = 1;	//identifing if number is being inserted in tree
			lead_zero = 1;
			if(tree.root != NULL)  free_tree(tree.root); //releasing the memory of previous tree
			tree.root = NULL;
			if(num_head != NULL)	num_head = free_list(num_head);
		}
		else if(digit == '+')	//inserting an number
		{
			num_head = NULL;
			lead_zero = 1;
			tree_active = 1;
		}
		else if((digit == ' ' || digit == '\n') && tree_active == 1 && num_head != NULL)
		{
			tree = insert_tree(num_head , tree);	//inserting number stored in linked list into tree
			num_head = NULL;
			lead_zero = 1;
			if(digit == '\n')	tree_active = 0;
		}
		else if(digit == 'P')
		{
  		preorder_print_tree(tree.root);		//printing the list in pre order traversal
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
			search_tree(num_head , tree.root);	//searching in tree
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
			succesor = find_succesor(num_head , tree.root);	//fnding succesor
			if(succesor != NULL)
      {
        print_number(succesor->number);
        // if(succesor->color == 0)  printf("R");
        // else if(succesor->color == 1)  printf("B");
      }
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
			tree = delete_from_tree(num_head , tree);	//to delete using sucessor
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
			find_child(num_head , tree.root);	//finding and printing the child
			child = 0;
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
