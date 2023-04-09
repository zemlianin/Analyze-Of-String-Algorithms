#include <iostream>
#include <vector>
#include <string>


std::vector<int> br;
std::vector<int> brs;

void computebr(std::string &pat, int m) {
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
}

void computebrs(int m) {
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
    int m = static_cast<int>(pat.length());
    int n = static_cast<int>(txt.length());
    br = std::vector<int>(m);

    computebr(pat, m);
    computebrs(m);
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
    int m = static_cast<int>(pat.length());
    int n = static_cast<int>(txt.length());
    br = std::vector<int>(m);

    computebr(pat, m);
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

static void rabinKarpSearch(std::string &txt, std::string &pat, std::vector<int> &res) {
    auto q = INT32_MAX;
    int m = static_cast<int>(pat.size());
    int n = static_cast<int>(txt.length());
    int i, j;
    int pattern_hash = 0;
    int text_hash = 0;
    int h = 1;

    for (i = 0; i < m - 1; i++) {
        h = (h * ded) % q;
    }

    for (i = 0; i < m; i++) {
        pattern_hash = (ded * pattern_hash + pat[i]) % q;
        text_hash = (ded * text_hash + txt[i]) % q;
    }

    for (i = 0; i <= n - m; i++) {

        if (pattern_hash == text_hash) {
            for (j = 0; j < m; j++) {
                if (txt[i + j] != pat[j]) {
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