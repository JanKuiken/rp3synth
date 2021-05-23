#ifndef LOGGER_H
#define LOGGER_H

#include <vector>
#include <sstream>
#include <fstream>
#include <string>
#include <iostream>  // TODO: moet weer weg

template<class T>
class Logger
{
public:
    Logger(std::string in_name, std::size_t size) {
        // std::cout << "Logger " << in_name << std::endl;
        static int obj_counter = 0;
        obj_counter++;
        id = obj_counter;
        name = in_name;
        data.reserve(size);
    }

    ~Logger() {
        // std::cout << "Logger destructor" << data.size() << std::endl;
        if (data.size() > 0) {
            std::stringstream ss;
            ss << name << "_" << id << ".log";
            std::ofstream tmpfile = std::ofstream(ss.str());
            for (T t: data) {
                tmpfile << t << "\n";
            }
            tmpfile.close();
        }
    }

    void Log(const T &t) {
        if (data.size() < data.capacity()) data.push_back(T(t));
    }

private:
    int id;
    std::string name;
    std::vector<T> data;
};

#endif // LOGGER_H
