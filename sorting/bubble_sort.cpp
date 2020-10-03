#include <bits/stdc++.h>
using namespace std;
void sort(vector<int>& arr, int n)
{
    int i, j,temp;
    for (i = 0; i < n-1; i++)
    {
      for (j = 0; j < n-i-1; j++)
      {
        if (arr[j] > arr[j+1])
          {
            temp=arr[j];
            arr[j]=arr[j+1];
            arr[j+1]=temp;
          }
        }
      }

}
int main()
{
  cout<<"enter size of array\n";
  int size;
  cin>>size;
  cout<<"enter numbers to be sorted\n";
  vector<int> numbers;
  int temp;
  for (int i = 0; i < size; i++)
  {
    cin>>temp;
    numbers.push_back(temp);
  }
sort(numbers,size);
for (int i = 0; i < size; i++)
{
  cout<<numbers[i]<<"  ";

}
  return 0;
}
//...
