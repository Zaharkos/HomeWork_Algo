#pragma once

#include "student.h"
#include <unordered_map>
#include <vector>
#include <string>
#include <map>

// --- Структура для хешування пар (місяць, день) ---
struct hash_pair {
    template <class T1, class T2>
    size_t operator()(const std::pair<T1, T2>& p) const {
        auto hash1 = std::hash<T1>{}(p.first);
        auto hash2 = std::hash<T2>{}(p.second);
        return hash1 ^ (hash2 << 1);
    }
};

// --- РІШЕННЯ 1: "Базове" (std::vector) ---
class Database_Vector {
private:
    std::vector<Student> m_students;

public:
    Database_Vector(const std::vector<Student>& students) : m_students(students) {}

    std::vector<Student*> op1_findByBirthday(int month, int day) {
        std::vector<Student*> result;
        for (Student& s : m_students) {
            if (s.m_birth_month == month && s.m_birth_day == day) {
                result.push_back(&s);
            }
        }
        return result;
    }

    bool op2_updateGroupByEmail(const std::string& email, const std::string& newGroup) {
        for (Student& s : m_students) {
            if (s.m_email == email) {
                s.m_group = newGroup;
                return true;
            }
        }
        return false;
    }

    std::string op3_findGroupMaxSharedBirthday() {
        std::map<std::string, std::map<std::pair<int, int>, int>> groupCounts;
        for (const auto& s : m_students) {
            groupCounts[s.m_group][{s.m_birth_month, s.m_birth_day}]++;
        }

        std::string maxGroup;
        int maxCount = 0;
        for (const auto& groupPair : groupCounts) {
            for (const auto& birthdayPair : groupPair.second) {
                if (birthdayPair.second > maxCount && birthdayPair.second > 1) {
                    maxCount = birthdayPair.second;
                    maxGroup = groupPair.first;
                }
            }
        }
        return maxGroup;
    }
};

// --- РІШЕННЯ 2: "Оптимізація Email" (vector + email_map) ---
class Database_EmailMap {
private:
    std::vector<Student> m_students;
    std::unordered_map<std::string, Student*> m_emailMap;

public:
    Database_EmailMap(const std::vector<Student>& students) : m_students(students) {
        for (Student& s : m_students) {
            m_emailMap[s.m_email] = &s;
        }
    }

    std::vector<Student*> op1_findByBirthday(int month, int day) {
        std::vector<Student*> result;
        for (Student& s : m_students) {
            if (s.m_birth_month == month && s.m_birth_day == day) {
                result.push_back(&s);
            }
        }
        return result;
    }

    bool op2_updateGroupByEmail(const std::string& email, const std::string& newGroup) {
        if (m_emailMap.count(email)) {
            m_emailMap.at(email)->m_group = newGroup;
            return true;
        }
        return false;
    }

    std::string op3_findGroupMaxSharedBirthday() {
        // Логіка та сама, що у Рішенні 1
        std::map<std::string, std::map<std::pair<int, int>, int>> groupCounts;
        for (const auto& s : m_students) {
            groupCounts[s.m_group][{s.m_birth_month, s.m_birth_day}]++;
        }
        std::string maxGroup;
        int maxCount = 0;
        for (const auto& groupPair : groupCounts) {
            for (const auto& birthdayPair : groupPair.second) {
                if (birthdayPair.second > maxCount && birthdayPair.second > 1) {
                    maxCount = birthdayPair.second;
                    maxGroup = groupPair.first;
                }
            }
        }
        return maxGroup;
    }
};

// --- РІШЕННЯ 3: "Повна Оптимізація" (vector + 2 maps) ---
class Database_Optimized {
private:
    std::vector<Student> m_students;
    std::unordered_map<std::string, Student*> m_emailMap;
    std::unordered_map<std::pair<int, int>, std::vector<Student*>, hash_pair> m_birthdayMap;

public:
    Database_Optimized(const std::vector<Student>& students) {
        m_students = students;
        for (Student& s : m_students) {
            m_emailMap[s.m_email] = &s;
            m_birthdayMap[{s.m_birth_month, s.m_birth_day}].push_back(&s);
        }
    }

    std::vector<Student*> op1_findByBirthday(int month, int day) {
        if (m_birthdayMap.count({month, day})) {
            return m_birthdayMap.at({month, day});
        }
        return {};
    }

    bool op2_updateGroupByEmail(const std::string& email, const std::string& newGroup) {
        if (m_emailMap.count(email)) {
            m_emailMap.at(email)->m_group = newGroup;
            return true;
        }
        return false;
    }

    std::string op3_findGroupMaxSharedBirthday() {
        std::unordered_map<std::string, std::unordered_map<std::pair<int, int>, int, hash_pair>> groupCounts;
        for (const auto& s : m_students) {
            groupCounts[s.m_group][{s.m_birth_month, s.m_birth_day}]++;
        }

        std::string maxGroup;
        int maxCount = 0;

        for (const auto& groupPair : groupCounts) {
            for (const auto& birthdayPair : groupPair.second) {
                if (birthdayPair.second > maxCount && birthdayPair.second > 1) {
                    maxCount = birthdayPair.second;
                    maxGroup = groupPair.first;
                }
            }
        }
        return maxGroup;
    }

    std::vector<Student>& getAllStudents() {
        return m_students;
    }
};