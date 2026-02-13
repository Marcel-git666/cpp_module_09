#ifndef PMERGEME_HPP
#define PMERGEME_HPP

#include <deque>
#include <iostream>
#include <vector>

class PmergeMe {
  private:
    std::vector<int> _sequenceVector;
    std::deque<int> _sequenceDeque;
    typedef std::pair<int, int> PairType;

    bool isNumeric(const std::string &str);
    void parseInput(int argc, char **argv);

    template <typename Container>
    void printContainer(Container const &container);

    template <typename Container>
    bool extractStraggler(Container &container, int &val);

    template <typename Container, typename PairContainer>
    void mergeInsertionSort(Container &arr);

    template <typename Container, typename PairContainer>
    PairContainer createPairs(Container &container, Container &mainChain);

    template <typename Container, typename PairContainer>
    void insertPendingElements(Container &mainChain, PairContainer &pairs);

    template <typename Container>
    void binaryInsert(Container &c, int val,
                      typename Container::iterator limit);

    template <typename Container>
    Container generateJacobsthalSequence(int limit);

    template <typename Container> Container buildInsertionSequence(int limit);

  public:
    // Orthodox Canonical Form
    PmergeMe();
    PmergeMe(const PmergeMe &other);
    PmergeMe &operator=(const PmergeMe &other);
    ~PmergeMe();

    void execute(int argc, char **argv);
    const std::vector<int> &getVector() const;
    const std::deque<int> &getDeque() const;
};

#include "PmergeMe.tpp"
#endif
