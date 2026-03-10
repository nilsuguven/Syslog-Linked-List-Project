#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @file main.c
 * @brief Linux Syslog verilerinin Çift Yönlü Bağlı Liste kullanılarak modellenmesi
 *
 * Bu projede Linux sistemlerinde kullanılan Syslog yapısı simüle edilmiştir.
 * Log kayıtları dosyadan okunarak Doubly Linked List veri yapısına aktarılır.
 */

// Log düğümü
typedef struct LogNode {
    char timestamp[32];
    char level[16];
    char message[256];
    struct LogNode *next;
    struct LogNode *prev;
} LogNode;

// Liste kontrol yapısı
typedef struct {
    LogNode *head;
    LogNode *tail;
    int count;
} LogList;

// Fonksiyonlar
LogNode* createLogNode(char *ts, char *lvl, char *msg);
void appendLog(LogList *list, char *ts, char *lvl, char *msg);
void displayLogs(LogList *list);
void createDummyLogFile();
void readLogsFromFile(LogList *list, const char *filename);
void freeList(LogList *list);

int main() {

    LogList myLogs = {NULL,NULL,0};

    printf("Ornek syslog dosyasi olusturuluyor...\n");
    createDummyLogFile();

    printf("Loglar dosyadan okunuyor...\n\n");
    readLogsFromFile(&myLogs,"ornek_syslog.log");

    displayLogs(&myLogs);

    freeList(&myLogs);

    return 0;
}

// Yeni log düğümü oluşturur
LogNode* createLogNode(char *ts, char *lvl, char *msg) {

    LogNode *newNode = (LogNode*)malloc(sizeof(LogNode));
    if(!newNode) return NULL;

    strncpy(newNode->timestamp,ts,31);
    strncpy(newNode->level,lvl,15);
    strncpy(newNode->message,msg,255);

    newNode->timestamp[31]='\0';
    newNode->level[15]='\0';
    newNode->message[255]='\0';

    newNode->next=NULL;
    newNode->prev=NULL;

    return newNode;
}

// Liste sonuna log ekleme
void appendLog(LogList *list,char *ts,char *lvl,char *msg){

    LogNode *newNode=createLogNode(ts,lvl,msg);
    if(!newNode) return;

    if(list->head==NULL){
        list->head=newNode;
        list->tail=newNode;
    }
    else{
        newNode->prev=list->tail;
        list->tail->next=newNode;
        list->tail=newNode;
    }

    list->count++;
}

// Listeyi yazdırma
void displayLogs(LogList *list){

    LogNode *current=list->head;

    printf("=========== SYSLOG LISTESI ===========\n");
    printf("%-20s | %-10s | %s\n","ZAMAN","SEVIYE","MESAJ");
    printf("--------------------------------------\n");

    while(current!=NULL){

        printf("%-20s | %-10s | %s\n",
               current->timestamp,
               current->level,
               current->message);

        current=current->next;
    }

    printf("--------------------------------------\n");
    printf("Toplam log sayisi: %d\n",list->count);
}

// Örnek log dosyası oluşturma
void createDummyLogFile(){

    FILE *f=fopen("ornek_syslog.log","w");
    if(!f) return;

    fprintf(f,"Mar 10 09:00:01 INFO System boot completed successfully\n");
    fprintf(f,"Mar 10 09:05:12 WARNING High memory usage detected\n");
    fprintf(f,"Mar 10 09:10:45 ERROR Failed to connect to database\n");
    fprintf(f,"Mar 10 09:15:20 INFO User 'nilsu' logged into system\n");
    fprintf(f,"Mar 10 09:20:00 CRITICAL Disk space below 5 percent\n");

    fclose(f);
}

// Dosyadan log okuma
void readLogsFromFile(LogList *list,const char *filename){

    FILE *f=fopen(filename,"r");
    if(!f) return;

    char line[512];

    while(fgets(line,sizeof(line),f)){

        char month[10];
        char day[5];
        char time[10];
        char level[16];
        char message[256];

        sscanf(line,"%s %s %s %s %[^\n]",month,day,time,level,message);

        char timestamp[32];
        sprintf(timestamp,"%s %s %s",month,day,time);

        appendLog(list,timestamp,level,message);
    }

    fclose(f);
}

// Bellek temizleme
void freeList(LogList *list){

    LogNode *current=list->head;

    while(current!=NULL){
        LogNode *temp=current;
        current=current->next;
        free(temp);
    }

    list->head=NULL;
    list->tail=NULL;
}
