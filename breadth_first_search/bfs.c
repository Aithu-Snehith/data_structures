#include<stdio.h>
#include<stdlib.h>

// for each node in linkedlist
struct linkedlist_node {
  int value;
  struct linkedlist_node* next;
  struct linkedlist_node* prev;
};

// for each vertex in graph
struct graph_node {
  struct linkedlist_node* neighbours;
  unsigned int color: 2;
  int parent;
};

// for each node in que
struct q_node{
  int value;
  struct q_node* next;
  struct q_node* prev;
};

// for a queue
struct queue{
  struct q_node* head;
  struct q_node* tail;
  int dequeued;
};

// checking if the queue is empty
int check_empty(struct queue q){
  if(q.head == NULL && q.tail == NULL)  return 1;
  else  return 0;
}

// printing the queue... for verification
void print_queue(struct queue q){
  printf("present queue: ");
  while(q.head != q.tail)
  {
    printf(" %d",q.head->value);
    q.head = q.head->next;
  }
  if(q.head != NULL)  printf(" %d\n", q.head->value);
  else  printf("\n");
  return;
}

// function to enqueue a number
struct queue enqueue(int number , struct queue q){
  struct q_node* temp = (struct q_node*)malloc(sizeof(struct q_node));
  if(check_empty(q) == 1) //if queue is empty
  {
    temp->value = number;
    temp->next = NULL;
    temp->prev = NULL;
    q.head = temp;
    q.tail = temp;
  }
  else
  {
    temp->value = number;
    q.tail->next = temp;
    temp->prev = q.tail;
    temp->next = NULL;
    q.tail = temp;
  }
  return q;
}

// function to deque from queue
struct queue dequeue(struct queue q){
  struct q_node* temp = q.head;
  if(check_empty(q) == 1)    q.dequeued = -1;   //if queue is empty
  else if(q.head == q.tail) //if queue has only one node
  {
    q.dequeued = q.head->value;
    q.head = NULL;
    q.tail = NULL;
    free(temp);
  }
  else
  {
    q.dequeued = q.head->value;
    q.head = q.head->next;
    q.head->prev = NULL;
    free(temp);
  }
  return q;
}

// function to empty the queue
struct queue empty_queue(struct queue q){
  while(check_empty(q) != 1)    q = dequeue(q); //dequeuing all the values
  return q;
}

// freeing the linked list
struct linkedlist_node* free_list(struct linkedlist_node* head){
  struct linkedlist_node* temp = NULL;
  while(head->next != NULL)      head = head->next;
  while(head != NULL)
  {
    temp = head->prev;
    free(head); //freeing each node
    head = temp;
  }
  return head;
}

// freeing the memory of graph
struct graph_node* free_graph(struct graph_node* graph , int no_nodes){
  for(int i = 0 ; i < no_nodes ; i++) graph[i].neighbours = free_list(graph[i].neighbours);
  free(graph);
  return graph;
}

// function to insert a number in a linked list
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
    while(head->next != NULL)      head = head->next;
    temp->value = digit;
    head->next = temp;
    temp->prev = head;
    temp->next = NULL;
    head = temp;
    while(head->prev != NULL) head = head->prev;
  }
  return head;
}

// function to perform breadth_first_search
struct graph_node* breadth_first_search(struct graph_node* graph , int bsf_node , int print , int no_nodes){
  struct queue q; //initializing the queue
  q.head = NULL;
  q.tail = NULL;

  // resetting the parameters to default
  for(int i = 0 ; i < no_nodes ; i++)
  {
    graph[i].color = 0;
    graph[i].parent = -1;
  }

  q = enqueue(bsf_node , q);  //enqueuing the ref node
  if(print) printf("%d ",bsf_node + 1);
  graph[bsf_node].color = 1;
  graph[bsf_node].parent = bsf_node;

  while(check_empty(q) != 1)
  {
    q = dequeue(q);
    graph[q.dequeued].color = 2;
    if(graph[q.dequeued].neighbours != NULL)
    {
      if(graph[graph[q.dequeued].neighbours->value].color == 0)
      {
        q = enqueue(graph[q.dequeued].neighbours->value , q); //enqueuing to queue
        if(print) printf("%d ",graph[q.dequeued].neighbours->value + 1);  //printing the nodes
        graph[graph[q.dequeued].neighbours->value].color = 1;   //updating the color
        graph[graph[q.dequeued].neighbours->value].parent = q.dequeued; //storing the parent of the node
      }
      while(graph[q.dequeued].neighbours->next != NULL)
      {
        graph[q.dequeued].neighbours = graph[q.dequeued].neighbours->next;
        if(graph[graph[q.dequeued].neighbours->value].color == 0)
        {
          q = enqueue(graph[q.dequeued].neighbours->value , q);
          if(print) printf("%d ",graph[q.dequeued].neighbours->value + 1);
          graph[graph[q.dequeued].neighbours->value].color = 1;
          graph[graph[q.dequeued].neighbours->value].parent = q.dequeued;
        }
      }
      // resetting the pointer back to default
      while(graph[q.dequeued].neighbours->prev != NULL) graph[q.dequeued].neighbours = graph[q.dequeued].neighbours->prev;
    }
  }
  if(print) printf("\n");
  return graph;
}

// function to find the path between reference node and other node
void find_path(struct graph_node* graph , int u , int v){
  struct linkedlist_node* printing = NULL;
  // inserting into linked list
  printing = insert_linkedlist(v , printing);
  while(v != u && v != -1)  //traversing from the node to reference node
  {
    v = graph[v].parent;
    printing = insert_linkedlist(v , printing);
  }
  if(v == u)  //checking wether the path exists or not
  {
    while(printing->next != NULL)      printing = printing->next;
    printf("%d ",printing->value + 1);
    while(printing->prev != NULL)
    {
      printing = printing->prev;
      printf("%d ",printing->value + 1);
    }
    printf("\n");
  }
  else  printf("-1\n");
  printing = free_list(printing);
  return;
}

int main() {
  struct graph_node* graph = NULL;  //initializing graph
  int digit , new_graph = 0;
  int graph_active = 0 , ref_node = -1 , start_node = 0;
  int check_link = 0;
  int bsf_active = 0 , prev_bsf_node = -1 ;
  int path_find = 0;
  int scanned_number = 0, scanned_digits[5] = {-1,-1,-1,-1,-1} , scan_point = 0;
  int no_nodes = 0;
  while((digit=fgetc(stdin))!=EOF){
    if(digit == 'N')
    {
      new_graph = 1;
      graph = free_graph(graph , no_nodes); //freeing the previous graph
      graph = NULL;
      prev_bsf_node = -1;
    }
    else if(digit == '\n' && new_graph == 1 && scanned_digits[0] != -1)
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
      graph = (struct graph_node*)malloc(no_nodes * sizeof(struct graph_node));

      // resetting the paramets to default
      new_graph = 0;
      scan_point = 0;
      scanned_number = 0;
      for(int i = 0 ; i<5 ; i++)  scanned_digits[i] = -1;
    }
    else if(digit == 'E') //for links
    {
      graph_active = 1;
      start_node = 1;
    }
    else if((digit == ' ' || digit == '\n') && graph_active == 1 && scanned_digits[0] != -1)
    {
      int temp = 1;
      for(int i = 0 ; i<scan_point ; i++)
      {
        scanned_number = scanned_number + scanned_digits[scan_point-1-i]*temp;
        temp = temp *10;
      }
      scanned_number = scanned_number - 1;

      if(start_node == 1) //verifying if reference node or link node
      {
        ref_node = scanned_number;
        start_node = 0;
      }
      else
      {
        // inserting the links to adjacency list
        graph[ref_node].neighbours = insert_linkedlist(scanned_number , graph[ref_node].neighbours);
      }

      if(digit == '\n')
      {
        graph_active = 0;
        ref_node = -1;
      }
      scan_point = 0;
      scanned_number = 0;
      for(int i = 0 ; i<5 ; i++)  scanned_digits[i] = -1;
    }
    else if(digit == '?')   //finding if the link exists
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
        // searching for the node in the ref_node's adjacency list
        if(graph[ref_node].neighbours != NULL && digit == '\n')
        {
          if(graph[ref_node].neighbours->value == scanned_number) printf("1\n");
          else
          {
            while(graph[ref_node].neighbours->next != NULL)
            {
              graph[ref_node].neighbours = graph[ref_node].neighbours->next;
              if(graph[ref_node].neighbours->value == scanned_number) printf("1\n");
            }
            printf("0\n");
            while(graph[ref_node].neighbours->prev != NULL)  graph[ref_node].neighbours = graph[ref_node].neighbours->prev;
          }
        }
      }

      if(digit == '\n') check_link = 0;
      scan_point = 0;
      scanned_number = 0;
      for(int i = 0 ; i<5 ; i++)  scanned_digits[i] = -1;
    }
    else if(digit == 'B')   //for breadth_first_search
    {
      bsf_active = 1;
    }
    else if((digit == ' ' || digit == '\n') && bsf_active == 1 && scanned_digits[0] != -1)
    {
      int temp = 1;
      for(int i = 0 ; i<scan_point ; i++)
      {
        // printf("%d %d\n", scan_point,scanned_number);
        scanned_number = scanned_number + scanned_digits[scan_point-1-i]*temp;
        temp = temp *10;
      }
      scanned_number = scanned_number - 1;

      prev_bsf_node = scanned_number;
      graph = breadth_first_search(graph ,scanned_number, 1 , no_nodes);    //performing bfs

      if(digit == '\n') bsf_active = 0;
      scan_point = 0;
      scanned_number = 0;
      for(int i = 0 ; i<5 ; i++)  scanned_digits[i] = -1;
    }
    else if(digit == 'P') //for finding the path
    {
      path_find = 1;
      start_node = 1;
    }
    else if((digit == ' ' || digit == '\n') && path_find == 1 && scanned_digits[0] != -1)
    {
      int temp = 1;
      for(int i = 0 ; i<scan_point ; i++)
      {
        scanned_number = scanned_number + scanned_digits[scan_point-1-i]*temp;
        temp = temp *10;
      }
      scanned_number = scanned_number - 1;

      if(start_node == 1) //checking if 'u' or 'v'
      {
        ref_node = scanned_number;
        start_node = 0;
        if(prev_bsf_node != scanned_number) //checking wether bfs for this node is performed or not
        {
          prev_bsf_node = scanned_number;
          graph = breadth_first_search(graph ,scanned_number, 0 , no_nodes);
        }
      }
      else
      {
        find_path(graph , ref_node , scanned_number); //finding the path from results of bfs
      }

      if(digit == '\n')
      {
        path_find = 0;
        ref_node = 0;
      }
      scan_point = 0;
      scanned_number = 0;
      for(int i = 0 ; i<5 ; i++)  scanned_digits[i] = -1;
    }
    else if(digit == 's')   //printing the whole adjacency list...ignore for now...written for verification
    {
      for(int i = 0 ; i < no_nodes ; i++)
      {
        printf("ref node: %d ",i+1);
        if(graph[i].neighbours != NULL)
        {
          printf(" -> %d",graph[i].neighbours->value+1);
          while(graph[i].neighbours->next != NULL)
          {
            graph[i].neighbours = graph[i].neighbours->next;
            printf(" -> %d",graph[i].neighbours->value+1);
          }
          while(graph[i].neighbours->prev != NULL)  graph[i].neighbours = graph[i].neighbours->prev;
        }
        printf("\n");
      }
    }
    else if(!(digit == ' ' || digit == '\n')) //storing the digits scanned into array
    {
      scanned_digits[scan_point] = digit - 48;
      scan_point++;
    }
  }
  return 0;
}
