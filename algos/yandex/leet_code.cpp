#include <iostream>
#include <vector>
#include <atomic>

using namespace std;

class Solution {
 public:
  int maxArea(vector<int>& height) {
    int left = 0;
    int right = height.size() - 1;
    int max_area = 0;
    while (left < right) {
      int currentArea = min(height[left], height[right]) * (right - left);
      max_area = max(max_area, currentArea);
      if (height[left] < height[right] ) {
        left++;
      } else {
        right--;
      }
    }
    return max_area;
  }
};

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
int main()
{
  pid_t pid;
  int rv;
  switch(pid=fork()) {
    case -1:
      perror("fork"); /* произошла ошибка */
      exit(1); /*выход из родительского процесса*/
    case 0:
      printf(" CHILD: Это процесс-потомок!\n");
      printf(" CHILD: Мой PID -- %d\n", getpid());
      printf(" CHILD: PID моего родителя -- %d\n",
             getppid());
          printf(" CHILD: Введите мой код возврата(как можно меньше):");
          scanf(" %d");
          printf(" CHILD: Выход!\n");
          exit(rv);
  default:
          printf("PARENT: Это процесс-родитель!\n");
          printf("PARENT: Мой PID -- %d\n", getpid());
          printf("PARENT: PID моего потомка %d\n",pid);
          printf("PARENT: Я жду, пока потомокне вызовет exit()...\n");
          wait();
          printf("PARENT: Код возврата потомка:%d\n",
                   WEXITSTATUS(rv));
          printf("PARENT: Выход!\n");
  }
}