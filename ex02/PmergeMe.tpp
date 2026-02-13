
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
    size_t firstWinnerIdx = 0; // Tady si uložíme index

    // 1. Najdeme prvek, který patří k nejmenšímu v mainChain
    for (size_t i = 0; i < pairs.size(); i++) {
        if (pairs[i].first == mainChain[0]) {
            looser = pairs[i].second;
            firstWinnerIdx = i; // Uložíme a později použijeme
            break;
        }
    }
    mainChain.insert(mainChain.begin(), looser);

    // 2. Vygenerujeme pořadí indexů
    // Používáme Container, přesně jak jsi chtěl (zachování typu vector/deque)
    Container insertionOrder = buildInsertionSequence<Container>(pairs.size());

    // 3. Iterace podle Jacobsthala
    for (size_t i = 0; i < insertionOrder.size(); i++) {
        int idx = insertionOrder[i]; // Toto je index do 'pairs'

        // Bezpečnostní kontrola
        if (idx >= (int)pairs.size()) continue;

        if ((size_t)idx == firstWinnerIdx) continue;

        int winner = pairs[idx].first;
        int pendingElement = pairs[idx].second;

        // Najdeme pozici vítěze v mainChain
        typename Container::iterator itWinner =
            std::find(mainChain.begin(), mainChain.end(), winner);

        // Binary insert
        binaryInsert(mainChain, pendingElement, itWinner);
    }
}

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

template <typename Container> Container PmergeMe::buildInsertionSequence(int limit) {
    Container jacobSeq = generateJacobsthalSequence<Container>(limit);
    Container zigZag;

    int lastJacob = 0;

    for (size_t i = 3; i < jacobSeq.size(); i++) {
        int currentJacob = jacobSeq[i];

        if (currentJacob > limit) {
            currentJacob = limit;
        }

        for (int j = currentJacob; j > lastJacob; j--) {
            zigZag.push_back(j - 1); // Ukládáme 0-based index
        }

        lastJacob = currentJacob;
        if (lastJacob == limit)
            break;
    }
    while (lastJacob < limit) {
        lastJacob++;
        zigZag.push_back(lastJacob - 1);
    }
    return zigZag;
}
