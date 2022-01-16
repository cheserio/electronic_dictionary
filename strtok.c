#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void parseWord(const char *trans){
    char *buff = (char *)malloc(sizeof(char) * (strlen(trans) + 1));
    strcpy(buff, trans);
    char *token;
    token = strtok(buff, "@");
    int i = 0;
    while(token){
        printf("i = %d, %s\n", i++, token);
        token = strtok(NULL, "@");
    }
    free(buff);
    return ;
}

int main(){
    // 注意这里的字符串只能使用字符数组
    char buff[1024] = "hello@#shi@@@yang#chen";
    // 如果使用字符串变量的话会有段错误
    // char *buff = "hello@#shi@@@yang#chen";
    int len = strlen(buff);
    printf("%s\n", buff); 

    parseWord(buff);
    // char *token;
    // token = strtok(buff, "@#");
    // int i = 0;
    // // 注意这里的strtok是会改变buff中内容将@或者#变成\0调用后打印原有字符串
    // // 只会输出第一个\0之前的数据
    // // 在头和尾巴上的分割符不会被改变
    // while(token){
    //     printf("i = %d, %s\n", i++, token);
    //     // strtok的调用分为两个部分第一个部分直接传入整个字符串
    //     // 第二次调用传入时传入NULL
    //     token = strtok(NULL, "@#");
    // }

    // 查看修改后的全部源字符串
    for(int i = 0; i < len; i++){
        printf("[%c]", buff[i]);
    }
    putchar(10);

    return 0;
}