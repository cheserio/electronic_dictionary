# 系统编程

## 文件IO --实现电子词典

这一章将实现一个电子词典的程序，围绕这个项目将会介绍系统编程中文件IO相关的系统调用。并介绍实现一个项目的流程。

### 1.项目流程规范

​	1.需求的产生

​		需求来源：用户的反馈，用户研究，需求挖掘，数据分析，竞品分析

​		需求提出人员：客户，老板，自己，运营

​		如何拒绝需求：产品经理

​	2.编写需求文档

​	3.需求评审会

​	4.根据评审结果修改需求文档

​	5.确定需求文档，安排分工

​	6.技术部门

整个项目实现的口诀：自上而下逐步分解进行分析，自下而上逐步积累实现功能。

### 2.电子词典

#### 2.1 电子词典的需求文档

​	1.开始（提示信息）

​	2.把字典数据加载到内存（数据结构，链表之类的数据结构体。不是只有读取文件）

​	3.启动字典，支持用户反复查询，支持退出操作

​	4.释放内存，关闭字典

​	5.结束
#### 2.2 流程图

![image](https://user-images.githubusercontent.com/68575033/149669595-9d8cc8e7-97d6-492f-925c-78241fa5279c.png)

**每个功能的具体实现**

**加载字典数据到内存功能**

在将字典数据加载到内存的部分需要先打开文件，按行读取，判断是否读取成功，如果不成功可能是已经读取到文件末尾或者出错，这样我们直接结束程序。如果读取成功就创建单词结构体节点，添加英文单词，再继续读取一行翻译（可以从下面的字典资源文件中看出一行英文一行翻译的对应关系），因为这里是具有对应关系的前面读取到这里应该不会出现读取错误所以不再判断。然后添加读取到的翻译到结构体中。将制作好的结构体节点插入链表中。然后继续循环读取，直到无法继续读入。返回一个字典链表的头指针。

其中用到的标准库函数

```c
#include <stdio.h>
# 打开文件 传入文件路径以及打开权限，这里只需要可读权限即可
FILE *fp = fopen("filename", "r");
# 按行从文件读取 读到换行结束 传入接收读取数据的字符数组，以及大小，FILE结构体指针（打开的文件）
fgets(buff, sizeof(buff), fp);
# 关闭文件
fclose(fp);
```

**字典资源文件的格式**

![image](https://user-images.githubusercontent.com/68575033/149670102-658b6a7b-13c6-4a77-afa0-62c70c8c0c3e.png)

**加载字典数据到内存功能的流程图**

![image](https://user-images.githubusercontent.com/68575033/149670093-8c5580ec-765c-48ba-acf7-82ca7338731f.png)

**单词节点结构体**

这里使用链表的形式存储字典信息，所以需要创建一个节点结构体来存储信息。其中的成员有：英文单词，翻译，指向下一个节点的指针。

```c
typedef struct Node{
    char* word;
    char* trans;
    struct Node* next;
}Node;
```

**加载字典的代码实现**

```c
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
```

功能实现中的一些细节，在节点单词成员空间申请时需要+1因为fgets会在最后自动加上‘\0’.使用strcpy来复制字符串，移动起始指针位置去除字典数据中英文单词前面的#。在后面翻译的复制中也类似的功能去除Trans。节点的添加使用头插法。每次新节点的next指针指向head，然后将当前节点更新成head节点。

**启动字典，支持用户反复查询以及退出操作功能**

首先输出提示信息：请输入英文单词。输出一个单词。判断是否是退出关键词q！是的话直接结束整个程序，否则在链表中查找节点。判断是否查找到，是则输出单词翻译，并返回到输入单词提示那里，否则给出错误提示。

**流程图如下**

![image](https://user-images.githubusercontent.com/68575033/149670114-38da50cb-a2a5-49f4-852f-cee6b34367b4.png)

代码实现

```c
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
      	// 输出
        if(findNode){
            parseWord(findNode->trans);
        }else{
            printf("find error,please check your input!\n");
        }
    }
    return ;
}
```

**单词查找流程图**

如果没有到链表结尾NULL就一直查找，strcmp比较两个字符串（输入，节点的英文单词）。找到就返回当前节点指针，因为使用head查找移动的，这里直接返回head即可。否则向后移动head = head->next.

![image](https://user-images.githubusercontent.com/68575033/149670123-82ed7811-b600-439a-9af4-fff4b7f69a8a.png)
代码实现

```c
Node* searchWord(Node *head, char *word){
    while(head != NULL && strcmp(head->word, word)){
        head = head->next;
    }
    // 此时的head会返回null或则有效节点
    return head;
}
```

附加功能：由于有一词多意，有多词性的情况，这里封装一个parseWord函数分割不同词性。例如下面的情况，可以看到原本的数据中不同的词性间使用@分割。我们可以使用strtok实现该功能。该功能在findword函数中调用。

```
#account
Trans:n. 帐目;报告;估计;原因;利润;算账;记录@vi. 解释;认为;占;解决;得分;杀死@vt. 认为
```

代码实现

```c
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
```

**释放内存数据结束程序**

```c
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
```

主函数

```c
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
```

