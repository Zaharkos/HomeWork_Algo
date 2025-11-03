#pragma once

#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>

struct Student {
    std::string m_name;
    std::string m_surname;
    std::string m_email;
    int m_birth_year;
    int m_birth_month;
    int m_birth_day;
    std::string m_group;
    float m_rating;
    std::string m_phone_number;
};

inline std::vector<Student> loadStudents(const std::string& filename) {
    std::vector<Student> students;
    std::ifstream file(filename);
    std::string line;

    if (!file.is_open()) {
        std::cerr << "Could not open file: " << filename << std::endl;
        return students;
    }

    std::getline(file, line);

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string field;
        Student s;

        std::getline(ss, s.m_name, ',');
        std::getline(ss, s.m_surname, ',');
        std::getline(ss, s.m_email, ',');
        std::getline(ss, field, ',');
        s.m_birth_year = std::stoi(field);
        std::getline(ss, field, ',');
        s.m_birth_month = std::stoi(field);
        std::getline(ss, field, ',');
        s.m_birth_day = std::stoi(field);
        std::getline(ss, s.m_group, ',');
        std::getline(ss, field, ',');
        s.m_rating = std::stof(field);
        std::getline(ss, s.m_phone_number, ',');
        
        students.push_back(s);
    }
    file.close();
    return students;
}

inline void saveToCSV(const std::vector<Student>& students, const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Could not open file for writing: " << filename << std::endl;
        return;
    }

    file << "name,surname,email,birth_year,birth_month,birth_day,group,rating,phone\n";
    for (const auto& s : students) {
        file << s.m_name << "," << s.m_surname << "," << s.m_email << ","
             << s.m_birth_year << "," << s.m_birth_month << "," << s.m_birth_day << ","
             << s.m_group << "," << s.m_rating << "," << s.m_phone_number << "\n";
    }
    file.close();
}