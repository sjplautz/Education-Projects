import os
import random
import re
import sys
import numpy as np

DAMPING = 0.85
SAMPLES = 10000
DELTA = .001


def main():
    if len(sys.argv) != 2:
        sys.exit("Usage: python pagerank.py corpus")
    corpus = crawl(sys.argv[1])
    ranks = sample_pagerank(corpus, DAMPING, SAMPLES)
    print(f"PageRank Results from Sampling (n = {SAMPLES})")
    for page in sorted(ranks):
        print(f"  {page}: {ranks[page]:.4f}")
    print(f"  Total pageweights: {sum(list(ranks.values())):.3f}")
    ranks = iterate_pagerank(corpus, DAMPING)
    normalize(ranks)
    print(f"PageRank Results from Iteration")
    for page in sorted(ranks):
        print(f"  {page}: {ranks[page]:.4f}")
    print(f"  Total pageweights: {sum(list(ranks.values())):.3f}")

def crawl(directory):
    """
    Parse a directory of HTML pages and check for links to other pages.
    Return a dictionary where each key is a page, and values are
    a list of all other pages in the corpus that are linked to by the page.
    """
    pages = dict()

    # Extract all links from HTML files
    for filename in os.listdir(directory):
        if not filename.endswith(".html"):
            continue
        with open(os.path.join(directory, filename)) as f:
            contents = f.read()
            links = re.findall(r"<a\s+(?:[^>]*?)href=\"([^\"]*)\"", contents)
            pages[filename] = set(links) - {filename}

    # Only include links to other pages in the corpus
    for filename in pages:
        pages[filename] = set(
            link for link in pages[filename]
            if link in pages
        )

    return pages


def transition_model(corpus, page, damping_factor):
    """
    Return a probability distribution over which page to visit next,
    given a current page.

    With probability `damping_factor`, choose a link at random
    linked to by `page`. With probability `1 - damping_factor`, choose
    a link at random chosen from all pages in the corpus.
    """

    corpusSize = len(corpus)
    linkedPages = corpus[page]
    linkCount = len(linkedPages)

    #in the event there are no linked pages choose from all pages
    if(linkCount == 0):
        probabilityDistribution = dict.fromkeys(corpus.keys(), 1/corpusSize)
        return probabilityDistribution
    
    #otherwise choose only from linked pages with appropriate probability
    else:
        probabilityDistribution = dict.fromkeys(corpus.keys(), 0)   
        linkProbability = damping_factor / linkCount
        baseProbability = (1-damping_factor) / corpusSize

        for memberPage in corpus:
            probabilityDistribution[memberPage] += baseProbability
            if(memberPage in linkedPages):
                probabilityDistribution[memberPage] += linkProbability

        return probabilityDistribution


def sample_pagerank(corpus, damping_factor, n):
    """
    Return PageRank values for each page by sampling `n` pages
    according to transition model, starting with a page at random.

    Return a dictionary where keys are page names, and values are
    their estimated PageRank value (a value between 0 and 1). All
    PageRank values should sum to 1.
    """
    pageRanks = dict.fromkeys(corpus.keys(), 0)
    pageKeys = list(corpus.keys())
    randomValue = random.randrange(0, len(pageKeys)-1)    
    randomPage = pageKeys[randomValue]

    pageRanks[randomPage] += 1/n
    probabilityDistribution = transition_model(corpus, randomPage, damping_factor)

    #cumulates page visits for each page in the corpus
    for i in range(n):
        nextPage = getProbablePage(probabilityDistribution)
        pageRanks[nextPage] += 1/n
        probabilityDistribution = transition_model(corpus, nextPage, damping_factor)

    return pageRanks

def getProbablePage(probabilityDistribution):
    """
    Returns a page from a set of pages according to the probability
    value assigned to each page
    """
    #will have to ensure keys are being matched with correct probs
    keys = list(probabilityDistribution.keys())
    probabilities = list(probabilityDistribution.values())
    pageSample = np.random.choice(keys, None, True, probabilities)

    return pageSample


def iterate_pagerank(corpus, damping_factor):
    """
    Return PageRank values for each page by iteratively updating
    PageRank values until convergence.

    Return a dictionary where keys are page names, and values are
    their estimated PageRank value (a value between 0 and 1). All
    PageRank values should sum to 1.
    """
    
    #first assign each page a rank of 1/N
    numPages = len(corpus.keys())
    pageRanks = dict.fromkeys(corpus.keys(), 1/numPages)

    #continue iterating until the largest change due to recalculation 
    #is smaller than the threshold
    largestDifference = 1
    while(largestDifference > DELTA):
        largestDifference = recalculate(pageRanks, corpus, damping_factor)

    return pageRanks

def recalculate(pageRanks, corpus, damping_factor):
    """
    Recalculates the pageranks based upon the Iterative algorithm provided
    """
    largestDifference = 0
    numPages = len(corpus.keys())
    baseProbability = (1 - damping_factor)/numPages
    temporaryDict = dict()

    for page in pageRanks:
        probability = baseProbability + incomingProbability(page, corpus, pageRanks, damping_factor)
        temporaryDict[page] = probability

        #used to determine if we have converged or not
        currentDifference = abs(pageRanks[page] - probability)
        if(currentDifference > largestDifference):
            largestDifference = currentDifference

    #now apply updates after loop to maintain normalization
    for member in temporaryDict:
        pageRanks[member] = temporaryDict[member]

    return largestDifference

def incomingProbability(page, corpus, pageRanks, damping_factor):
    """
    Returns the probability of landing on a page from the incoming links to it
    """
    probability = 0
    linkingPages = getPagesLinkingToPage(page, corpus)

    for memberPage in linkingPages:
        numLinks = len(corpus[memberPage])
        probability += (pageRanks[memberPage])/numLinks

    return damping_factor * probability

def getPagesLinkingToPage(page, corpus):
    """
    Returns a list of all the pages that link to a given page
    """
    linkingPages = []

    for memberPage in corpus:
        if(page in corpus[memberPage]):
            linkingPages.append(memberPage)

    return linkingPages

def normalize(inputDictionary):
    """
    normalizes the results of assigning probability distribution in the case of a sink node
    """
    values = list(inputDictionary.values())
    baseSum = sum(values)
    for page, value in inputDictionary.items():
        newValue = value/baseSum
        inputDictionary[page] = newValue

if __name__ == "__main__":
    main()