/*
 * Mars Kolonisi Yasam Destek Simulatoru
 * Algoritma ve Programlama dersi proje odevi
 *
 * Muhammed Emin CAMCI - 1260505015
 *
 * Derleme : gcc mars_koloni.c -o mars_koloni
 * Calistir: ./mars_koloni
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* ---------- oyunun durum degiskenleri ---------- */
int saglik = 100;
int enerji = 100;
int oksijen = 100;
int su = 0;
int yedekParca = 0;
int modulHasari = 0;
int kilitMuhurlu = 0;   /* 0: muhurlu degil, 1: muhurlu */
int gun = 1;
int tur = 0;            /* gecerli komut sayaci */

/* ---------- zorluga gore degisen ayarlar ---------- */
char zorluk;
int firtinaTurSayisi;   /* kum firtinasi kac tur surecek */
int firtinaHasari;      /* firtinada bir olayin taban hasari */
int bulmaBonusu;        /* kesifte su/parca bulma esigine eklenen deger */

int kazandi = 0;

void zorlukSec(void);
void durumSatiri(void);
void degerleriSinirla(void);
void kesifYap(void);
void oksijenUret(void);
void tamirEt(void);
void durumRaporu(void);
void dinlen(void);
void kumFirtinasi(void);
void havaKilidi(void);
void zamanIlerlet(void);
void sonucRaporu(void);
void satirTemizle(void);

int main(void)
{
    char komut;
    int komutGecerli;   /* 1 ise tur sayaci artar */

    srand(time(NULL));

    printf("==============================================\n");
    printf("   MARS KOLONISI YASAM DESTEK SIMULATORU\n");
    printf("==============================================\n");
    printf("Arastirma istasyonunda tek basina kaldin.\n");
    printf("Kurtarma ekibi 15 gun sonra gelecek. Hayatta kal!\n\n");

    zorlukSec();

    do {
        printf("\n----------------------------------------------\n");
        durumSatiri();
        printf("Komutlar: G-Kesif  O-Oksijen  T-Tamir  D-Durum\n");
        printf("          U-Uyu  K-Kum firtinasi  H-Hava kilidi  Q-Cikis\n");
        printf("Komut > ");
        if (scanf(" %c", &komut) != 1) {
            komut = 'q';   /* giris biterse (EOF) programdan cik */
        }

        komutGecerli = 1;

        switch (komut) {
            case 'g':
            case 'G':
                if (enerji < 15) {
                    printf("Enerji yetersiz! Kesif icin en az 15 enerji gerekli.\n");
                    komutGecerli = 0;
                } else {
                    kesifYap();
                }
                break;

            case 'o':
            case 'O':
                if (enerji < 10) {
                    printf("Enerji yetersiz! Oksijen uretmek icin en az 10 enerji gerekli.\n");
                    komutGecerli = 0;
                } else if (su < 2) {
                    printf("Depoda en az 2 birim su olmali. (Su: %d)\n", su);
                    komutGecerli = 0;
                } else {
                    oksijenUret();
                }
                break;

            case 't':
            case 'T':
                if (yedekParca == 0) {
                    printf("Yedek parca yok, tamir yapilamaz!\n");
                    komutGecerli = 0;
                } else if (enerji < 10) {
                    printf("Enerji yetersiz! Tamir icin en az 10 enerji gerekli.\n");
                    komutGecerli = 0;
                } else if (modulHasari == 0) {
                    printf("Modulde hasar yok, tamire gerek yok.\n");
                    komutGecerli = 0;
                } else {
                    tamirEt();
                }
                break;

            case 'd':
            case 'D':
                durumRaporu();
                komutGecerli = 0;   /* durum raporu tur harcamaz */
                break;

            case 'u':
            case 'U':
                dinlen();
                break;

            case 'k':
            case 'K':
                if (enerji < 10) {
                    printf("Enerji yetersiz! Firtinaya hazirlanmak icin 10 enerji gerekli.\n");
                    komutGecerli = 0;
                } else {
                    kumFirtinasi();
                }
                break;

            case 'h':
            case 'H':
                if (kilitMuhurlu == 1) {
                    printf("Hava kilidi zaten muhurlu.\n");
                    komutGecerli = 0;
                } else if (enerji < 5) {
                    printf("Enerji yetersiz! Sifre denemesi icin en az 5 enerji gerekli.\n");
                    komutGecerli = 0;
                } else {
                    havaKilidi();
                }
                break;

            case 'q':
            case 'Q':
                printf("Simulasyondan cikiliyor...\n");
                komutGecerli = 0;
                break;

            default:
                printf("UYARI: '%c' tanimsiz bir komut! Tur harcanmadi.\n", komut);
                komutGecerli = 0;
                break;
        }

        degerleriSinirla();

        if (komutGecerli == 1) {
            zamanIlerlet();
            degerleriSinirla();
        }

        /* oyun sonu kontrolu */
        if (saglik <= 0) {
            printf("\n*** SAGLIGIN TUKENDI. Muhendis hayatini kaybetti. ***\n");
        } else if (oksijen <= 0) {
            printf("\n*** OKSIJEN BITTI. Istasyonda nefes alinamiyor. ***\n");
        } else if (kazandi == 1) {
            printf("\n*** 15. GUN TAMAMLANDI! Kurtarma ekibi geldi. ***\n");
        }

    } while (komut != 'q' && komut != 'Q' && saglik > 0 && oksijen > 0 && kazandi == 0);

    sonucRaporu();
    return 0;
}

/* Zorluk secimi: gecersiz giriste tekrar sorulur */
void zorlukSec(void)
{
    int gecerli;

    do {
        gecerli = 1;
        printf("Zorluk seviyesi secin (K: Kolay, N: Normal, Z: Zor): ");
        if (scanf(" %c", &zorluk) != 1) {
            exit(0);
        }

        switch (zorluk) {
            case 'k':
            case 'K':
                su = 10;
                yedekParca = 3;
                oksijen = 90;
                modulHasari = 10;
                firtinaTurSayisi = 3;
                firtinaHasari = 4;
                bulmaBonusu = 10;
                printf("Kolay seviye secildi.\n");
                break;
            case 'n':
            case 'N':
                su = 6;
                yedekParca = 2;
                oksijen = 80;
                modulHasari = 20;
                firtinaTurSayisi = 5;
                firtinaHasari = 6;
                bulmaBonusu = 0;
                printf("Normal seviye secildi.\n");
                break;
            case 'z':
            case 'Z':
                su = 3;
                yedekParca = 1;
                oksijen = 70;
                modulHasari = 35;
                firtinaTurSayisi = 7;
                firtinaHasari = 9;
                bulmaBonusu = -10;
                printf("Zor seviye secildi.\n");
                break;
            default:
                printf("Gecersiz seviye! Lutfen K, N veya Z girin.\n");
                gecerli = 0;
                break;
        }
    } while (gecerli == 0);
}

void durumSatiri(void)
{
    printf("Gun %d | Tur %d | Saglik %d | Enerji %d | Oksijen %d | Su %d | Parca %d | Hasar %d | Kilit %s\n",
           gun, tur, saglik, enerji, oksijen, su, yedekParca, modulHasari,
           kilitMuhurlu == 1 ? "MUHURLU" : "ACIK");
}

/* Degerler 0-100 araliginda tutulur, her komuttan sonra cagrilir */
void degerleriSinirla(void)
{
    if (saglik < 0) saglik = 0;
    if (saglik > 100) saglik = 100;

    if (enerji < 0) enerji = 0;
    if (enerji > 100) enerji = 100;

    if (oksijen < 0) oksijen = 0;
    if (oksijen > 100) oksijen = 100;

    if (modulHasari < 0) modulHasari = 0;
    if (modulHasari > 100) modulHasari = 100;

    if (su < 0) su = 0;
    if (yedekParca < 0) yedekParca = 0;
}

/* G komutu */
void kesifYap(void)
{
    int zar, esik, parcaEsik, bulunan;

    /* esik, kesfe cikmadan onceki duruma gore belirlenir */
    if (enerji > 40 && oksijen > 30) {
        esik = 45 + bulmaBonusu;
    } else {
        esik = 20 + bulmaBonusu;
    }
    parcaEsik = esik + 25;

    enerji = enerji - 15;
    oksijen = oksijen - 10;
    printf("Kesfe ciktin. (Enerji -15, Oksijen -10)\n");

    zar = rand() % 100;
    if (zar < esik) {
        bulunan = rand() % 3 + 2;   /* 2-4 birim */
        su = su + bulunan;
        printf("Zar: %d / Esik: %d -> BASARILI. Buz tabakasi buldun, +%d su.\n", zar, esik, bulunan);
    } else if (zar < parcaEsik) {
        yedekParca = yedekParca + 1;
        printf("Zar: %d / Esik: %d -> Su yok ama enkaz icinde 1 yedek parca buldun.\n", zar, parcaEsik);
    } else {
        printf("Zar: %d / Esik: %d -> BASARISIZ. Eli bos dondun.\n", zar, parcaEsik);
    }

    /* yaralanma kontrolu */
    zar = rand() % 100;
    if (zar < 10 || modulHasari > 70) {
        saglik = saglik - 12;
        printf("Zar: %d / Esik: 10 (veya hasar > 70) -> YARALANDIN! Saglik -12\n", zar);
    } else {
        printf("Zar: %d / Esik: 10 -> Yaralanmadan dondun.\n", zar);
    }
}

/* O komutu: 2 su -> 3 oksijen */
void oksijenUret(void)
{
    int miktar, kullanilan, artan, uretilen;

    printf("Kac birim su kullanilsin? (Depoda %d var): ", su);
    if (scanf("%d", &miktar) != 1) {
        satirTemizle();
        miktar = 0;
    }

    while (miktar <= 0 || miktar > su) {
        printf("Gecersiz miktar! 1 ile %d arasinda girin: ", su);
        if (scanf("%d", &miktar) != 1) {
            if (feof(stdin)) {
                return;
            }
            satirTemizle();
            miktar = 0;
        }
    }

    artan = miktar % 2;           /* tek sayi girildiyse 1 birim artar */
    kullanilan = miktar - artan;

    if (kullanilan == 0) {
        printf("1 birim su ile oksijen uretilemez, su depoda kaldi.\n");
        return;
    }

    uretilen = (kullanilan / 2) * 3;

    if (oksijen + uretilen > 100) {
        printf("Oksijen tanki %d'de doldu, fazlasi bosa gitti.\n", 100);
    }

    su = su - kullanilan;
    oksijen = oksijen + uretilen;
    enerji = enerji - 10;

    printf("%d su kullanildi -> +%d oksijen. (Enerji -10)\n", kullanilan, uretilen);
    if (artan == 1) {
        printf("Girilen miktar tek oldugu icin 1 birim su depoda kaldi.\n");
    }
}

/* T komutu: hasar seviyesine gore 3 kademe */
void tamirEt(void)
{
    int gerekenParca, onarim;

    if (modulHasari <= 30) {
        printf("Hasar seviyesi: HAFIF (%d)\n", modulHasari);
        gerekenParca = 1;
        onarim = 15;
    } else if (modulHasari <= 70) {
        printf("Hasar seviyesi: ORTA (%d)\n", modulHasari);
        gerekenParca = 2;
        onarim = 30;
    } else {
        printf("Hasar seviyesi: AGIR (%d)\n", modulHasari);
        gerekenParca = 3;
        onarim = 50;
    }

    /* parca yetmiyorsa elimizdekiyle kismi tamir yapilir */
    if (yedekParca < gerekenParca) {
        onarim = onarim * yedekParca / gerekenParca;
        gerekenParca = yedekParca;
        printf("Yeterli parca yok, %d parca ile kismi tamir yapiliyor.\n", gerekenParca);
    }

    yedekParca = yedekParca - gerekenParca;
    modulHasari = modulHasari - onarim;
    enerji = enerji - 10;

    printf("%d parca harcandi, hasar %d azaldi. (Enerji -10)\n", gerekenParca, onarim);
}

/* D komutu */
void durumRaporu(void)
{
    printf("\n========== DURUM RAPORU ==========\n");
    printf("Gun            : %d / 15\n", gun);
    printf("Tur            : %d\n", tur);
    printf("Saglik         : %d\n", saglik);
    printf("Enerji         : %d\n", enerji);
    printf("Oksijen        : %d\n", oksijen);
    printf("Su             : %d birim\n", su);
    printf("Yedek parca    : %d\n", yedekParca);
    printf("Modul hasari   : %d\n", modulHasari);
    printf("Hava kilidi    : %s\n", kilitMuhurlu == 1 ? "MUHURLU" : "ACIK");
    printf("----------------------------------\n");

    if (oksijen < 25) {
        printf("[KRITIK] Oksijen seviyesi cok dusuk!\n");
    }
    if (saglik < 30) {
        printf("[KRITIK] Saglik durumu tehlikede!\n");
    }
    if (modulHasari > 70) {
        printf("[KRITIK] Modul agir hasarli!\n");
    }
    if (enerji < 20) {
        printf("[UYARI] Enerji azaldi, dinlenmen gerekebilir.\n");
    }
    if (su == 0) {
        printf("[UYARI] Su deposu bos.\n");
    }

    if (oksijen >= 25 && saglik >= 30 && modulHasari <= 70 && enerji >= 20) {
        printf(">>> TUM SISTEMLER NORMAL <<<\n");
    }
    printf("==================================\n");
}

/* U komutu */
void dinlen(void)
{
    enerji = enerji + 25;
    oksijen = oksijen - 5;   /* uyurken de nefes aliniyor */

    if (oksijen < 20) {
        saglik = saglik - 8;
        printf("Oksijen az oldugu icin uyku kalitesiz gecti. Saglik -8\n");
    } else {
        saglik = saglik + 10;
        printf("Iyi dinlendin. Saglik +10\n");
    }
    printf("Enerji +25, Oksijen -5\n");
}

/* K komutu: FOR dongusu ile kum firtinasi */
void kumFirtinasi(void)
{
    int i, zar, hasar;
    int hasarliTur = 0;
    int kayipSaglik = 0;
    int kayipOksijen = 0;
    int korunmaEsik;

    enerji = enerji - 10;
    printf("\n!!! KUM FIRTINASI BASLADI (%d tur) !!! (Hazirlik: Enerji -10)\n", firtinaTurSayisi);

    /* modul hasarliysa korunma ihtimali duser */
    if (modulHasari > 50) {
        korunmaEsik = 85;
    } else {
        korunmaEsik = 70;
    }

    for (i = 1; i <= firtinaTurSayisi; i++) {
        zar = rand() % 100;
        hasar = firtinaHasari;

        if (i % 2 == 0) {
            hasar = hasar * 2;
            printf("Tur %d: Ruzgar siddetlendi! Hasar 2 katina cikti. ", i);
        } else {
            printf("Tur %d: ", i);
        }

        if (zar < 25) {
            saglik = saglik - hasar;
            kayipSaglik = kayipSaglik + hasar;
            hasarliTur++;
            printf("Zar: %d / Esik: 25 -> Kum yaraladi, Saglik -%d\n", zar, hasar);
        } else if (zar < 50) {
            if (kilitMuhurlu == 1) {
                printf("Zar: %d / Esik: 50 -> Hava kilidi muhurlu, sizinti olmadi.\n", zar);
                continue;
            }
            oksijen = oksijen - hasar;
            kayipOksijen = kayipOksijen + hasar;
            hasarliTur++;
            printf("Zar: %d / Esik: 50 -> Oksijen sizintisi! Oksijen -%d\n", zar, hasar);
        } else if (zar < korunmaEsik) {
            modulHasari = modulHasari + hasar;
            hasarliTur++;
            printf("Zar: %d / Esik: %d -> Modul darbe aldi, Hasar +%d\n", zar, korunmaEsik, hasar);
        } else {
            printf("Zar: %d / Esik: %d -> Siginakta guvendesin, zarar yok.\n", zar, korunmaEsik);
        }

        if (saglik <= 0 || oksijen <= 0) {
            printf("Firtina sirasinda hayati deger sifira dustu!\n");
            break;
        }
    }

    printf("--- Firtina Ozeti ---\n");
    printf("Hasar alinan tur : %d\n", hasarliTur);
    printf("Kaybedilen saglik: %d\n", kayipSaglik);
    printf("Kaybedilen oksijen: %d\n", kayipOksijen);
}

/* H komutu: DO-WHILE ile sinirli denemeli sifre */
void havaKilidi(void)
{
    char sifre, tahmin;
    int deneme = 0;

    sifre = 'A' + (gun % 26);

    printf("Hava kilidi sifresi tek bir harftir (A-Z).\n");
    printf("Ipucu: Sifre, alfabede A harfinden bugunun gun sayisi kadar ilerideki harftir.\n");

    do {
        printf("Deneme %d/3 - Harf girin: ", deneme + 1);
        if (scanf(" %c", &tahmin) != 1) {
            tahmin = '?';
        }

        if (tahmin >= 'a' && tahmin <= 'z') {
            tahmin = tahmin - 32;   /* kucuk harfi buyuk harfe cevir */
        }
        deneme++;

        if (tahmin != sifre) {
            enerji = enerji - 5;
            if (tahmin < sifre) {
                printf("Yanlis! Ileri git. (Enerji -5)\n");
            } else {
                printf("Yanlis! Geri git. (Enerji -5)\n");
            }
        }
    } while (tahmin != sifre && deneme < 3);

    if (tahmin == sifre) {
        kilitMuhurlu = 1;
        printf("Dogru sifre! Hava kilidi muhurlendi.\n");
    } else {
        modulHasari = modulHasari + 20;
        printf("3 deneme de basarisiz. Kilit zorlandi, Modul hasari +20 (Sifre: %c)\n", sifre);
    }
}

/* Her gecerli komutta tur artar, 3 turda bir yeni gun baslar */
void zamanIlerlet(void)
{
    tur++;

    if (tur % 3 == 0) {
        if (gun == 15) {
            kazandi = 1;   /* 15. gun de bitti */
            return;
        }
        gun++;
        oksijen = oksijen - 8;
        printf("\n>> GUN %d BASLADI. Gece boyunca oksijen tuketildi (Oksijen -8)\n", gun);
        if (modulHasari > 70) {
            saglik = saglik - 5;
            printf(">> Modul hasari yuksek, kotu kosullar sagligi etkiledi (Saglik -5)\n");
        }
    }
}

void sonucRaporu(void)
{
    int puan;

    puan = (gun * 100) + saglik + oksijen - modulHasari;

    printf("\n############ SONUC RAPORU ############\n");
    if (kazandi == 1) {
        printf("Sonuc          : KAZANDIN, kurtarildin!\n");
    } else if (saglik <= 0 || oksijen <= 0) {
        printf("Sonuc          : KAYBETTIN\n");
    } else {
        printf("Sonuc          : Simulasyondan cikildi\n");
    }
    printf("Hayatta kalinan gun: %d\n", gun);
    printf("Toplam tur     : %d\n", tur);
    printf("Saglik         : %d\n", saglik);
    printf("Enerji         : %d\n", enerji);
    printf("Oksijen        : %d\n", oksijen);
    printf("Su             : %d\n", su);
    printf("Yedek parca    : %d\n", yedekParca);
    printf("Modul hasari   : %d\n", modulHasari);
    printf("PUAN = (%d x 100) + %d + %d - %d = %d\n", gun, saglik, oksijen, modulHasari, puan);
    printf("######################################\n");
}

void satirTemizle(void)
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
    }
}
