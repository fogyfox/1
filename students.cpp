#include <iostream>
#include <vector>
#include <string>
#include <thread>
#include <chrono>
#include <random>
#include <mutex>
#include <iomanip>

using namespace std;

struct SessionResult {
    int semester;
    string subject;
    int grade;
};

struct Student {
    string fio;
    string group;
    vector<SessionResult> sessions;
};

long long global_sum = 0;
long long global_count = 0;
mutex mtx;


vector<Student> generateData(int size) {
    vector<Student> students;
    students.reserve(size);
    
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> groupDist(1, 10);
    uniform_int_distribution<> semDist(1, 8);
    uniform_int_distribution<> gradeDist(2, 5);
    
    for(int i = 0; i < size; ++i) {
        Student s;
        s.fio = "Student_" + to_string(i);
        s.group = "G" + to_string(groupDist(gen)); 
        
        int numRecords = 10; 
        for(int j = 0; j < numRecords; ++j) {
            SessionResult sr;
            sr.semester = semDist(gen);
            sr.subject = "Subject_" + to_string(j);
            sr.grade = gradeDist(gen);
            s.sessions.push_back(sr);
        }
        students.push_back(s);
    }
    return students;
}


void processChunk(const vector<Student>& students, int start, int end, 
                  const string& targetGroup, int targetSemester, 
                  long long& localSum, long long& localCount) {
    localSum = 0;
    localCount = 0;
    
    for (int i = start; i < end; ++i) {
        if (students[i].group == targetGroup) {
            for (const auto& session : students[i].sessions) {
                if (session.semester == targetSemester) {
                    localSum += session.grade;
                    localCount++;
                }
            }
        }
    }
}


void worker(const vector<Student>& students, int start, int end, 
           const string& targetGroup, int targetSemester) {
    long long localSum = 0;
    long long localCount = 0;
    
    processChunk(students, start, end, targetGroup, targetSemester, localSum, localCount);
    
    lock_guard<mutex> lock(mtx);
    global_sum += localSum;
    global_count += localCount;
}

int main() { 

    int dataSize;
    int numThreads;
    string targetGroup;
    int targetSemester;

    dataSize = 1000000;
    numThreads = 10; 
    targetGroup = "G1"; 
    targetSemester = 1;

    vector<Student> students = generateData(dataSize);

    cout << "Запуск однопоточной обработки" << endl;
    auto startSingle = chrono::high_resolution_clock::now();
    
    long long singleSum = 0;
    long long singleCount = 0;
    processChunk(students, 0, dataSize, targetGroup, targetSemester, singleSum, singleCount);
    
    auto endSingle = chrono::high_resolution_clock::now();
    chrono::duration<double> durationSingle = endSingle - startSingle;
    
    double avgSingle = (singleCount > 0) ? (double)singleSum / singleCount : 0.0;


    cout << "Запуск многопоточной обработки (" << numThreads << " потоков)" << endl;
    
    global_sum = 0;
    global_count = 0;
    
    vector<thread> threads;
    int chunkSize = dataSize / numThreads;
    
    auto startMulti = chrono::high_resolution_clock::now();
    
    for (int i = 0; i < numThreads; ++i) {
        int start = i * chunkSize;
        int end = (i == numThreads - 1) ? dataSize : (i + 1) * chunkSize;
        
        threads.emplace_back(worker, ref(students), start, end, targetGroup, targetSemester);
    }
    
    for (auto& t : threads) {
        t.join();
    }
    
    auto endMulti = chrono::high_resolution_clock::now();
    chrono::duration<double> durationMulti = endMulti - startMulti;
    
    double avgMulti = (global_count > 0) ? (double)global_sum / global_count : 0.0;


    cout << fixed << setprecision(5);

    cout << "Средняя оценка: " << avgSingle << endl;
    cout << "Время (однопоток): " << durationSingle.count() << " сек" << endl;
    cout << "Время (многопоток):  " << durationMulti.count() << " сек" << endl;

    return 0;
}
