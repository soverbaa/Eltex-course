#include <assert.h>
#include <string.h>
#include "phonebook.h"

static Contact make_contact(const char *surname, const char *name)
{
    Contact c = {0};
    strcpy(c.surname, surname);
    strcpy(c.name, name);
    return c;
}

int main(void)
{
    PhoneBook book;
    book_init(&book);
    Contact p = make_contact("Петров", "Пётр");
    Contact i = make_contact("Иванов", "Иван");
    Contact s = make_contact("Сидоров", "Сидор");
    assert(book_add(&book, &p));
    assert(book_add(&book, &s));
    assert(book_add(&book, &i));
    assert(book_size(&book) == 3);
    assert(strcmp(book_get(&book, 0)->surname, "Иванов") == 0);
    assert(strcmp(book_get(&book, 1)->surname, "Петров") == 0);
    assert(strcmp(book_get(&book, 2)->surname, "Сидоров") == 0);
    Contact edited = make_contact("Кузнецов", "Кузьма");
    assert(book_edit(&book, 0, &edited));
    assert(strcmp(book_get(&book, 0)->surname, "Кузнецов") == 0);
    assert(book_delete(&book, 1));
    assert(book_size(&book) == 2);
    assert(!book_delete(&book, 10));
    book_free(&book);
    assert(book_size(&book) == 0);
    return 0;
}
