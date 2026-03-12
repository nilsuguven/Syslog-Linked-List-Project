#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @struct LogNode
 * @brief Her bir sistem günlüğünü temsil eden düğüm yapısı.
 * * Bu yapı, bir bağlı listenin parçasıdır ve hem önceki hem de 
 * sonraki düğüme işaret ederek "Çift Yönlü Bağlı Liste" oluşturur.
 */
typedef struct LogNode {
    char timestamp[32];      // Olayın gerçekleştiği tarih ve saat (Örn: Mar 10 09:00)
    char level[16];          // Günlüğün önem seviyesi (INFO, ERROR, WARNING vb.)
    char message[256];       // Günlük mesajının detaylı içeriği
    struct LogNode *next;    // Bir sonraki günlük kaydına işaretçi
    struct LogNode *prev;    // Bir önceki günlük kaydına işaretçi
} LogNode;

/**
 * @struct LogList
 * @brief Bağlı listeyi yönetmek için kullanılan kontrol yapısı.
 * * Listenin başını ve sonunu tutarak ekleme işlemlerini hızlandırır.
 */
typedef struct {
    LogNode *head;           // Listenin ilk elemanı
    LogNode *tail;           // Listenin son elemanı
    int count;               // Listede toplam kaç adet günlük kaydı olduğunu tutar
} LogList;

/* --- Fonksiyon Prototipleri --- */
LogNode* createLogNode(char *ts, char *lvl, char *msg);
void appendLog(LogList *list, char *ts, char *lvl, char *msg);
void displayLogs(LogList *list);
void createDummyLogFile();
void readLogsFromFile(LogList *list, const char *filename);
void freeList(LogList *list);

int main() {
    // Liste yönetim yapısını ilklendiriyoruz (Initialize)
    LogList myLogs = {NULL, NULL, 0};

    // 1. Adım: Test amaçlı sahte bir log dosyası oluşturuluyor
    createDummyLogFile();

    // 2. Adım: Oluşturulan dosyadan veriler okunup Bağlı Liste yapısına aktarılıyor
    printf("Sistem gunlukleri dosyadan okunuyor...\n\n");
    readLogsFromFile(&myLogs, "ornek_syslog.log");

    // 3. Adım: Liste üzerindeki veriler ekrana formatlı şekilde basılıyor
    displayLogs(&myLogs);

    // 4. Adım: Program kapanırken ayrılan bellek sisteme iade ediliyor
    freeList(&myLogs);

    return 0;
}

/**
 * @brief Dinamik olarak yeni bir LogNode oluşturur.
 * @return Oluşturulan düğümün adresi (Hata durumunda NULL)
 */
LogNode* createLogNode(char *ts, char *lvl, char *msg) {
    // sizeof(LogNode) kadar bellekte yer açıyoruz
    LogNode *newNode = (LogNode*)malloc(sizeof(LogNode));
    
    // Bellek doluluğu kontrolü (Güvenlik için şarttır)
    if(!newNode) return NULL;

    // Parametre olarak gelen verileri düğümün içine güvenli şekilde kopyalıyoruz
    strncpy(newNode->timestamp, ts, 31);
    strncpy(newNode->level, lvl, 15);
    strncpy(newNode->message, msg, 255);

    // String sonlandırma karakterlerini garantiye alıyoruz
    newNode->timestamp[31] = '\0';
    newNode->level[15] = '\0';
    newNode->message[255] = '\0';

    // Yeni düğümün bağlantılarını başlangıçta boş (NULL) yapıyoruz
    newNode->next = NULL;
    newNode->prev = NULL;

    return newNode;
}

/**
 * @brief Listenin sonuna yeni bir günlük kaydı ekler.
 * Çift yönlü bağlı liste mantığıyla pointers güncellenir.
 */
void appendLog(LogList *list, char *ts, char *lvl, char *msg) {
    LogNode *newNode = createLogNode(ts, lvl, msg);
    if(!newNode) return;

    // Eğer liste boşsa, yeni eleman hem baş hem son olur
    if(list->head == NULL) {
        list->head = newNode;
        list->tail = newNode;
    } 
    else {
        // Liste doluysa, mevcut sonuncunun (tail) sonrasına ekle
        newNode->prev = list->tail; // Yeni elemanın arkasına eski sonuncuyu bağla
        list->tail->next = newNode; // Eski sonuncunun önüne yeni elemanı bağla
        list->tail = newNode;       // Listenin sonuncusunu güncelle
    }

    list->count++; // Toplam kayıt sayısını artır
}

/**
 * @brief Listeyi baştan sona tarayarak ekrana yazdırır.
 */
void displayLogs(LogList *list) {
    LogNode *current = list->head; // Taramaya baştan başlıyoruz

    printf("=========== SYSLOG LISTESI (Bagli Liste) ===========\n");
    printf("%-20s | %-10s | %s\n", "ZAMAN", "SEVIYE", "MESAJ");
    printf("----------------------------------------------------\n");

    // Liste sonuna kadar (NULL görene dek) ilerle
    while(current != NULL) {
        printf("%-20s | %-10s | %s\n", 
               current->timestamp, 
               current->level, 
               current->message);
        current = current->next; // Bir sonraki düğüme geç
    }

    printf("----------------------------------------------------\n");
    printf("Sistemdeki toplam log kaydi: %d\n", list->count);
}

/**
 * @brief Analiz edilecek sahte bir syslog dosyası oluşturur.
 */
void createDummyLogFile() {
    FILE *f = fopen("ornek_syslog.log", "w");
    if(!f) return;

    // Gerçek Linux Syslog formatına benzer satırlar ekleniyor
    fprintf(f, "Mar 10 09:00:01 INFO Sistem-Baslatildi\n");
    fprintf(f, "Mar 10 09:05:12 WARNING Yuksek-Bellek-Kullanimi\n");
    fprintf(f, "Mar 10 09:10:45 ERROR Veritabani-Baglantisi-Kesildi\n");
    fprintf(f, "Mar 10 09:15:20 INFO Kullanici-Giris-Yapti\n");
    fprintf(f, "Mar 10 09:20:00 CRITICAL Disk-Alani-Yetersiz\n");

    fclose(f);
}

/**
 * @brief Belirtilen dosyayı satır satır okur ve listeye ekler.
 */
void readLogsFromFile(LogList *list, const char *filename) {
    FILE *f = fopen(filename, "r");
    if(!f) {
        printf("Hata: Dosya acilamadi!\n");
        return;
    }

    char line[512];
    // Dosyanın sonuna kadar her satırı oku
    while(fgets(line, sizeof(line), f)) {
        char month[10], day[5], time[10], level[16], message[256];

        // Formatlı okuma ile satırı parçalara ayırıyoruz
        if(sscanf(line, "%s %s %s %s %[^\n]", month, day, time, level, message) == 5) {
            char timestamp[32];
            sprintf(timestamp, "%s %s %s", month, day, time);
            
            // Ayıklanan veriyi bağlı listeye gönder
            appendLog(list, timestamp, level, message);
        }
    }

    fclose(f);
}

/**
 * @brief Bellek sızıntısını önlemek için tüm listeyi temizler.
 */
void freeList(LogList *list) {
    LogNode *current = list->head;
    while(current != NULL) {
        LogNode *temp = current; // Silinecek düğümü yedekle
        current = current->next; // Sonrakine geç
        free(temp);              // Yedeklenen düğümü bellekten sil
    }
    list->head = NULL;
    list->tail = NULL;
    list->count = 0;
}
