# Linux Syslog Management System (Linked List)

Bu proje, Linux işletim sistemindeki sistem günlüklerini (Syslog) anlamak ve bu verileri **Çift Yönlü Bağlı Liste (Doubly Linked List)** veri yapısı ile yönetmek amacıyla geliştirilmiştir.

## 🛠 Teknik Özellikler
- **Dil:** C
- **Veri Yapısı:** Doubly Linked List (Çift Yönlü Bağlı Liste)
- **Geliştirme Ortamı:** macOS / Linux
- **Log Kaynağı:** `ornek_syslog.log` (Simüle edilmiş sistem günlüğü)

## ❓ Neden Çift Yönlü Bağlı Liste Seçildi?
Syslog kayıtları kronolojik bir sırayla oluşur. Çift yönlü bağlı liste kullanılmasının temel teknik sebepleri şunlardır:
1. **Hızlı Ekleme:** Yeni loglar her zaman listenin sonuna (Tail) $O(1)$ karmaşıklığında eklenir.
2. **Esnek Navigasyon:** Hata analizi yapılırken genellikle en yeni logdan en eski loga doğru bir tarama yapılır. `prev` işaretçileri sayesinde bu işlem çok hızlı gerçekleştirilir.
3. **Dinamik Bellek:** Sabit boyutlu diziler yerine, sistemin ihtiyacı kadar bellek alanı `malloc` ile dinamik olarak ayrılır.

## 🚀 Çalıştırma
Kodu derlemek ve çalıştırmak için:
```bash
gcc main.c -o syslog_app
./syslog_app
