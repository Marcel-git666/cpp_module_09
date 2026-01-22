#include "BitcoinExchange.hpp"
#include <cctype>
#include <cstddef>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>

BitcoinExchange::BitcoinExchange() {}

BitcoinExchange::BitcoinExchange(const BitcoinExchange &other)
    : database(other.database) {}

BitcoinExchange &BitcoinExchange::operator=(const BitcoinExchange &other) {
    if (this != &other) {
        database = other.database;
    }
    return *this;
}

BitcoinExchange::~BitcoinExchange() {}

void BitcoinExchange::loadDatabase(const std::string &filename) {
    std::ifstream file(filename.c_str());

    if (!file.is_open()) {
        throw std::runtime_error("Error: could not open database file.");
    }
    std::string line;
    std::getline(file, line); // skip header
    while (std::getline(file, line)) {
        std::string date;
        float value;
        size_t commaPosition;
        if ((commaPosition = line.find(',')) != std::string::npos) {
            date = line.substr(0, commaPosition);
            value = std::atof(line.substr(commaPosition + 1).c_str());
            database[date] = value;
        }
    }
}

bool BitcoinExchange::isLeapYear(int year) {
    if (year % 400 == 0) {
        return true;
    } else if (year % 100 == 0) {
        return false;
    } else if (year % 4 == 0) {
        return true;
    } else {
        return false;
    }
}

bool BitcoinExchange::isValidDate(const std::string &date) {
    if (date.size() != 10)
        return false;
    if (!std::isdigit(date[0]) || !std::isdigit(date[1]) ||
        !std::isdigit(date[2]) || !std::isdigit(date[3]) || date[4] != '-' ||
        !std::isdigit(date[5]) || !std::isdigit(date[6]) || date[7] != '-' ||
        !std::isdigit(date[8]) || !std::isdigit(date[9]))
        return false;
    int year = std::atoi(date.substr(0, 4).c_str());
    int month = std::atoi(date.substr(5, 2).c_str());
    int day = std::atoi(date.substr(8, 2).c_str());
    if (year < 0 || month < 1 || month > 12 || day < 1 || day > 31)
        return false;
    int daysLimit[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (isLeapYear(year)) {
        daysLimit[2] = 29;
    }
    if (day > daysLimit[month])
        return false;
    return true;
}

bool BitcoinExchange::isValidValue(float value) {
    if (value < 0 || value > 1000)
        return false;
    return true;
}

std::string BitcoinExchange::removeSpaces(const std::string &value) {
    std::string newValue = "";
    for (size_t i = 0; i < value.size(); i++) {
        if (value[i] != ' ')
            newValue += value[i];
    }
    return newValue;
}

void BitcoinExchange::processInput(const std::string &filename) {
    std::ifstream file(filename.c_str());
    if (!file.is_open()) {
        throw std::runtime_error("Error: could not open input file.");
    }

    std::string line;
    std::getline(file, line); // skip header
    while (std::getline(file, line)) {
        processLine(line);
    }
}

void BitcoinExchange::processLine(const std::string &line) {
    size_t pipePosition = line.find('|');

    if (pipePosition == std::string::npos) {
        std::cerr << "Error: bad input => " << line << std::endl;
        return;
    }

    std::string date =
        BitcoinExchange::removeSpaces(line.substr(0, pipePosition));
    std::string valueStr =
        BitcoinExchange::removeSpaces(line.substr(pipePosition + 1));
    float value = std::atof(valueStr.c_str());

    if (!BitcoinExchange::isValidDate(date)) {
        std::cerr << "Error: bad input => " << date << std::endl;
        return;
    }
    if (!BitcoinExchange::isValidValue(value)) {
        if (value < 0)
            std::cerr << "Error: not a positive number." << std::endl;
        else
            std::cerr << "Error: too large a number." << std::endl;
        return;
    }

    std::map<std::string, float>::iterator it = database.lower_bound(date);
    if (it == database.begin() && it->first != date) {
        std::cerr << "Error: date too early => " << date << std::endl;
        return;
    }

    if (it == database.end() || it->first != date) {
        it--;
    }
    float result = value * it->second;

    std::cout << date << " => " << value << " = " << result << std::endl;
}
