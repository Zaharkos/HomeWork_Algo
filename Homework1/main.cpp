#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include <random> 

#include "student.h"
#include "database.h"
#include "sorting.h"

template<typename DB_Type>
void run_v2_test(const std::vector<Student>& students, const std::vector<std::string>& emails) {

    DB_Type db(students);
    long long op_count = 0;

    const int RATIO_SUM = 106;
    const int RATIO_A = 100;
    const int RATIO_B = 5;

    int email_index = 0;
    int N = emails.size();

    auto start_time = std::chrono::high_resolution_clock::now();

    while (true) {
        auto now = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::seconds>(now - start_time);
        if (duration.count() >= 10) {
            break;
        }

        int r = rand() % RATIO_SUM;

        if (r < RATIO_A) {
            int rand_month = (rand() % 12) + 1;
            int rand_day = (rand() % 28) + 1;
            db.op1_findByBirthday(rand_month, rand_day);

        } else if (r < RATIO_A + RATIO_B) {
            db.op2_updateGroupByEmail(emails[email_index], "NEW-GR");
            email_index = (email_index + 1) % N;

        } else {
            db.op3_findGroupMaxSharedBirthday();
        }

        op_count++;
    }

    std::cout << op_count << " operations\n";
}


int main() {
    std::cout << "Loading ALL students from students.csv..." << std::endl;
    std::vector<Student> all_students = loadStudents("students.csv");
    if (all_students.empty()) {
        return 1;
    }
    std::cout << "Loaded " << all_students.size() << " students." << std::endl;

    std::vector<std::string> all_emails;
    for(const auto& s : all_students) {
        all_emails.push_back(s.m_email);
    }

    std::cout << "\n--- Running Task II (V2) 10-second tests ---" << std::endl;

    std::vector<int> test_sizes = {100, 1000, 10000, 100000};

    for (int N : test_sizes) {
        if (N > all_students.size()) break;

        std::cout << "\n--- Testing for N = " << N << " students ---" << std::endl;

        std::vector<Student> test_data(all_students.begin(), all_students.begin() + N);
        std::vector<std::string> test_emails(all_emails.begin(), all_emails.begin() + N);

        std::cout << "  1. Solution 1 (Vector):   ";
        run_v2_test<Database_Vector>(test_data, test_emails);

        std::cout << "  2. Solution 2 (EmailMap): ";
        run_v2_test<Database_EmailMap>(test_data, test_emails);

        std::cout << "  3. Solution 3 (Optimized):";
        run_v2_test<Database_Optimized>(test_data, test_emails);
    }

    std::cout << "\n--- Performing Sorting (S5) on N=100000 ---" << std::endl;

    std::vector<Student> students_std = all_students;
    std::vector<Student> students_radix = all_students;

    auto start_std = std::chrono::high_resolution_clock::now();
    SortTask::sort_std(students_std);
    auto end_std = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> time_std = end_std - start_std;
    std::cout << "1. std::sort time: " << time_std.count() << " ms" << std::endl;

    auto start_radix = std::chrono::high_resolution_clock::now();
    SortTask::sort_radix(students_radix);
    auto end_radix = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> time_radix = end_radix - start_radix;
    std::cout << "2. Radix Sort time: " << time_radix.count() << " ms" << std::endl;

    saveToCSV(students_std, "students_sorted_s5.csv");
    std::cout << "Saved sorted results to students_sorted_s5.csv" << std::endl;

    return 0;
}
