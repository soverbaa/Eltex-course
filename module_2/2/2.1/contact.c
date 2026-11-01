#include <stdio.h>
#include "contact.h"

static int count_valid(int count)
{
    return count >= 0 && count <= MAX_ITEMS;
}

int contact_valid(const Contact *contact)
{
    return contact != NULL &&
           contact->surname[0] != '\0' &&
           contact->name[0] != '\0' &&
           count_valid(contact->phone_count) &&
           count_valid(contact->email_count) &&
           count_valid(contact->social_count) &&
           count_valid(contact->messenger_count);
}

void contact_print(const Contact *contact)
{
    if (!contact) {
        return;
    }

    printf("%s %s", contact->surname, contact->name);
    if (contact->patronymic[0]) {
        printf(" %s", contact->patronymic);
    }
    putchar('\n');

    if (contact->workplace[0]) {
        printf("Место работы: %s\n", contact->workplace);
    }
    if (contact->position[0]) {
        printf("Должность: %s\n", contact->position);
    }
    for (int i = 0; i < contact->phone_count; ++i) {
        printf("Телефон: %s\n", contact->phones[i]);
    }
    for (int i = 0; i < contact->email_count; ++i) {
        printf("Email: %s\n", contact->emails[i]);
    }
    for (int i = 0; i < contact->social_count; ++i) {
        printf("Соцсеть: %s\n", contact->social[i]);
    }
    for (int i = 0; i < contact->messenger_count; ++i) {
        printf("Мессенджер: %s\n", contact->messengers[i]);
    }
}
