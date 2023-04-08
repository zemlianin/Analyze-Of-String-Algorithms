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