#include <iostream>
#include <vector>
#include <unordered_map>
#include <string>
#include "math.h"

int64_t rar = 0;
int64_t qrar = 0;

void sumbr(std::vector<int> &br) {
    for (int i = 0; i < br.size(); ++i) {
        rar += br[i];
    }
    qrar += br.size();
}

void computebr(std::vector<int> &br, std::string &pat, int m) {
    int len = 0;
    br[0] = 0;
    int i = 1;
    while (i < m) {
        if (pat[i] == pat[len] || pat[i] == '?' || pat[len] == '?') {
            len++;
            br[i] = len;
            i++;
        } else {
            if (len != 0) {
                len = br[len - 1];
            } else {
                br[i] = 0;
                i++;
            }
        }
    }
    sumbr(br);
}

void computebrs(std::vector<int> &br, std::vector<int> &brs, int m) {
    brs = std::vector<int>(m);
    for (size_t i = 0; i < m; ++i) {
        if (i + 1 == m || br[i] >= br[i + 1]) {
            brs[i] = br[i];
        }
    }
}

void simpleSearch(std::string &text, std::string &pattern, std::vector<int> &ans) {
    int n = static_cast<int>(text.length());
    int m = static_cast<int>(pattern.length());
    for (int i = 0; i <= n - m; i++) {
        int j;
        for (j = 0; j < m; j++) {
            if (text[i + j] != pattern[j] && pattern[j] != '?') {
                break;
            }
        }

        if (j == m) {
            ans.push_back(i);
        }
    }
}

void kmpWithBrs(std::string &txt, std::string &pat, std::vector<int> &result) {
    std::vector<int> brs;
    std::vector<int> br;
    int m = static_cast<int>(pat.length());
    int n = static_cast<int>(txt.length());
    br = std::vector<int>(m);

    computebr(br, pat, m);
    computebrs(br, brs, m);
    int i = 0;
    int j = 0;
    int count = 0;

    while (i < n) {
        if (pat[j] == txt[i] || pat[j] == '?') {
            j++;
            i++;
        }

        if (j == m) {
            count++;
            result.push_back(i - j);
            j = brs[j - 1];
        } else if (i < n && pat[j] != txt[i] && pat[j] != '?') {
            if (j != 0) {
                j = brs[j - 1];
            } else {
                i++;
            }
        }
    }
}

void kmp(std::string &txt, std::string &pat, std::vector<int> &result) {
    std::vector<int> br;
    int m = static_cast<int>(pat.length());
    int n = static_cast<int>(txt.length());
    br = std::vector<int>(m);

    computebr(br, pat, m);
    int i = 0;
    int j = 0;
    int count = 0;

    while (i < n) {
        if (pat[j] == txt[i] || pat[j] == '?') {
            j++;
            i++;
        }

        if (j == m) {
            count++;
            result.push_back(i - j);
            j = br[j - 1];
        } else if (i < n && pat[j] != txt[i] && pat[j] != '?') {
            if (j != 0) {
                j = br[j - 1];
            } else {
                i++;
            }
        }
    }
}

#define ded 256

bool checkQues(std::unordered_map<int, int> &ques_indexes, int text_hash, int pattern_hash) {
    if(ques_indexes.empty()){
        return false;
    }
    for (auto it: ques_indexes) {
        if (text_hash == pattern_hash + it.second * '1' ||
            text_hash == pattern_hash + it.second * '0') {
            return true;
        }
    }
    return false;
}

void rabinKarpSearch(std::string &txt, std::string &pat, std::vector<int> &res) {
    auto q = INT32_MAX;
    int m = static_cast<int>(pat.size());
    int n = static_cast<int>(txt.length());
    int i, j;
    int pattern_hash = 0;
    int text_hash = 0;
    int h = 1;
    std::unordered_map<int, int> ques_indexes;

    for (i = 0; i < m; i++) {
        if (pat[i] == '?') {
            ques_indexes[i] = 0;
            pattern_hash = (ded * pattern_hash) % q;
        } else {
            pattern_hash = (ded * pattern_hash + pat[i]) % q;
        }

        text_hash = (ded * text_hash + txt[i]) % q;
    }
    int count = ques_indexes.size();
    for (i = 0; i < m - 1; i++) {
        if (count != 0 && ques_indexes.contains(i)) {
           ques_indexes[i] = h;
           count--;
      }
        h = (h * ded) % q;
    }

    for (i = 0; i <= n - m; i++) {

        if (pattern_hash == text_hash || checkQues(ques_indexes, text_hash, pattern_hash)) {
            for (j = 0; j < m; j++) {
                if (txt[i + j] != pat[j] && pat[j] != '?') {
                    break;
                }
            }

            if (j == m) {
                res.push_back(i);
            }
        }

        if (i < n - m) {
            text_hash = (ded * (text_hash - txt[i] * h) + txt[i + m]) % q;

            if (text_hash < 0) {
                text_hash = (text_hash + q);
            }
        }
    }
}