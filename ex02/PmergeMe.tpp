#include <algorithm>

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

// Checks if the container has an odd number of elements.
// If so, removes the last element, stores it in val, and returns true.
template <typename Container>
bool PmergeMe::extractStraggler(Container &container, int &val) {
    if (container.size() % 2 != 0) {
        val = container.back();
        container.pop_back();
        return true;
    }
    return false;
}

// Pairs adjacent elements, putting the larger one as "winner" (first)
// and the smaller as "loser" (second). Winners go into mainChain.
template <typename Container, typename PairContainer>
PairContainer PmergeMe::createPairs(Container &container,
                                    Container &mainChain) {
    PairContainer pairs;
    for (size_t i = 0; i < container.size(); i += 2) {
        _comparisonCount++;
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

// Inserts val into sorted container c using binary search.
// Searches only in range [begin, limit).
template <typename Container>
void PmergeMe::binaryInsert(Container &c, int val,
                            typename Container::iterator limit) {
#ifdef DEBUG
    std::cout << "  binaryInsert val=" << val
              << " range=" << std::distance(c.begin(), limit)
              << "\n";
#endif
    typename Container::iterator pos =
        std::upper_bound(c.begin(), limit, val, CountingCompare(&_comparisonCount));
    c.insert(pos, val);
}

// Core Ford-Johnson merge-insertion sort algorithm.
// 1. Extracts straggler if odd number of elements
// 2. Creates pairs (winner/loser) and builds main chain from winners
// 3. Recursively sorts main chain
// 4. Inserts pending (loser) elements using Jacobsthal ordering
// 5. Inserts straggler at the end if present
template <typename Container, typename PairContainer>
void PmergeMe::mergeInsertionSort(Container &arr, int depth) {
#ifdef DEBUG
    std::cout << "[depth " << depth << "] size: " << arr.size() << " | ";
    printContainer(arr);
    std::cout << "\n";
#endif
    if (arr.size() <= 1)
        return;
    int straggler = 0;
    bool hasStraggler = extractStraggler(arr, straggler);
    Container mainChain;
    PairContainer pairs =
        createPairs<Container, PairContainer>(arr, mainChain);
    mergeInsertionSort<Container, PairContainer>(mainChain, depth + 1);
    insertLosers(mainChain, pairs, hasStraggler, straggler);
    arr = mainChain;
}

// Inserts pending (loser) elements into the sorted main chain.
// 1. Finds and inserts the loser paired with the smallest winner (free insert at position 0)
// 2. Generates Jacobsthal-based insertion order for remaining losers
// 3. For each loser, finds its winner's position and binary-inserts before it
template <typename Container, typename PairContainer>
void PmergeMe::insertLosers(Container &mainChain,
            PairContainer &pairs, bool hasStraggler, int straggler) {
    if (hasStraggler) {
        pairs.push_back(std::make_pair(INT_MAX, straggler));
    }
    if (pairs.empty()) return;

    PairContainer reordered(mainChain.size());
    for (size_t j = 0; j < mainChain.size(); j++) {
        for (size_t i = 0; i < pairs.size(); i++) {
            if (pairs[i].first == mainChain[j]) {
                reordered[j] = pairs[i];
                break;
            }
        }
    }

    if (hasStraggler) {
        for (size_t i = 0; i < pairs.size(); i++) {
            if (pairs[i].first == INT_MAX) {
                reordered.push_back(pairs[i]);
                break;
            }
        }
    }

    mainChain.insert(mainChain.begin(), reordered[0].second);

    std::vector<int> aPos(reordered.size());
    for (size_t k = 0; k < reordered.size(); k++) {
        aPos[k] = static_cast<int>(k + 1);
    }
    if (hasStraggler) {
        aPos[reordered.size() - 1] = static_cast<int>(mainChain.size());
    }

    Container insertionOrder = buildInsertionSequence<Container>(reordered.size());

    for (size_t i = 0; i < insertionOrder.size(); i++) {
        int idx = insertionOrder[i];
        if (static_cast<size_t>(idx) >= reordered.size()) continue;
        if (idx == 0) continue;

        int loser = reordered[idx].second;
        int bound = aPos[idx];

        typename Container::iterator limitIt = mainChain.begin();
        std::advance(limitIt, bound);

        binaryInsert(mainChain, loser, limitIt);

        typename Container::iterator insertedAt =
            std::find(mainChain.begin(), mainChain.end(), loser);
        int insertedPos = static_cast<int>(std::distance(mainChain.begin(), insertedAt));
        for (size_t k = 0; k < aPos.size(); k++) {
            if (aPos[k] >= insertedPos) {
                aPos[k]++;
            }
        }
    }
}

// Generates Jacobsthal sequence up to limit.
// Sequence: 0, 1, 1, 3, 5, 11, 21, 43, ...
// Each number = 2 * pre-previous + previous.
template <typename Container>
Container PmergeMe::generateJacobsthalSequence(int limit) {
    Container jacobSeq;
    jacobSeq.push_back(0);
    jacobSeq.push_back(1);
    while (true) {
        size_t size = jacobSeq.size();
        int last = jacobSeq[size - 1];
        int preLast = jacobSeq[size - 2];
        int next = 2 * preLast + last;
        if (next >= limit)
            break;
        jacobSeq.push_back(next);
    }
    return jacobSeq;
}

// Builds the insertion order for losers using Jacobsthal sequence.
// Instead of inserting 1, 2, 3, 4, 5... it produces e.g. 2, 1, 4, 3, 10, 9, 8, 7, 6, 5...
// Within each Jacobsthal group, elements are inserted in reverse order.
// This minimizes binary search comparisons by keeping search ranges close to powers of 2.
template <typename Container>
Container PmergeMe::buildInsertionSequence(int limit) {
    Container jacobSeq = generateJacobsthalSequence<Container>(limit);
    Container zigZag;
    int lastJacob = 0;
    for (size_t i = 3; i < jacobSeq.size(); i++) {
        int currentJacob = jacobSeq[i];
        if (currentJacob > limit)
            currentJacob = limit;
        for (int j = currentJacob; j > lastJacob; j--) {
            zigZag.push_back(j - 1);
        }
        lastJacob = currentJacob;
        if (lastJacob == limit)
            break;
    }
    for (int j = limit; j > lastJacob; j--) {
        zigZag.push_back(j - 1);
    }
    return zigZag;
}
