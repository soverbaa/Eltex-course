#include "permissions.h"

#include <stdio.h>
#include <string.h>

#define INPUT_SIZE 256

int main(void)
{
    int choice;
    char input[INPUT_SIZE];

    mode_t permissions = 0;
    int permissions_available = 0;

    while (1) {
        printf("\n=== Расчет маски прав доступа ===\n");
        printf("1. Ввести права и показать представления\n");
        printf("2. Получить права файла через stat()\n");
        printf("3. Изменить права с помощью команды chmod\n");
        printf("0. Выход\n");
        printf("Выберите пункт: ");

        if (scanf("%d", &choice) != 1) {
            printf("Ошибка ввода.\n");

            int ch;
            while ((ch = getchar()) != '\n' && ch != EOF) {
            }

            continue;
        }

        int ch;
        while ((ch = getchar()) != '\n' && ch != EOF) {
        }

        if (choice == 0) {
            printf("Завершение программы.\n");
            break;
        }

        switch (choice) {
            case 1: {
                printf("Введите права (например, 755 или rwxr-xr-x): ");

                if (fgets(input, sizeof(input), stdin) == NULL) {
                    printf("Ошибка чтения.\n");
                    break;
                }

                input[strcspn(input, "\n")] = '\0';

                if (!parse_permissions(input, &permissions)) {
                    printf("Некорректный формат прав доступа.\n");
                    break;
                }

                permissions_available = 1;

                printf("\nРезультат:\n");
                print_permissions(permissions);
                break;
            }

            case 2: {
                struct stat file_info;

                printf("Введите имя файла: ");

                if (fgets(input, sizeof(input), stdin) == NULL) {
                    printf("Ошибка чтения.\n");
                    break;
                }

                input[strcspn(input, "\n")] = '\0';

                if (input[0] == '\0') {
                    printf("Имя файла не должно быть пустым.\n");
                    break;
                }

                if (stat(input, &file_info) == -1) {
                    perror("Не удалось получить информацию о файле");
                    break;
                }

                permissions = file_info.st_mode & 0777;
                permissions_available = 1;

                printf("\nПрава доступа к файлу: %s\n", input);
                print_permissions(permissions);

                printf("\nСравнение с ls -l:\n");
                printf("Выполните в терминале: ls -l -- ");
                printf("'%s'\n", input);
                break;
            }

            case 3: {
                if (!permissions_available) {
                    printf("Сначала выполните пункт 1 или 2.\n");
                    break;
                }

                printf("Текущие права:\n");
                print_permissions(permissions);

                printf("\nВведите команду (например, u+x или g-w,o=r): ");

                if (fgets(input, sizeof(input), stdin) == NULL) {
                    printf("Ошибка чтения.\n");
                    break;
                }

                input[strcspn(input, "\n")] = '\0';

                mode_t modified_permissions = permissions;

                if (!modify_permissions(&modified_permissions, input)) {
                    printf("Некорректная команда модификации прав.\n");
                    break;
                }

                permissions = modified_permissions;

                printf("\nРезультат модификации:\n");
                print_permissions(permissions);

                printf("\nПрава реального файла не изменялись.\n");
                break;
            }

            default:
                printf("Неизвестный пункт меню.\n");
                break;
        }
    }

    return 0;
}
