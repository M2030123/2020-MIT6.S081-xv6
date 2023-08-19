#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
    int p1[2], p2[2];         // 定义两个管道，p1 用于父进程向子进程发送数据，p2 用于子进程向父进程发送数据
    char buffer[] = {'l'};    // 定义一个缓冲区，用于读取和写入数据
    int len = sizeof(buffer); // 缓冲区的大小
    pipe(p1);                 // 创建管道 p1
    pipe(p2);                 // 创建管道 p2

    if (fork() == 0) // 创建子进程
    {
        // 子进程
        close(p1[1]); // 关闭管道 p1 的写入端
        close(p2[0]); // 关闭管道 p2 的读取端

        // 从管道 p1 读取数据到缓冲区，检查是否成功读取指定字节数
        if (read(p1[0], buffer, len) != len)
        {
            printf("child read error!\n");
            exit(1);
        }

        printf("%d: received ping\n", getpid()); // 打印子进程的进程 ID 和接收消息

        // 向管道 p2 写入数据，检查是否成功写入指定字节数
        if (write(p2[1], buffer, len) != len)
        {
            printf("child write error\n");
            exit(1);
        }
    }
    else
    {
        // 父进程
        close(p1[0]); // 关闭管道 p1 的读取端
        close(p2[1]); // 关闭管道 p2 的写入端

        // 向管道 p1 写入数据，检查是否成功写入指定字节数
        if (write(p1[1], buffer, len) != len)
        {
            printf("parent write error!\n");
            exit(1);
        }

        // 从管道 p2 读取数据到缓冲区，检查是否成功读取指定字节数
        if (read(p2[0], buffer, len) != len)
        {
            printf("parent read error!\n");
            exit(1);
        }

        printf("%d: received pong\n", getpid()); // 打印父进程的进程 ID 和接收消息
    }

    exit(0); // 退出进程
}
