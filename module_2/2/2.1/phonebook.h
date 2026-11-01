#ifndef PHONEBOOK_H
#define PHONEBOOK_H
#include <stddef.h>
#include "contact.h"

typedef struct {
    Contact *items;
    size_t size;
    size_t capacity;
} PhoneBook;

void book_init(PhoneBook *b);
void book_free(PhoneBook *b);
int book_add(PhoneBook *b, const Contact *c);
int book_edit(PhoneBook *b, size_t index, const Contact *c);
int book_delete(PhoneBook *b, size_t index);

#endif
