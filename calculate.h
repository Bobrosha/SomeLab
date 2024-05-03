#ifndef CALCULATE_H
#define CALCULATE_H

#include <windows.h>
#include <algorithm>
#include <vector>
#include <numeric>
#include <cmath>
#include <string>

using namespace std;

inline double calculate_mean_value(vector<double> array) {
    return accumulate(begin(array), end(array), 0.) / static_cast<double>(array.size());
}

// вычисление выборочных медианы, минимального, максимального значений выборки
inline tuple<double, double, double> median_min_max(vector<double> array) {
    // [N/2 + 1]-порядковая статистика
    const auto pos = begin(array) + array.size() / 2; // NOLINT(*-narrowing-conversions)
    nth_element(begin(array), pos, end(array));

    double median;
    double min;

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
    const double max = *max_element(pos, end(array));

    return {median, min, max};
}

/**
* Calculate dispersion by distribution values
* @param array values of distribution
* @return value of dispersion
*/
inline double calculate_dispersion(const vector<double> &array) {
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
inline double asymmetry(const vector<double> &array) {
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
inline double calculate_excess(const vector<double> &array) {
    const auto mean = calculate_mean_value(array);
    vector<double> x;

    for (const double elem: array) {
        x.push_back(pow(elem - mean, 4));
    }

    return calculate_mean_value(x) / pow(calculate_dispersion(array), 2);
}

#endif
