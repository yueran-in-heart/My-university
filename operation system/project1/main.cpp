#define _CRT_SECURE_NO_WARNINGS 1
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include<assert.h>
using std::string;
#define RUNNING 0;
#define N 100
#define BLOCKED 1;
#define READY 2;
#define DESTORYED 3;
#define init 0;
#define user 1;
#define system 2;
//定义进程控制块和资源控制块，在Queue.h和resQueue.h中已经定义过
//定义两个队列
typedef struct STATE{
    int type;
    struct RCB *RorB_List;
    int BlockedNum;
}STATE;
typedef struct PCB{
    char name[64];
    int pid;
    int priority;
    struct STATE state;
    struct resQueue *res_list;
    struct PCB *parent_pcb;//父进程
    struct Queue *child_list;//子进程
}QDataType;


//定义链表队列结点
typedef struct QNode{
    QDataType val;
    struct QNode *next;
} QNode;

//队列
typedef struct Queue{
    int size; //队列元素个数
    QNode *front;//指向队列第一个元素，如果队列为空，则等于NULL
    QNode *rear;//指向队列最后一个元素，如果队列为空，则等于NULL
} Queue;

//初始化
void QueueInit(Queue *queue);/*
                              
                              //销毁
                              void QueueDestroy(Queue *queue);*/

//插入，由于是队列，只能从队尾插结点
void QueuePush(Queue *queue, QDataType val);

//删除，只能从队首删
void QueuePop(Queue *queue);

//从队列中删除指定结点
void QueueDelete(Queue *queue, QDataType val);

//返回队首结点的值
QDataType QueueFront(const Queue *queue);

//判断队列是否为空，为空返回1，不空返回0
int QueueEmpty(const Queue *queue);

//返回队列结点的个数
int QueueSize(const Queue *queue);

//打印
void QueuePrint(const Queue *queue);
typedef struct RCB{
    char rid[64];
    int init_amount;
    int avail_amount;
    struct Queue *waiting_queue;//资源的等候队列
}RCB;
//定义链表队列结点
typedef struct resQNode{
    RCB val;
    int own_res_num;
    struct resQNode *next;
} resQNode;

//队列
typedef struct resQueue{
    int size; //队列元素个数
    resQNode *front;//指向队列第一个元素，如果队列为空，则等于NULL
    resQNode *rear;//指向队列最后一个元素，如果队列为空，则等于NULL
} resQueue;

//初始化
void resQueueInit(resQueue *queue);

//销毁
void resQueueDestroy(resQueue *queue);

//插入，由于是队列，只能从队尾插结点
void resQueuePush(resQueue *queue, RCB val);

//删除，只能从队首删
void resQueuePop(resQueue *queue);

//从队列中删除指定结点
void resQueueDelete(resQueue *queue, RCB val);

//返回队首结点的值
RCB resQueueFront(const resQueue *queue);

//判断队列是否为空，为空返回1，不空返回0
int resQueueEmpty(const resQueue *queue);

//返回队列结点的个数
int resQueueSize(const resQueue *queue);

//打印
void resQueuePrint(const resQueue *queue);
Queue *Ready_queue[3];//三个不同优先级的就绪队列分别为：0：initial,1:user;2:system
Queue *Blocked_queue;
int pid;//记录当前进程号，新进程的进程号在原来进程号基础上+1即可
struct PCB currentProcess;
struct PCB *cPCB;

RCB *myrcb[5];

void initrcb();

//初始化PCB
PCB initpcb();

//根据已知条件创建一个新的进程，并将其加入父进程的孩子队列和就绪队列中
void createNewProcess(string name, int priority, PCB *parent);

//根据进程名和优先级，插销该进程，并将其所有孩子进程杀死，用到了递归
//由于每个PCB，要么在就绪队列，要么在阻塞队列中，所以在这两个队列中查询进程名为name的PCB
void C_Destory(string name);
void Destory(string name);

//在就绪队列中寻找该名字的进程PCB，找到返回1，将其从就绪队列中移除，否则返回0
int Destory_from_readyQ(string name);

//在阻塞队列中寻找该名字的进程PCB，找到返回1，将其从阻塞队列中移除，否则返回0
int Destory_from_blockedQ(string name);

//释放给定资源队列的所有资源
void release_res_list(resQueue *r);

//释放该进程的所有资源，并依次撤销其子进程
void Kill_Tree(PCB killed_pcb);

//根据申请的资源及其数量进行分配
bool RequestRes(string rid, int num);

//根据资源id和其数量释放资源

int C_Release(string rid, int num);
void Release(string rid, int num);

//找到需要调度的进程
void select_schedul_pro(Queue *q);
int search_maxprio();
void Scheduler();
void resScheduler();

void Time_out();
int main(int argc, char *argv[]);


//初始化RCB
void initrcb(){
    myrcb[0] = (RCB*)malloc(sizeof(RCB));
    strcpy(myrcb[0]->rid, "R0");
    myrcb[0]->init_amount = 0;
    myrcb[0]->avail_amount = 0;
    myrcb[0]->waiting_queue = (Queue*)malloc(sizeof(Queue));
    QueueInit(myrcb[0]->waiting_queue);
    
    myrcb[1] = (RCB*)malloc(sizeof(RCB));
    strcpy(myrcb[1]->rid, "R1");
    myrcb[1]->init_amount = 1;
    myrcb[1]->avail_amount = 1;
    myrcb[1]->waiting_queue = (Queue*)malloc(sizeof(Queue));
    QueueInit(myrcb[1]->waiting_queue);
    
    myrcb[2] = (RCB*)malloc(sizeof(RCB));
    strcpy(myrcb[2]->rid, "R2");
    myrcb[2]->init_amount = 2;
    myrcb[2]->avail_amount = 2;
    myrcb[2]->waiting_queue = (Queue*)malloc(sizeof(Queue));
    QueueInit(myrcb[2]->waiting_queue);
    
    myrcb[3] = (RCB*)malloc(sizeof(RCB));
    strcpy(myrcb[3]->rid, "R3");
    myrcb[3]->init_amount = 3;
    myrcb[3]->avail_amount = 3;
    myrcb[3]->waiting_queue = (Queue*)malloc(sizeof(Queue));
    QueueInit(myrcb[3]->waiting_queue);
    
    myrcb[4] = (RCB*)malloc(sizeof(RCB));
    strcpy(myrcb[4]->rid, "R4");
    myrcb[4]->init_amount = 4;
    myrcb[4]->avail_amount = 4;
    myrcb[4]->waiting_queue = (Queue*)malloc(sizeof(Queue));
    QueueInit(myrcb[4]->waiting_queue);
    //printf("RCB初始化完成\n");
}

struct PCB *newpcb = (PCB*)malloc(sizeof(PCB));

void QueueInit(Queue *queue)
{
    queue->size = 0;
    queue->front = queue->rear = NULL;
}

//销毁
void QueueDestroy(Queue *queue)
{
    //队内如果有元素，就先释放
    QNode *next;
    for (QNode *cur = queue->front; cur != NULL; cur = next)
    {
        next = cur->next;
        free(cur);
    }
    queue->front = queue->rear = NULL;
    queue->size = 0;
}

//插入
void QueuePush(Queue *queue, QDataType val)
{
    //先创建结点
    QNode *node = (QNode *)malloc(sizeof(QNode));
    assert(node != NULL);
    node->val = val;
    node->next = NULL;
    queue->size++;
    //再判断队列是否为空
    if (queue->rear == NULL)
    {
        queue->front = queue->rear = node;
    }
    else
    {
        queue->rear->next = node;
        queue->rear = node;
    }
}
//删除
void QueuePop(Queue *queue)
{
    assert(queue->rear != NULL);//确保队列不为空
    queue->size--;
    QNode *old_front = queue->front;
    queue->front = queue->front->next;
    free(old_front);
    //如果删完队列空了的话，就要设置rear的指向NULL
    if (queue->front == NULL)
    {
        queue->rear = NULL;
    }
}

//从就绪或者阻塞队列中删除指定结点
void QueueDelete(Queue *queue, QDataType val){
    QNode *p = queue->front;
    QNode *q = queue->front;
    if (p->val.pid == val.pid)
    {
        queue->front = queue->front->next;
        queue->size--;
        free(q);
        if (queue->front == NULL)
        {
            queue->rear = queue->front;
        }
    }
    else
    {
        while (p != NULL)
        {
            
            q = p->next;
            if (q->val.pid == val.pid)
            {
                if (q->next == NULL)
                {
                    p->next = NULL;
                    queue->size--;
                    queue->rear = p;
                    free(q);
                }
                else
                {
                    p->next = q->next;
                    queue->size--;
                    free(q);
                }
                break;
            }
            p = p->next;
        }
    }
}

//返回队首结点的值
QDataType QueueFront(const Queue *queue)
{
    assert(queue->size > 0);
    return queue->front->val;
}

//判断队列是否为空，为空返回1，不空返回0
int QueueEmpty(const Queue *queue)
{
    return queue->size == 0 ? 1 : 0;
}

//返回队列结点的个数
int QueueSize(const Queue *queue)
{
    return queue->size;
}

void QueuePrint(const Queue *queue)
{
    for (QNode *cur = queue->front; cur != NULL; cur = cur->next)
    {
        printf("%s,%d-->", cur->val.name, cur->val.state.type);
    }
    printf("\n");
}
void resQueueInit(resQueue *queue)
{
    queue->front = queue->rear = NULL;
    queue->size = 0;
}

//销毁
void resQueueDestroy(resQueue *queue)
{
    //队内如果有元素，就先释放
    resQNode *next;
    for (resQNode *cur = queue->front; cur != NULL; cur = next)
    {
        next = cur->next;
        free(cur);
    }
    queue->front = queue->rear = NULL;
    queue->size = 0;
}

//插入
void resQueuePush(resQueue *queue, RCB val)
{
    //先创建结点
    resQNode *node = (resQNode *)malloc(sizeof(resQNode));
    assert(node != NULL);
    node->val = val;
    node->next = NULL;
    queue->size++;
    //再判断队列是否为空
    if (queue->rear == NULL)
    {
        queue->front = queue->rear = node;
    }
    else
    {
        queue->rear->next = node;
        queue->rear = node;
    }
}

//删除
void resQueuePop(resQueue *queue)
{
    assert(queue->rear != NULL);//确保队列不为空
    queue->size--;
    resQNode *old_front = queue->front;
    queue->front = queue->front->next;
    free(old_front);
    //如果删完队列空了的话，就要设置rear的指向NULL
    if (queue->front == NULL)
    {
        queue->rear = NULL;
    }
}

//从队列中删除指定结点
void resQueueDelete(resQueue *queue, RCB val){
    resQNode *p = queue->front;
    resQNode *q = queue->front;
    if (p->val.rid == val.rid)
    {
        queue->front = queue->front->next;
        queue->size--;
        free(q);
        if (queue->front == NULL)
        {
            queue->rear = queue->front;
        }
    }
    else
    {
        while (p != NULL)
        {
            
            q = p->next;
            if (q->val.rid == val.rid)
            {
                if (q->next == NULL)
                {
                    p->next = NULL;
                    queue->size--;
                    queue->rear = p;
                    free(q);
                }
                else
                {
                    p->next = q->next;
                    queue->size--;
                    free(q);
                }
                break;
            }
            p = p->next;
        }
    }
}



//返回队首结点
RCB resQueueFront(const resQueue *queue)
{
    assert(queue->size > 0);
    return queue->front->val;
}

//判断队列是否为空，为空返回1，不空返回0
int resQueueEmpty(const resQueue *queue)
{
    return queue->size == 0 ? 1 : 0;
}

//返回队列结点的个数
int resQueueSize(const resQueue *queue)
{
    return queue->size;
}

void resQueuePrint(const resQueue *queue)
{
    for (resQNode *cur = queue->front; cur != NULL; cur = cur->next)
    {
        printf("%s-->", cur->val.rid);
    }
    printf("\n");
}
//初始化PCB
PCB initpcb(){
    strcpy(newpcb->name, "init");
    newpcb->pid = pid;
    newpcb->priority = init;
    newpcb->state.type = READY;
    newpcb->state.RorB_List = (RCB*)malloc(sizeof(RCB));
    strcpy(newpcb->state.RorB_List->rid, "R0");
    newpcb->state.RorB_List->avail_amount = 0;
    newpcb->state.RorB_List->init_amount = 0;
    newpcb->state.RorB_List->waiting_queue = (Queue*)malloc(sizeof(Queue));
    QueueInit(newpcb->state.RorB_List->waiting_queue);
    //没有阻塞，所以阻塞队列为0号资源（空资源）
    newpcb->state.BlockedNum = 0;
    
    newpcb->res_list = (resQueue*)malloc(sizeof(resQueue));
    resQueueInit(newpcb->res_list);
    newpcb->parent_pcb = (PCB*)malloc(sizeof(PCB));
    
    newpcb->child_list = (Queue*)malloc(sizeof(Queue));
    QueueInit(newpcb->child_list);
    
    //printf("init_PCB初始化完成:%s\n", newpcb->name);
    
    //printf("初始化进程：进程名：%s,进程id：%d,进程优先级：%d,进程状态：%d\n", newpcb->name, newpcb->pid, newpcb->priority, newpcb->state.type);
    return *newpcb;
}
//根据已知条件创建一个新的进程，并将其加入父进程的孩子队列和就绪队列中
void createNewProcess(char name[64], int priority, PCB *parent){
    //parent为创建新进程时的当前进程
    pid++;
    PCB temp_pcb = initpcb();
    //初始化pcb的参数
    strcpy(temp_pcb.name, name);
    temp_pcb.pid = pid;
    
    temp_pcb.priority = priority;
    
    
    temp_pcb.parent_pcb = parent;
    
    
    //将产生的进程加入当前进程的子进程队列中
    QueuePush(parent->child_list, temp_pcb);
    //将新产生的进程放入就绪队列中
    QueuePush(Ready_queue[priority], temp_pcb);
    //printf("have created a new process:%s\n", temp_pcb.name);
    //printf("优先级为%d就绪队列队首进程名：%s,第二个进程名：%s,就绪队列元素个数：%d,就绪队列队末进程id：%d\n", priority, Ready_queue[priority]->front->val.name, Ready_queue[priority]->front->next->val.name, Ready_queue[priority]->size, Ready_queue[priority]->rear->val.pid);
    Scheduler();
}

//根据进程名和优先级，插销该进程，并将其所有孩子进程杀死，用到了递归
//由于每个PCB，要么在就绪队列，要么在阻塞队列中，所以在这两个队列中查询进程名为name的PCB

void C_Destory(string name){
    Destory(name);
    int i = search_maxprio();
    Ready_queue[i]->front->val.state.type = 0;
    currentProcess = Ready_queue[i]->front->val;
    Scheduler();
}
void Destory(string name){
    int i = Destory_from_readyQ(name);
    if (i == 0){
        int j = Destory_from_blockedQ(name);
    }
    
}


//以下为Destory的子函数
//在就绪队列中寻找该名字的进程PCB，找到返回1，将其从就绪队列中移除，否则返回0
int Destory_from_readyQ(string name){
    QNode *temp_node;
    for (int prio = 0; prio < 3; prio++)
    {
        temp_node = Ready_queue[prio]->front;
        while (temp_node != NULL)
        {
            if (temp_node->val.name == name)
            {
                temp_node->val.state.type = 3;
                Kill_Tree(temp_node->val);
                QueueDelete(Ready_queue[prio], temp_node->val);
                return 1;
            }
            else
            {
                temp_node = temp_node->next;
            }
        }
    }
    return 0;
}
//在阻塞队列中寻找该名字的进程PCB，找到返回1，将其从阻塞队列中移除，否则返回0
int Destory_from_blockedQ(string name){
    QNode *temp_node;
    temp_node = Blocked_queue->front;
    while (temp_node != NULL)
    {
        if (temp_node->val.name == name)
        {
            for (int i = 1; i < 5; i++)
            {
                if (myrcb[i]->rid == temp_node->val.state.RorB_List->rid)
                QueueDelete(myrcb[i]->waiting_queue, temp_node->val);
            }
            temp_node->val.state.type = 3;
            Kill_Tree(temp_node->val);
            QueueDelete(Blocked_queue, temp_node->val);
            
            return 1;
        }
        else
        {
            temp_node = temp_node->next;
        }
    }
    return 0;
}
//释放给定资源队列的所有资源
void release_res_list(resQueue *r){
    while (r->front != NULL)
    {
        Release(r->front->val.rid, r->front->own_res_num);
        resQueuePop(r);
    }
    //resQueueDestroy(r);
}
//释放该进程的所有资源，并依次撤销其子进程
void Kill_Tree(PCB killed_pcb){
    release_res_list(killed_pcb.res_list);//释放该进行的所有资源
    Queue *temp_childQ = killed_pcb.child_list;//该进程的孩子进程队列
    while (temp_childQ->front != NULL)
    {
        Destory(temp_childQ->front->val.name);
        QueuePop(temp_childQ);
    }
}

//根据申请的资源及其数量进行分配
bool RequestRes(string rid, int num){
    for (int i = 1; i < 5; i++)
    {
        if (rid == myrcb[i]->rid)
        {
            int availNum = myrcb[i]->avail_amount - num;
            if (availNum < 0){//资源不够，则不分配
                currentProcess.state.type = BLOCKED;
                //当前进程阻塞的资源为myrcb[i],阻塞的数量为num
                currentProcess.state.RorB_List = myrcb[i];
                currentProcess.state.BlockedNum = num;
                QueuePop(Ready_queue[currentProcess.priority]);//当前进程从就绪队列中移除
                QueuePush(Blocked_queue, currentProcess);//将进程放入阻塞队列中
                QueuePush(myrcb[i]->waiting_queue, currentProcess);
                int currID = search_maxprio();
                Ready_queue[currID]->front->val.state.type = 0;
                currentProcess = Ready_queue[currID]->front->val;
                resScheduler();
                return false;//可用资源不够，阻塞
            }
            else
            {
                myrcb[i]->avail_amount = availNum;
                resQueuePush(currentProcess.res_list, *myrcb[i]);//当前进程所拥有的资源队列增加一项
                currentProcess.res_list->rear->own_res_num = num;//当前进程拥有此类新增资源的数量
                resScheduler();
                return true;
            }
        }
    }
    return false;
}
int C_Release(string rid, int num){
    resQNode *res = currentProcess.res_list->front;
    while (res != NULL)
    {
        if (res->val.rid == rid)
        {
            res->own_res_num = res->own_res_num - num;
            if (res->own_res_num == 0)
            {
                resQueueDelete(currentProcess.res_list, res->val);
            }
            break;
        }
        res = res->next;
    }
    Release(rid, num);
    Scheduler();
    return 1;
}
//根据资源id和其数量释放资源
void Release(string rid, int num){
    for (int i = 1; i < 5; i++)
    {
        if (myrcb[i]->rid == rid)
        {
            //resQueueDelete(currentProcess.res_list, *myrcb[i]);
            myrcb[i]->avail_amount = myrcb[i]->avail_amount + num;
            while (myrcb[i]->waiting_queue->size != 0 && myrcb[i]->waiting_queue->front->val.state.BlockedNum <= myrcb[i]->avail_amount)
            {
                //如果该资源的等待队列中，有进程不再被阻塞
                //资源数量减少
                myrcb[i]->avail_amount = myrcb[i]->avail_amount - myrcb[i]->waiting_queue->front->val.state.BlockedNum;
                //进程需要的资源数
                int m = myrcb[i]->waiting_queue->front->val.state.BlockedNum;
                //找到等待队列中的进程PCB
                PCB q = QueueFront(myrcb[i]->waiting_queue);
                //将该进程从资源的等待队列中移除
                QueuePop(myrcb[i]->waiting_queue);
                //将该进程从阻塞队列中移除
                if (q.pid == Blocked_queue->front->val.pid&&q.state.type == 1)
                {
                    QueuePop(Blocked_queue);
                }
                else
                {
                    QueueDelete(Blocked_queue, q);
                }
                //改变该进程的状态
                q.state.type = READY;
                q.state.RorB_List = myrcb[0];
                q.state.BlockedNum = 0;
                //将获得的资源放入该进程的资源列表中
                resQueuePush(q.res_list, *myrcb[i]);
                q.res_list->rear->own_res_num = m;
                //将该进程放入就绪队列中
                QueuePush(Ready_queue[q.priority], q);
            }
            
        }
    }
}
//找到需要调度的进程
void select_schedul_pro(Queue *q){
    QNode *p = q->front;
    if (currentProcess.priority < p->val.priority || currentProcess.state.type != 0 || currentProcess.state.type == 3 || currentProcess.pid == 0)
    {
        if (currentProcess.name == "init")
        {
            QueuePop(q);
        }
        int j = currentProcess.priority;
        PCB temp = Ready_queue[j]->front->val;
        QueuePop(Ready_queue[j]);
        temp.state.type = 2;
        QueuePush(Ready_queue[j], temp);
        //printf("%d", currentProcess.state.type);
        p->val.state.type = 0;//RUNNING=0
        currentProcess = p->val;
        
    }
    printf("%s ", currentProcess.name);
}
int search_maxprio(){
    
    if (Ready_queue[2]->size != 0 && Ready_queue[2]->front->val.priority == 2 && Ready_queue[2]->rear->val.priority == 2 && Ready_queue[2]->front->val.state.type != 1)
    {
        return 2;//currentProcess = Ready_queue[2]->front->val;
    }
    else if (Ready_queue[1]->size != 0)
    {
        //currentProcess = Ready_queue[1]->front->val;
        return 1;
    }
    return 0;
}
void Scheduler(){
    int i = search_maxprio();
    select_schedul_pro(Ready_queue[i]);
    //select_schedul_pro(Ready_queue[currentProcess.priority]);
    
}
void resScheduler(){
    select_schedul_pro(Ready_queue[currentProcess.priority]);
}

void Time_out(){
    int i = currentProcess.priority;
    PCB p = currentProcess;
    QueuePop(Ready_queue[i]);
    p.state.type = 2;
    QueuePush(Ready_queue[i], p);
    Ready_queue[i]->front->val.state.type = 0;
    currentProcess = Ready_queue[i]->front->val;
    Scheduler();
}
void display_rcb(){
    //printf("当前资源情况如下：\n");
    //printf("资源ID%s,初始数量%d,可用数量%d\n", myrcb[0]->rid, myrcb[0]->init_amount, myrcb[0]->avail_amount);
    printf("资源ID:%s,初始数量:%d,可用数量:%d\n", myrcb[1]->rid, myrcb[1]->init_amount, myrcb[1]->avail_amount);
    printf("资源ID:%s,初始数量:%d,可用数量:%d\n", myrcb[2]->rid, myrcb[2]->init_amount, myrcb[2]->avail_amount);
    printf("资源ID:%s,初始数量:%d,可用数量:%d\n", myrcb[3]->rid, myrcb[3]->init_amount, myrcb[3]->avail_amount);
    printf("资源ID:%s,初始数量:%d,可用数量:%d\n", myrcb[4]->rid, myrcb[4]->init_amount, myrcb[4]->avail_amount);
}



int main(int argc, char **argv)
{
    //初始化就绪队列，并将初始进程放入优先级为0的就绪队列中
    Ready_queue[0] = (Queue *)malloc(sizeof(Queue));
    QueueInit(Ready_queue[0]);
    Ready_queue[1] = (Queue *)malloc(sizeof(Queue));
    QueueInit(Ready_queue[1]);
    Ready_queue[2] = (Queue *)malloc(sizeof(Queue));
    QueueInit(Ready_queue[2]);
    
    //初始化阻塞队列
    Blocked_queue = (Queue *)malloc(sizeof(Queue));
    QueueInit(Blocked_queue);
    
    pid = 0;
    //printf("运行为0，阻塞为1，就绪为2，销毁为3\n");
    currentProcess = initpcb();
    currentProcess.state.type = 0;
    QueuePush(Ready_queue[0], currentProcess);
    initrcb();
    //display_rcb();
    //printf("当前进程的阻塞资源：%d,当前进程名：%s,当前进程id：%d,当前进程优先级%d,当前进程状态%d\n", currentProcess.state.RorB_List->init_amount, currentProcess.name, currentProcess.pid, currentProcess.priority, currentProcess.state.type);
    //printf("hello world\n");
    //display_rcb();
    FILE *fp;
    if ((fp = fopen(argv[1], "rt")) == NULL){
        printf("Cannot open file, press any key to exit!\n");
        getchar();
        //return 0;
        exit(1);
    }
    char str[N + 1];
    printf("init ");
    while (!feof(fp))//如果文件没有到达末尾则循环
    {
        fgets(str, N, fp);//读取一行
        if (&str[6] == "\n" || str[2] == '\n' || str[8] == '\n' || str[4] == '\n')
        {
            str[strlen(str) - 1] = 0;
        }
        char *ptr = NULL;
        char *op = NULL;
        char *n = NULL;
        char *a = NULL;
        ptr = strtok(str, " ");
        int f = 0;
        while (ptr != NULL) {
            if (f == 0) op = ptr;
            if (f == 1) n = ptr;
            if (f == 2) a = ptr;
            ptr = strtok(NULL, " ");
            f++;
        }
        //printf("%d", f);
        if (strcmp(op, "cr") == 0)
        {
            createNewProcess(n, atoi(a), &currentProcess);
            //printf("创建：%s，进程：%s,数量为：%d\n", op, n, atoi(a));
        }
        if (strcmp(op, "req") == 0)
        {
            RequestRes(n, atoi(a));
            //printf("申请：%s，资源名：%s,数量为：%d\n", op, n, atoi(a));
        }
        if (strcmp(op, "rel") == 0)
        {
            C_Release(n, atoi(a));
            //printf("释放：%s，资源名：%s,数量为：%d\n", op, n, atoi(a));
        }
        if (strcmp(op, "de") == 0)
        {
            C_Destory(n);
            //printf("撤销:%s,进程为：%s\n", op, n);
        }
        if (strcmp(op, "to") == 0)
        {
            Time_out();
            //printf("操作为%s\n", op);
        }
        
    }
    fclose(fp);/*
                createNewProcess("x", 1, &currentProcess);
                createNewProcess("p", 1, &currentProcess);
                createNewProcess("q", 1, &currentProcess);
                createNewProcess("r", 1, &currentProcess);
                Time_out();
                Time_out();
                Time_out();
                RequestRes("R1", 1);
                Time_out();
                RequestRes("R2", 1);
                Time_out();
                RequestRes("R3", 2);
                Time_out();
                Time_out();
                C_Release("R1", 1);
                Time_out();
                RequestRes("R3", 3);
                C_Destory("p");
                Time_out();*/
    
    
    
    //int curp = currentProcess.priority;
    //printf("当前进程为：%s,%d\n", currentProcess.name, currentProcess.priority);
    //printf("就绪队列为(优先级分别为0，1，2)\n");
    /*for (int i = 0; i <= curp; i++){
     QueuePrint(Ready_queue[i]);
     }*/
    //printf("阻塞队列为\n");
    //QueuePrint(Blocked_queue);
    //printf("当前进程的子进程为：\n");
    //QueuePrint(currentProcess.child_list);
    //display_rcb();
    //    getchar();
    return 1;
}
