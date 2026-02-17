#include "PmergeMe.hpp"
#include <cctype>
#include <climits> 
#include <cstdlib>
#include <iostream>
#include <set>
#include <stdexcept>
#include <sys/time.h>

// Orthodox Canonical Form
PmergeMe::PmergeMe() : _comparisonCount(0) {}

PmergeMe::PmergeMe(const PmergeMe &other)
    : _sequenceVector(other._sequenceVector),
      _sequenceDeque(other._sequenceDeque),
      _comparisonCount(other._comparisonCount) {}

PmergeMe &PmergeMe::operator=(const PmergeMe &other) {
    if (this != &other) {
        _sequenceVector = other._sequenceVector;
        _sequenceDeque = other._sequenceDeque;
        _comparisonCount = other._comparisonCount;
    }
    return *this;
}

PmergeMe::~PmergeMe() {}

// Returns true if the string contains only digit characters.
bool PmergeMe::isNumeric(const std::string &str) {
    for (size_t i = 0; i < str.size(); i++) {
        if (!isdigit(str[i])) {
            return false;
        }
    }
    return true;
}

// Parses command-line arguments into both containers.
// Validates: non-empty, numeric, positive, within INT_MAX, no duplicates.
void PmergeMe::parseInput(int argc, char **argv) {
    if (argc == 1) {
        throw std::runtime_error("Usage: ./PmergeMe 3 5 9 7 4");
    }
    std::set<int> duplicateCheck;
    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        if (arg.empty()) {
            throw std::runtime_error("Error");
        }
        if (!isNumeric(arg)) {
            throw std::runtime_error("Error");
        }
        long number = std::atol(arg.c_str());
        if (number > INT_MAX || number <= 0) {
            throw std::runtime_error("Error");
        }
        if (duplicateCheck.insert(static_cast<int>(number)).second == false) {
            throw std::runtime_error("Error");
        }

        _sequenceVector.push_back(static_cast<int>(number));
        _sequenceDeque.push_back(static_cast<int>(number));
    }
}

// Main entry point: parses input, sorts with both containers, measures and prints results.
void PmergeMe::execute(int argc, char **argv) {
    try {
        PmergeMe::parseInput(argc, argv);
    } catch (const std::exception &e) {
        std::cerr << e.what() << '\n';
        return;
    }
    std::cout << "Before: ";
    PmergeMe::printContainer(_sequenceVector);
    std::cout << "\n";
    struct timeval start_time;
    struct timeval end_time;

    gettimeofday(&start_time, NULL);
    typedef std::vector<int> VecType;
    typedef std::vector<PairType> VecPairType;
    _comparisonCount = 0;
    mergeInsertionSort<VecType, VecPairType>(_sequenceVector);

    gettimeofday(&end_time, NULL);
    double measurement1;
    double measurement2;

    measurement1 = (end_time.tv_sec - start_time.tv_sec) * 1000000;
    measurement1 += end_time.tv_usec - start_time.tv_usec;

    gettimeofday(&start_time, NULL);

#ifdef DEBUG
    std::cout << "Comparisons (vector): " << _comparisonCount << "\n";
#endif

    // Sorting 2 comes here;

    typedef std::deque<int> DequeType;
    typedef std::deque<PairType> DequePairType;
    _comparisonCount = 0;
    mergeInsertionSort<DequeType, DequePairType>(_sequenceDeque);
    gettimeofday(&end_time, NULL);
    measurement2 = (end_time.tv_sec - start_time.tv_sec) * 1000000;
    measurement2 += end_time.tv_usec - start_time.tv_usec;
#ifdef DEBUG
    std::cout << "Comparisons (deque): " << _comparisonCount << "\n";
#endif

    std::cout << "After:  ";
    PmergeMe::printContainer(_sequenceVector);
    std::cout << "\n";
    std::cout << "Time to process a range of " << _sequenceVector.size()
              << " elements with std::vector : " << measurement1 << " us\n";
    std::cout << "Time to process a range of " << _sequenceVector.size()
              << " elements with std::deque : " << measurement2 << " us\n";
}
