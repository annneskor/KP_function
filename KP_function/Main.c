#include <stdio.h>
#include <locale.h>
#include <math.h>

// Структуры для возврата результатов
typedef struct {
    double min_x;
    double max_x;
    double min_val;
    double max_val;
} ExtremumResult;

typedef struct {
    double x_found;
    int found;
} XByYResult;

typedef struct {
    double value;
    int valid;
} DerivativeResult;

// Объявление функций

double f(double x);

int write_file(const char* name, double a, double b, double h, int mode);

/**
 * Строит таблицу значений функции f(x) на отрезке [a, b] с шагом h.
 * @param a - начало интервала
 * @param b - конец интервала
 * @param h - шаг (должен быть > 0)
 * @param out_stream - указатель на поток вывода
 * @return 0 если успешно, -1 если ошибка (некорректные входные данные)
 */
int build_table(double a, double b, double h, FILE* out_stream);

/**
 * Находит приближённые значения минимума и максимума функции на отрезке [a, b].
 * @param a - начало интервала
 * @param b - конец интервала
 * @param step - шаг поиска экстремума
 * @return структура с результатами
 */
ExtremumResult find_extremum(double a, double b, double step);

/**
 * Находит значение x, при котором f(x) приближённо равно заданному y_target.
 * @param y_target - целевое значение функции
 * @param a - начало интервала поиска
 * @param b - конец интервала поиска
 * @param tolerance - допустимая погрешность
 * @return структура с результатом поиска
 */
XByYResult find_x_by_y(double y_target, double a, double b, double tolerance);

/**
 * Вычисляет производную функции f(x) в точке x численно (метод центральной разности).
 * @param x - точка, в которой вычисляется производная
 * @param h_step - шаг численного дифференцирования
 * @return структура с результатом
 */
DerivativeResult calculate_derivative(double x, double h_step);

// Главное меню
int main() {
    setlocale(LC_ALL, "");
    int choice;
    double x, y_target, tolerance = 0.001, result, h;

    printf("=== Добрый день, уважаемый пользователь! ===\n");
    printf("===   Работу выполнила Нескоромная Анна  ===\n");
    printf("===       Студентка группы бИЦ-251       ===\n");
    printf("=== Программа для работы с функцией f(x) ===\n");

    do {
        printf("\nВыберите операцию:\n");
        printf("1. Значение функции в точке\n");
        printf("2. Таблица значений\n");
        printf("3. Поиск минимума/максимума\n");
        printf("4. Поиск X по Y\n");
        printf("5. Производная в точке\n");
        printf("6. Запись табулирования в файл function.txt\n");
        printf("0. Выход\n");
        printf("Ваш выбор: ");
        scanf("%d", &choice);

        switch (choice) {
        case 1: {
            printf("Введите x: ");
            scanf("%lf", &x);
            result = f(x);
            printf("f(%.6f) = %.6f\n", x, result);
            break;
        }

        case 2: {
            double a_local, b_local, h_local;
            printf("Введите интервал [a, b]: ");
            scanf("%lf %lf", &a_local, &b_local);
            printf("Введите шаг h: ");
            scanf("%lf", &h_local);

            if (build_table(a_local, b_local, h_local, stdout) == 0) {
                // Успешно
            }
            else {
                printf("Ошибка: неверный интервал или шаг.\n");
            }
            break;
        }

        case 3: {
            double a_local, b_local, h_local;
            printf("Введите интервал [a, b]: ");
            scanf("%lf %lf", &a_local, &b_local);
            printf("Введите шаг для поиска: ");
            scanf("%lf", &h_local);

            if (h_local > 0 && a_local <= b_local) {
                ExtremumResult ext = find_extremum(a_local, b_local, h_local);
                printf("Минимум: x=%.6f, f(x)=%.6f\n", ext.min_x, ext.min_val);
                printf("Максимум: x=%.6f, f(x)=%.6f\n", ext.max_x, ext.max_val);
            }
            else {
                printf("Ошибка: не удалось найти экстремумы.\n");
            }
            break;
        }

        case 4: {
            printf("Введите целевое значение Y: ");
            scanf("%lf", &y_target);
            double a_local, b_local;
            printf("Введите интервал поиска [a, b]: ");
            scanf("%lf %lf", &a_local, &b_local);

            XByYResult res = find_x_by_y(y_target, a_local, b_local, tolerance);
            if (res.found) {
                printf("Найдено x = %.6f, где f(x) = %.6f\n", res.x_found, y_target);
            }
            else {
                printf("Ошибка: x не найден в заданном интервале.\n");
            }
            break;
        }

        case 5: {
            printf("Введите x: ");
            scanf("%lf", &x);
            printf("Введите шаг для производной (h): ");
            scanf("%lf", &h);

            DerivativeResult res = calculate_derivative(x, h);
            if (res.valid) {
                printf("f'(%.6f) = %.6f\n", x, res.value);
            }
            else {
                printf("Ошибка: x вне области определения или шаг недопустим.\n");
            }
            break;
        }

        case 6: {
            double a_local, b_local, h_local;
            printf("Введите интервал [a, b]: ");
            scanf("%lf %lf", &a_local, &b_local);
            printf("Введите шаг табуляции h: ");
            scanf("%lf", &h_local);

            printf("1 - Перезаписать файл\n");
            printf("2 - Добавить в конец\n");
            printf("Ваш выбор: ");
            int mode;
            scanf("%d", &mode);

            if (write_file("function.txt", a_local, b_local, h_local, mode) == 0) {
                printf("\nДанные успешно записаны в function.txt\n");
            }
            else {
                printf("\nОшибка: не удалось открыть файл!\n");
            }
            break;
        }

        case 0:
            printf("Выход из программы.\n");
            break;

        default:
            printf("Неверный выбор. Попробуйте снова.\n");
        }

    } while (choice != 0);

    return 0;
}

double f(double x) {
    if (x < 0) {
        return 1.0 / sqrt(1.0 - x);
    }
    else if (x < 1.0) {
        return tanh(x);
    }
    else { // x >= 1
        double sum = 0.0;
        double term = 1.0;
        double factorial = 1.0;

        for (int n = 0; n <= 13; n++) {
            sum += term / factorial;

            if (n < 13) {
                term *= x * x * x;
                int k = 3 * (n + 1);
                factorial *= (k - 2) * (k - 1) * k;
            }
        }
        return sum;
    }
}

// Функция записи в файл
int write_file(const char* name, double a, double b, double h, int mode) {
    FILE* file = fopen(name, (mode == 1) ? "w" : "a");
    if (file == NULL) {
        return -1;
    }

    if (mode == 1) {
        fprintf(file, "|     x      |     f(x)     |\n");
        fprintf(file, "|------------|--------------|\n");
    }

    for (double x = a; x <= b + 1e-9; x += h) {
        double y = f(x);
        fprintf(file, "| %8.6f  | %12.6f |\n", x, y);
    }

    fclose(file);
    return 0;
}

int build_table(double a, double b, double h, FILE* out_stream) {
    if (h <= 0 || a > b || out_stream == NULL) return -1;

    fprintf(out_stream, "| %8s | %12s |\n", "x", "f(x)");
    fprintf(out_stream, "|----------|--------------|\n");

    for (double x = a; x <= b + 1e-9; x += h) {
        double fx = f(x);
        fprintf(out_stream, "| %8.6f | %12.6f |\n", x, fx);
    }
    return 0;
}

ExtremumResult find_extremum(double a, double b, double step) {
    ExtremumResult res = { 0, 0, 0, 0 };

    double x = a;
    double first_val = f(x);

    res.min_x = x;
    res.max_x = x;
    res.min_val = first_val;
    res.max_val = first_val;

    for (x = a + step; x <= b + step / 2; x += step) {
        double fx = f(x);

        if (fx < res.min_val) {
            res.min_val = fx;
            res.min_x = x;
        }
        if (fx > res.max_val) {
            res.max_val = fx;
            res.max_x = x;
        }
    }

    return res;
}

XByYResult find_x_by_y(double y_target, double a, double b, double tolerance) {
    XByYResult res = { 0, 0 };

    if (tolerance <= 0 || a > b) {
        return res;
    }

    double step = (b - a) / 1000.0;
    for (double x = a; x <= b; x += step) {
        double fx = f(x);

        if (fabs(fx - y_target) <= tolerance) {
            res.x_found = x;
            res.found = 1;
            return res;
        }
    }

    return res;
}

DerivativeResult calculate_derivative(double x, double h_step) {
    DerivativeResult res = { 0, 0 };

    if (h_step <= 0) {
        return res;
    }

    double f_x_plus_h = f(x + h_step);
    double f_x_minus_h = f(x - h_step);

    res.value = (f_x_plus_h - f_x_minus_h) / (2.0 * h_step);
    res.valid = 1;
    return res;
}