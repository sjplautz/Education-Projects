import csv
import itertools
import sys

PROBS = {

    # Unconditional probabilities for having gene
    "gene": {
        2: 0.01,
        1: 0.03,
        0: 0.96
    },

    "trait": {

        # Probability of trait given two copies of gene
        2: {
            True: 0.65,
            False: 0.35
        },

        # Probability of trait given one copy of gene
        1: {
            True: 0.56,
            False: 0.44
        },

        # Probability of trait given no gene
        0: {
            True: 0.01,
            False: 0.99
        }
    },

    # Mutation probability
    "mutation": 0.01
}


def main():

    # Check for proper usage
    if len(sys.argv) != 2:
        sys.exit("Usage: python heredity.py data.csv")
    people = load_data(sys.argv[1])

    # Keep track of gene and trait probabilities for each person
    probabilities = {
        person: {
            "gene": {
                2: 0,
                1: 0,
                0: 0
            },
            "trait": {
                True: 0,
                False: 0
            }
        }
        for person in people
    }

    # Loop over all sets of people who might have the trait
    names = set(people)
    for have_trait in powerset(names):

        # Check if current set of people violates known information
        fails_evidence = any(
            (people[person]["trait"] is not None and
             people[person]["trait"] != (person in have_trait))
            for person in names
        )
        # If any of the members in this subset failed then go to next subset
        if fails_evidence:
            continue

        # Loop over all sets of people who might have the gene
        for one_gene in powerset(names):
            for two_genes in powerset(names - one_gene):

                # Update probabilities with new joint probability
                p = joint_probability(people, one_gene, two_genes, have_trait)
                #p = joint_probability(people, set(["Harry"]), set(["James"]), set(["James"]))
                update(probabilities, one_gene, two_genes, have_trait, p)
                #update(probabilities, set(["Harry"]), set(["James"]), set(["James"]), p)

    # Ensure probabilities sum to 1
    normalize(probabilities)

    # Print results
    for person in people:
        print(f"{person}:")
        for field in probabilities[person]:
            print(f"  {field.capitalize()}:")
            for value in probabilities[person][field]:
                p = probabilities[person][field][value]
                print(f"    {value}: {p:.4f}")


def load_data(filename):
    """
    Load gene and trait data from a file into a dictionary.
    File assumed to be a CSV containing fields name, mother, father, trait.
    mother, father must both be blank, or both be valid names in the CSV.
    trait should be 0 or 1 if trait is known, blank otherwise.
    """
    data = dict()
    with open(filename) as f:
        reader = csv.DictReader(f)
        for row in reader:
            name = row["name"]
            data[name] = {
                "name": name,
                "mother": row["mother"] or None,
                "father": row["father"] or None,
                "trait": (True if row["trait"] == "1" else
                          False if row["trait"] == "0" else None)
            }
    return data


def powerset(s):
    """
    Return a list of all possible subsets of set s.
    """
    s = list(s)
    return [
        set(s) for s in itertools.chain.from_iterable(
            itertools.combinations(s, r) for r in range(len(s) + 1)
        )
    ]


def joint_probability(people, one_gene, two_genes, have_trait):
    """
    Compute and return a joint probability.

    The probability returned should be the probability that
        * everyone in set `one_gene` has one copy of the gene, and
        * everyone in set `two_genes` has two copies of the gene, and
        * everyone not in `one_gene` or `two_gene` does not have the gene, and
        * everyone in set `have_trait` has the trait, and
        * everyone not in set` have_trait` does not have the trait.
    """
  
    peopleSet = set(people)
    p = 1.0

    #infer missing subsets
    no_genes = peopleSet - (one_gene | two_genes)
    no_trait = peopleSet - have_trait
    geneSetDictionary = {"no_genes" : frozenset(no_genes), "one_gene" : frozenset(one_gene), "two_genes" : frozenset(two_genes)}

    #calculate joint probability of subset no_genes
    p = p * geneHelper(0, people, no_genes, geneSetDictionary)    
    #calculate joint probability of subset one_gene
    p = p * geneHelper(1, people, one_gene, geneSetDictionary)
    #calculate joint probability of subset two_genes
    p = p * geneHelper(2, people, two_genes, geneSetDictionary)
    #calculate conditional probability of subset no_trait
    p = p * traitHelper(0, no_trait, geneSetDictionary)
    #calculate conditional probability of subset have_trait
    p = p * traitHelper(1, have_trait, geneSetDictionary)

    return p

def geneHelper(numGenes, population, queryNames, geneSetDictionary):
    """
    Computes the probability of the population subset having the specified number of mutated genes 
    """
    p = 1.0

    #if no one applies return
    if(len(queryNames) == 0):
        return p

    for name in queryNames:
        parents = getParentNames(name, population)
        
        #if no parent information can be found use unconditional gene probabilities given
        if(parents is None):
            p = p * PROBS["gene"][numGenes]

        #if parents were found make conditional gene probability calculations
        else:
            motherGeneCount = getGeneCount(parents["mother"], geneSetDictionary)
            fatherGeneCount = getGeneCount(parents["father"], geneSetDictionary)
            pMother = parentProbability(motherGeneCount)
            pFather = parentProbability(fatherGeneCount)

            p = p * calculateGeneCountProbability(numGenes, pMother, pFather)

    return p

def getParentNames(person, population):
    """
    retrieves a persons parent names from the population dictionary if they exist
    returns either None or a new dictionary
    """
    parents = {}
    
    for name, data in population.items():
        if(name == person):
            motherName = data["mother"]
            fatherName = data["father"]
            if(motherName is None):
                return None
            else:
                parents["mother"] = motherName
                parents["father"] = fatherName
                break

    return parents

def getGeneCount(person, geneSetDictionary):
    """
    determines how many genes a person is assumed to have based upon the query information provided
    """
    if person in geneSetDictionary["no_genes"]:
        gene_count = 0
    elif person in geneSetDictionary["one_gene"]:
        gene_count = 1
    else:
        gene_count = 2

    return gene_count


def parentProbability(geneNum):
    """
    determines the probabilty of a parent passing on a mutated gene
    each parent possesses 2 genes with .5 chance of passing either on
    each gene passed on has a .01 chance to mutate to the opposite gene type
    """
    if(geneNum == 0):
        p = 0 + PROBS["mutation"]
    elif(geneNum == 1):
        p = .5
    else:
        p = 1 - PROBS["mutation"]

    return p

def calculateGeneCountProbability(numGenes, pMother, pFather):
    """
    determines probability of parents passing on the specified number of mutated genes given their mutated gene counts
    """
    p = 1.0

    #p(0 genes) = pMother^ & pFather^
    if(numGenes == 0):
        p = p * (1 - pMother) * (1 - pFather)
    #p(1 gene) = pMother^ & pFather || pFather^ & pMother
    elif(numGenes == 1):
        p = p * (1 - pMother) * pFather + (1 - pFather) * pMother
    #p(2 genes) = pMother & pFather
    else:
        p = p * pMother * pFather

    return p

def traitHelper(traitPresence, peopleSubset, geneSetDictionary):
    """
    determines the probability of a trait being present or not in person(s) based upon the geneCount(s) assumed
    """
    p = 1.0

    for person in peopleSubset:
        #determine how many genes we are assuming the person has
        numGenes = getGeneCount(person, geneSetDictionary)
        #calculate conditional probability based on the number of genes assumed, and trait presence assumption
        p = p * PROBS["trait"][numGenes][traitPresence]
 
    return p

def update(probabilities, one_gene, two_genes, have_trait, p):
    """
    Add to `probabilities` a new joint probability `p`.
    Each person should have their "gene" and "trait" distributions updated.
    Which value for each distribution is updated depends on whether
    the person is in `have_gene` and `have_trait`, respectively.
    """
    #derive inferred sets
    peopleSet = set(probabilities)
    no_genes = peopleSet - (one_gene | two_genes)
    no_trait = peopleSet - have_trait

    updateProbabilities("gene", 0, no_genes, probabilities, p)
    updateProbabilities("gene", 1, one_gene, probabilities, p)
    updateProbabilities("gene", 2, two_genes, probabilities, p)
    updateProbabilities("trait", 0, no_trait, probabilities, p)
    updateProbabilities("trait", 1, have_trait, probabilities, p)
    
def updateProbabilities(field, fieldKey, peopleSet, probabilities, p):
    """
    updates the appropriate field of the given peopleSet with the passed in probability
    """
    #if no people to update simply return
    if(len(peopleSet) == 0):
        return

    for person in peopleSet:
        probabilities[person][field][fieldKey] += p

def normalize(probabilities):
    """
    Update `probabilities` such that each probability distribution
    is normalized (i.e., sums to 1, with relative proportions the same).
    """
    for person, data in probabilities.items():
        geneProbabilityTotal = sum(data["gene"].values())
        traitProbabiltyTotal = sum(data["trait"].values())
        for geneCount in data["gene"]:
            currentProbability = data["gene"][geneCount]
            data["gene"][geneCount] = currentProbability/geneProbabilityTotal
        for traitCount in data["trait"]:
            currentProbability = data["trait"][traitCount]
            data["trait"][traitCount] = currentProbability/traitProbabiltyTotal

if __name__ == "__main__":
    main()