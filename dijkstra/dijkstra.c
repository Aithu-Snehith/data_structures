#include<stdio.h>
#include<stdlib.h>
#include<limits.h>

// for each node in linkedlist
struct linkedlist_node {
  int value;
  int weight;
  struct linkedlist_node* next;
  struct linkedlist_node* prev;
};

// for each vertex in graph
struct graph_node {
  struct linkedlist_node* neighbours;
  unsigned int color: 2;
  int parent;
  int distance;
};

// for each node in a heap
struct min_heap_node{
  int vertex;
  int key;
};

// for the minimum priority heap
struct min_heap{
  struct min_heap_node extracted_min;
  int active_size;
  int max_size;
  struct min_heap_node* heap_array;
};

// to maintain the heap properties
struct min_heap* heapify_min(struct min_heap* heap, int index){
  int min_index = index;
  // checking if the children of the node are smaller than the value or not
  if(2*index+1 < heap->active_size && heap->heap_array[min_index].key > heap->heap_array[2*index+1].key) min_index = 2*index+1;
  if(2*index+2 < heap->active_size && heap->heap_array[min_index].key > heap->heap_array[2*index+2].key) min_index = 2*index+2;
  // swaping both if child is smaller than parent
  if(min_index != index)
  {
    struct min_heap_node temp = heap->heap_array[index];
    heap->heap_array[index] = heap->heap_array[min_index];
    heap->heap_array[min_index] = temp;
    heap = heapify_min(heap , min_index);
  }
  return heap;
}

// function to extract the minimum value from heap
struct min_heap* extract_min(struct min_heap* heap){
  heap->extracted_min = heap->heap_array[0];    //extracting the min value(it will always be in first node)
  heap->heap_array[0] = heap->heap_array[heap->active_size-1];  //replacing the first with last
  heap->heap_array[heap->active_size-1].vertex = 0;
  heap->heap_array[heap->active_size-1].key = 0;
  if(heap->active_size >0) heap->active_size--;
  heap = heapify_min(heap , 0); //fixing the properties
  return heap;
}

// function to insert a value in min-heap
struct min_heap* insert_heap(struct min_heap* heap , int vertex , int value){
  if(heap->active_size < heap->max_size)  //taking care of overflow
  {
    // inserting a new value
    heap->heap_array[heap->active_size].key = value;
    heap->heap_array[heap->active_size].vertex = vertex;
    int index = heap->active_size;
    // fixing the voilated properties
    while(heap->heap_array[index].key < heap->heap_array[(index - 1)/2].key)
    {
      struct min_heap_node temp = heap->heap_array[index];
      heap->heap_array[index] = heap->heap_array[(index - 1)/2];
      heap->heap_array[(index - 1)/2] = temp;
      index = (index - 1)/2;
    }
    heap->active_size++;
  }
}

// function to decrease a value already inserted in min-heap
struct min_heap* decrease_key(struct min_heap* heap , int vertex , int value){
  for(int i = 0 ; i < heap->active_size ; i++)
  {
    if(heap->heap_array[i].vertex == vertex)
    {
      // decreasing the value
      heap->heap_array[i].key = value;
      heap->heap_array[i].vertex = vertex;
      // fixing the voilated properties
      int index = i;
      while(heap->heap_array[index].key < heap->heap_array[(index - 1)/2].key)
      {
        struct min_heap_node temp = heap->heap_array[index];
        heap->heap_array[index] = heap->heap_array[(index - 1)/2];
        heap->heap_array[(index - 1)/2] = temp;
        index = (index - 1)/2;
      }
    }
  }
  return heap;
}

// freeing the linked list
struct linkedlist_node* free_list(struct linkedlist_node* head){
  if(head == NULL)  return head;
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
  for(int i = 0 ; i < no_nodes ; i++)    graph[i].neighbours = free_list(graph[i].neighbours);
  free(graph);
  return graph;
}

// function to insert a number in a linked list
struct linkedlist_node* insert_linkedlist(int vertex , int w , struct linkedlist_node* head){
  struct linkedlist_node* temp = (struct linkedlist_node*)malloc(sizeof(struct linkedlist_node));
  // For initializing the linked list
  if(head == NULL)
  {
    temp->value = vertex;
    temp->weight = w;
    temp->next = NULL;
    temp->prev = NULL;
    head = temp;
  }
  // For adding a node to the list
  else
  {
    while(head->next != NULL)      head = head->next;
    temp->value = vertex;
    temp->weight = w;
    head->next = temp;
    temp->prev = head;
    temp->next = NULL;
    head = temp;
    while(head->prev != NULL) head = head->prev;
  }
  return head;
}

// function to perform dijkstra's algorithm
struct graph_node* dijkstra(struct graph_node* graph , int dj_node , int print , int no_nodes){
  struct min_heap* heap = NULL; //crwating a min heap
  heap = (struct min_heap*)malloc(sizeof(struct min_heap));
  heap->heap_array = (struct min_heap_node *)malloc(no_nodes * sizeof(struct min_heap_node));
  heap->max_size = no_nodes;
  heap->active_size = 0;

  // setting the values to default
  for(int i = 0 ; i< no_nodes ; i++)
  {
    graph[i].distance = INT_MAX;
    graph[i].parent = -1;
    heap = insert_heap(heap , i , INT_MAX);
  }
  // taking care of starting node
  heap = decrease_key(heap , dj_node , 0);
  graph[dj_node].distance = 0;
  graph[dj_node].parent = dj_node;

  while(heap->active_size != 0)
  {
    heap = extract_min(heap);
    if(print) //printing the vertices and the correcponding distances from ref node
    {
      if(heap->extracted_min.key == INT_MAX)  printf("%d -1\n", heap->extracted_min.vertex+1);
      else  printf("%d %d\n", heap->extracted_min.vertex+1 , heap->extracted_min.key);
    }
    if(graph[heap->extracted_min.vertex].neighbours != NULL)
    {
      if(heap->extracted_min.key + graph[heap->extracted_min.vertex].neighbours->weight < graph[graph[heap->extracted_min.vertex].neighbours->value].distance)
      {
        // updating the distaces in heap
        heap = decrease_key(heap , graph[heap->extracted_min.vertex].neighbours->value , heap->extracted_min.key + graph[heap->extracted_min.vertex].neighbours->weight);
        // updating distances in graph
        graph[graph[heap->extracted_min.vertex].neighbours->value].distance = heap->extracted_min.key + graph[heap->extracted_min.vertex].neighbours->weight;
        // updating the parent
        graph[graph[heap->extracted_min.vertex].neighbours->value].parent = heap->extracted_min.vertex;
      }
      while(graph[heap->extracted_min.vertex].neighbours->next != NULL)
      {
        graph[heap->extracted_min.vertex].neighbours = graph[heap->extracted_min.vertex].neighbours->next;
        if(heap->extracted_min.key + graph[heap->extracted_min.vertex].neighbours->weight < graph[graph[heap->extracted_min.vertex].neighbours->value].distance)
        {
          heap = decrease_key(heap , graph[heap->extracted_min.vertex].neighbours->value , heap->extracted_min.key + graph[heap->extracted_min.vertex].neighbours->weight);
          graph[graph[heap->extracted_min.vertex].neighbours->value].distance = heap->extracted_min.key + graph[heap->extracted_min.vertex].neighbours->weight;
          graph[graph[heap->extracted_min.vertex].neighbours->value].parent = heap->extracted_min.vertex;
        }
      }
      // setting the pointer to default
      while(graph[heap->extracted_min.vertex].neighbours->prev != NULL) graph[heap->extracted_min.vertex].neighbours = graph[heap->extracted_min.vertex].neighbours->prev;
    }
  }
  free(heap->heap_array); //freeing the array in heap
  free(heap); //freeing the heap;
  return graph;
}

void find_path(struct graph_node* graph , int u , int v){
  struct linkedlist_node* printing = NULL;
  // inserting into linked list
  printing = insert_linkedlist(v ,0, printing);
  if(graph[v].distance == INT_MAX)  printf("-1\n"); //checking wether the path exists or not
  else
  {
    printf("%d ", graph[v].distance); //printing the distance
    while(v != u && v != -1)  //traversing from the node to reference node
    {
      // printf("sdd\n");
      v = graph[v].parent;
      printing = insert_linkedlist(v, 0 , printing);
    }
    if(v == u)  //checking wether the path exists or not
    {
      while(printing->next != NULL)      printing = printing->next;
      printf("%d ",printing->value + 1);
      while(printing->prev != NULL)
      {
        printing = printing->prev;
        printf("%d ",printing->value + 1);  //printing the path
      }
      printf("\n");
    }
  }
  printing = free_list(printing); //freeing the list
  return;
}

int main() {
  struct graph_node* graph = NULL;  //initializing graph
  int digit , new_graph = 0;
  int graph_active = 0 , ref_node = -1 , start_node = 0 , link_node = 0 , link_active = 0, weight_active = 0;
  int check_link = 0;
  int dj_active = 0 , prev_dj_node = -1 ;
  int path_find = 0;
  int scanned_number = 0, scanned_digits[5] = {-1,-1,-1,-1,-1} , scan_point = 0;
  int no_nodes = 0;
  while((digit=fgetc(stdin))!=EOF){
    if(digit == 'N')
    {
      new_graph = 1;
      graph = free_graph(graph , no_nodes); //freeing the previous graph
      graph = NULL;
      prev_dj_node = -1;
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
      link_active = 0;
      weight_active = 0;
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
        link_active = 1;
      }
      else if(link_active ==1 && weight_active == 0)  //verifing if node or weight
      {
        link_node = scanned_number;
        link_active = 0;
        weight_active = 1;
      }
      else if(link_active ==0 && weight_active == 1)
      {
        // inserting the links to adjacency list
        graph[ref_node].neighbours = insert_linkedlist(link_node , scanned_number+1 , graph[ref_node].neighbours);
        weight_active = 0;
        link_active = 1;
      }

      if(digit == '\n')
      {
        graph_active = 0;
        ref_node = -1;
        link_node = -1;
        link_active = 0;
        weight_active = 0;
      }
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
        int printed = 1;  //taking care of one of boundary condition(if breaks at last node)
        // searching for the node in the ref_node's adjacency list
        if(graph[ref_node].neighbours == NULL) printf("-1\n");
        if(graph[ref_node].neighbours != NULL && digit == '\n')
        {
          if(graph[ref_node].neighbours->value == scanned_number) printf("%d\n",graph[ref_node].neighbours->weight);
          else
          {
            while(graph[ref_node].neighbours->next != NULL)
            {
              graph[ref_node].neighbours = graph[ref_node].neighbours->next;
              if(graph[ref_node].neighbours->value == scanned_number)
              {
                printf("%d\n",graph[ref_node].neighbours->weight);
                printed = 0;
                break;
              }
            }
            if(graph[ref_node].neighbours->next == NULL)  if(printed) printf("-1\n");
            while(graph[ref_node].neighbours->prev != NULL)  graph[ref_node].neighbours = graph[ref_node].neighbours->prev;
          }
        }
      }

      if(digit == '\n') check_link = 0;
      scan_point = 0;
      scanned_number = 0;
      for(int i = 0 ; i<5 ; i++)  scanned_digits[i] = -1;
    }
    else if(digit == 'D')   //for dijkstra's algorithm
    {
      dj_active = 1;
    }
    else if((digit == ' ' || digit == '\n') && dj_active == 1 && scanned_digits[0] != -1)
    {
      int temp = 1;
      for(int i = 0 ; i<scan_point ; i++)
      {
        scanned_number = scanned_number + scanned_digits[scan_point-1-i]*temp;
        temp = temp *10;
      }
      scanned_number = scanned_number - 1;

      prev_dj_node = scanned_number;
      graph = dijkstra(graph ,scanned_number, 1 , no_nodes);    //performing dijkstra's algorithm

      if(digit == '\n') dj_active = 0;
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
        if(prev_dj_node != scanned_number) //checking wether dijkstra's algorithm for this node is performed or not
        {
          prev_dj_node = scanned_number;
          graph = dijkstra(graph ,scanned_number, 0 , no_nodes);
        }
      }
      else
      {
        find_path(graph , ref_node , scanned_number); //finding the path from results of dijkstra's algorithm
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
    else if(!(digit == ' ' || digit == '\n')) //storing the digits scanned into array
    {
      scanned_digits[scan_point] = digit - 48;
      scan_point++;
    }
  }
  return 0;
}
