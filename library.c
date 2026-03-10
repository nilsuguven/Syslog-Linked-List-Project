#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/**
 * @file main.c
 * @brief Linux Syslog yapisinin Cift Yonlu Bagli Liste ile modellenmesi.
 * * Teknik Gereksinim: Syslog verilerini islemek icin "Doubly Linked List"
 * kullanilmistir. Bu yapi, loglar arasinda ileri ve geri hareket etmeyi,
 * son eklenen loga hizli erisimi (Tail pointer sayesinde) saglar.
 */

// Her bir log kaydini temsil eden dugum yapisi
typedef struct LogNode {
    char timestamp[32];   // Logun zaman damgasi
    char level[16];       // Log seviyesi (INFO, ERROR, WARNING)
    char message[256];    // Log mesaji
    struct LogNode *next; // Sonraki dugume isaretci
    struct LogNode *prev; // Onceki dugume isaretci
} LogNode;

// Bagli listeyi yoneten ana kontrol yapisi
typedef struct {
    LogNode *head;        // Listenin baslangici
    LogNode *tail;        // Listenin sonu
    int count;            // Toplam dugum sayisi
} LogList;

// Fonksiyon Prototipleri
LogNode* createLogNode(char *ts, char *lvl, char *msg);
void appendLog(LogList *list, char *ts, char *lvl, char *msg);
void displayLogs(LogList *list);
void createDummyLogFile();
void readLogsFromFile(LogList *list, const char *filename);
void freeList(LogList *list);

int main() {
    LogList myLogs = {NULL, NULL, 0};

    // 1. Adim: Calisabilir bir demo icin ornek bir log dosyasi olustur
    printf("Sistem hazirlaniyor: Ornek log dosyasi olusturuluyor...\n");
    createDummyLogFile();

    // 2. Adim: Dosyadan verileri oku ve bagli listeye aktar
    printf("Veriler 'ornek_syslog.log' dosyasindan Bagli Listeye okunuyor...\n\n");
    readLogsFromFile(&myLogs, "ornek_syslog.log");

    // 3. Adim: Bagli listeyi ekrana bas (Demo)
    displayLogs(&myLogs);

    // 4. Adim: Bellegi temizle
    freeList(&myLogs);

    return 0;
}

// Yeni bir log dugumu olusturur ve bellek ayirir
LogNode* createLogNode(char *ts, char *lvl, char *msg) {
    LogNode *newNode = (LogNode*)malloc(sizeof(LogNode));
    if (!newNode) return NULL;

    strncpy(newNode->timestamp, ts, 31);
    strncpy(newNode->level, lvl, 15);
    strncpy(newNode->message, msg, 255);

    newNode->next = NULL;
    newNode->prev = NULL;
    return newNode;
}

// Listeye (kuyruga) yeni log ekler - O(1) karmasikligi ile calisir
void appendLog(LogList *list, char *ts, char *lvl, char *msg) {
    LogNode *newNode = createLogNode(ts, lvl, msg);
    if (!newNode) return;

    if (list->head == NULL) {
        list->head = newNode;
        list->tail = newNode;
    } else {
        newNode->prev = list->tail;
        list->tail->next = newNode;
        list->tail = newNode;
    }
    list->count++;
}

// Bagli listeyi bastan sona tarayarak teknik detaylariyla basar
void displayLogs(LogList *list) {
    LogNode *current = list->head;
    printf("============ BAGLI LISTE SYSLOG DEMOSU ============\n");
    printf("%-20s | %-10s | %s\n", "ZAMAN", "SEVIYE", "MESAJ");
    printf("---------------------------------------------------\n");

    while (current != NULL) {
        printf("%-20s | %-10s | %s", current->timestamp, current->level, current->message);
        current = current->next;
    }
    printf("===================================================\n");
    printf("Bilgi: Toplam %d adet log dugumu olusturuldu.\n", list->count);
}

// Demo icin sistemde gecici bir log dosyasi olusturan yardimci fonksiyon
void createDummyLogFile() {
    FILE *f = fopen("ornek_syslog.log", "w");
    if (!f) return;
    fprintf(f, "Mar 10 09:00:01 INFO System boot completed successfully.\n");
    fprintf(f, "Mar 10 09:05:12 WARNING High memory usage detected.\n");
    fprintf(f, "Mar 10 09:10:45 ERROR Failed to connect to database.\n");
    fprintf(f, "Mar 10 09:15:20 INFO User 'nilsu' logged into the system.\n");
    fprintf(f, "Mar 10 09:20:00 CRITICAL Disk space below 5%%!\n");
    fclose(f);
}

// Dosyayi satir satir okuyup parse eder
void readLogsFromFile(LogList *list, const char *filename) {
    FILE *f = fopen(filename, "r");
    if (!f) return;

    char line[512];
    while (fgets(line, sizeof(line), f)) {
        char ts[20], lvl[16], msg[256];
        // Basit bir ayristirma (Parse) islemi
        sscanf(line, "%15[^0-9]%*s %15s %[^\n]", ts, lvl, msg);
        // Not: Gercek syslog formatina gore basitlestirilmistir.
        appendLog(list, "Mar 10 09:xx", lvl, msg);
    }
    fclose(f);
}

// Dinamik ayrilan bellegi iade eder
void freeList(LogList *list) {
    LogNode *current = list->head;
    while (current != NULL) {
        LogNode *temp = current;
        current = current->next;
        free(temp);
    }
    list->head = NULL;
    list->tail = NULL;
}