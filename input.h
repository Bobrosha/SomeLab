#ifndef INPUT_H
#define INPUT_H

#include <iostream>
#include <string>

using namespace std;

const string MESSAGE_DISTRIBUTION =
        "Введите название распределения из предложенных (truncatedcos -	усеченное косинусно-степенное, cospowerlaw - косинусно-степенное)\n"
        "или введите число (0 < v < 1), если хотите сами задать параметры формы:";
const string MESSAGE_LABEL = "Введите текстовую метку, описывающую объект:";
const string MESSAGE_SIZE = "Введите размер выборки N:";
const string MESSAGE_SIZE_ERROR =
        "Введено неверное значение(объем выборки должен быть от 100 до 1000000000). Вве-дите размер выборки N:";
const string MESSAGE_SHIFT = "Введите значение сдвига:\n"
        "Для того чтобы не было сдвига введите 0:";
const string MESSAGE_SHIFT_ERROR = "Введено неверное значение. Введите значение сдвига:\n"
        "Для того чтобы значение было по умолчанию введите 0:";
const string MESSAGE_SCALE = "Введите значение масштаба:\n"
        "Для того чтобы значение было по умолчанию введите 1:";
const string MESSAGE_SCALE_ERROR = "Введено неверное значение(масштаб не может быть 0). Введите значение масштаба:"
        "Для того чтобы значение было по умолчанию введите 1:";

const string FILE_NAME_SAMPLE = "sample.txt";
const string FILE_NAME_COSEXP = "cosexp.txt";

class Utility_io {
    string text;
    string label;
    int n = 0;
    double theta = 0;
    double lambda = 0;

public:
    [[nodiscard]] string get_text() const {
        return text;
    }

    [[nodiscard]] string get_label() const {
        return label;
    }

    [[nodiscard]] int get_n() const {
        return n;
    }

    [[nodiscard]] double get_theta() const {
        return theta;
    }

    [[nodiscard]] double get_lambda() const {
        return lambda;
    }

private:
    void input_distribution() {
        cout << MESSAGE_DISTRIBUTION << endl;
        getline(cin, text);
    }

    void input_label() {
        cout << MESSAGE_LABEL << endl;
        cin >> label;
    }

    void input_n() {
        cout << MESSAGE_SIZE << endl;
        cin >> n;

        while (!cin || n < 100 || n > 1000000000) {
            cin.clear();
            cin.ignore(10000, '\n');

            cout << MESSAGE_SIZE_ERROR << endl;
            cin >> n;
        }
    }

    void input_theta() {
        cout << MESSAGE_SHIFT << endl;
        cin >> theta;

        while (!cin) {
            cin.clear();
            cin.ignore(10000, '\n');

            cout << MESSAGE_SHIFT_ERROR << endl;
            cin >> theta;
        }
    }

    void input_lambda() {
        cout << MESSAGE_SCALE << endl;
        cin >> lambda;

        while (!cin || lambda == 0.0) {
            cin.clear();
            cin.ignore(10000, '\n');

            cout << MESSAGE_SCALE_ERROR << endl;
            cin >> lambda;
        }
    }

public:
    static Utility_io input() {
        auto cl = Utility_io();

        cl.input_distribution();
        cl.input_label();
        cl.input_n();
        cl.input_theta();
        cl.input_lambda();

        return cl;
    }

    static void save_sample(const vector<double> &array) {
        write_to_file(array, FILE_NAME_SAMPLE);
    }

    static void save_cosexp(const vector<double> &array) {
        write_to_file(array, FILE_NAME_COSEXP);
    }

private:
    static void write_to_file(const vector<double> &array, const string &filename) {
        ofstream fout;
        fout.open(filename);
        for (const auto elem: array) {
            fout << elem << "\n";
        }
        fout.close();
    }
};

#endif
