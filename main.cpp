#include <iostream>
#include <string>
#include <vector>
#include "StringSearcher.cpp"
#include "chrono"
#include <fstream>

int numOfTry = 20;
int startIndex = 34;
std::ofstream fout;

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

void outputToFile(std::string s) {
    fout << s;
}

std::vector<int> search(std::string &text, std::string &pattern, std::vector<int64_t> &resultTime,
                        void (*func)(std::string &, std::string &, std::vector<int> &)) {
    std::vector<int> result;
    auto start = std::chrono::high_resolution_clock::now();
    func(text, pattern, result);
    auto elapsed = std::chrono::high_resolution_clock::now() - start;
    int64_t time = std::chrono::duration_cast<std::chrono::nanoseconds>(elapsed).count();
    resultTime.push_back(time);
    return result;
}

void createResult(std::vector<std::string> texts) {
    std::vector<std::vector<std::vector<int64_t>>> resultTime(3, std::vector<std::vector<int64_t>>(3000 / 100));
    std::string pattern;
    std::vector<int> result;
    int n = 0;
    for (int len = 100; len <= 3000; len += 100) {
        for (size_t i = 0; i < texts.size(); ++i) {
            //std::vector<int> (*pt2Func)(std::string,std::string,std::vector<int64_t> ) = nullptr;
            //pt2Func = &searcher.simpleSearch;
            pattern = texts[i].substr(startIndex, len);
                search(texts[i], pattern, resultTime[0][n], simpleSearch);
                search(texts[i], pattern, resultTime[1][n], kmp);
                search(texts[i], pattern, resultTime[2][n], kmpWithBrs);
            /* auto start = std::chrono::high_resolution_clock::now();
             simpleSearch(texts[i], pattern, result);
             auto elapsed = std::chrono::high_resolution_clock::now() - start;
             int64_t time = std::chrono::duration_cast<std::chrono::nanoseconds>(elapsed).count();
             resultTime[0][n].push_back(time);
             result = std::vector<int>();

             start = std::chrono::high_resolution_clock::now();
             kmp(texts[i], pattern, result);
             elapsed = std::chrono::high_resolution_clock::now() - start;
             time = std::chrono::duration_cast<std::chrono::nanoseconds>(elapsed).count();
             resultTime[1][n].push_back(time);
             result = std::vector<int>();

             start = std::chrono::high_resolution_clock::now();
             kmpWithBrs(texts[i], pattern, result);
             elapsed = std::chrono::high_resolution_clock::now() - start;
             time = std::chrono::duration_cast<std::chrono::nanoseconds>(elapsed).count();
             resultTime[2][n].push_back(time);*/
        }
        n++;
    }
    std::vector<std::vector<int64_t>> middle(3);
    int64_t sum;
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < resultTime[i].size(); ++j) {
            sum = 0;
            for (int k = 0; k < resultTime[i][j].size(); ++k) {
                sum += resultTime[i][j][k];
            }
            middle[i].push_back(sum / resultTime[i][j].size());
        }
    }
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < middle[i].size(); ++j) {
            fout << std::to_string(middle[i][j]) << " ";
        }
        fout << "\n";
    }
}

void doLab() {
    auto texts10_2 = getText(10000, 2);
    auto texts10_4 = getText(10000, 4);
    auto texts100_2 = getText(100000, 2);
    auto texts100_4 = getText(100000, 4);
    fout << "=====texts10_2====\n";
    createResult(texts10_2);
    fout << "=====texts10_4====\n";
    createResult(texts10_4);
    fout << "=====texts100_2====\n";
    createResult(texts100_2);
    fout << "=====texts100_4====\n";
    createResult(texts100_4);
}

int main() {
    fout.open("../output.txt");
    doLab();
    fout.close();
    return 0;
}
