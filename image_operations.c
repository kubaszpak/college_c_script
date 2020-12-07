#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

struct obraz
{
    char *nazwa;
    char standard[3];
    int wysokosc;
    int szerokosc;
    int glebia;
    int **piksele;
};

struct baza_obrazow
{
    struct obraz *lista_obrazow;
    size_t len;
    size_t alloc;
};

int rozszerz(struct baza_obrazow *baza)
{
    assert(baza != NULL);
    assert(baza->lista_obrazow != NULL);
    struct obraz *nowa_lista = malloc(2 * baza->alloc);
    if (nowa_lista == NULL)
    {
        return 0; //fail
    }
    memcpy(nowa_lista, baza->lista_obrazow, sizeof(struct obraz) * baza->len);
    free(baza->lista_obrazow);
    baza->lista_obrazow = nowa_lista;
    baza->alloc *= 2;
    return 1; //success
}

struct baza_obrazow *stworz_tablice_struktur(int poczatkowy_rozmiar)
{
    struct baza_obrazow *baza = (struct baza_obrazow *)malloc(sizeof(struct baza_obrazow));
    baza->lista_obrazow = (struct obraz *)malloc(sizeof(struct obraz) * poczatkowy_rozmiar);
    baza->alloc = poczatkowy_rozmiar;
    baza->len = 0;
    return baza;
}

void dodaj_obraz(struct baza_obrazow *baza, struct obraz *obraz)
{
    assert(baza != NULL);
    assert(obraz != NULL);
    if (baza->len + 1 > baza->alloc)
    {
        assert(rozszerz(baza) != 0);
    }
    baza->lista_obrazow[baza->len++] = *obraz;
    // TODO: lista_obrazow być może powinna być tablicą wskaźników
}

void usun_obraz(struct baza_obrazow *baza, int index)
{
    assert(baza != NULL);
    assert(baza->lista_obrazow != NULL);
    assert(baza->len != 0);
    assert(index >= 0);

    for (int i = index; i < baza->len - 1; i++)
    {
        baza->lista_obrazow[i] = baza->lista_obrazow[i + 1];
    }

    baza->len--;

    struct obraz *nowa_lista_obrazow = (struct obraz *)realloc(baza->lista_obrazow, baza->len * sizeof(struct obraz));
    assert(nowa_lista_obrazow != NULL);
    baza->lista_obrazow = nowa_lista_obrazow;
}

struct obraz *wczytaj_obraz()
{
    struct obraz *obr = (struct obraz *)malloc(sizeof(struct obraz));
    FILE *plik;
    char nazwa[50];
    printf("Podaj nazwe pliku: \n");
    scanf("%s", nazwa);
    strcat(nazwa, ".pgm");
    obr->nazwa = nazwa;
    plik = fopen(nazwa, "r");
    fscanf(plik, "%s", &(obr->standard));
    fscanf(plik, "%d %d", &(obr->szerokosc), &(obr->wysokosc));
    fscanf(plik, "%d", &(obr->glebia));
    obr->piksele = malloc(obr->wysokosc * sizeof(*obr->piksele));
    if (obr->piksele != NULL)
    {
        for (int i = 0; i < obr->wysokosc; i++)
        {
            obr->piksele[i] = malloc(obr->szerokosc * sizeof(obr->piksele)); // *
        }
        for (int i = 0; i < obr->wysokosc; i++)
        {
            for (int j = 0; j < obr->szerokosc; j++)
            {
                fscanf(plik, "%d", &(obr->piksele[i][j]));
            }
        }
    }
    return obr;
}

void zapisz_obraz(struct baza_obrazow *baza, int index)
{
    FILE *plik;
    char nazwa[50];
    printf("Podaj nazwe pliku: \n");
    scanf("%s", nazwa);
    strcat(nazwa, ".pgm");
    plik = fopen(nazwa, "w");
    fprintf(plik, "%s\n", baza->lista_obrazow[index].standard);
    fprintf(plik, "%d %d\n", baza->lista_obrazow[index].szerokosc, baza->lista_obrazow[index].wysokosc);
    fprintf(plik, "%d\n", baza->lista_obrazow[index].glebia);
    for (int i = 0; i < baza->lista_obrazow[index].wysokosc; i++)
    {
        for (int j = 0; j < baza->lista_obrazow[index].szerokosc; j++)
        {
            fprintf(plik, "%d ", (baza->lista_obrazow[index].piksele[i][j]));
        }
        fprintf(plik, "\n");
    }
    fclose(plik);
}

void usun_baze(struct baza_obrazow *baza)
{
    for (int iter = 0; iter < baza->len; iter++)
    {
        for (int i = 0; i < baza->lista_obrazow[iter].wysokosc; i++)
        {
            free(baza->lista_obrazow[iter].piksele[i]);
        }
        free(baza->lista_obrazow[iter].piksele);
    }
    free(baza->lista_obrazow);
    free(baza);
}

void wypisz_obraz(struct obraz *obr)
{
    printf("%d %d\n", obr->szerokosc, obr->wysokosc);
    printf("%d \n", obr->glebia);
    printf("%s \n", &(obr->standard));
    for (int i = 0; i < obr->wysokosc; i++)
    {
        for (int j = 0; j < obr->szerokosc; j++)
        {
            printf("%d ", obr->piksele[i][j]);
        }
        printf("\n");
    }
}

void wypisz_baze(struct baza_obrazow *baza)
{
    for (int i = 0; i < baza->len; i++)
    {
        printf("Obraz: %d \n", i);
    }
}

int main()
{
    int wybor;
    struct baza_obrazow *baza = stworz_tablice_struktur(3);
    int index = -1;

    while (1)
    {
        printf("Menu:\n1. Dodaj obraz do bazy\n2. Wybierz aktywny obraz\n3. Zapisz do pliku\n4. Usun aktywny obraz\n5. Wypisz baze obrazow\n6. Zamknij\n");

        if (index < 0)
        {
            printf("Nie wybrales jeszcze obrazu.\n");
        }
        else
        {
            printf("Aktywny obraz ma index: %d\n", index);
        }

        scanf("%d", &wybor);

        switch (wybor)
        {
        case 0:
            printf("Wprowadziles napis zamiast liczby, sprobuj ponownie\n");
            break;

        case 1:
            dodaj_obraz(baza, wczytaj_obraz());
            printf("Pomyslnie odczytano obraz.\n");
            break;

        case 2:
            if (baza->len < 1)
            {
                printf("Baza jest jeszcze pusta\n");
                break;
            }
            wypisz_baze(baza);
            printf("Podaj numer obrazu\n");

            int wybor2;
            scanf("%d", &wybor2);

            if (wybor2 < 0 || wybor2 >= baza->len)
            {
                printf("Niepoprawny wybor");
                break;
            }

            index = wybor2;
            printf("Wybrany obraz to: %d %s\n", index, baza->lista_obrazow[index].nazwa);
            break;

        case 3:
            printf("Zapis do pliku aktualnie wybranego obrazu\n");
            zapisz_obraz(baza, index);
            break;

        case 4:
            printf("Usuwanie aktualnie wybranego obrazu\n");
            usun_obraz(baza, index);
            index = -1;
            break;

        case 5:
            wypisz_baze(baza);
            break;
        case 6:
            usun_baze(baza);
            return 0;

        default:
            printf("Niepoprawny wybor, sprobuj ponownie\n");
            break;
        }
    }
    usun_baze(baza);
}
