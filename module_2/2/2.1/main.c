#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "phonebook.h"

static int read_line(const char *prompt, char *buffer, size_t size)
{
    printf("%s", prompt);
    if (!fgets(buffer, (int)size, stdin)) {
        buffer[0] = '\0';
        return 0;
    }

    size_t length = strcspn(buffer, "\n");
    if (buffer[length] == '\n') {
        buffer[length] = '\0';
    } else {
        int ch;
        while ((ch = getchar()) != '\n' && ch != EOF) {
        }
    }
    return 1;
}

static int read_menu_choice(int *choice)
{
    char line[32];
    char *end;
    long value;

    if (!read_line("Выбор: ", line, sizeof(line))) {
        return 0;
    }
    value = strtol(line, &end, 10);
    if (end == line || *end != '\0' || value < 0 || value > 4) {
        puts("Введите число от 0 до 4.");
        return -1;
    }
    *choice = (int)value;
    return 1;
}

static void read_multiple(const char *title,
                          char values[MAX_ITEMS][ITEM_TEXT], int *count)
{
    char prompt[80];
    *count = 0;
    printf("%s (до %d значений; пустая строка — закончить):\n",
           title, MAX_ITEMS);

    while (*count < MAX_ITEMS) {
        (void)snprintf(prompt, sizeof(prompt), "  Значение %d: ", *count + 1);
        if (!read_line(prompt, values[*count], ITEM_TEXT)) {
            values[*count][0] = '\0';
            return;
        }
        if (values[*count][0] == '\0') {
            break;
        }
        ++(*count);
    }
}

static void read_contact(Contact *contact)
{
    memset(contact, 0, sizeof(*contact));
    (void)read_line("Фамилия (обязательно): ", contact->surname,
                    sizeof(contact->surname));
    (void)read_line("Имя (обязательно): ", contact->name,
                    sizeof(contact->name));
    (void)read_line("Отчество (необязательно): ", contact->patronymic,
                    sizeof(contact->patronymic));
    (void)read_line("Место работы (необязательно): ", contact->workplace,
                    sizeof(contact->workplace));
    (void)read_line("Должность (необязательно): ", contact->position,
                    sizeof(contact->position));

    read_multiple("Телефоны", contact->phones, &contact->phone_count);
    read_multiple("Адреса электронной почты", contact->emails,
                  &contact->email_count);
    read_multiple("Ссылки на социальные сети", contact->social,
                  &contact->social_count);
    read_multiple("Профили в мессенджерах", contact->messengers,
                  &contact->messenger_count);
}

static int read_contact_index(size_t *index)
{
    char line[32];
    char *end;
    unsigned long value;

    if (!read_line("Номер контакта: ", line, sizeof(line))) {
        return 0;
    }
    value = strtoul(line, &end, 10);
    if (end == line || *end != '\0' || value == 0) {
        puts("Введите положительный номер контакта.");
        return -1;
    }
    *index = (size_t)(value - 1);
    return 1;
}

int main(void)
{
    PhoneBook book;
    int choice = -1;
    book_init(&book);

    while (choice != 0) {
        int input_status;
        puts("\nТелефонная книга");
        puts("1. Добавить контакт");
        puts("2. Показать все контакты");
        puts("3. Редактировать контакт");
        puts("4. Удалить контакт");
        puts("0. Выход");

        input_status = read_menu_choice(&choice);
        if (input_status == 0) {
            break;
        }
        if (input_status < 0) {
            continue;
        }

        if (choice == 1 || choice == 3) {
            size_t index = 0;
            if (choice == 3) {
                int status = read_contact_index(&index);
                if (status == 0) {
                    break;
                }
                if (status < 0) {
                    continue;
                }
                if (index >= book.size) {
                    puts("Контакт с таким номером не найден.");
                    continue;
                }
                puts("Введите новые данные контакта.");
            }

            Contact contact;
            read_contact(&contact);
            if (!contact_valid(&contact)) {
                puts("Фамилия и имя обязательны. Контакт не сохранён.");
            } else if (choice == 1) {
                puts(book_add(&book, &contact)
                         ? "Контакт добавлен."
                         : "Не удалось добавить контакт.");
            } else {
                puts(book_edit(&book, index, &contact)
                         ? "Контакт обновлён."
                         : "Не удалось изменить контакт.");
            }
        } else if (choice == 2) {
            if (book.size == 0) {
                puts("Список контактов пуст.");
            }
            for (size_t i = 0; i < book.size; ++i) {
                printf("\n%zu. ", i + 1);
                contact_print(&book.items[i]);
            }
        } else if (choice == 4) {
            size_t index;
            int status = read_contact_index(&index);
            if (status == 0) {
                break;
            }
            if (status < 0) {
                continue;
            }
            puts(book_delete(&book, index)
                     ? "Контакт удалён."
                     : "Контакт с таким номером не найден.");
        }
    }

    book_free(&book);
    return 0;
}
