#pragma once

#include "student.h"
#include <algorithm>
#include <vector>
#include <string>
#include <map>

namespace SortTask {

    inline void sort_std(std::vector<Student>& students) {
        std::sort(students.begin(), students.end(), [](const Student& a, const Student& b) {
            return a.m_group < b.m_group;
        });
    }

    inline void sort_radix(std::vector<Student>& students) {
        int n = students.size();
        if (n == 0) return;

        int maxLen = 6;
        std::vector<Student> output(n);

        for (int pos = maxLen - 1; pos >= 0; --pos) {
            std::vector<int> count(256, 0);

            for (int i = 0; i < n; ++i) {
                count[static_cast<unsigned char>(students[i].m_group[pos])]++;
            }

            for (int i = 1; i < 256; ++i) {
                count[i] += count[i - 1];
            }

            for (int i = n - 1; i >= 0; --i) {
                output[count[static_cast<unsigned char>(students[i].m_group[pos])] - 1] = students[i];
                count[static_cast<unsigned char>(students[i].m_group[pos])]--;
            }

            for (int i = 0; i < n; ++i) {
                students[i] = output[i];
            }
        }
    }
}