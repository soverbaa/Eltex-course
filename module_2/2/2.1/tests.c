#include <assert.h>
#include <string.h>
#include "phonebook.h"

static Contact make_contact(void)
{
    Contact contact = {0};
    strcpy(contact.surname, "Иванова");
    strcpy(contact.name, "Анна");
    return contact;
}

int main(void)
{
    PhoneBook book;
    Contact contact = make_contact();

    book_init(&book);

    /* Фамилия и имя обязательны, остальные поля могут отсутствовать. */
    assert(contact_valid(&contact));
    assert(book_add(&book, &contact));
    assert(book.size == 1);

    /* Отчество и несколько значений дополнительных полей сохраняются. */
    strcpy(contact.patronymic, "Сергеевна");
    strcpy(contact.workplace, "ООО Пример");
    strcpy(contact.position, "Разработчик");
    strcpy(contact.phones[0], "+7 900 111-22-33");
    strcpy(contact.phones[1], "+7 900 444-55-66");
    contact.phone_count = 2;
    strcpy(contact.emails[0], "anna@example.com");
    strcpy(contact.emails[1], "a.ivanova@example.com");
    contact.email_count = 2;
    strcpy(contact.social[0], "https://example.com/social");
    contact.social_count = 1;
    strcpy(contact.messengers[0], "@anna");
    contact.messenger_count = 1;
    assert(contact_valid(&contact));
    assert(book_edit(&book, 0, &contact));
    assert(strcmp(book.items[0].patronymic, "Сергеевна") == 0);
    assert(book.items[0].phone_count == 2);
    assert(strcmp(book.items[0].phones[1], "+7 900 444-55-66") == 0);
    assert(book.items[0].email_count == 2);

    /* Пустые фамилия/имя и недопустимые количества отвергаются. */
    Contact invalid = make_contact();
    invalid.surname[0] = '\0';
    assert(!contact_valid(&invalid));
    invalid = make_contact();
    invalid.name[0] = '\0';
    assert(!book_add(&book, &invalid));
    invalid = make_contact();
    invalid.phone_count = MAX_ITEMS + 1;
    assert(!contact_valid(&invalid));

    /* Редактирование и удаление с проверкой индексов. */
    Contact second = make_contact();
    strcpy(second.surname, "Петрова");
    strcpy(second.name, "Мария");
    assert(book_add(&book, &second));
    assert(book.size == 2);
    assert(!book_edit(&book, 5, &contact));
    assert(book_delete(&book, 0));
    assert(book.size == 1);
    assert(strcmp(book.items[0].surname, "Петрова") == 0);
    assert(!book_delete(&book, 1));

    book_free(&book);
    assert(book.size == 0 && book.items == NULL);
    return 0;
}
