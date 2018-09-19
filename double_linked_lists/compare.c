#include <stdio.h>
#include <stdlib.h>


struct node
{
  int value;
  struct node* next ;
  struct node* prev ;
};

// initializing the pointers to the lists
struct node* head_1 = NULL;
struct node* head_2 = NULL;
struct node* head_add = NULL;

// function to print the list
void print_list(struct node* head)
{
  while(head != NULL)
  {
    // Taking care of leading zeros
    if(head->value == 0)    
    {
      head = head->prev;
      // If both numbers are zeros
      if(head == NULL)  printf("0\n");
    }
    else
    {
      // printing the list
      while(head != NULL)
      {
        printf("%d", head->value);
        head = head->prev;
      }
      printf("\n");
    } 
  }
}

// making the linked list free of memory
struct node* free_list(struct node* head)
{
  struct node* temp = NULL;
  while(head != NULL)
  {
    temp = head->prev;
    free(head);
    head = temp;
  }
  return head;
}

// comparing two umbers
int compare(struct node* head_1 , struct node* head_2)
{
  while(!(head_1 ==NULL && head_2 == NULL))
  {
    // comparin the length of each number
    if(head_1 == NULL && head_2 != NULL)  return 0;
    else if(head_1 != NULL && head_2 == NULL) return 1;
    else if(head_1->prev == NULL && head_2->prev == NULL)
    {
      while(head_2 != NULL)
      {
        // comparing the digits if the lengths are equal
        if(head_1->value > head_2->value) return 1;
        else if(head_1->value < head_2->value)  return 0;
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
  return 1;
}

// function to insert a node
struct node* insert(int digit , struct node* head)
{
  struct node* temp = (struct node*)malloc(sizeof(struct node));
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
  // printf("%d ",head->value);
  return head;
}

// Function to add two numbers stored in a list
struct node* add(struct node* head_1 , struct node* head_2)
{
  // creating a new linked list to store the added value
  struct node* head_add= NULL;
  int carry = 0, added;
  
  while(!(head_1 ==NULL && head_2 == NULL))
  {
    int t1_val=0, t2_val=0;
    // Taking care of unequal number of digits
    if(head_1 != NULL)  t1_val = head_1->value;
    if(head_2 != NULL)  t2_val = head_2->value;
    // finding the number and carry
    added = (t1_val + t2_val + carry)%10;
    carry = (t1_val + t2_val + carry)/10;
    // inserting the digit to the list
    head_add = insert(added , head_add);
    if(head_1 != NULL)  head_1 = head_1->prev;
    if(head_2 != NULL)  head_2 = head_2->prev;
  }
  // inerting the carry left
  head_add = insert(carry , head_add);
  print_list(head_add);
  return head_add;
}

int main(){
  int digit , num = 1, lead_zero = 1, compared; 
  // Taking the input continuosly
  while((digit=fgetc(stdin))!=EOF){
    if(digit=='\n'){
      // adding the two numbers
      // head_add = add(head_1 , head_2);
      compared = compare(head_1,head_2);
      printf("%d\n", compared);
      head_1 = free_list(head_1);
      head_2 = free_list(head_2);
      head_add = free_list(head_add);
      num = 1;
      lead_zero = 1;
    }
    else if(digit==' '){
      // to specify new linked list to the second number
      num = 2;
      lead_zero = 1;
    }
    else{
      if(!(digit == '0' && lead_zero == 1))   //taking care of leading zeros in the input
      {
        if(num == 1)  head_1 =  insert(digit-48 ,  head_1);  // inserting the numbers to the lists
        else  head_2 = insert(digit-48 , head_2);
        lead_zero = 0;
      }      
    }
  }
  return(0);
}