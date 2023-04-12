#include <iostream>
#include <string>
#include <vector>
#include "StringSearcher.cpp"
#include "chrono"
#include <fstream>

// Число попыток
int numOfTry = 20;
// Стартовый индекс
int startIndex = 34;
// Поток вывода в файл
std::ofstream fout;
// Общее число алгоритмов поиска
int numOfAlg = 4;

// Простой чекер, в случае ошибки выводит сообщение об ошибке
// Способен дать ложно положительный ответ
void check(std::string &text, std::string &pattern, std::vector<int> &res) {
    int index = 0;
    while (index != -1) {
        index = text.find(pattern, index + 1);
        if (index != -1 && std::find(res.begin(), res.end(), index) == res.end()) {
            std::cout << "Error!\n";
        }
    }
}

// Метод генерации текста, генерирует текст в зависиомоти от параметров
std::vector<std::string> getText(int len, int a) {
    srand(55);
    std::vector<std::string> ans = std::vector<std::string>(numOfTry);
    for (int i = 0; i < numOfTry; ++i) {
        for (int j = 0; j < len; ++j) {
            ans[i] += std::to_string((rand() % a));
        }
    }
    return ans;
}

// Функция поиска подстроки в строке, принимает указатель на функцию,
// замеряет время, запускает функцию, сверяет результатт ее работы и возвращает ответ
std::vector<int> search(std::string &text, std::string &pattern, std::vector<int64_t> &resultTime,
                        void (*func)(std::string &, std::string &, std::vector<int> &)) {
    std::vector<int> result;
    auto start = std::chrono::high_resolution_clock::now();
    func(text, pattern, result);
    auto elapsed = std::chrono::high_resolution_clock::now() - start;
    int64_t time = std::chrono::duration_cast<std::chrono::nanoseconds>(elapsed).count();
    check(text, pattern, result);
    resultTime.push_back(time);
    return result;
}

// Функция проведения опыта, принимает набор текстов и зпускает их,
// как различные попытки, на разных функциях с разными паттернами
void createResult(std::vector<std::string> texts, int num_quest) {
    std::vector<std::vector<std::vector<int64_t>>> resultTime(numOfAlg, std::vector<std::vector<int64_t>>(3000 / 100));
    std::string pattern;
    std::vector<int> result;
    int n = 0;
    for (int len = 100; len <= 3000; len += 100) {
        for (size_t i = 0; i < texts.size(); ++i) {
            pattern = texts[i].substr(startIndex, len);
            for (int j = 0; j < num_quest; ++j) {
                pattern[rand() % pattern.size()] = '?';
            }
            search(texts[i], pattern, resultTime[0][n], simpleSearch);
            search(texts[i], pattern, resultTime[1][n], kmp);
            search(texts[i], pattern, resultTime[2][n], kmpWithBrs);
            search(texts[i], pattern, resultTime[3][n], rabinKarpSearch);
        }
        n++;
    }
    std::vector<std::vector<int64_t>> middle(4);
    int64_t sum;
    for (int i = 0; i < numOfAlg; ++i) {
        for (int j = 0; j < resultTime[i].size(); ++j) {
            sum = 0;
            for (int k = 0; k < resultTime[i][j].size(); ++k) {
                sum += resultTime[i][j][k];
            }
            middle[i].push_back(sum / resultTime[i][j].size());
        }
    }
    for (int i = 0; i < numOfAlg; ++i) {
        for (int j = 0; j < middle[i].size(); ++j) {
            fout << std::to_string(middle[i][j]) << ";";
        }
        fout << "\n";
    }
}

// Основной метод лабороторной работы, производит генерацию опытов и
// запуск их исполнения в зависимости от параметров
void doLab() {
    auto texts10_2 = getText(10000, 2);
    auto texts10_4 = getText(10000, 4);
    auto texts100_2 = getText(100000, 2);
    auto texts100_4 = getText(100000, 4);

    for (int i = 0; i < 5; ++i) {
        fout << "=====texts10_2_" + std::to_string(i) + "=====\n";
        fout << "\n";
        createResult(texts10_2, i);
        std::cout << rar << "\n" << qrar << "\n" << rar / (double) qrar << "\n";
        rar = 0;
        qrar = 0;
        fout << "\n";
        fout << "=====texts10_4_" + std::to_string(i) + "=====\n";
        createResult(texts10_4, i);
        std::cout << rar << "\n" << qrar << "\n" << rar / (double) qrar << "\n";
        rar = 0;
        qrar = 0;
        fout << "\n";
        fout << "=====texts100_2_" + std::to_string(i) + "=====\n";
        createResult(texts100_2, i);
        std::cout << rar << "\n" << qrar << "\n" << rar / (double) qrar << "\n";
        rar = 0;
        qrar = 0;
        fout << "\n";
        fout << "=====texts100_4_" + std::to_string(i) + "=====\n";
        createResult(texts100_4, i);
        std::cout << rar << "\n" << qrar << "\n" << rar / (double) qrar << "\n";
        rar = 0;
        qrar = 0;
        fout << "\n";
    }
}

// Метод открывает-закрывает поток вывода в файл и
// запускает лабороторную работу.
int main() {
    fout.open("../output.txt");
    doLab();
    fout.close();
    return 0;
}
