#include "ipv4.h"

#include <assert.h>
#include <stdio.h>

static void test_parse_ipv4(void)
{
    uint32_t address;

    assert(parse_ipv4("192.168.1.1", &address));
    assert(!parse_ipv4("300.168.1.1", &address));
    assert(!parse_ipv4("192.168.1", &address));

    puts("[OK] test_parse_ipv4");
}

static void test_valid_mask(void)
{
    uint32_t mask;

    assert(parse_ipv4("255.255.255.0", &mask));
    assert(is_valid_mask(mask));

    assert(parse_ipv4("255.255.255.128", &mask));
    assert(is_valid_mask(mask));

    assert(parse_ipv4("255.0.255.0", &mask));
    assert(!is_valid_mask(mask));

    puts("[OK] test_valid_mask");
}

static void test_same_subnet(void)
{
    uint32_t gateway;
    uint32_t local;
    uint32_t external;
    uint32_t mask;

    assert(parse_ipv4("192.168.1.1", &gateway));
    assert(parse_ipv4("192.168.1.25", &local));
    assert(parse_ipv4("192.168.2.25", &external));
    assert(parse_ipv4("255.255.255.0", &mask));

    assert(same_subnet(gateway, local, mask));
    assert(!same_subnet(gateway, external, mask));

    puts("[OK] test_same_subnet");
}

int main(void)
{
    test_parse_ipv4();
    test_valid_mask();
    test_same_subnet();

    puts("Все тесты пройдены.");
    return 0;
}
