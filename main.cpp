#include "calculate.h"
#include "utility.h"
#include "CosinusExponentialWorker.h"

int main() {
    // setlocale(LC_ALL, "Russian");

    const auto worker = CosinusExponentialWorker::from(Utility_io::input());

    const auto sample = worker.sample_gen();
    Utility_io::save_sample(sample);

    const auto f = worker.distribution_fun(sample);
    Utility_io::save_cosexp(f);

    cout << "Параметр формы задан равным " << worker.get_v() << endl << endl;

    cout << endl << worker.check_p() << endl; // ???

    cout << endl << "Среднее арифметическое = " << calculate_mean_value(sample) << endl;
    cout << "Мат ожидание = " << worker.calculate_expected_value() << endl << endl;

    const auto [median, min, max] = median_min_max(sample);
    cout << "Медиана = " << median << endl;
    cout << "Теоретическая медиана = " << worker.calculate_theoretical_median() << endl << endl;

    cout << "Min = " << min << endl;

    cout << "Max = " << max << endl;

    cout << "Дисперсия = " << calculate_dispersion(sample) << endl;
    cout << "Теоретическая дисперсия = " << worker.calculate_theoretical_dispersion() << endl << endl;

    cout << "Коэффициент ассиметрии = " << asymmetry(sample) << endl << endl;
    cout << "Теоретический коэффициент ассиметрии = " << CosinusExponentialWorker::theoretical_asymmetry() << endl;

    cout << "Коэффициент эксцесса = " << calculate_excess(sample) << endl;
    cout << "Теоретический коэффициент эксцесса = " << worker.calculate_theoretical_excess() << endl;
}
