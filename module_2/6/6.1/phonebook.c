#include <stdlib.h>
#include <string.h>
#include "phonebook.h"

static int compare_contact(const Contact *a, const Contact *b)
{
    int result = strcmp(a->surname, b->surname);
    if (result == 0) result = strcmp(a->name, b->name);
    if (result == 0) result = strcmp(a->patronymic, b->patronymic);
    return result;
}

void book_init(PhoneBook *book)
{
    if (book) { book->head = NULL; book->tail = NULL; book->size = 0; }
}

void book_free(PhoneBook *book)
{
    if (!book) return;
    ContactNode *node = book->head;
    while (node) {
        ContactNode *next = node->next;
        free(node);
        node = next;
    }
    book_init(book);
}

size_t book_size(const PhoneBook *book) { return book ? book->size : 0; }

const Contact *book_get(const PhoneBook *book, size_t index)
{
    if (!book || index >= book->size) return NULL;
    ContactNode *node = book->head;
    for (size_t i = 0; i < index; ++i) node = node->next;
    return &node->data;
}

static void insert_node(PhoneBook *book, ContactNode *node)
{
    ContactNode *current = book->head;

    /* Ищем первое место, где новый контакт должен стоять перед текущим. */
    while (current && compare_contact(&current->data, &node->data) <= 0) {
        current = current->next;
    }

    if (!current) {
        /* Вставка в конец списка. */
        node->prev = book->tail;
        if (book->tail) {
            book->tail->next = node;
        } else {
            book->head = node;
        }
        book->tail = node;
    } else {
        /* Вставка перед найденным узлом. */
        node->next = current;
        node->prev = current->prev;
        if (current->prev) {
            current->prev->next = node;
        } else {
            book->head = node;
        }
        current->prev = node;
    }
    ++book->size;
}

int book_add(PhoneBook *book, const Contact *contact)
{
    if (!book || !contact_valid(contact)) {
        return 0;
    }

    ContactNode *node = malloc(sizeof(*node));
    if (!node) {
        return 0;
    }

    node->data = *contact;
    node->prev = NULL;
    node->next = NULL;
    insert_node(book, node);
    return 1;
}

int book_delete(PhoneBook *book, size_t index)
{
    if (!book || index >= book->size) return 0;
    ContactNode *node = book->head;
    for (size_t i = 0; i < index; ++i) node = node->next;
    if (node->prev) node->prev->next = node->next;
    else book->head = node->next;
    if (node->next) node->next->prev = node->prev;
    else book->tail = node->prev;
    free(node);
    --book->size;
    return 1;
}

int book_edit(PhoneBook *book, size_t index, const Contact *contact)
{
    if (!book || index >= book->size || !contact_valid(contact)) {
        return 0;
    }

    /* Сначала выделяем новый узел, чтобы не потерять старый контакт
       в случае нехватки памяти. */
    ContactNode *new_node = malloc(sizeof(*new_node));
    if (!new_node) {
        return 0;
    }
    new_node->data = *contact;
    new_node->prev = NULL;
    new_node->next = NULL;

    ContactNode *old_node = book->head;
    for (size_t i = 0; i < index; ++i) {
        old_node = old_node->next;
    }

    /* Удаляем старый узел, не меняя общий размер списка. */
    if (old_node->prev) {
        old_node->prev->next = old_node->next;
    } else {
        book->head = old_node->next;
    }
    if (old_node->next) {
        old_node->next->prev = old_node->prev;
    } else {
        book->tail = old_node->prev;
    }
    free(old_node);
    --book->size;

    /* Вставляем обновлённый контакт на его новое отсортированное место. */
    insert_node(book, new_node);
    return 1;
}
