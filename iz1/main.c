// Выполнила Плаксина Юлия, АПО-11

//Задача A-9. Задача об отображении темы письма
//        Time limit:	14 s
//        Memory limit:	64 M
//        Чтобы показать информацию о пришедшем письме, нужно сначала её найти.
//Для этого в файле письма необходимо найти специальные заголовки.
//
//Составить программу построчной фильтрации текста.
//Суть фильтра — отбор строк, начинающихся с одного из следующих выражений: «Date:», «From:»,«To:», «Subject:».
//Текстовые строки подаются на стандартный ввод программы, результат программы должен подаваться на стандартный вывод.
//
//Процедура фильтрации должна быть оформлена в виде отдельной функции, которой подается на вход массив строк, выделенных в динамической памяти и его длина.
//На выходе функция возвращает указатель на NULL-терминированный массив с найденными строками (последним элементом массива добавлен NULL для обозначения, что данных больше нет).
//
//Программа должна уметь обрабатывать возникающие ошибки (например, ошибки выделения памяти). В случае возникновения ошибки нужно вывести в поток стандартного вывода сообщение "[error]" и завершить выполнение программы.
//



#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define STR 256


/*Функция удаляет выделенную память*/
void del_mem(char **str, size_t col);

/*Возвращает 1, если ошибок нет, печатает сообщение и возвращает 0, если ошибки найдены*/
int err_mem(char **str);

/*Возвращает 1, если ошибок нет, печатает сообщение и возвращает 0, если ошибки найдены*/
int err_read(char **str);

/*Читает строки из потока ввода, возвращает NULL при ошибках*/
char ** read_str(size_t * col);

/*Печатает строки*/
void print_str(char ** str);

/*Фильтрует массив строк, возвращает массив из отфильтрованных строк или NULL, если произошла ошибка*/
char **filter_str(char **str, size_t col);

int main() {

    size_t col = 0;
    char **str = read_str(&col);
    if (str == NULL) {
        return 0;
    }

    char **fil_str = filter_str(str, col);
    if (fil_str == NULL) {
        return 0;
    }

    print_str(fil_str);

    del_mem(str, col);
    free(fil_str);


    return 0;
}



char **filter_str(char **str, size_t col) {

    char *temp[] = {"Date:", "From:", "To:", "Subject:"};                   // массив элементов, с которых могут начинаться строки
    size_t num_str = 4;

    char **fil_str = calloc(col+1, sizeof(char *));                         // выделяем память под такой же массив, который был прочитан
    if (!err_mem(fil_str)) {                                                // проверка на ошибки
        return NULL;
    }

    size_t k = 0;

    for (size_t j = 0; j < col; ++j) {                                      // цикл по элементам прочитанного массива строк

        char * check_str = str[j];

        for (size_t i = 0; i < num_str; ++i) {                              // цикл по элементам статического массива походящих элементов

            char * need_str = temp[i];
            int isMatch = 1;

            for (size_t m = 0; m < strlen(need_str); ++m) {                 // цикл по символам каждого элемента

                if (check_str[m] != need_str[m]) {                          // выход из цикла, если символы не совпадают
                    isMatch = 0;
                    break;
                }
            }

            if (isMatch) {                                                  // если строчка подходит, то сохраняем ее и
                                                                            // переходим к следующей строчке начального массива
                fil_str[k] = check_str;
                ++k;
                break;
            }


        }
    }


    return fil_str;
}



void print_str(char ** str) {

    for (size_t i = 0; str[i] != NULL; ++i) {
        printf("%s", str[i]);
    }
}




void del_mem(char **str, size_t col) {

    for (size_t i = 0; i <= col; ++i) {
        free(str[i]);
    }
    free(str);
}

int err_mem(char **str) {

    if (str != NULL) {
        return 1;

    }
    else {
        printf("%s", "[error]");
        return 0;
    }
}

int err_read(char **str) {

        if ( ferror(stdin) == 0 ) {
            return 1;
        }
        else {
            printf("%s", "[error]");
            return 0;
        }
}

char ** read_str(size_t *col) {
    char **strings = NULL;
    char ** test = NULL;

    size_t num_str = 0;

    while (1) {

        test = (char **)realloc(strings, (num_str+1)*sizeof(char *));                          // Выделяем память под массив строчек
        if (err_mem(test)) {                                                                   // проверка на ошибки
            strings = test;
        }
        else {
            if (strings != NULL) {
                del_mem(strings, num_str);
            }
            return NULL;
        }

        strings[num_str] = (char *)malloc(STR*sizeof(char));                                   // выделяем память под строчку
        if (!err_mem(&strings[num_str])) {                                                     // проверка на ошибки
            del_mem(strings, num_str);
            return NULL;
        }


        char* buffer = fgets(strings[num_str], STR, stdin);                                    // читаем 256 символов или строчку, пока она не закончится

        if (buffer == NULL) {                                                                  // Проверка чтения
            if (err_read(&buffer)) {
                break;
            }
            else {
                del_mem(strings, num_str);
                return NULL;
            }
        }

        size_t str_len = strlen(buffer);                                                       // длина прочитанной строчки

        while (strings[num_str][str_len-1] != '\n') {                                                    // пока последний символ не символ перевода строки

            char * test_str = (char *)realloc(strings[num_str], 2*str_len*sizeof(char));       // увеличиваем память в два раза

            if (err_mem(&test_str)) {                                                          // проверка выделения памяти
                strings[num_str] = test_str;
            }
            else {
                del_mem(strings, num_str);
                return NULL;
            }

            buffer = fgets(strings[num_str] + str_len, STR, stdin);                        // читаем 256 символов или строчку, пока она не закончится

            if (buffer == NULL) {                                                              // Проверка чтения
                if (err_read(&buffer)) {
                    break;
                }
                else {
                    del_mem(strings, num_str);
                    return NULL;
                }
            }

            str_len += strlen(buffer);                                                         // Новая длина прочитанной строчки
        }

        ++num_str;
    }

    *col = num_str;

    return strings;
}
