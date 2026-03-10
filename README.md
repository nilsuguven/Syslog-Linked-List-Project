# Linux Syslog Linked List Project

Bu proje Linux işletim sistemlerinde kullanılan **Syslog sistem günlüklerinin** 
bağlı liste veri yapısı kullanılarak nasıl işlenebileceğini göstermek amacıyla geliştirilmiştir.

Projede syslog kayıtları bir dosyadan okunarak **Çift Yönlü Bağlı Liste (Doubly Linked List)** 
veri yapısı içinde saklanmaktadır.

---

# 1. Syslog Mekanizması Nedir?

Syslog, Linux ve Unix tabanlı işletim sistemlerinde kullanılan bir **sistem günlükleme (logging) mekanizmasıdır**.

İşletim sistemi çekirdeği, servisler ve uygulamalar çalışırken çeşitli olaylar üretir.  
Bu olaylar belirli bir formatta sistem günlük dosyalarına yazılır.

Linux sistemlerinde syslog kayıtları genellikle şu dosyalarda bulunur:

- `/var/log/syslog`
- `/var/log/messages`

Bir syslog kaydı genellikle şu bilgileri içerir:

- Zaman damgası (Timestamp)
- Log seviyesi (INFO, WARNING, ERROR vb.)
- Mesaj içeriği

---

# 2. Syslog Ne İçin Kullanılır?

Syslog kayıtları sistem yöneticileri için oldukça önemlidir.

Bu kayıtlar sayesinde:

- Sistem hataları tespit edilebilir
- Güvenlik olayları analiz edilebilir
- Sistem performansı izlenebilir
- Servislerin çalışıp çalışmadığı kontrol edilebilir

Bu nedenle syslog kayıtlarının doğru şekilde saklanması ve analiz edilmesi önemlidir.

---

# 3. Syslog Verileri Bağlı Liste ile Nasıl Tutulabilir?

Bu projede syslog kayıtları bir dosyadan okunmaktadır.

Dosyada bulunan her satır bir **log kaydını** temsil eder.

Her log kaydı şu alanlara ayrılır:

- timestamp
- log seviyesi
- mesaj

Her kayıt için bir **düğüm (node)** oluşturulur ve bu düğüm bağlı listeye eklenir.

Her düğüm şu alanları içerir:

- timestamp
- level
- message
- next pointer
- prev pointer

Bu düğümler birbirine bağlanarak syslog kayıtlarını temsil eden bir bağlı liste oluşturur.

---

# 4. Kullanılan Bağlı Liste Türü

Bu projede **Doubly Linked List (Çift Yönlü Bağlı Liste)** veri yapısı kullanılmıştır.

Bu veri yapısında her düğüm iki işaretçi içerir:

- `next` → sonraki düğümü gösterir
- `prev` → önceki düğümü gösterir

Bu sayede liste içerisinde hem ileri hem de geri yönde hareket etmek mümkündür.

---

# 5. Neden Doubly Linked List Tercih Edildi?

Syslog kayıtları kronolojik olarak oluşur ve sürekli yeni kayıtlar eklenir.

Bu nedenle doubly linked list kullanmanın bazı avantajları vardır:

1. Yeni log kayıtları listenin sonuna hızlı şekilde eklenebilir.
2. Tail pointer sayesinde ekleme işlemi **O(1)** karmaşıklığında yapılır.
3. Log kayıtları hem ileri hem geri yönde gezilebilir.
4. Dinamik bellek kullanıldığı için sınırsız sayıda log kaydı tutulabilir.

Bu özellikler syslog verilerini yönetmek için doubly linked list veri yapısını uygun hale getirir.

---

# 6. Programın Çalışma Mantığı

Program aşağıdaki adımlarla çalışmaktadır:

1. Program örnek bir syslog dosyası oluşturur.
2. Bu dosya satır satır okunur.
3. Her satır bir log kaydı olarak ayrıştırılır.
4. Her log kaydı için bir düğüm oluşturulur.
5. Düğümler doubly linked list veri yapısına eklenir.
6. Tüm log kayıtları ekrana yazdırılır.
7. Program sonunda dinamik olarak ayrılan bellek serbest bırakılır.

---

# 7. Kullanılan Teknolojiler

- Programlama Dili: **C**
- Veri Yapısı: **Doubly Linked List**
- Derleyici: **GCC**

---

# 8. Projeyi Çalıştırma

Programı derlemek ve çalıştırmak için aşağıdaki komutlar kullanılabilir:

```bash
gcc main.c -o syslog
./syslog
