#ifndef CONTACT_H
#define CONTACT_H

#define TEXT 100
#define MAX_ITEMS 5
#define ITEM_TEXT 100

typedef struct {
    char surname[TEXT];
    char name[TEXT];
    char patronymic[TEXT];
    char workplace[TEXT];
    char position[TEXT];
    char phones[MAX_ITEMS][ITEM_TEXT];
    char emails[MAX_ITEMS][ITEM_TEXT];
    char social[MAX_ITEMS][ITEM_TEXT];
    char messengers[MAX_ITEMS][ITEM_TEXT];
    int phone_count;
    int email_count;
    int social_count;
    int messenger_count;
} Contact;

int contact_valid(const Contact *contact);
void contact_print(const Contact *contact);

#endif
