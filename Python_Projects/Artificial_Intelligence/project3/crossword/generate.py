import sys

from crossword import *


class CrosswordCreator():

    def __init__(self, crossword):
        """
        Create new CSP crossword generate.
        """
        self.crossword = crossword
        self.domains = {
            var: self.crossword.words.copy()
            for var in self.crossword.variables
        }

    def letter_grid(self, assignment):
        """
        Return 2D array representing a given assignment.
        """
        letters = [
            [None for _ in range(self.crossword.width)]
            for _ in range(self.crossword.height)
        ]
        for variable, word in assignment.items():
            direction = variable.direction
            for k in range(len(word)):
                i = variable.i + (k if direction == Variable.DOWN else 0)
                j = variable.j + (k if direction == Variable.ACROSS else 0)
                letters[i][j] = word[k]
        return letters

    def print(self, assignment):
        """
        Print crossword assignment to the terminal.
        """
        letters = self.letter_grid(assignment)
        for i in range(self.crossword.height):
            for j in range(self.crossword.width):
                if self.crossword.structure[i][j]:
                    print(letters[i][j] or " ", end="")
                else:
                    print("█", end="")
            print()

    def save(self, assignment, filename):
        """
        Save crossword assignment to an image file.
        """
        from PIL import Image, ImageDraw, ImageFont
        cell_size = 100
        cell_border = 2
        interior_size = cell_size - 2 * cell_border
        letters = self.letter_grid(assignment)

        # Create a blank canvas
        img = Image.new(
            "RGBA",
            (self.crossword.width * cell_size,
             self.crossword.height * cell_size),
            "black"
        )
        font = ImageFont.truetype("assets/fonts/OpenSans-Regular.ttf", 80)
        draw = ImageDraw.Draw(img)

        for i in range(self.crossword.height):
            for j in range(self.crossword.width):

                rect = [
                    (j * cell_size + cell_border,
                     i * cell_size + cell_border),
                    ((j + 1) * cell_size - cell_border,
                     (i + 1) * cell_size - cell_border)
                ]
                if self.crossword.structure[i][j]:
                    draw.rectangle(rect, fill="white")
                    if letters[i][j]:
                        w, h = draw.textsize(letters[i][j], font=font)
                        draw.text(
                            (rect[0][0] + ((interior_size - w) / 2),
                             rect[0][1] + ((interior_size - h) / 2) - 10),
                            letters[i][j], fill="black", font=font
                        )

                        img.save(filename)

    def solve(self):
        """
        Enforce node and arc consistency, and then solve the CSP.
        """
        self.enforce_node_consistency()
        self.ac3(None)
        return self.backtrack(dict())

    def enforce_node_consistency(self):
        """
        Update `self.domains` such that each variable is node-consistent.
        (Remove any values that are inconsistent with a variable's unary
         constraints; in this case, the length of the word.)
        """
        for variable, words in self.domains.items():
            for word in words.copy():
                if(len(word) != variable.length):
                    words.remove(word)

    def revise(self, x, y):
        """
        Make variable `x` arc consistent with variable `y`.
        To do so, remove values from `self.domains[x]` for which there is no
        possible corresponding value for `y` in `self.domains[y]`.

        Return True if a revision was made to the domain of `x`; return
        False if no revision was made.
        """
        revised = False

        for wordx in self.domains[x].copy():
            validpair = False
            for wordy in self.domains[y]:
                # if there is a single compatible word in y's domain for wordx, wordx can stay in x's domain
                if(self.check_overlap_compatibility(x, y, wordx, wordy)):
                    validpair = True
                    break
            # no words in y's domain were compatible with wordx
            if(validpair == False):
                self.domains[x].remove(wordx)
                revised = True

        return revised

    def check_overlap_compatibility(self, x, y, wordx, wordy):
        """
        use the crossword's overlap dictionary to find the index of their overlap
        and then check if the overlap for the specific words is compatible (the same letter)
        returns true if compatible, false if not
        """
        overlap = self.crossword.overlaps[x, y]
        xindex = overlap[0]
        yindex = overlap[1]
        if (wordx[xindex] == wordy[yindex]):
            return True
        return False

    def ac3(self, arcs):
        """
        Update `self.domains` such that each variable is arc consistent.
        If `arcs` is None, begin with initial list of all arcs in the problem.
        Otherwise, use `arcs` as the initial list of arcs to make consistent.

        Return True if arc consistency is enforced and no domains are empty;
        return False if one or more domains end up empty.
        """
        queue = []
        # if no arcs were provided, add all possible arcs
        if(arcs == None):
            for var1 in self.crossword.variables:
                for var2 in self.crossword.variables:
                    if(var1 != var2):
                        if(self.check_if_neighbor(var1, var2)):
                            queue.append((var1, var2))

        # otherwise use the provided arcs
        else:
            for vartuple in arcs:
                queue.append(vartuple)

        while(queue):
            vartuple = queue.pop()
            x = vartuple[0]
            y = vartuple[1]
            if(self.revise(x, y)):
                if(len(self.domains[x]) == 0):
                    return False
                for z in (self.crossword.neighbors(x) - {y}):
                    queue.append((z, x))

        return True

    def check_if_neighbor(self, x, y):
        """
        checks if var x has an overlap (is a neighbor) with var y
        returns true if neighbors, false if not
        """
        neighborset = self.crossword.neighbors(x)
        if(y in neighborset):
            return True
        return False

    def assignment_complete(self, assignment):
        """
        Return True if `assignment` is complete (i.e., assigns a value to each
        crossword variable); return False otherwise.
        """
        for var in self.crossword.variables:
            try:
                assignment[var]
            except:
                return False

        return True

    def consistent(self, assignment):
        """
        Return True if `assignment` is consistent (i.e., words fit in crossword
        puzzle without conflicting characters); return False otherwise.
        """
        # first check all overlaps for correctness
        for key in assignment.keys():
            for neighbor in self.crossword.neighbors(key):
                # avoids checking against vars that arent yet part of the assignment
                if(neighbor in assignment.keys()):
                    if(not self.check_overlap_compatibility(key, neighbor, assignment[key], assignment[neighbor])):
                        return False

        # now check all assignments for uniqueness
        for key1 in assignment.keys():
            for key2 in assignment.keys() - {key1}:
                if(not self.check_uniqueness(assignment[key1], assignment[key2])):
                    return False

        return True

    def check_uniqueness(self, word1, word2):
        """
        Checks to see if two words are unique or not
        """
        if(word1 == word2):
            return False
        return True

    def order_domain_values(self, var, assignment):
        """
        Return a list of values in the domain of `var`, in order by
        the number of values they rule out for neighboring variables.
        The first value in the list, for example, should be the one
        that rules out the fewest values among the neighbors of `var`.
        """
        # get all the unassigned neighbors
        neighborset = (x for x in self.crossword.neighbors(var)
                       if x not in assignment.keys())

        # choose the domain item that is compatible with the most unsassigned neighbor vars possible
        compatibleCountList = []
        for wordv in self.domains[var]:
            count = 0
            for neighbor in neighborset:
                for wordn in self.domains[neighbor]:
                    if not self.check_overlap_compatibility(var, neighbor, wordv, wordn):
                        count += 1
            compatibleCountList.append((wordv, count))

        # now sort the list by highest count before returning it
        compatibleCountList.sort(key=lambda x: x[1])
        # extract only the words list for the variable
        wordlist = [tup[0] for tup in compatibleCountList]

        return wordlist

    def select_unassigned_variable(self, assignment):
        """
        Return an unassigned variable not already part of `assignment`.
        Choose the variable with the minimum number of remaining values
        in its domain. If there is a tie, choose the variable with the highest
        degree. If there is a tie, any of the tied variables are acceptable
        return values.
        """
        # first get all the unassigned vars
        varlist = [
            var for var in self.crossword.variables if var not in assignment.keys()]
        # now sort by smallest domain and largest degree
        varlist.sort(key=lambda v: (
            len(self.domains[v]), -len(self.crossword.neighbors(v))))
        return varlist[0]

    def backtrack(self, assignment):
        """
        Using Backtracking Search, take as input a partial assignment for the
        crossword and return a complete assignment if possible to do so.

        `assignment` is a mapping from variables (keys) to words (values).

        If no assignment is possible, return None.
        """
        if(self.assignment_complete(assignment)):
            return assignment

        var = self.select_unassigned_variable(assignment)
        for value in self.order_domain_values(var, assignment):
            newAssignment = assignment.copy()
            newAssignment[var] = value
            if(self.consistent(newAssignment)):
                neighborset = self.crossword.neighbors(var)
                queue = []
                for neighbor in neighborset:
                    queue.append((neighbor, var))
                # maintaining arc consistency at every iteration
                self.ac3(queue)
                result = self.backtrack(newAssignment)
                if(result is not None):
                    return result

        return None


def main():

    # Check usage
    if len(sys.argv) not in [3, 4]:
        sys.exit("Usage: python generate.py structure words [output]")

    # Parse command-line arguments
    structure = sys.argv[1]
    words = sys.argv[2]
    output = sys.argv[3] if len(sys.argv) == 4 else None

    # Generate crossword
    crossword = Crossword(structure, words)
    creator = CrosswordCreator(crossword)
    assignment = creator.solve()

    # Print result
    if assignment is None:
        print("No solution.")
    else:
        creator.print(assignment)
        if output:
            creator.save(assignment, output)


if __name__ == "__main__":
    main()
