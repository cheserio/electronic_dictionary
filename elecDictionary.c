#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct Node{
    char* word;
    char* trans;
    struct Node* next;
}Node;

void parseWord(const char *trans){
    char *buff = (char *)malloc(sizeof(char) * (strlen(trans) + 1));
    strcpy(buff, trans);
    char *token;
    token = strtok(buff, "@");
    printf("[-------翻译------]\n");
    while(token){
        // 有颜色的格式化输出 3前景色 4背景色
        // 选择完前景色和后景色后的数字是颜色 m表示开始 0m表示结束
        printf("\033[33m-%s\033[0m\n", token);
        token = strtok(NULL, "@");
    }
    free(buff);
    return ;
}

Node *loadDict(char *filename){
    FILE *fp = fopen(filename, "r");
    if(fp == NULL){
        perror("fopen:");
        exit(1);
    }
    // 循环读取 非空继续读取
    char *s;
    char buff[1024] = {0}; // 存入缓冲区
    // 创建链表头节点
    Node *head = NULL;

    while(fgets(buff, sizeof(buff), fp) != NULL){
        // 创建节点
        Node *node = (Node *)malloc(sizeof(Node));

        // 把单词存入节点 最后会有\0fget自己添加的
        node->word = (char *)malloc(sizeof(char) * (strlen(buff) + 1));
        strcpy(node->word, buff + 1); // 去除英文单词开始的#
        // 去除\n  把\n变成\0 下次查询时到\0直接结束
        node->word[strlen(node->word) - 1] = '\0';

        // 翻译的添加
        if(fgets(buff, sizeof(buff), fp) != NULL){
            node->trans = (char *)malloc(sizeof(char) * (strlen(buff) + 1)); // size多了一个\0
            strcpy(node->trans, buff + 6);// 去除Trans:
            node->trans[strlen(node->trans) - 1] = '\0';
        }
        // 把整个节点添加到链表 头插法
        node->next = head;
        head = node;
    }
    fclose(fp);
    return head;
}

// 测试函数
void test(Node *head){
    int i = 0;
    while(head && (i++ < 100)){
        printf("[word]:%s\n", head->word);
        parseWord(head->trans);
        head = head->next;
    } 
    return ;
}

Node* searchWord(Node *head, char *word){
    while(head != NULL && strcmp(head->word, word)){
        head = head->next;
    }
    // 此时的head会返回null或则有效节点
    return head;
}


void findWord(Node *head){
    char buff[1024];
    while(1){
        printf("Please input a word:");
        scanf("%s", buff);
        if(!strcmp(buff, "q!")){
            printf("\033[32msee u later~\033[0m\n");
            break; // 之后还要手动释放内存所以这里不用exit
        }
        Node *findNode = searchWord(head, buff);
        if(findNode){
            parseWord(findNode->trans);
        }else{
            printf("find error,please check your input!\n");
        }
    }
    return ;
}

void freeDict(Node *head){
    Node *temp;
    while(head){
        temp = head;
        head = head->next;
        free(temp->word);
        free(temp->trans);
        free(temp);
    }
    return ;
}

int main(){
    // 执行语法 ./a.out 
    // 提示信息：输入英文单词:word
    // 输出单词解释 
    // a : [词性：一个]  
    //     [词性： ]


    // 把字典数据加载到内存
    // 打开文件
    // 读取文件
    // 使用链表的数据结构存储单词信息，节点结构体（单词，翻译，指针指向下一个节点）
    char *filename = "./dict.txt";
    Node *node;
    node = loadDict(filename);
    // test(node);


    // 启动字典支持用户反复查询，支持退出的操作
    // 错误提示 是否查找到
    findWord(node);



    // 释放内存关闭字典
    freeDict(node);
    return 0;
}