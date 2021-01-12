#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include <stdbool.h>
#include <conio.h>
// #include <locale.h>

struct data
{
    char first_name[20];
    char last_name[20];
    char number[20];
    char group[20];
};

struct element
{
    struct element *next;
    struct data contact;
    struct element *previous;
};

char *trimwhitespace(char *str)
{
    char *end;

    // Trim leading space
    while (isspace((unsigned char)*str))
        str++;

    if (*str == 0) // All spaces?
        return str;

    // Trim trailing space
    end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end))
        end--;

    // Write new null terminator character
    end[1] = '\0';

    return str;
}

char *tr(char *str)
{
    static char buff[256];
    char cp[] = "\245\206\251\210\344\242\230\276\253\244\217\250\235\343\340\227\275\215¹æê³ñóœ¿Ÿ¥ÆÊ£ÑÓŒ¯";
    if (strlen(str) >= sizeof(buff))
        return str;
    char *bf = buff;
    while (*str)
    {
        char *pos = strchr(cp + 18, *str);
        *(bf++) = pos ? *(pos - 18) : *str;
        ++str;
    }
    *bf = 0;
    return buff;
}

void print_contact(struct data contact)
{
    printf("%s\t%s\t%s\t%s\n\n", contact.first_name, contact.last_name, contact.number, contact.group);
}

struct element *new_element(struct data contact, struct element *previous)
{
    struct element *new = malloc(sizeof(struct element));
    new->contact = contact;
    new->previous = previous;
    new->next = NULL;
    return new;
}

struct data create_contact(char *first_name, char *last_name, char *number, char *group)
{
    struct data contact;
    strcpy(contact.first_name, trimwhitespace(tr(first_name)));
    strcpy(contact.last_name, trimwhitespace(tr(last_name)));
    strcpy(contact.number, trimwhitespace(tr(number)));
    strcpy(contact.group, trimwhitespace(tr(group)));
    return contact;
}

// struct element *get_the_beginning(struct element *element)
// {
//     struct element *current = element;
//     while (current->previous != NULL)
//     {
//         // print_contact(current->contact);
//         current = current->previous;
//     }
//     return current;
// }

void swap_node_with_its_next(struct element *element)
{

    if (element->previous != NULL)
    {
        element->previous->next = element->next;
    }
    element->next->previous = element->previous;
    element->previous = element->next;
    element->next = element->previous->next;
    element->previous->next = element;
    if (element->next != NULL)
    {
        element->next->previous = element;
    }
}

struct data create_new_contact_data()
{
    struct data new_contact;
    printf("First name: ");
    scanf("%s", new_contact.first_name);
    printf("%s\n", new_contact.first_name);
    printf("\nLast name: ");
    scanf("%s", new_contact.last_name);
    printf("\nNumber: ");
    scanf("%s", new_contact.number);
    printf("\nGroup: ");
    scanf("%s", new_contact.group);
    printf("\n");
    // scanf("First name: %s\nLast name: %s\nNumber: %s\nGroup: %s\n",new_contact.first_name,new_contact.last_name, new_contact.number, new_contact.group);
    return new_contact;
}

struct element *new_element_over_this(struct element *base, struct data contact)
{
    struct element *new_element = malloc(sizeof(struct element));
    new_element->contact = contact;
    new_element->next = base;
    new_element->previous = base->previous;
    base->previous = new_element;
    if (new_element->previous != NULL)
    {
        new_element->previous->next = new_element;
    }
    return new_element;
}

struct element *add_new_element(struct element *start, int sort_type)
{
    struct data new_contact = create_new_contact_data();
    print_contact(new_contact);
    struct element *current = start;

    if (start == NULL)
    {
        start = new_element(new_contact, NULL);
        return start;
    }
    else if (sort_type == 0)
    {
        struct element *node = malloc(sizeof(struct element));
        node->contact = new_contact;
        node->previous = NULL;
        node->next = start;
        start->previous = node;

        return node;
    }
    else
    {
        char compare_buffer1[20];
        char compare_buffer2[20];

        while (current->next != NULL)
        {

            if (sort_type == 1) // sort by first name
            {
                strcpy(compare_buffer1, current->contact.first_name);
                strcpy(compare_buffer2, new_contact.first_name);
            }
            else if (sort_type == 2)
            { // sort by last name
                strcpy(compare_buffer1, current->contact.last_name);
                strcpy(compare_buffer2, new_contact.last_name);
            }
            else
            { // sort by group
                strcpy(compare_buffer1, current->contact.group);
                strcpy(compare_buffer2, new_contact.group);
            }

            for (int j = 0; compare_buffer1[j]; j++)
            {
                compare_buffer1[j] = tolower(compare_buffer1[j]);
            }
            for (int j = 0; compare_buffer2[j]; j++)
            {
                compare_buffer2[j] = tolower(compare_buffer2[j]);
            }

            int outcome = strcoll(compare_buffer1, compare_buffer2);
            if (outcome == 1)
            {
                struct element *new_element = new_element_over_this(current, new_contact);
                if (new_element->previous == NULL)
                {
                    return new_element;
                }
                else
                {
                    return start;
                }
            }

            current = current->next;
        }
        struct element *new = new_element(new_contact, current);
        current->next = new;
        return start; // should never happen
    }
}

struct element *sort(struct element *start, int type)
{
    // other, easier way to execute this algorithm is to leave element nodes as they are and just swap pointers to structs with contact data between them
    // but in this case it was specified in the manual how the algorithm should look like
    if (start == NULL)
    {
        printf("\nList is empty\n\n");
        return NULL;
    }
    struct element *ptr1;
    struct element *lptr = NULL;
    bool swapped, swapped_this_turn;
    char compare_buffer1[20];
    char compare_buffer2[20];
    do
    {
        swapped = false;
        ptr1 = start;
        while (ptr1->next != lptr)
        {
            swapped_this_turn = false;
            if (type == 1) // sort by first name
            {
                strcpy(compare_buffer1, ptr1->contact.first_name);
                strcpy(compare_buffer2, ptr1->next->contact.first_name);
            }
            else if (type == 2)
            { // sort by last name
                strcpy(compare_buffer1, ptr1->contact.last_name);
                strcpy(compare_buffer2, ptr1->next->contact.last_name);
            }
            else
            { // sort by group
                strcpy(compare_buffer1, ptr1->contact.group);
                strcpy(compare_buffer2, ptr1->next->contact.group);
            }
            for (int j = 0; compare_buffer1[j]; j++)
            {
                compare_buffer1[j] = tolower(compare_buffer1[j]);
            }
            for (int j = 0; compare_buffer2[j]; j++)
            {
                compare_buffer2[j] = tolower(compare_buffer2[j]);
            }
            int outcome = strcoll(compare_buffer1, compare_buffer2);
            if (outcome == 1)
            {
                swap_node_with_its_next(ptr1);
                if (ptr1 == start)
                {
                    start = ptr1->previous;
                }
                swapped = 1;
                swapped_this_turn = 1;
            }

            if (!swapped_this_turn)
                ptr1 = ptr1->next;
        }
        lptr = ptr1;
    } while (swapped);
    return start;
}

struct element *create_list_from_csv()
{
    int result;
    char file_name[20];
    printf("Enter the name of CSV file with the contact list:\n");
    scanf("%s", file_name);
    strcat(file_name, ".CSV");
    FILE *f = fopen(file_name, "r");
    // assert(f != NULL);
    char line[128];
    char first_name[20];
    char last_name[20];
    char number[20];
    char group[20];

    struct element *start = NULL;
    struct element *previous = NULL;
    struct element *new;

    while (fscanf(f, "%s", line) == 1) // expect 1 successful conversion
    {

        result = sscanf(line, "%[^;] ; %[^;] ; %[^;] ; %s", first_name, last_name, number, group);
        if (previous == NULL)
        {
            start = new_element(create_contact(first_name, last_name, number, group), previous);
            previous = start;
        }
        else
        {
            new = new_element(create_contact(first_name, last_name, number, group), previous);
            previous->next = new;
            previous = new;
        }

        // print_contact(previous->contact);
    }
    // Not needed
    if (feof(f))
    {
        // printf("End\n");
    }
    else
    {
        // some other error interrupted the read
    }
    fclose(f);
    return start;
}

void display_list(struct element *start)
{
    // assert(start != NULL);
    if (start == NULL)
    {
        printf("\nList is empty\n\n");
        return;
    }
    assert(start->previous == NULL);
    int i = 1;
    struct element *current = start;
    while (current != NULL)
    {
        printf("Line %d\n", i);
        print_contact(current->contact);
        current = current->next;
        i++;
    }
}

void cleanup(struct element *start)
{

    if (start == NULL)
    {
        return;
    }
    assert(start->previous == NULL);
    struct element *current = start;
    while (current->next != NULL)
    {
        current = current->next;
        free(current->previous);
    }
    free(current);
}

void append_char(char *s, char c)
{
    int len = strlen(s);
    s[len] = c;
    s[len + 1] = '\0';
}

void find_all_contacts(struct element *start, int type)
{
    if (start == NULL)
    {
        printf("List is empty\n");
        return;
    }
    assert(start->previous == NULL);
    struct element *current;
    char compare_buffer1[50] = {
        0,
    };
    char compare_buffer2[50];
    char compare_buffer3[50];
    getchar();

    while (1)
    {
        char c = _getch();
        if (c == 13)
        {
            printf("KONIEC\n");
            return;
        }
        else if (c >= 65 && c <= 95)
        {
            append_char(compare_buffer1, c + 32);
        }
        else if (c >= 97 && c <= 122)
        {
            append_char(compare_buffer1, c);
        }
        else
        {
            char cc[2];
            cc[0] = c;
            cc[1] = '\0';
            strcat(compare_buffer1, tr(cc));
        }
        char *tab;
        system("CLS");
        printf("Looking for: %s\n\n", compare_buffer1);
        current = start;
        while (current->next != NULL)
        {
            if (type == 1) // find in name
            {
                strcpy(compare_buffer2, current->contact.first_name);
                strcpy(compare_buffer3, current->contact.last_name);
                for (int j = 0; compare_buffer2[j]; j++)
                {
                    compare_buffer2[j] = tolower(compare_buffer2[j]);
                }
                for (int j = 0; compare_buffer3[j]; j++)
                {
                    compare_buffer3[j] = tolower(compare_buffer3[j]);
                }
                if (strstr(compare_buffer2, compare_buffer1) != NULL || strstr(compare_buffer3, compare_buffer1) != NULL)
                {
                    print_contact(current->contact);
                }
            }
            else if (type == 2)
            { // sort by last name
                strcpy(compare_buffer2, current->contact.group);
                for (int j = 0; compare_buffer2[j]; j++)
                {
                    compare_buffer2[j] = tolower(compare_buffer2[j]);
                }
                if ( strstr(compare_buffer2, compare_buffer1) != NULL)
                {
                    
                    print_contact(current->contact);
                }
            }

            current = current->next;
        }
        printf("\nHit enter to stop searching or enter another character ...\n");
    }
}

int main()
{
    // setlocale(LC_CTYPE, ".1250");
    // setlocale(LC_ALL,"Polish");
    // setlocale(LC_CTYPE, "Polish");
    struct element *start = NULL;
    int sort_type = 0; // deafult
    int choice;
    while (true)
    {

        printf("Menu:\n1. Create List from CSV file \n2. Display List\n3. Sort List\n4. Add new element to the list\n5. Find a contact by name\n6. Find a contact by group\n7. Exit\n");
        char array[100] = {
            0,
        };
        scanf("%s", array);
        system("CLS");
        choice = 0;
        int check = sscanf(array, "%d", &choice);
        switch (choice)
        {
        case 0:
            printf("Pass a valid integer from the menu instead\n");
            break;

        case 1:
            cleanup(start);
            sort_type = 0;
            start = create_list_from_csv();
            display_list(start);
            break;

        case 2:
            display_list(start);
            break;

        case 3:
            system("CLS");
            printf("\n1. Sort by first name\n2. Sort by last name\n3. Sort by group\n");
            scanf("%s", array);
            check = sscanf(array, "%d", &choice);
            switch (choice)
            {
            case 0:
                printf("Invalid input pass an integer from the menu\n");
                break;
            case 1:
                sort_type = 1;
                break;
            case 2:
                sort_type = 2;
                break;
            case 3:
                sort_type = 3;
                break;
            deafult:
                printf("Invalid choice\n");
                break;
            }
            start = sort(start, sort_type);
            break;
        case 4:
            start = add_new_element(start, sort_type);
            break;
        case 5:
            find_all_contacts(start, 1);
            break;
        case 6:
            find_all_contacts(start, 2);
            break;
        case 7:
            printf("Exiting the application ...\n");
            cleanup(start);
            return 0;
            break;
        default:
            printf("Invalid choice, pass an integer from the menu\n");
            break;
        }
    }
    cleanup(start);
    return 0;
}