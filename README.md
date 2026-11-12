# Mars Kolonisi Yaşam Destek Simülatörü

C dili ile yazılmış, tek karakterli komutlarla oynanan metin tabanlı bir hayatta kalma simülasyonu.
Algoritma ve Programlama dersi proje ödevi olarak geliştirilmiştir.

Oyuncu, Mars'taki küçük bir araştırma istasyonunda tek başına kalmış bir mühendisi yönetir.
Amaç, kurtarma ekibi gelene kadar **15 gün** hayatta kalmaktır. Sağlık, enerji, oksijen, su,
yedek parça ve modül hasarı her komutta güncellenir.

**Hazırlayan:** Muhammed Emin CAMCI (1260505015)

---

## İçindekiler

- [Derleme ve Çalıştırma](#derleme-ve-çalıştırma)
- [Komutlar](#komutlar)
- [Zorluk Seviyeleri](#zorluk-seviyeleri)
- [Oyun Kuralları](#oyun-kuralları)
- [Kullanılan C Yapıları](#kullanılan-c-yapıları)
- [Proje Yapısı](#proje-yapısı)
- [Örnek Çıktı](#örnek-çıktı)

---

## Derleme ve Çalıştırma

GCC kurulu herhangi bir sistemde derlenebilir. Standart kütüphane dışında bağımlılığı yoktur.

```bash
gcc src/main.c -o main
./main
```

Windows'ta (MinGW / MSYS2):

```bash
gcc src/main.c -o main.exe
main.exe
```

> Konsolda karakter sorunu yaşanmaması için program çıktılarında Türkçe karakter kullanılmamıştır.

---

## Komutlar

Komutlar büyük/küçük harf duyarsızdır (`g` ve `G` aynı işi yapar).

| Komut | İşlev | Enerji Maliyeti |
|:-----:|-------|:---------------:|
| `G` | Keşif görevi. Su, yedek parça bulunabilir veya eli boş dönülür. Yaralanma ihtimali vardır. | 15 (+10 oksijen) |
| `O` | Oksijen üretimi. Her 2 birim su 3 birim oksijene dönüşür. | 10 |
| `T` | Tamir. Hasar seviyesine göre yedek parça harcayarak modül hasarını azaltır. | 10 |
| `D` | Durum raporu. Tüm değerleri ve kritik uyarıları gösterir. **Tur harcamaz.** | - |
| `U` | Uyu / dinlen. Enerji ve sağlık artar, oksijen tüketimi devam eder. | - |
| `K` | Kum fırtınası simülasyonu (`for` döngüsü). | 10 |
| `H` | Hava kilidi şifresi (`do-while` döngüsü, en fazla 3 deneme). | Yanlış deneme başına 5 |
| `Q` | Simülasyondan çıkış ve sonuç raporu. | - |

Tanımsız bir komut girildiğinde uyarı verilir ve tur harcanmaz.

---

## Zorluk Seviyeleri

| Değer | Kolay (K) | Normal (N) | Zor (Z) |
|-------|:---------:|:----------:|:-------:|
| Başlangıç suyu | 10 | 6 | 3 |
| Yedek parça | 3 | 2 | 1 |
| Başlangıç oksijeni | 90 | 80 | 70 |
| Başlangıç modül hasarı | 10 | 20 | 35 |
| Kum fırtınası süresi (tur) | 3 | 5 | 7 |
| Fırtına taban hasarı | 4 | 6 | 9 |
| Keşif bulma eşiği bonusu | +10 | 0 | -10 |

Geçersiz bir seviye girilirse seçim tekrar istenir.

---

## Oyun Kuralları

- Rastgele olaylar `srand(time(NULL))` ve `rand() % 100` ile üretilir. Her rastgele olayda zar ve eşik değeri ekrana yazdırılır:
  `Zar: 37 / Esik: 45 -> BASARILI`
- Sağlık, enerji, oksijen ve modül hasarı her komuttan sonra **0-100** aralığına sınırlandırılır.
- Enerjisi yetmeyen komut çalıştırılmaz, iptal edilir ve tur harcanmaz.
- Geçerli her komut tur sayacını 1 artırır (`D` ve tanımsız komutlar hariç).
- Her 3 turda bir (`tur % 3 == 0`) yeni gün başlar: oksijen 8 azalır, modül hasarı 70'in üzerindeyse sağlık 5 azalır.
- Sağlık veya oksijen 0'a düşerse oyun kaybedilir; 15. gün tamamlanırsa oyun kazanılır.
- Oyun nasıl biterse bitsin sonuç raporu ve puan gösterilir:

```
Puan = (gun x 100) + saglik + oksijen - modulHasari
```

Gün katsayısının 100 seçilmesinin nedeni, hayatta kalınan her günün puanda en belirleyici unsur olmasıdır.
Sağlık ve oksijen 0-100 aralığında olduğu için tek bir günün değerini geçemez, modül hasarı ise ceza olarak çıkarılır.

---

## Kullanılan C Yapıları

| Yapı | Nerede Kullanıldı |
|------|-------------------|
| `switch-case` (fall-through) | Komut sistemi ve zorluk seçimi (`case 'g': case 'G':`) |
| `do-while` | Ana komut döngüsü, zorluk seçiminin tekrarı, hava kilidi şifresi |
| `for` | Kum fırtınası turları |
| `break` / `continue` | Fırtınada değer 0'a düşünce döngüden çıkış / kilit mühürlüyse sızıntı olayını atlama |
| `if - else if - else` | Tamir kademeleri, keşif sonuçları, fırtına olayları |
| İç içe `if` | Dinlenmede oksijen 20'nin altındaysa sağlığın azalması |
| `%` (mod) operatörü | Oksijen üretiminde tek/çift kontrolü, fırtınada çift turlar, gün geçişi, şifre üretimi |
| `&&` ve `\|\|` | Keşif eşiği, yaralanma koşulu, "TÜM SİSTEMLER NORMAL" kontrolü, döngü koşulları |

---

## Proje Yapısı

```
mars-koloni-simulatoru/
├── src/
│   └── main.c                 # programın tüm kaynak kodu
├── ornek-ciktilar/
│   ├── normal_oyun.txt        # tüm komutların denendiği örnek oyun
│   └── kaybetme_senaryosu.txt # zor seviyede kaybedilen oyun
├── rapor/
│   └── Rapor.pdf              # kod mantığını açıklayan proje raporu
└── README.md
```

---

## Örnek Çıktı

```
Gun 2 | Tur 5 | Saglik 100 | Enerji 80 | Oksijen 63 | Su 5 | Parca 1 | Hasar 5 | Kilit MUHURLU
Komut > k
!!! KUM FIRTINASI BASLADI (5 tur) !!! (Hazirlik: Enerji -10)
Tur 1: Zar: 49 / Esik: 50 -> Hava kilidi muhurlu, sizinti olmadi.
Tur 2: Ruzgar siddetlendi! Hasar 2 katina cikti. Zar: 57 / Esik: 70 -> Modul darbe aldi, Hasar +12
Tur 3: Zar: 28 / Esik: 50 -> Hava kilidi muhurlu, sizinti olmadi.
Tur 4: Ruzgar siddetlendi! Hasar 2 katina cikti. Zar: 13 / Esik: 25 -> Kum yaraladi, Saglik -12
Tur 5: Zar: 41 / Esik: 50 -> Hava kilidi muhurlu, sizinti olmadi.
--- Firtina Ozeti ---
Hasar alinan tur : 2
Kaybedilen saglik: 12
Kaybedilen oksijen: 0
```

Tam oyun çıktıları için [`ornek-ciktilar`](ornek-ciktilar/) klasörüne bakabilirsiniz.
