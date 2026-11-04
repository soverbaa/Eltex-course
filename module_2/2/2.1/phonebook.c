#include <stdlib.h>
#include "phonebook.h"

void book_init(PhoneBook *book)
{
    if (book) { 
        book->items = NULL;
        book->size = 0;
        book->capacity = 0;
    }
}

void book_free(PhoneBook *book)
{
    if (book) {
        free(book->items);
        book_init(book);
    }
}

int book_add(PhoneBook *book, const Contact *contact)
{
    if (!book || !contact_valid(contact)) {
        return 0;
    }

    if (book->size == book->capacity) {
        size_t new_capacity = book->capacity ? book->capacity * 2 : 4;
        Contact *new_items = realloc(book->items,
                                     new_capacity * sizeof(*new_items));
        if (!new_items) {
            return 0;
        }
        book->items = new_items;
        book->capacity = new_capacity;
    }

    book->items[book->size++] = *contact;
    return 1;
}

int book_edit(PhoneBook *book, size_t index, const Contact *contact)
{
    if (!book || index >= book->size || !contact_valid(contact)) {
        return 0;
    }

    book->items[index] = *contact;
    return 1;
}

int book_delete(PhoneBook *book, size_t index)
{
    if (!book || index >= book->size) {
        return 0;
    }

    for (size_t i = index; i + 1 < book->size; ++i) {
        book->items[i] = book->items[i + 1];
    }
    --book->size;
    return 1;
}
