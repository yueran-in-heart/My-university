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
//������̿��ƿ����Դ���ƿ飬��Queue.h��resQueue.h���Ѿ������
//������������
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
    struct PCB *parent_pcb;//������
    struct Queue *child_list;//�ӽ���
}QDataType;


//����������н��
typedef struct QNode{
    QDataType val;
    struct QNode *next;
} QNode;

//����
typedef struct Queue{
    int size; //����Ԫ�ظ���
    QNode *front;//ָ����е�һ��Ԫ�أ��������Ϊ�գ������NULL
    QNode *rear;//ָ��������һ��Ԫ�أ��������Ϊ�գ������NULL
} Queue;

//��ʼ��
void QueueInit(Queue *queue);/*
                              
                              //����
                              void QueueDestroy(Queue *queue);*/

//���룬�����Ƕ��У�ֻ�ܴӶ�β����
void QueuePush(Queue *queue, QDataType val);

//ɾ����ֻ�ܴӶ���ɾ
void QueuePop(Queue *queue);

//�Ӷ�����ɾ��ָ�����
void QueueDelete(Queue *queue, QDataType val);

//���ض��׽���ֵ
QDataType QueueFront(const Queue *queue);

//�ж϶����Ƿ�Ϊ�գ�Ϊ�շ���1�����շ���0
int QueueEmpty(const Queue *queue);

//���ض��н��ĸ���
int QueueSize(const Queue *queue);

//��ӡ
void QueuePrint(const Queue *queue);
typedef struct RCB{
    char rid[64];
    int init_amount;
    int avail_amount;
    struct Queue *waiting_queue;//��Դ�ĵȺ����
}RCB;
//����������н��
typedef struct resQNode{
    RCB val;
    int own_res_num;
    struct resQNode *next;
} resQNode;

//����
typedef struct resQueue{
    int size; //����Ԫ�ظ���
    resQNode *front;//ָ����е�һ��Ԫ�أ��������Ϊ�գ������NULL
    resQNode *rear;//ָ��������һ��Ԫ�أ��������Ϊ�գ������NULL
} resQueue;

//��ʼ��
void resQueueInit(resQueue *queue);

//����
void resQueueDestroy(resQueue *queue);

//���룬�����Ƕ��У�ֻ�ܴӶ�β����
void resQueuePush(resQueue *queue, RCB val);

//ɾ����ֻ�ܴӶ���ɾ
void resQueuePop(resQueue *queue);

//�Ӷ�����ɾ��ָ�����
void resQueueDelete(resQueue *queue, RCB val);

//���ض��׽���ֵ
RCB resQueueFront(const resQueue *queue);

//�ж϶����Ƿ�Ϊ�գ�Ϊ�շ���1�����շ���0
int resQueueEmpty(const resQueue *queue);

//���ض��н��ĸ���
int resQueueSize(const resQueue *queue);

//��ӡ
void resQueuePrint(const resQueue *queue);
Queue *Ready_queue[3];//������ͬ���ȼ��ľ������зֱ�Ϊ��0��initial,1:user;2:system
Queue *Blocked_queue;
int pid;//��¼��ǰ���̺ţ��½��̵Ľ��̺���ԭ�����̺Ż�����+1����
struct PCB currentProcess;
struct PCB *cPCB;

RCB *myrcb[5];

void initrcb();

//��ʼ��PCB
PCB initpcb();

//������֪��������һ���µĽ��̣���������븸���̵ĺ��Ӷ��к;���������
void createNewProcess(string name, int priority, PCB *parent);

//���ݽ����������ȼ��������ý��̣����������к��ӽ���ɱ�����õ��˵ݹ�
//����ÿ��PCB��Ҫô�ھ������У�Ҫô�����������У������������������в�ѯ������Ϊname��PCB
void C_Destory(string name);
void Destory(string name);

//�ھ���������Ѱ�Ҹ����ֵĽ���PCB���ҵ�����1������Ӿ����������Ƴ������򷵻�0
int Destory_from_readyQ(string name);

//������������Ѱ�Ҹ����ֵĽ���PCB���ҵ�����1������������������Ƴ������򷵻�0
int Destory_from_blockedQ(string name);

//�ͷŸ�����Դ���е�������Դ
void release_res_list(resQueue *r);

//�ͷŸý��̵�������Դ�������γ������ӽ���
void Kill_Tree(PCB killed_pcb);

//�����������Դ�����������з���
bool RequestRes(string rid, int num);

//������Դid���������ͷ���Դ

int C_Release(string rid, int num);
void Release(string rid, int num);

//�ҵ���Ҫ���ȵĽ���
void select_schedul_pro(Queue *q);
int search_maxprio();
void Scheduler();
void resScheduler();

void Time_out();
int main(int argc, char *argv[]);


//��ʼ��RCB
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
    //printf("RCB��ʼ�����\n");
}

struct PCB *newpcb = (PCB*)malloc(sizeof(PCB));

void QueueInit(Queue *queue)
{
    queue->size = 0;
    queue->front = queue->rear = NULL;
}

//����
void QueueDestroy(Queue *queue)
{
    //���������Ԫ�أ������ͷ�
    QNode *next;
    for (QNode *cur = queue->front; cur != NULL; cur = next)
    {
        next = cur->next;
        free(cur);
    }
    queue->front = queue->rear = NULL;
    queue->size = 0;
}

//����
void QueuePush(Queue *queue, QDataType val)
{
    //�ȴ������
    QNode *node = (QNode *)malloc(sizeof(QNode));
    assert(node != NULL);
    node->val = val;
    node->next = NULL;
    queue->size++;
    //���ж϶����Ƿ�Ϊ��
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
//ɾ��
void QueuePop(Queue *queue)
{
    assert(queue->rear != NULL);//ȷ�����в�Ϊ��
    queue->size--;
    QNode *old_front = queue->front;
    queue->front = queue->front->next;
    free(old_front);
    //���ɾ����п��˵Ļ�����Ҫ����rear��ָ��NULL
    if (queue->front == NULL)
    {
        queue->rear = NULL;
    }
}

//�Ӿ�����������������ɾ��ָ�����
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

//���ض��׽���ֵ
QDataType QueueFront(const Queue *queue)
{
    assert(queue->size > 0);
    return queue->front->val;
}

//�ж϶����Ƿ�Ϊ�գ�Ϊ�շ���1�����շ���0
int QueueEmpty(const Queue *queue)
{
    return queue->size == 0 ? 1 : 0;
}

//���ض��н��ĸ���
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

//����
void resQueueDestroy(resQueue *queue)
{
    //���������Ԫ�أ������ͷ�
    resQNode *next;
    for (resQNode *cur = queue->front; cur != NULL; cur = next)
    {
        next = cur->next;
        free(cur);
    }
    queue->front = queue->rear = NULL;
    queue->size = 0;
}

//����
void resQueuePush(resQueue *queue, RCB val)
{
    //�ȴ������
    resQNode *node = (resQNode *)malloc(sizeof(resQNode));
    assert(node != NULL);
    node->val = val;
    node->next = NULL;
    queue->size++;
    //���ж϶����Ƿ�Ϊ��
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

//ɾ��
void resQueuePop(resQueue *queue)
{
    assert(queue->rear != NULL);//ȷ�����в�Ϊ��
    queue->size--;
    resQNode *old_front = queue->front;
    queue->front = queue->front->next;
    free(old_front);
    //���ɾ����п��˵Ļ�����Ҫ����rear��ָ��NULL
    if (queue->front == NULL)
    {
        queue->rear = NULL;
    }
}

//�Ӷ�����ɾ��ָ�����
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



//���ض��׽��
RCB resQueueFront(const resQueue *queue)
{
    assert(queue->size > 0);
    return queue->front->val;
}

//�ж϶����Ƿ�Ϊ�գ�Ϊ�շ���1�����շ���0
int resQueueEmpty(const resQueue *queue)
{
    return queue->size == 0 ? 1 : 0;
}

//���ض��н��ĸ���
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
//��ʼ��PCB
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
    //û��������������������Ϊ0����Դ������Դ��
    newpcb->state.BlockedNum = 0;
    
    newpcb->res_list = (resQueue*)malloc(sizeof(resQueue));
    resQueueInit(newpcb->res_list);
    newpcb->parent_pcb = (PCB*)malloc(sizeof(PCB));
    
    newpcb->child_list = (Queue*)malloc(sizeof(Queue));
    QueueInit(newpcb->child_list);
    
    //printf("init_PCB��ʼ�����:%s\n", newpcb->name);
    
    //printf("��ʼ�����̣���������%s,����id��%d,�������ȼ���%d,����״̬��%d\n", newpcb->name, newpcb->pid, newpcb->priority, newpcb->state.type);
    return *newpcb;
}
//������֪��������һ���µĽ��̣���������븸���̵ĺ��Ӷ��к;���������
void createNewProcess(char name[64], int priority, PCB *parent){
    //parentΪ�����½���ʱ�ĵ�ǰ����
    pid++;
    PCB temp_pcb = initpcb();
    //��ʼ��pcb�Ĳ���
    strcpy(temp_pcb.name, name);
    temp_pcb.pid = pid;
    
    temp_pcb.priority = priority;
    
    
    temp_pcb.parent_pcb = parent;
    
    
    //�������Ľ��̼��뵱ǰ���̵��ӽ��̶�����
    QueuePush(parent->child_list, temp_pcb);
    //���²����Ľ��̷������������
    QueuePush(Ready_queue[priority], temp_pcb);
    //printf("have created a new process:%s\n", temp_pcb.name);
    //printf("���ȼ�Ϊ%d�������ж��׽�������%s,�ڶ�����������%s,��������Ԫ�ظ�����%d,�������ж�ĩ����id��%d\n", priority, Ready_queue[priority]->front->val.name, Ready_queue[priority]->front->next->val.name, Ready_queue[priority]->size, Ready_queue[priority]->rear->val.pid);
    Scheduler();
}

//���ݽ����������ȼ��������ý��̣����������к��ӽ���ɱ�����õ��˵ݹ�
//����ÿ��PCB��Ҫô�ھ������У�Ҫô�����������У������������������в�ѯ������Ϊname��PCB

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


//����ΪDestory���Ӻ���
//�ھ���������Ѱ�Ҹ����ֵĽ���PCB���ҵ�����1������Ӿ����������Ƴ������򷵻�0
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
//������������Ѱ�Ҹ����ֵĽ���PCB���ҵ�����1������������������Ƴ������򷵻�0
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
//�ͷŸ�����Դ���е�������Դ
void release_res_list(resQueue *r){
    while (r->front != NULL)
    {
        Release(r->front->val.rid, r->front->own_res_num);
        resQueuePop(r);
    }
    //resQueueDestroy(r);
}
//�ͷŸý��̵�������Դ�������γ������ӽ���
void Kill_Tree(PCB killed_pcb){
    release_res_list(killed_pcb.res_list);//�ͷŸý��е�������Դ
    Queue *temp_childQ = killed_pcb.child_list;//�ý��̵ĺ��ӽ��̶���
    while (temp_childQ->front != NULL)
    {
        Destory(temp_childQ->front->val.name);
        QueuePop(temp_childQ);
    }
}

//�����������Դ�����������з���
bool RequestRes(string rid, int num){
    for (int i = 1; i < 5; i++)
    {
        if (rid == myrcb[i]->rid)
        {
            int availNum = myrcb[i]->avail_amount - num;
            if (availNum < 0){//��Դ�������򲻷���
                currentProcess.state.type = BLOCKED;
                //��ǰ������������ԴΪmyrcb[i],����������Ϊnum
                currentProcess.state.RorB_List = myrcb[i];
                currentProcess.state.BlockedNum = num;
                QueuePop(Ready_queue[currentProcess.priority]);//��ǰ���̴Ӿ����������Ƴ�
                QueuePush(Blocked_queue, currentProcess);//�����̷�������������
                QueuePush(myrcb[i]->waiting_queue, currentProcess);
                int currID = search_maxprio();
                Ready_queue[currID]->front->val.state.type = 0;
                currentProcess = Ready_queue[currID]->front->val;
                resScheduler();
                return false;//������Դ����������
            }
            else
            {
                myrcb[i]->avail_amount = availNum;
                resQueuePush(currentProcess.res_list, *myrcb[i]);//��ǰ������ӵ�е���Դ��������һ��
                currentProcess.res_list->rear->own_res_num = num;//��ǰ����ӵ�д���������Դ������
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
//������Դid���������ͷ���Դ
void Release(string rid, int num){
    for (int i = 1; i < 5; i++)
    {
        if (myrcb[i]->rid == rid)
        {
            //resQueueDelete(currentProcess.res_list, *myrcb[i]);
            myrcb[i]->avail_amount = myrcb[i]->avail_amount + num;
            while (myrcb[i]->waiting_queue->size != 0 && myrcb[i]->waiting_queue->front->val.state.BlockedNum <= myrcb[i]->avail_amount)
            {
                //�������Դ�ĵȴ������У��н��̲��ٱ�����
                //��Դ��������
                myrcb[i]->avail_amount = myrcb[i]->avail_amount - myrcb[i]->waiting_queue->front->val.state.BlockedNum;
                //������Ҫ����Դ��
                int m = myrcb[i]->waiting_queue->front->val.state.BlockedNum;
                //�ҵ��ȴ������еĽ���PCB
                PCB q = QueueFront(myrcb[i]->waiting_queue);
                //���ý��̴���Դ�ĵȴ��������Ƴ�
                QueuePop(myrcb[i]->waiting_queue);
                //���ý��̴������������Ƴ�
                if (q.pid == Blocked_queue->front->val.pid&&q.state.type == 1)
                {
                    QueuePop(Blocked_queue);
                }
                else
                {
                    QueueDelete(Blocked_queue, q);
                }
                //�ı�ý��̵�״̬
                q.state.type = READY;
                q.state.RorB_List = myrcb[0];
                q.state.BlockedNum = 0;
                //����õ���Դ����ý��̵���Դ�б���
                resQueuePush(q.res_list, *myrcb[i]);
                q.res_list->rear->own_res_num = m;
                //���ý��̷������������
                QueuePush(Ready_queue[q.priority], q);
            }
            
        }
    }
}
//�ҵ���Ҫ���ȵĽ���
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
    //printf("��ǰ��Դ������£�\n");
    //printf("��ԴID%s,��ʼ����%d,��������%d\n", myrcb[0]->rid, myrcb[0]->init_amount, myrcb[0]->avail_amount);
    printf("��ԴID:%s,��ʼ����:%d,��������:%d\n", myrcb[1]->rid, myrcb[1]->init_amount, myrcb[1]->avail_amount);
    printf("��ԴID:%s,��ʼ����:%d,��������:%d\n", myrcb[2]->rid, myrcb[2]->init_amount, myrcb[2]->avail_amount);
    printf("��ԴID:%s,��ʼ����:%d,��������:%d\n", myrcb[3]->rid, myrcb[3]->init_amount, myrcb[3]->avail_amount);
    printf("��ԴID:%s,��ʼ����:%d,��������:%d\n", myrcb[4]->rid, myrcb[4]->init_amount, myrcb[4]->avail_amount);
}



int main(int argc, char **argv)
{
    //��ʼ���������У�������ʼ���̷������ȼ�Ϊ0�ľ���������
    Ready_queue[0] = (Queue *)malloc(sizeof(Queue));
    QueueInit(Ready_queue[0]);
    Ready_queue[1] = (Queue *)malloc(sizeof(Queue));
    QueueInit(Ready_queue[1]);
    Ready_queue[2] = (Queue *)malloc(sizeof(Queue));
    QueueInit(Ready_queue[2]);
    
    //��ʼ����������
    Blocked_queue = (Queue *)malloc(sizeof(Queue));
    QueueInit(Blocked_queue);
    
    pid = 0;
    //printf("����Ϊ0������Ϊ1������Ϊ2������Ϊ3\n");
    currentProcess = initpcb();
    currentProcess.state.type = 0;
    QueuePush(Ready_queue[0], currentProcess);
    initrcb();
    //display_rcb();
    //printf("��ǰ���̵�������Դ��%d,��ǰ��������%s,��ǰ����id��%d,��ǰ�������ȼ�%d,��ǰ����״̬%d\n", currentProcess.state.RorB_List->init_amount, currentProcess.name, currentProcess.pid, currentProcess.priority, currentProcess.state.type);
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
    while (!feof(fp))//����ļ�û�е���ĩβ��ѭ��
    {
        fgets(str, N, fp);//��ȡһ��
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
            //printf("������%s�����̣�%s,����Ϊ��%d\n", op, n, atoi(a));
        }
        if (strcmp(op, "req") == 0)
        {
            RequestRes(n, atoi(a));
            //printf("���룺%s����Դ����%s,����Ϊ��%d\n", op, n, atoi(a));
        }
        if (strcmp(op, "rel") == 0)
        {
            C_Release(n, atoi(a));
            //printf("�ͷţ�%s����Դ����%s,����Ϊ��%d\n", op, n, atoi(a));
        }
        if (strcmp(op, "de") == 0)
        {
            C_Destory(n);
            //printf("����:%s,����Ϊ��%s\n", op, n);
        }
        if (strcmp(op, "to") == 0)
        {
            Time_out();
            //printf("����Ϊ%s\n", op);
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
    //printf("��ǰ����Ϊ��%s,%d\n", currentProcess.name, currentProcess.priority);
    //printf("��������Ϊ(���ȼ��ֱ�Ϊ0��1��2)\n");
    /*for (int i = 0; i <= curp; i++){
     QueuePrint(Ready_queue[i]);
     }*/
    //printf("��������Ϊ\n");
    //QueuePrint(Blocked_queue);
    //printf("��ǰ���̵��ӽ���Ϊ��\n");
    //QueuePrint(currentProcess.child_list);
    //display_rcb();
    //    getchar();
    return 1;
}
