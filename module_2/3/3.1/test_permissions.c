#include "permissions.h"

#include <assert.h>
#include <stdio.h>

// проверка преобразования цифровой записи
static void test_parse_octal(void)
{
    mode_t permissions = 0;

    assert(parse_permissions("755", &permissions));
    assert(permissions == 0755);

    assert(parse_permissions("0644", &permissions));
    assert(permissions == 0644);

    assert(parse_permissions("000", &permissions));
    assert(permissions == 0000);

    printf("[OK] test_parse_octal\n");
}

// проверка преобразования буквенной записи
static void test_parse_symbolic(void)
{
    mode_t permissions = 0;

    assert(parse_permissions("rwxr-xr-x", &permissions));
    assert(permissions == 0755);

    assert(parse_permissions("rw-r-----", &permissions));
    assert(permissions == 0640);

    assert(parse_permissions("---------", &permissions));
    assert(permissions == 0000);

    printf("[OK] test_parse_symbolic\n");
}

// проверка некорректного ввода
static void test_parse_invalid(void)
{
    mode_t permissions = 0;

    assert(!parse_permissions("888", &permissions));
    assert(!parse_permissions("78", &permissions));
    assert(!parse_permissions("7777", &permissions));
    assert(!parse_permissions("rw-r--x", &permissions));
    assert(!parse_permissions("", &permissions));
    assert(!parse_permissions(NULL, &permissions));

    printf("[OK] test_parse_invalid\n");
}

//проверка добавления прав
static void test_add_permissions(void)
{
    mode_t permissions = 0640;

    assert(modify_permissions(&permissions, "u+x"));
    assert(permissions == 0740);

    permissions = 0755;

    assert(modify_permissions(&permissions, "g+w"));
    assert(permissions == 0775);

    printf("[OK] test_add_permissions\n");
}

// проверка удаления прав
static void test_remove_permissions(void)
{
    mode_t permissions = 0755;

    assert(modify_permissions(&permissions, "o-r"));
    assert(permissions == 0751);

    permissions = 0777;

    assert(modify_permissions(&permissions, "a-w"));
    assert(permissions == 0555);

    printf("[OK] test_remove_permissions\n");
}

// проверка назначения прав через =
static void test_assign_permissions(void)
{
    mode_t permissions = 0755;

    assert(modify_permissions(&permissions, "u=rw"));
    assert(permissions == 0655);

    permissions = 0755;

    assert(modify_permissions(&permissions, "g="));
    assert(permissions == 0705);

    permissions = 0755;

    assert(modify_permissions(&permissions, "o=r"));
    assert(permissions == 0754);

    printf("[OK] test_assign_permissions\n");
}

// проверка последовательности команд
static void test_multiple_commands(void)
{
    mode_t permissions = 0755;

    assert(modify_permissions(&permissions, "g-w,o=r"));
    assert(permissions == 0754);

    permissions = 0640;

    assert(modify_permissions(&permissions, "u+x,g+x"));
    assert(permissions == 0750);

    printf("[OK] test_multiple_commands\n");
}

// проверка обработки ошибок без изменения исходных прав
static void test_modify_invalid(void)
{
    mode_t permissions = 0755;

    assert(!modify_permissions(&permissions, "g+z"));
    assert(permissions == 0755);

    assert(!modify_permissions(&permissions, "u"));
    assert(permissions == 0755);

    assert(!modify_permissions(&permissions, ""));
    assert(permissions == 0755);

    assert(!modify_permissions(&permissions, "u+x,"));
    assert(permissions == 0755);

    assert(!modify_permissions(NULL, "u+x"));

    printf("[OK] test_modify_invalid\n");
}

int main(void)
{
    test_parse_octal();
    test_parse_symbolic();
    test_parse_invalid();

    test_add_permissions();
    test_remove_permissions();
    test_assign_permissions();
    test_multiple_commands();
    test_modify_invalid();

    printf("\nВсе unit-тесты пройдены успешно!\n");

    return 0;
}
