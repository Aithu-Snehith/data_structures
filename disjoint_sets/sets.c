#include<stdio.h>
#include<stdlib.h>
#include<limits.h>

// for each set initially
struct set_node{
  int parent;
  int rank;
};

// function to find the represetative of the set
int find_rep(struct set_node* sets , int val)
{
  int rep_element = -1;
  if(sets[val].parent == -1)  return val;
  else
  {
    rep_element = find_rep(sets , sets[val].parent);  //traversing recursively to rep element
  }
  return rep_element;
}

int main() {
  struct set_node* sets = NULL;  //initializing set
  int digit , new_sets = 0 , rank_active=0;
  int search_active = 0 , rep_node = -1 , union_active = 0;
  int check_link = 0 , start_node = 0 , ref_node = 0,rep_1 = -1 , rep_2 = -1;
  int scanned_number = 0, scanned_digits[5] = {-1,-1,-1,-1,-1} , scan_point = 0;
  int no_nodes = 0;
  while((digit=fgetc(stdin))!=EOF){
    if(digit == 'N')
    {
      new_sets = 1;
      free(sets); //releasing the memory of previous set
      sets = NULL;
    }
    else if(digit == '\n' && new_sets == 1 && scanned_digits[0] != -1)
    {
      int temp = 1;
      // converting the scanned digits into a number
      for(int i = 0 ; i<scan_point ; i++)
      {
        scanned_number = scanned_number + scanned_digits[scan_point-1-i]*temp;
        temp = temp *10;
      }
      no_nodes = scanned_number;
      // creating memory for the graph dynamically
      sets = (struct set_node*)malloc(no_nodes * sizeof(struct set_node));

      for(int i = 0 ; i < no_nodes ; i++)
      {
        sets[i].parent = -1;
        sets[i].rank = 1;
      }
      // resetting the paramets to default
      new_sets = 0;
      scan_point = 0;
      scanned_number = 0;
      for(int i = 0 ; i<5 ; i++)  scanned_digits[i] = -1;
    }
    else if(digit == 'S')   //for finding the representative element
    {
      search_active = 1;
    }
    else if((digit == ' ' || digit == '\n') && search_active == 1 && scanned_digits[0] != -1)
    {
      int temp = 1;
      for(int i = 0 ; i<scan_point ; i++)
      {
        scanned_number = scanned_number + scanned_digits[scan_point-1-i]*temp;
        temp = temp *10;
      }
      scanned_number = scanned_number - 1;
      if(scanned_number < no_nodes) printf("%d\n",find_rep(sets , scanned_number)+1);
      else  printf("-1\n");

      if(digit == '\n') search_active = 0;
      scan_point = 0;
      scanned_number = 0;
      for(int i = 0 ; i<5 ; i++)  scanned_digits[i] = -1;
    }
    else if(digit == 'R')   //for finding the rank
    {
      rank_active = 1;
    }
    else if((digit == ' ' || digit == '\n') && rank_active == 1 && scanned_digits[0] != -1)
    {
      int temp = 1;
      for(int i = 0 ; i<scan_point ; i++)
      {
        scanned_number = scanned_number + scanned_digits[scan_point-1-i]*temp;
        temp = temp *10;
      }
      scanned_number = scanned_number - 1;
      if(scanned_number < no_nodes)  printf("%d\n",sets[scanned_number].rank);
      else  printf("-1\n");

      if(digit == '\n') rank_active = 0;
      scan_point = 0;
      scanned_number = 0;
      for(int i = 0 ; i<5 ; i++)  scanned_digits[i] = -1;
    }
    else if(digit == '?')   //for finding if the link exists
    {
      check_link = 1;
      start_node = 1;
    }
    else if((digit == ' ' || digit == '\n') && check_link == 1 && scanned_digits[0] != -1)
    {
      int temp = 1;
      for(int i = 0 ; i<scan_point ; i++)
      {
        scanned_number = scanned_number + scanned_digits[scan_point-1-i]*temp;
        temp = temp *10;
      }
      scanned_number = scanned_number - 1;

      if(start_node == 1)
      {
        ref_node = scanned_number;
        start_node = 0;
      }
      else
      {
        if(ref_node < no_nodes && scanned_number < no_nodes)
        {
          rep_1 = find_rep(sets , ref_node);
          rep_2 = find_rep(sets , scanned_number);
          if(rep_1 == rep_2)  printf("1\n");  //comparing the representative elements
          else  printf("0\n");
        }
        else  printf("-1\n");
      }

      if(digit == '\n') check_link = 0;
      scan_point = 0;
      scanned_number = 0;
      for(int i = 0 ; i<5 ; i++)  scanned_digits[i] = -1;
    }
    else if(digit == 'U')   //for union
    {
      union_active = 1;
      start_node = 1;
    }
    else if((digit == ' ' || digit == '\n') && union_active == 1 && scanned_digits[0] != -1)
    {
      int temp = 1;
      for(int i = 0 ; i<scan_point ; i++)
      {
        scanned_number = scanned_number + scanned_digits[scan_point-1-i]*temp;
        temp = temp *10;
      }
      scanned_number = scanned_number - 1;

      if(start_node == 1)
      {
        ref_node = scanned_number;
        start_node = 0;
      }
      else
      {
        if(ref_node < no_nodes && scanned_number < no_nodes && scanned_number!= ref_node)
        {
          rep_1 = find_rep(sets , ref_node);
          rep_2 = find_rep(sets , scanned_number);
          if(sets[rep_1].rank > sets[rep_2].rank)  sets[rep_2].parent = rep_1;
          else if(sets[rep_1].rank < sets[rep_2].rank)  sets[rep_1].parent = rep_2;
          else  //if both the rep elements of set have same rank
          {
            sets[rep_2].parent = rep_1;
            sets[rep_1].rank++;
          }
        }
      }

      if(digit == '\n') union_active = 0;
      scan_point = 0;
      scanned_number = 0;
      for(int i = 0 ; i<5 ; i++)  scanned_digits[i] = -1;
    }
    else if(!(digit == ' ' || digit == '\n')) //storing the digits scanned into array
    {
      scanned_digits[scan_point] = digit - 48;
      scan_point++;
    }
  }
  return 0;
}
