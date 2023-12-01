#include <algorithm>
#include <fstream>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

// could be a struct, just used class for learning
class Value {
private:
    std::string m_Value;

public:
    Value(std::string &s) {
        m_Value = s;
    }
    std::string &Get() {
        return m_Value;
    }
};

// template example
template <typename T>
class ValueHolder {
private:
    T m_Value;

public:
    ValueHolder(T &t) {
        m_Value = t;
    }
    T &Get() {
        return m_Value;
    }
};

main() {

    // read in lines from a text file example
    std::vector<ValueHolder<std::string>> values;
    std::string line;
    std::ifstream input("input.txt");
    if (input.is_open()) {
        while (std::getline(input, line)) {
            ValueHolder<std::string> my_value(line);
            values.push_back(my_value);
        }
        input.close();
    }

    // pair example
    std::pair<int, int> val(1, 10);
    std::cout << val.second << std::endl;

    // Sort with lambda
    std::sort(values.begin(), values.end(), [](auto &a, auto &b) {
        return a.Get().size() > b.Get().size();
    });

    // Gets the sum of the first three values
    int total_chars = std::accumulate(values.begin(), values.begin() + std::min<int>(3, values.size()), 0, [](int sum, auto &a) {
        return sum + a.Get().size();
    });
    std::cout << total_chars << std::endl;

    for (auto &&i : values) {
        std::cout << i.Get() << std::endl;
    }

    // Pointer example
    std::string s = "hello world";
    std::string *pS = &s;
    std::cout << *pS << std::endl;
}