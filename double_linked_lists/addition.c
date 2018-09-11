#include <stdio.h>
#include <stdlib.h>


struct node
{
  int value;
  struct node* next ;
  struct node* prev ;
};

// initializing the pointers to the lists
struct node* tail_1 = NULL;
struct node* tail_2 = NULL;
struct node* tail_add = NULL;

// function to print the list
void print_list(struct node* tail)
{
  while(tail != NULL)
  {
    // Taking care of leading zeros
    if(tail->value == 0)    
    {
      tail = tail->prev;
      // If both numbers are zeros
      if(tail == NULL)  printf("0\n");
    }
    else
    {
      // printing the list
      while(tail != NULL)
      {
        printf("%d", tail->value);
        tail = tail->prev;
      }
      printf("\n");
    } 
  }
}

// making the linked list free of memory
struct node* free_list(struct node* tail)
{
  struct node* temp = NULL;
  while(tail != NULL)
  {
    temp = tail->prev;
    free(tail);
    tail = temp;
  }
  return tail;
}


// function to insert a node
struct node* insert(int digit , struct node* tail)
{
  struct node* temp = (struct node*)malloc(sizeof(struct node));
  // For initializing the linked list
  if(tail == NULL)
  {
    temp->value = digit;
    temp->next = NULL;
    temp->prev = NULL;
    tail = temp;
  } 
  // For adding a node to the list
  else
  {
    temp->value = digit;
    tail->next = temp;
    temp->prev = tail;
    temp->next = NULL;
    tail = temp;
  }
  return tail;
}

// Function to add two numbers stored in a list
struct node* add(struct node* tail_1 , struct node* tail_2)
{
  // creating a new linked list to store the added value
  struct node* tail_add= NULL;
  int carry = 0, added;
  
  while(!(tail_1 ==NULL && tail_2 == NULL))
  {
    int t1_val=0, t2_val=0;
    // Taking care of unequal number of digits
    if(tail_1 != NULL)  t1_val = tail_1->value;
    if(tail_2 != NULL)  t2_val = tail_2->value;
    // finding the number and carry
    added = (t1_val + t2_val + carry)%10;
    carry = (t1_val + t2_val + carry)/10;
    // inserting the digit to the list
    tail_add = insert(added , tail_add);
    if(tail_1 != NULL)  tail_1 = tail_1->prev;
    if(tail_2 != NULL)  tail_2 = tail_2->prev;
  }
  // inerting the carry left
  tail_add = insert(carry , tail_add);
  print_list(tail_add);
  return tail_add;
}

int main(){
  int digit , num = 1; 
  // Taking the input continuosly
  while((digit=fgetc(stdin))!=EOF){
    if(digit=='\n'){
      // adding the two numbers
      tail_add = add(tail_1 , tail_2);
      tail_1 = free_list(tail_1);
      tail_2 = free_list(tail_2);
      tail_add = free_list(tail_add);
      num = 1;
    }
    else if(digit==' '){
      // to specify new linked list to the second number
      num = 2;
    }
    else{
      // inserting the numbers to the lists
      if(num == 1)  tail_1 =  insert(digit-48 ,  tail_1);
      else  tail_2 = insert(digit-48 , tail_2);
    }
  }
  return(0);
}