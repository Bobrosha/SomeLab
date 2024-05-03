#ifndef WORKER_H
#define WORKER_H
#define USE_MATH_DEFINES

#include <cassert>
#include <random>
#include <vector>
#include <cmath>
#include <string>

using namespace std;

const double PI = acos(-1.0);

inline random_device rnd;
inline default_random_engine random(rnd());

class CosinusExponentialWorker {
    double _v;
    double theta = 0;
    double lambda = 0;
    string label;
    int n = 0;

public:
    explicit CosinusExponentialWorker(
        const double _v,
        const double _theta,
        const double _lambda,
        const string &_label,
        const int _n
    ) {
        this->_v = _v;

        theta = _theta;
        lambda = _lambda;
        label = _label;
        n = _n;
    }

    static CosinusExponentialWorker from(const Utility_io &io) {
        return CosinusExponentialWorker(io.get_v(), io.get_theta(), io.get_lambda(), io.get_label(), io.get_n());
    }

    [[nodiscard]] double get_v() const {
        return _v;
    }

    [[nodiscard]] double generate_cosexp_value(const double p) const {
        vector<double> probabilities;

        probabilities.push_back(p);
        for (int i = 1; i < 3; i++) {
            probabilities.push_back((1 - p) / 2);
        }

        discrete_distribution<> d_distr(probabilities.begin(), probabilities.end());
        uniform_real_distribution<> r_distr(0, 1);
        exponential_distribution<> e_distr(1 / check_a());

        const auto z = d_distr(random);
        const auto x2 = e_distr(random);

        double x = NULL;
        if (z == 0) {
            bool flag = true;
            while (flag) {
                const auto r1 = r_distr(random);
                const auto r2 = r_distr(random);

                if (const double x1 = (2 * r1 - 1) * _v; r2 <= pow(cos(PI * x1 / 2), 2)) {
                    x = x1;
                    flag = false;
                }
            }
        } else if (z == 1) {
            x = _v + x2;
        } else if (z == 2) {
            x = -_v - x2;
        }
        assert(x != NULL);
        return x * lambda + theta;
    }

    [[nodiscard]] vector<double> sample_gen() const {
        vector<double> result;
        for (int i = 0; i < n; i++) {
            result.push_back(generate_cosexp_value(check_p()));
        }

        return result;
    }

    [[nodiscard]] vector<double> distribution_fun(const vector<double> &array) const {
        vector<double> result_array;
        for (double elem: array) {
            double result;
            if (abs(elem) <= _v) {
                result = 1 / lambda * fun_m((elem - theta) / lambda);
            } else {
                result = 1 / lambda * fun_b((elem - theta) / lambda);
            }

            result_array.push_back(result);
        }

        return result_array;
    }

    [[nodiscard]] double check_a() const {
        return PI * tan(PI * _v / 2);
    }

    [[nodiscard]] double check_p() const {
        return (sin(PI * _v) / PI + _v) / check_k(check_a());
    }

    [[nodiscard]] double check_k(const double a) const {
        return sin(PI * _v) * (1 / PI + PI / pow(a, 2)) + _v;
    }

    static double fun_m(const double x) {
        return pow(cos(PI * x / 2), 2);
    }

    [[nodiscard]] double fun_b(const double x) const {
        return pow(cos(PI * _v / 2), 2) * exp(-check_a() * (abs(x) - _v));
    }

    /**
     * Calculate theoretical dispersion
     * @return theoretical dispersion
     */
    [[nodiscard]] double calculate_theoretical_dispersion() const {
        const double a = check_a();
        const double k = check_k(a);
        return (
                   2 / (pow(PI, 3) * k)
                   * (
                       pow(PI * _v, 3) / 6
                       + (pow(PI * _v, 2) / 2 - 1) * sin(PI * _v)
                       + PI * _v * cos(PI * _v)
                   )
                   + 2 * pow(cos(PI * _v / 2), 2) / k * (
                       2 / pow(a, 3)
                       + 2 * _v / pow(a, 2)
                       + pow(_v, 2) / a
                   )
               ) * pow(lambda, 2);
    }

    /**
     * Calculate theoretical excess
     * @return theoretical excess
     */
    [[nodiscard]] double calculate_theoretical_excess() const {
        const double a = check_a();
        const double k = check_k(a);
        const double d = calculate_theoretical_dispersion();
        return 4 / (pow(d, 2) * pow(PI, 5) * k) * (
                   pow(PI * _v, 5) / 20 + (pow(PI * _v, 4) / 4 - 3 * pow(PI * _v, 2) + 6) * sin(PI * _v) + (
                       pow(PI * _v, 3) - 6 * PI * _v) * cos(PI * _v))
               + 2 * pow(cos(PI * _v / 2), 2) / (pow(d, 2) * k) * (
                   24 / pow(a, 5) + 24 * _v / pow(a, 4) + 12 * pow(_v, 2) / pow(a, 3) + 4 * pow(_v, 3) / pow(a, 2) +
                   pow(_v, 4) / a);
    }

    /**
     * Calculate theoretical median
     * @return theoretical median
     */
    [[nodiscard]] double calculate_theoretical_median() const {
        return theta;
    }

    [[nodiscard]] double calculate_expected_value() const {
        return theta;
    }

    /**
     * Calculate theoretical asymmetry
     * @return theoretical asymmetry
     */
    static double theoretical_asymmetry() {
        return 0;
    }
};

#endif
