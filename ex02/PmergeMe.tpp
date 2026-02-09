
template <typename Container>
void PmergeMe::printContainer(Container const &container) {
    for (size_t i = 0; i < container.size(); i++) {
        if (i < 100) {
            std::cout << container[i] << " ";
        } else {
            std::cout << "[...]";
            break;
        }
    }
}
template <typename Container>
bool PmergeMe::extractStraggler(Container &container, int &val) {
    if (container.size() % 2 != 0) {
        val = container.back();
        container.pop_back();
        return true;
    }
    return false;
}

template <typename Container, typename PairContainer>
PairContainer PmergeMe::createPairs(Container &container,
                                    Container &mainChain) {
    PairContainer pairs;
    for (size_t i = 0; i < container.size(); i += 2) {
        if (container[i] > container[i + 1]) {
            pairs.push_back(std::make_pair(container[i], container[i + 1]));
            mainChain.push_back(container[i]);
        } else {
            pairs.push_back(std::make_pair(container[i + 1], container[i]));
            mainChain.push_back(container[i + 1]);
        }
    }

    return pairs;
}

template <typename Container>
void PmergeMe::binaryInsert(Container &c, int val,
                            typename Container::iterator limit) {
    typename Container::iterator pos = std::upper_bound(c.begin(), limit, val);
    c.insert(pos, val);
}

template <typename Container, typename PairContainer>
void PmergeMe::mergeInsertionSort(Container &arr) {
    if (arr.size() <= 1)
        return;
    int straggler = 0;
    bool hasStraggler = extractStraggler(arr, straggler);

    Container mainChain;
    PairContainer pairs =
        PmergeMe::createPairs<Container, PairContainer>(arr, mainChain);
    mergeInsertionSort<Container, PairContainer>(mainChain);
    insertPendingElements(mainChain, pairs);
    if (hasStraggler) {
        binaryInsert(mainChain, straggler, mainChain.end());
    }
    arr = mainChain;
}

template <typename Container, typename PairContainer>
void PmergeMe::insertPendingElements(Container &mainChain,
                                     PairContainer &pairs) {
    int looser;
    int firstWinner;
    for (size_t i = 0; i < pairs.size(); i++) {
        if (pairs[i].first == mainChain[0]) {
            looser = pairs[i].second;
            firstWinner = pairs[i].first;
            break;
        }
    }
    mainChain.insert(mainChain.begin(), looser);
    for (size_t i = 0; i < pairs.size(); i++) {
        int winner = pairs[i].first;
        int looser = pairs[i].second;

        if (winner == firstWinner) {
            continue;
        }
        typename Container::iterator itWinner =
            std::find(mainChain.begin(), mainChain.end(), winner);

        binaryInsert(mainChain, looser, itWinner);
    }
}
