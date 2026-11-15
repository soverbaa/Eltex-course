#ifndef PHONEBOOK_H
#define PHONEBOOK_H
#include <stddef.h>
#include "contact.h"

typedef struct TreeNode {
    Contact data;
    struct TreeNode *left;
    struct TreeNode *right;
} TreeNode;

typedef struct {
    TreeNode *root;
    size_t size;
    size_t changes_since_balance;
} PhoneBook;

void book_init(PhoneBook *book);
void book_free(PhoneBook *book);
size_t book_size(const PhoneBook *book);
const Contact *book_get(const PhoneBook *book, size_t index);
int book_add(PhoneBook *book, const Contact *contact);
int book_edit(PhoneBook *book, size_t index, const Contact *contact);
int book_delete(PhoneBook *book, size_t index);
void book_balance(PhoneBook *book);
#endif
