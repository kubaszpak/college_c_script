#define _USE_MATH_DEFINES
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#define ILOSC_ZMIENNYCH 3

void wspolczynniki_funkcji(double tablica_wspolczynnikow[])
{
    double liczba;
    for (int i = 0; i < ILOSC_ZMIENNYCH; i++) {
        printf("Podaj kolejny wspolczynnik: "); //WSPOLCZYNNIKI PO KOLEI: A, B, C
        scanf_s("%lf", &liczba);
        tablica_wspolczynnikow[i] = liczba;

        while (!tablica_wspolczynnikow[i]) {

            printf("Wspolczynnik nie moze byc rowny zero\n");
            printf("Podaj inny wspolczynnik: \n");
            scanf_s("%lf", &tablica_wspolczynnikow[i]);

        }
    }
}

void wypisz_tablice(double tablica[], int rozmiar) {
    for (int i = 0; i < rozmiar; i++) {
        printf("y = %lf \n", *(tablica + i));

    }
    return;
}

int dziedzina_funkcji(double parametry[])
{
    printf("podaj dMin = ");
    scanf_s("%lf", (parametry));
    printf("podaj dMax = ");
    scanf_s("%lf", (parametry + 1));
    if (parametry[0] > parametry[1]) {
        printf("Program nie zadziala, dMax musi byc wieksze od dMin\n");
        return 1;
    }
    return 0;

}

double funkcja(double x, double A, double B, double C) {
    double y = A * sin(x / B) * cos(x * C);
    
    return y;
}

void generujaca_funkcje(double tablica_wspolczynnikow[], double dMin, double dMax, double tablica_wynikowa[], int rozmiar) {
    double wynik;
    wynik = (dMax - dMin) / rozmiar;

    double x;
    x = dMin;
    double y;


    for (int i = 0; i < rozmiar; i++)
    {

        y = funkcja(x, tablica_wspolczynnikow[0], tablica_wspolczynnikow[1], tablica_wspolczynnikow[2]);
        tablica_wynikowa[i] = y;
        x = x + wynik;
        
    }
}

void zapisujaca_tablice(double tablica[], int rozmiar)
{
    FILE* arkusz_k;  //plik w arkuszku kalkulacyjnym
    arkusz_k = fopen("arkusz_k.CSV", "w");
    if (arkusz_k == NULL) {
        printf("Nie udalo sie zapisac do pliku");
    }
    else {
        for (int i = 0; i < rozmiar; i++) {
            fprintf(arkusz_k, "%lf\n", *(tablica + i));
        }
        fclose(arkusz_k);
    }
    return;
}

void zaklocenia(double tabica_wynikowa[], double tablica_szumow[], double szumy[], int rozmiar)
{
    srand((unsigned int)time(NULL));
    
    for (int i = 0; i < rozmiar; i++) {
        double t = (double)(rand() % 100 + 1.0) / 100.0;
        double z = (double)(rand() % 100 + 1.0) / 100.0;
        double A = (double)(rand() % 101  - 50.0) / 10.0; // A - amplituda
        double prawdopodobienstwo = (double)(rand() % 100 + 1.0) / 100.0;
        double losowy_szum;
        if (prawdopodobienstwo < 0.2){
            losowy_szum = A * (sin(2 * M_PI * t) + cos(2 * M_PI * z) + rand() % 10 + 1) / 10.0;
        }
        else {
            losowy_szum = 0;
        }
        //printf("%lf \n", losowy_szum);
        szumy[i] = losowy_szum;
        tablica_szumow[i] = tabica_wynikowa[i] + losowy_szum;
    }

}

int wczytaj_rozmiar()
{
    int rozmiar;
    printf("Poodaj jaki chcesz rozmiar tablicy wynikowej = ");
    scanf_s("%d", &rozmiar);
    return rozmiar; 
}

double* wygeneruj_tablice_dynamiczna(int rozmiar)
{
    double* tablicaWynikowa;
    tablicaWynikowa = calloc(rozmiar, sizeof(double));
    return tablicaWynikowa;
}

int wczytaj_rozmiar_z_excela()
{
    FILE* arkusz_k;
    arkusz_k = fopen("arkusz_k.CSV", "r");
    if (arkusz_k == NULL)
    {
        printf("Blad z plikiem ");
        return 0;
    }

    int linia = 0;
    while (!feof(arkusz_k))
    {
        double wartosc;
        fscanf_s(arkusz_k, "%lf\n", &wartosc);
        linia++;
    }

    fclose(arkusz_k);

    return linia;

}

double* wczytaj_tablice_wynikowa_z_excela(int rozmiar) {
    FILE* arkusz_k;  //plik w arkuszku kalkulacyjnym                                        
    arkusz_k = fopen("arkusz_k.CSV", "r");
    double* tablica;
    tablica = calloc(rozmiar, sizeof(double));
    for (int i = 0;i < rozmiar;i++) {
        fscanf_s(arkusz_k, "%lf", (tablica + i));
        
    }
    fclose(arkusz_k);
    return tablica;
}

double filtr_sredni(double* tablica_do_sredniej) {
    
double suma = 0;

for (int i= 0; i < 5; i++) {

    suma += tablica_do_sredniej[i];

}
 return suma/5.0;
}

void funkcja_odfiltrowujaca(double* tablica_zaszumiona, double* odszumiona,int rozmiar) {
    double tablica_do_sredniej[5];
    tablica_do_sredniej[0] = tablica_zaszumiona[0];
    tablica_do_sredniej[1] = tablica_zaszumiona[1];
    tablica_do_sredniej[2] = tablica_zaszumiona[1];
    tablica_do_sredniej[3] = tablica_zaszumiona[2];
    tablica_do_sredniej[4] = tablica_zaszumiona[2];
    odszumiona[0] = filtr_sredni(tablica_do_sredniej);
    tablica_do_sredniej[0] = tablica_zaszumiona[0];
    tablica_do_sredniej[1] = tablica_zaszumiona[1];
    tablica_do_sredniej[2] = tablica_zaszumiona[2];
    tablica_do_sredniej[3] = tablica_zaszumiona[3];
    tablica_do_sredniej[4] = tablica_zaszumiona[3];
    odszumiona[1] = filtr_sredni(tablica_do_sredniej);
    tablica_do_sredniej[0] = tablica_zaszumiona[rozmiar-1];
    tablica_do_sredniej[1] = tablica_zaszumiona[rozmiar-2];
    tablica_do_sredniej[2] = tablica_zaszumiona[rozmiar-2];
    tablica_do_sredniej[3] = tablica_zaszumiona[rozmiar-3];
    tablica_do_sredniej[4] = tablica_zaszumiona[rozmiar-3];
    odszumiona[rozmiar-1] = filtr_sredni(tablica_do_sredniej);
    tablica_do_sredniej[0] = tablica_zaszumiona[rozmiar-1];
    tablica_do_sredniej[1] = tablica_zaszumiona[rozmiar-2];
    tablica_do_sredniej[2] = tablica_zaszumiona[rozmiar-3];
    tablica_do_sredniej[3] = tablica_zaszumiona[rozmiar-4];
    tablica_do_sredniej[4] = tablica_zaszumiona[rozmiar-4];
    odszumiona[rozmiar-2] = filtr_sredni(tablica_do_sredniej);
    for (int i = 2; i < rozmiar - 2; i++) {
        for (int j = 0; j < 5; j++) {
            tablica_do_sredniej[j] = tablica_zaszumiona[i+j-2];
        }
        odszumiona[i] = filtr_sredni(tablica_do_sredniej);
        
    }
}


int main()
{

    double tablica_wspolczynnikow[ILOSC_ZMIENNYCH];
    double parametry[2];//dMin i dMax
    double* tablica_wynikowa = NULL;
    double* szumy = NULL;
    double* tablica_zaszumiona = NULL;
    double* odszumiona = NULL;
    int rozmiar;
    //tworzenie menu 
    while (1) {
        printf("==============MENU=============== \n");
        printf("1.Generujaca Funkcje \n2.Generuj szumy\n3.Zapisz do pliku\n4.Wypisz tablice\n5.Odfiltruj sygnal\n6.Wczytaj nowa tablice wynikowa z pliku\n7.Zakoncz\n");
        printf("================================== \n");
        int wybor = 0;
        scanf_s("%d", &wybor);
        system("cls");
        switch (wybor) {
        case 1:
            wspolczynniki_funkcji(tablica_wspolczynnikow);
            if (dziedzina_funkcji(parametry) != 0)
            {
                return 0;
            }
            rozmiar = wczytaj_rozmiar();
            if (rozmiar < 5) {
                printf("Rozmiar nie moze byc mniejszy od 5 - %d \n", rozmiar);
                return 0;
            }
            if (tablica_wynikowa != NULL) {
                free(tablica_wynikowa);
                tablica_wynikowa = NULL;
            }
            if (tablica_zaszumiona != NULL) {
                free(tablica_zaszumiona);
                tablica_zaszumiona = NULL;
            }
            if (odszumiona != NULL) {
                free(odszumiona);
                odszumiona = NULL;
            }
            if (szumy != NULL) {
                free(szumy);
                szumy = NULL;
            }
            tablica_wynikowa = wygeneruj_tablice_dynamiczna(rozmiar);
            generujaca_funkcje(tablica_wspolczynnikow, *parametry, *(parametry + 1), tablica_wynikowa, rozmiar);
            system("cls");
            break;

        case 2:
            if (tablica_wynikowa != NULL) {
                if (tablica_zaszumiona != NULL) {
                    free(tablica_zaszumiona);
                    tablica_zaszumiona = NULL;
                }
                if (szumy != NULL) {
                    free(szumy);
                    szumy = NULL;
                }
                if (odszumiona != NULL) {
                    free(odszumiona);
                    odszumiona = NULL;
                }
                tablica_zaszumiona = wygeneruj_tablice_dynamiczna(rozmiar);
                szumy = wygeneruj_tablice_dynamiczna(rozmiar);
                zaklocenia(tablica_wynikowa, tablica_zaszumiona, szumy, rozmiar);
            }
            else {
                printf("Nie mo�na wygenerowac szumow poniewaz,\nnajpierw trzeba wygenerowac\nlub wczytac tablice wynikowa\n");
            }
            break;

        case 3:
            printf("Ktora tablica chcesz zapisac do pliku?\n1.Tablica wynikowa\n2.Tablica zaszumiona\n3.Tablica odfiltrowana\n");
            int wybor1;
            scanf_s("%d", &wybor1);
            system("cls");
            switch (wybor1) {
            case 1:
                if (tablica_wynikowa != NULL) {
                    zapisujaca_tablice(tablica_wynikowa,rozmiar);
                }
                else {
                    printf("Nie ma takiej tablicy\n");
                }
                break;
            case 2:
                if (tablica_zaszumiona != NULL) {
                    zapisujaca_tablice(tablica_zaszumiona,rozmiar);
                }
                else {
                    printf("Nie ma takiej tablicy\n");
                }
                break;
            case 3:
                if (odszumiona != NULL) {
                    zapisujaca_tablice(odszumiona,rozmiar);
                }
                else {
                    printf("Nie ma takiej tablicy\n");
                }
                break;
            default:
                printf("Nie ma takiego Case'a\n");
                break;
            }
            break;
        case 4:
            printf("Ktora tablica chcesz wypisac?\n1.Tablica wynikowa\n2.Tablica zaszumiona\n3.Tablica odfiltrowana\n");
            scanf_s("%d", &wybor);
            system("cls");
            switch (wybor) {
            case 1:
                if (tablica_wynikowa != NULL) {
                    wypisz_tablice(tablica_wynikowa, rozmiar);
                }
                else {
                    printf("Nie ma takiej tablicy\n");
                }
                break;
            case 2:
                if (tablica_zaszumiona != NULL) {
                    wypisz_tablice(tablica_zaszumiona, rozmiar);
                }
                else {
                    printf("Nie ma takiej tablicy\n");
                }
                break;
            case 3:
                if (odszumiona != NULL) {
                    wypisz_tablice(odszumiona, rozmiar);
                }
                else {
                    printf("Nie ma takiej tablicy\n");
                }
                break;
            default:
                printf("Nie ma takiego Case'a\n");
                break;
            }
            break;
        case 5:
            if (tablica_zaszumiona != NULL) {
                if (odszumiona != NULL) {
                    free(odszumiona);
                    odszumiona = NULL;
                }
                odszumiona = wygeneruj_tablice_dynamiczna(rozmiar);
                funkcja_odfiltrowujaca(tablica_zaszumiona, odszumiona, rozmiar);
            }
            else {
                printf("Zeby odfiltrowac tablice najpierw trzeba ja wygenerowac\ni nalozyc na nia szum\n");
            }
            break;
        case 6:
            if (tablica_wynikowa != NULL) {
                free(tablica_wynikowa);
                tablica_wynikowa = NULL;
            }
            if (tablica_zaszumiona != NULL) {
                free(tablica_zaszumiona);
                tablica_zaszumiona = NULL;
            }
            if (odszumiona != NULL) {
                free(odszumiona);
                odszumiona = NULL;
            }
            if (szumy != NULL) {
                free(szumy);
                szumy = NULL;
            }
            rozmiar = wczytaj_rozmiar_z_excela();
            if (rozmiar == 0) {
                return 0;
            }
            tablica_wynikowa = wczytaj_tablice_wynikowa_z_excela(rozmiar);

            break;
        case 7:
            if (tablica_wynikowa != NULL) {
                free(tablica_wynikowa);
            }
            if (tablica_zaszumiona != NULL) {
                free(tablica_zaszumiona);
            }
            if (odszumiona != NULL) {
                free(odszumiona);
            }
            if (szumy != NULL) {
                free(szumy);
            }
            return;
        default:
            printf("Nie ma tego Case'a \n\n\n");
            break;
        }
    }
    free(tablica_zaszumiona);
    free(tablica_wynikowa);
    free(odszumiona);
    free(szumy);
    return 0;
}