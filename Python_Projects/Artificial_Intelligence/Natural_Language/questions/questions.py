import nltk
import re
import sys
import string
import os
import math
from heapq import nlargest

FILE_MATCHES = 1
SENTENCE_MATCHES = 2


def main():

    # Check command-line arguments
    if len(sys.argv) != 2:
        sys.exit("Usage: python questions.py corpus")

    # Calculate IDF values across files
    files = load_files(sys.argv[1])
    file_words = {
        filename: tokenize(files[filename])
        for filename in files
    }
    file_idfs = compute_idfs(file_words)

    # Prompt user for query
    query = set(tokenize(input("Query: ")))

    # Determine top file matches according to TF-IDF
    filenames = top_files(query, file_words, file_idfs, n=FILE_MATCHES)

    # Extract sentences from top files
    sentences = dict()
    for filename in filenames:
        for passage in files[filename].split("\n"):
            for sentence in nltk.sent_tokenize(passage):
                tokens = tokenize(sentence)
                if tokens:
                    sentences[sentence] = tokens

    # Compute IDF values across sentences
    idfs = compute_idfs(sentences)

    # Determine top sentence matches
    matches = top_sentences(query, sentences, idfs, n=SENTENCE_MATCHES)
    for match in matches:
        print(match)


def load_files(directory):
    """
    Given a directory name, return a dictionary mapping the filename of each
    `.txt` file inside that directory to the file's contents as a string.
    """
    corpus = dict()
    documents = os.listdir(directory)
    for document in documents:
        f = open(directory + os.sep + document, "r")
        text = f.read()
        f.close()
        corpus[document] = text

    return corpus


def tokenize(document):
    """
    Given a document (represented as a string), return a list of all of the
    words in that document, in order.

    Process document by coverting all words to lowercase, and removing any
    punctuation or English stopwords.
    """
    wordlist = nltk.word_tokenize(document)

    wordlist = [word.lower() for word in wordlist
                if word.lower() not in nltk.corpus.stopwords.words("english")
                if re.search('[a-zA-Z0-9]', word)
                ]

    return wordlist


def compute_idfs(documents):
    """
    Given a dictionary of `documents` that maps names of documents to a list
    of words, return a dictionary that maps words to their IDF values.

    Any word that appears in at least one of the documents should be in the
    resulting dictionary.
    """
    wordset = set()
    for value in documents.values():
        wordset.update(value)

    idfs = dict()
    for word in wordset:
        idfs[word] = compute_idf(word, documents)

    return idfs


def compute_idf(word, documents):
    """
    compute the idf for a single word
    """
    num_occurs = 0
    for document in documents:
        if(word in documents[document]):
            num_occurs += 1

    return math.log(len(documents)/num_occurs)


def top_files(query, files, idfs, n):
    """
    Given a `query` (a set of words), `files` (a dictionary mapping names of
    files to a list of their words), and `idfs` (a dictionary mapping words
    to their IDF values), return a list of the filenames of the the `n` top
    files that match the query, ranked according to tf-idf.
    """
    totals = dict()
    for word in query:
        for f in files:
            tf_idf = calculate_tf_idf(word, f, files, idfs)
            try:
                totals[f] += tf_idf
            except:
                totals[f] = tf_idf

    # now get the top n filenames from totals
    topN = nlargest(n, totals, key=totals.get)
    return topN


def calculate_tf_idf(word, filename, files, idfs):
    """
    calculate the tf_idf for a single word in a single document
    """
    tf_idf = occurrences(word, filename, files) * idfs[word]
    return tf_idf


def occurrences(word, document, documents):
    """
    count the number of times a word appears in a document
    """
    count = 0
    for string in documents[document]:
        if(word == string):
            count += 1

    return count


def top_sentences(query, sentences, idfs, n):
    """
    Given a `query` (a set of words), `sentences` (a dictionary mapping
    sentences to a list of their words), and `idfs` (a dictionary mapping words
    to their IDF values), return a list of the `n` top sentences that match
    the query, ranked according to idf. If there are ties, preference should
    be given to sentences that have a higher query term density.
    """

    totals = dict()
    for word in query:
        for s in sentences:
            if(word in sentences[s]):
                idf = idfs[word]
                try:
                    totals[s] += idf
                except:
                    totals[s] = idf

    largest = []
    for i in range(n):
        s = extract_largest_sentence(query, sentences, totals)
        largest.extend(s)
        # remove the sentence so the next highest sentence can be extracted next
        del totals[s[0]]

    return largest

def extract_largest_sentence(query, sentences, totals):
    """
    given a dictionary of sentence keys, returns the sentence with the largest value
    in the case of ties, uses term density to determine correct sentence
    """
    largest = []
    maxvalue = max(totals.values())
    for key, value in totals.items():
        if(value == maxvalue):
            largest.append(key)

    if(len(largest) > 1):
        return term_density(query, sentences, largest)

    return largest

def term_density(query, s, l):
    """
    resolves tied sentences using term frequency
    """
    count = 0
    sentences = dict()
    for sentence in l:
        for word in query:
            if word in sentence:
               count += 1
        sentences[sentence] = count/len(s[sentence])    
        count = 0

    return nlargest(1, sentences, key=sentences.get) 

if __name__ == "__main__":
    main()