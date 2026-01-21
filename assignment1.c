#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>

int main() {
    int arr[30];
    int fd1[2], fd2[2];
    pid_t p1, p2;

    srand(time(NULL));

    printf("Array initialized by Parent:\n");
    for (int i = 0; i < 30; i++) {
        arr[i] = rand() % 50 + 1;
        printf("%d ", arr[i]);
    }
    printf("\n");

    pipe(fd1);
    pipe(fd2);

    p1 = fork();
    if (p1 == 0) {
        close(fd1[0]);
        int sum = 0;
        for (int i = 0; i < 15; i++) {
            if (arr[i] % 2 == 0)
                sum += arr[i];
        }
        write(fd1[1], &sum, sizeof(sum));
        close(fd1[1]);
        _exit(0);
    }

    p2 = fork();
    if (p2 == 0) {
        close(fd2[0]);
        int sum = 0;
        for (int i = 15; i < 30; i++) {
            if (arr[i] % 2 == 0)
                sum += arr[i];
        }
        write(fd2[1], &sum, sizeof(sum));
        close(fd2[1]);
        _exit(0);
    }

    close(fd1[1]);
    close(fd2[1]);

    int sum1, sum2;
    read(fd1[0], &sum1, sizeof(sum1));
    read(fd2[0], &sum2, sizeof(sum2));

    waitpid(p1, NULL, 0);
    waitpid(p2, NULL, 0);

    printf("\nTotal Even Sum of left side and right side are: %d and %d\n",
           sum1, sum2);

    return 0;
}
