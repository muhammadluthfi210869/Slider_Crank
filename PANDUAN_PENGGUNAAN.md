
# PANDUAN PENGGUNAAN & LAPORAN TANTANGAN AKADEMIK

## Simulasi Kinematika Slider-Crank (Dashboard Interaktif)

Simulasi ini dibuat untuk merepresentasikan mekanisme slider-crank dalam bentuk visual interaktif berbasis koordinat 2D. Fokus utama dari pengembangan sistem ini adalah bagaimana hubungan antar parameter mekanik, seperti panjang crank dan rod, dapat diamati secara langsung melalui perubahan gerak piston serta grafik turunannya (kecepatan dan percepatan).

Pendekatan ini dipilih agar konsep kinematika yang bersifat abstrak dapat lebih mudah dipahami melalui visualisasi yang dinamis, bukan hanya melalui perhitungan statis.

---

### 1. INSTRUKSI PENGOPERASIAN (INPUT)

Pengguna dapat mengendalikan simulasi melalui dua metode utama, yaitu interaksi langsung pada dashboard dan penggunaan shortcut keyboard.

#### A. Kontrol Mouse (Interaksi Dashboard)

Panel di sisi kiri digunakan untuk mengatur parameter simulasi secara real-time. Perubahan yang dilakukan akan langsung mempengaruhi perilaku sistem, sehingga hubungan antar variabel dapat diamati secara langsung.

* **Slider Kecepatan (RPM)**
  Digunakan untuk mengatur kecepatan rotasi sistem. Peningkatan nilai RPM akan mempercepat siklus gerak piston, yang juga berdampak pada perubahan grafik kecepatan dan percepatan.

* **Slider Radius Crank (r)**
  Parameter ini menentukan panjang lengan crank. Perubahannya akan mempengaruhi jarak tempuh piston (stroke), sehingga amplitudo gerak menjadi lebih besar atau kecil.

* **Slider Panjang Rod (L)**
  Digunakan untuk mengatur panjang batang penghubung. Rasio antara r dan L berpengaruh terhadap karakteristik gerak piston, khususnya pada bentuk kurva percepatan yang dihasilkan.

#### B. Shortcut Keyboard

* **Tombol [O]**
  Mengubah mode visual menjadi blueprint (outline). Mode ini digunakan untuk menampilkan struktur geometris tanpa elemen visual tambahan, sehingga lebih fokus pada analisis bentuk dan pergerakan.

* **Tombol [Esc]**
  Menutup aplikasi simulasi.

---

### 2. ANALISIS VISUAL (DASHBOARD)

Beberapa elemen visual disediakan untuk membantu interpretasi data selama simulasi berjalan.

1. **Vektor Inersia (Panah Merah)**
   Panah merah pada piston merepresentasikan gaya inersia yang sebanding dengan percepatan. Perubahan panjang panah menunjukkan perubahan besar percepatan secara relatif, sehingga memudahkan identifikasi titik dengan percepatan tinggi.

2. **Ignition Flash (Simulasi Gaya Gas)**
   Kilatan visual di ujung silinder digunakan untuk mensimulasikan momen pembakaran saat piston mendekati Titik Mati Atas (TDC). Elemen ini bersifat representatif dan digunakan untuk memperkuat pemahaman siklus kerja mesin.

3. **Grafik Kalkulus (Panel Kanan)**
   Tiga grafik ditampilkan secara berurutan, yaitu posisi, kecepatan, dan percepatan terhadap waktu. Nilai kecepatan dan percepatan diperoleh melalui pendekatan diferensiasi numerik dari data posisi, sehingga hasilnya bergantung pada interval sampling yang digunakan.

---

### 3. CHECKLIST TANTANGAN DOSEN (VERIFIKASI FITUR)

Berikut adalah implementasi fitur berdasarkan kriteria yang diberikan:

| Tantangan / Challenge                        |     Status    | Penjelasan Implementasi                                                                                                                                                                                                  
| **Abstraksi Kompleks (Bukan Gambar Statis)** | **TERPENUHI** | Struktur mekanisme dibangun menggunakan kombinasi bentuk dasar berbasis koordinat titik. Pendekatan ini memungkinkan fleksibilitas dalam perubahan parameter, meskipun belum difokuskan pada detail visual tingkat tinggi. |
| **Lintasan & Dinamika Non-Linear**           | **TERPENUHI** | Gerak piston mengikuti model kinematika slider-crank, sehingga pola gerak yang dihasilkan tidak linear. Grafik posisi menunjukkan pola periodik yang mendekati bentuk sinusoidal.                                          |
| **Kalkulus dalam Pemrograman**               | **TERPENUHI** | Kecepatan dan percepatan dihitung menggunakan diferensiasi numerik dari data posisi antar frame. Pendekatan ini cukup representatif, meskipun memiliki keterbatasan pada perubahan nilai yang sangat cepat.                |
| **Vektor Gaya Dinamis**                      | **TERPENUHI** | Vektor gaya divisualisasikan dalam bentuk panah dengan panjang yang berubah mengikuti percepatan. Visualisasi ini digunakan sebagai representasi relatif, bukan nilai absolut gaya.                                        |
| **Simulasi Gaya Gas / Pembakaran**           | **TERPENUHI** | Efek visual pembakaran ditampilkan pada kondisi sudut tertentu (mendekati TDC) sebagai ilustrasi siklus kerja mesin. Implementasi ini bersifat konseptual, bukan simulasi termodinamika aktual.                            |
| **Interaktivitas Real-Time**                 | **TERPENUHI** | Perubahan parameter dapat dilakukan saat simulasi berjalan. Dampaknya terhadap gerak dan grafik dapat diamati secara langsung, meskipun terdapat sedikit delay pada kondisi perubahan ekstrem.                             |

---

### Catatan Teknis

Simulasi dilengkapi dengan mekanisme pembatasan perubahan nilai (spike protection) untuk mengurangi lonjakan data saat parameter diubah secara tiba-tiba. Hal ini bertujuan menjaga kestabilan grafik, terutama selama demonstrasi. Namun, pada perubahan yang sangat cepat, fluktuasi kecil masih dapat terjadi karena keterbatasan pendekatan numerik yang digunakan.
