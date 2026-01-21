#ifndef BITCOINEXCHANGE_HPP
#define BITCOINEXCHANGE_HPP

#include <map>
#include <string>

class BitcoinExchange {
  private:
    std::map<std::string, float> database;

    static bool isLeapYear(int year);
    bool isValidDate(const std::string &date);
    bool isValidValue(float value);
    std::string removeSpaces(const std::string &value);
    void processLine(const std::string &line);

  public:
    // Orthodox Canonical Form
    BitcoinExchange();
    BitcoinExchange(const BitcoinExchange &other);
    BitcoinExchange &operator=(const BitcoinExchange &other);
    ~BitcoinExchange();

    void loadDatabase(const std::string &filename);
    void processInput(const std::string &filename);
};

#endif
