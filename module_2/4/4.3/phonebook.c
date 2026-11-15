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

static void free_nodes(TreeNode *node)
{
    if (!node) return;
    free_nodes(node->left);
    free_nodes(node->right);
    free(node);
}

static TreeNode *insert_node(TreeNode *root, TreeNode *node)
{
    if (!root) return node;
    if (compare_contact(&node->data, &root->data) < 0)
        root->left = insert_node(root->left, node);
    else
        root->right = insert_node(root->right, node);
    return root;
}

static void collect_inorder(TreeNode *node, Contact *array, size_t *index)
{
    if (!node) return;
    collect_inorder(node->left, array, index);
    array[(*index)++] = node->data;
    collect_inorder(node->right, array, index);
}

static TreeNode *build_balanced(const Contact *array, size_t begin, size_t end)
{
    if (begin >= end) return NULL;
    size_t middle = begin + (end - begin) / 2;
    TreeNode *node = malloc(sizeof(*node));
    if (!node) return NULL;
    node->data = array[middle];
    node->left = build_balanced(array, begin, middle);
    node->right = build_balanced(array, middle + 1, end);
    return node;
}

void book_balance(PhoneBook *book)
{
    if (!book || book->size < 2) { if (book) book->changes_since_balance = 0; return; }
    Contact *array = malloc(book->size * sizeof(*array));
    if (!array) return;
    size_t index = 0;
    collect_inorder(book->root, array, &index);
    TreeNode *new_root = build_balanced(array, 0, index);
    if (new_root) {
        free_nodes(book->root);
        book->root = new_root;
        book->changes_since_balance = 0;
    }
    free(array);
}

void book_init(PhoneBook *book)
{
    if (book) { book->root = NULL; book->size = 0; book->changes_since_balance = 0; }
}

void book_free(PhoneBook *book)
{
    if (!book) return;
    free_nodes(book->root);
    book_init(book);
}

size_t book_size(const PhoneBook *book) { return book ? book->size : 0; }

static const Contact *get_inorder(TreeNode *node, size_t *index, size_t target)
{
    if (!node) return NULL;
    const Contact *found = get_inorder(node->left, index, target);
    if (found) return found;
    if ((*index)++ == target) return &node->data;
    return get_inorder(node->right, index, target);
}

const Contact *book_get(const PhoneBook *book, size_t index)
{
    if (!book || index >= book->size) return NULL;
    size_t current = 0;
    return get_inorder(book->root, &current, index);
}

int book_add(PhoneBook *book, const Contact *contact)
{
    if (!book || !contact_valid(contact)) return 0;
    TreeNode *node = malloc(sizeof(*node));
    if (!node) return 0;
    node->data = *contact;
    node->left = node->right = NULL;
    book->root = insert_node(book->root, node);
    ++book->size;
    if (++book->changes_since_balance >= 5) book_balance(book);
    return 1;
}

static TreeNode *remove_min(TreeNode *root, TreeNode **minimum)
{
    if (!root->left) { *minimum = root; return root->right; }
    root->left = remove_min(root->left, minimum);
    return root;
}

static TreeNode *delete_key(TreeNode *root, const Contact *key, int *removed)
{
    if (!root) return NULL;
    int cmp = compare_contact(key, &root->data);
    if (cmp < 0) root->left = delete_key(root->left, key, removed);
    else if (cmp > 0) root->right = delete_key(root->right, key, removed);
    else {
        *removed = 1;
        if (!root->left) { TreeNode *right = root->right; free(root); return right; }
        if (!root->right) { TreeNode *left = root->left; free(root); return left; }
        TreeNode *minimum = NULL;
        TreeNode *right = remove_min(root->right, &minimum);
        minimum->left = root->left;
        minimum->right = right;
        free(root);
        return minimum;
    }
    return root;
}

int book_delete(PhoneBook *book, size_t index)
{
    const Contact *found = book_get(book, index);
    if (!found) return 0;
    Contact key = *found;
    int removed = 0;
    book->root = delete_key(book->root, &key, &removed);
    if (removed) {
        --book->size;
        if (++book->changes_since_balance >= 5) book_balance(book);
    }
    return removed;
}

int book_edit(PhoneBook *book, size_t index, const Contact *contact)
{
    if (!book || !contact_valid(contact)) return 0;
    const Contact *old = book_get(book, index);
    if (!old) return 0;
    Contact key = *old;
    int removed = 0;
    book->root = delete_key(book->root, &key, &removed);
    if (!removed) return 0;
    --book->size;
    if (!book_add(book, contact)) {
        TreeNode *node = malloc(sizeof(*node));
        if (!node) { book_balance(book); return 0; }
        node->data = key; node->left = node->right = NULL;
        book->root = insert_node(book->root, node); ++book->size;
        return 0;
    }
    return 1;
}
