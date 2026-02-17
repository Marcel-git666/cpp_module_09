#ifndef PMERGEME_HPP
#define PMERGEME_HPP

#include <deque>
#include <iostream>
#include <vector>

class PmergeMe {
  private:
    std::vector<int> _sequenceVector;
    std::deque<int> _sequenceDeque;
    size_t _comparisonCount;

    typedef std::pair<int, int> PairType;

    static bool isNumeric(const std::string &str);
    void parseInput(int argc, char **argv);

    template <typename Container>
    void printContainer(Container const &container);

    template <typename Container>
    bool extractStraggler(Container &container, int &val);

    template <typename Container, typename PairContainer>
    void mergeInsertionSort(Container &arr, int depth = 0);

    template <typename Container, typename PairContainer>
    PairContainer createPairs(Container &container, Container &mainChain);

    template <typename Container, typename PairContainer>
    void insertLosers(Container &mainChain, PairContainer &pairs, bool hasStraggler, int straggler);

    template <typename Container>
    void binaryInsert(Container &c, int val,
                      typename Container::iterator limit);

    template <typename Container>
    Container generateJacobsthalSequence(int limit);

    template <typename Container> Container buildInsertionSequence(int limit);

    struct CountingCompare {
        size_t *counter;
        CountingCompare(size_t *c) : counter(c) {}
        bool operator()(int a, int b) {
            (*counter)++;
            return a < b;
        }
    };

  public:
    // Orthodox Canonical Form
    PmergeMe();
    PmergeMe(const PmergeMe &other);
    PmergeMe &operator=(const PmergeMe &other);
    ~PmergeMe();

    void execute(int argc, char **argv);
};

#include "PmergeMe.tpp"
#endif
