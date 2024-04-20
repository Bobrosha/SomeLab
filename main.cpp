#define USE_MATH_DEFINES
#include <iostream>
#include <random>
#include <algorithm>
#include <vector>
#include <numeric>
#include <fstream>
#include <cmath>
#include<string>
#include <windows.h>

#include "input.h"
#include "CosExpWorker.h"

double calculate_mean_value(vector<double> array) {
    return accumulate(begin(array), end(array), 0.) / array.size();
}

// вычисление выборочных медианы, минимального, максимального значений выборки
void median_min_max(vector<double> array, double &median, double &min, double &max) {
    // [N/2 + 1]-порядковая статистика
    const auto pos = begin(array) + array.size() / 2;
    nth_element(begin(array), pos, end(array));

    // для нечетного размера выборки
    if (array.size() % 2) {
        median = *pos;

        // ищем в первой половине выборки
        min = *min_element(begin(array), pos);
    } else /* для четного размера выборки */ {
        // ищем в первой половине выборки
        const auto [minimum, maximum] = minmax_element(begin(array), pos);

        // second - [N/2]-порядковая статистика
        median = (*pos + *maximum) / 2.;

        // first - минимальный элемент выборки
        min = *minimum;
    }

    // ищем во второй половине выборки
    max = *max_element(pos, end(array));
}

/**
* Calculate dispersion by distribution values
* @param array values of distribution
* @return theoretical dispersion
*/
double calculate_dispersion(const vector<double> &array) {
    const auto mean = calculate_mean_value(array);
    vector<double> x;

    for (const double elem: array) {
        x.push_back(pow(elem - mean, 2));
    }

    return calculate_mean_value(x);
}

/**
 * Calculate the asymmetry by distribution
 * @param array values of distribution
 * @return value of asymmetry
 */
double asymmetry(const vector<double> &array) {
    const auto mean = calculate_mean_value(array);
    vector<double> x;

    for (const double elem: array) {
        x.push_back(pow(elem - mean, 3));
    }

    return calculate_mean_value(x) / pow(calculate_dispersion(array), 3 / 2);
}

/**
* Calculate excess by dispersion values
* @param array values of dispersion
* @return excess value
*/
double calculate_excess(const vector<double> &array) {
    const auto mean = calculate_mean_value(array);
    vector<double> x;

    for (const double elem: array) {
        x.push_back(pow(elem - mean, 4));
    }

    return calculate_mean_value(x) / pow(calculate_dispersion(array), 2);
}

bool isNumeric(string const &str) {
    return !str.empty();
}

int main() {
    // setlocale(LC_ALL, "Russian");

    const auto input = Utility_io::input();

    CosExpWorker C;
    if (isNumeric(input.get_text())) {
        C = CosExpWorker(stod(input.get_text()));
    } else {
        C = CosExpWorker(input.get_text());
    }

    cout << "Параметр формы задан равным " << C.get_v() << endl << endl;

    C.set_label(input.get_label());
    C.set_theta(input.get_theta());
    C.set_lambda(input.get_lambda());

    const auto sample = C.sample_gen(input.get_n());
    Utility_io::save_sample(sample);

    const auto f = C.distribution_fun(sample);
    Utility_io::save_cosexp(f);

    cout << endl << C.check_p() << endl; // ???

    cout << endl << "Среднее арифметическое = " << calculate_mean_value(sample) << endl;
    cout << "Мат ожидание = " << C.calculate_expected_value() << endl << endl;

    double median;
    double min;
    double max;

    median_min_max(sample, median, min, max);
    cout << "Медиана = " << median << endl;
    cout << "Теоретическая медиана = " << C.calculate_theoretical_median() << endl << endl;

    cout << "Min = " << min << endl;
    //cout << "Теоретический Min = " << C.min_calc() << endl << endl;

    cout << "Max = " << max << endl;
    //cout << "Теоретический Max = " << C.max_calc() << endl << endl;

    cout << "Дисперсия = " << calculate_dispersion(sample) << endl;
    cout << "Теоретическая дисперсия = " << C.calculate_theoretical_dispersion() << endl << endl;

    cout << "Коэффициент ассиметрии = " << asymmetry(sample) << endl << endl;
    cout << "Теоретический коэффициент ассиметрии = " << CosExpWorker::theoretical_asymmetry() << endl;

    cout << "Коэффициент эксцесса = " << calculate_excess(sample) << endl;
    cout << "Теоретический коэффициент эксцесса = " << C.calculate_theoretical_excess() << endl;
}
