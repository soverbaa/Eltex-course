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
    Contact contacts[] = {
        make_contact("Сидоров", "Сидор"), make_contact("Иванов", "Иван"),
        make_contact("Петров", "Пётр"), make_contact("Алексеев", "Алексей"),
        make_contact("Орлов", "Олег"), make_contact("Кузнецов", "Кузьма"),
        make_contact("Волков", "Владимир")
    };
    for (size_t i = 0; i < sizeof(contacts) / sizeof(contacts[0]); ++i)
        assert(book_add(&book, &contacts[i]));
    assert(book_size(&book) == 7);
    /* После пяти изменений балансировка выполняется автоматически. */
    assert(book.changes_since_balance == 2);
    const char *expected[] = {"Алексеев", "Волков", "Иванов", "Кузнецов", "Орлов", "Петров", "Сидоров"};
    for (size_t i = 0; i < 7; ++i)
        assert(strcmp(book_get(&book, i)->surname, expected[i]) == 0);
    Contact edited = make_contact("Яковлев", "Яков");
    assert(book_edit(&book, 0, &edited));
    assert(strcmp(book_get(&book, 6)->surname, "Яковлев") == 0);
    assert(book_delete(&book, 2));
    assert(book_size(&book) == 6);
    assert(!book_delete(&book, 20));
    book_balance(&book);
    assert(book.changes_since_balance == 0);
    book_free(&book);
    assert(book_size(&book) == 0);
    return 0;
}
