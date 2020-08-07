import itertools
import random

class Minesweeper():
    """
    Minesweeper game representation
    """

    def __init__(self, height=8, width=8, mines=8):

        # Set initial width, height, and number of mines
        self.height = height
        self.width = width
        self.mines = set()

        # Initialize an empty field with no mines
        self.board = []
        for i in range(self.height):
            row = []
            for j in range(self.width):
                row.append(False)
            self.board.append(row)

        # Add mines randomly
        while len(self.mines) != mines:
            i = random.randrange(height)
            j = random.randrange(width)
            if not self.board[i][j]:
                self.mines.add((i, j))
                self.board[i][j] = True

        # At first, player has found no mines
        self.mines_found = set()

    def print(self):
        """
        Prints a text-based representation
        of where mines are located.
        """
        for i in range(self.height):
            print("--" * self.width + "-")
            for j in range(self.width):
                if self.board[i][j]:
                    print("|X", end="")
                else:
                    print("| ", end="")
            print("|")
        print("--" * self.width + "-")

    def is_mine(self, cell):
        i, j = cell
        return self.board[i][j]

    def nearby_mines(self, cell):
        """
        Returns the number of mines that are
        within one row and column of a given cell,
        not including the cell itself.
        """

        # Keep count of nearby mines
        count = 0

        # Loop over all cells within one row and column
        for i in range(cell[0] - 1, cell[0] + 2):
            for j in range(cell[1] - 1, cell[1] + 2):

                # Ignore the cell itself
                if (i, j) == cell:
                    continue

                # Update count if cell in bounds and is mine
                if 0 <= i < self.height and 0 <= j < self.width:
                    if self.board[i][j]:
                        count += 1

        return count

    def won(self):
        """
        Checks if all mines have been flagged.
        """
        return self.mines_found == self.mines


class Sentence():
    """
    Logical statement about a Minesweeper game
    A sentence consists of a set of board cells,
    and a count of the number of those cells which are mines.
    """

    def __init__(self, cells, count):
        #memory error linked to this line in init
        self.cells = set(cells)
        self.count = count

    def __eq__(self, other):
        return self.cells == other.cells and self.count == other.count

    def __hash__(self):
        return hash(('cells', tuple(self.cells), 'count', self.count))

    def __str__(self):
        return f"{self.cells} = {self.count}"

    def known_mines(self):
        """
        Returns the set of all cells in self.cells known to be mines.
        """
        if(self.count == len(self.cells)):
            return self.cells
        return None

    def known_safes(self):
        """
        Returns the set of all cells in self.cells known to be safe.
        """
        if(self.count == 0):
            return self.cells
        return None

    def mark_mine(self, cell):
        """
        Updates internal knowledge representation given the fact that
        a cell is known to be a mine.
        """
        if(cell in self.cells):
            self.cells.remove(cell)
            self.count -= 1

    def mark_safe(self, cell):
        """
        Updates internal knowledge representation given the fact that
        a cell is known to be safe.
        """
        if(cell in self.cells):
            self.cells.remove(cell)


class MinesweeperAI():
    """
    Minesweeper game player
    """

    def __init__(self, height, width):

        # Set initial height and width
        self.height = height
        self.width = width

        # Keep track of which cells have been clicked on
        self.moves_made = set()

        # Keep track of cells known to be safe or mines
        self.mines = set()
        self.safes = set()

        # Set of all the possible moves to be made
        self.allMoves = self.generateAllMoves()

        # List of sentences about the game known to be true
        self.knowledge = []

    def mark_mine(self, cell):
        """
        Marks a cell as a mine, and updates all knowledge
        to mark that cell as a mine as well.
        """
        self.mines.add(cell)
        for sentence in self.knowledge:
            sentence.mark_mine(cell)
            if(len(sentence.cells) == 0):
                self.knowledge.remove(sentence)

    def mark_safe(self, cell):
        """
        Marks a cell as safe, and updates all knowledge
        to mark that cell as safe as well.
        """
        self.safes.add(cell)
        for sentence in self.knowledge:
            sentence.mark_safe(cell)
            if(len(sentence.cells) == 0):
                self.knowledge.remove(sentence)

    def add_knowledge(self, cell, count):
        """
        Called when the Minesweeper board tells us, for a given
        safe cell, how many neighboring cells have mines in them.
        """
        #add the cell to the set of moves already made
        self.moves_made.add(cell)
        #mark the cell as being safe
        self.mark_safe(cell)
        #add a sentence representing knowledge of the neighboring cells
        self.add_sentence(cell, count)
        #checks if any sentences are now safe or contain mines
        self.mark_cells()
        #checks if any sentence subset inferences can be made
        self.make_inferences()
        
    def add_sentence(self, cell, count):
        """
        Adds one new sentence to the AI's KB based upon the cell and count given
        """
        #get all the neighboring cells
        neighbors = self.getNeighbors(cell)
        adjustedCount = count

        #now adjust the sentence for information already known
        for cell in neighbors:
            if(cell in self.mines):
                neighbors.remove(cell)
                adjustedCount -= 1
            if(cell in self.safes):
                neighbors.remove(cell)

        if(len(neighbors) == 0):
            return
        else:
            sentence = Sentence(neighbors, adjustedCount)
            self.knowledge.append(sentence)

    def getNeighbors(self, cell):
        """
        Returns the grid of cels surrounding a cell, given that they are part of the board
        """
        neighbors = []

        i = cell[0]
        j = cell[1]
        for a in range(i-1, i+2):
            for b in range(j-1, j+2):
                cell = (a, b)
                if(cell in self.allMoves and cell not in self.moves_made):
                    neighbors.append(cell)

        return neighbors

    def mark_cells(self):
        """
        Checks all of the AI KB sentences,
        determines if simple inferences can mark cells as safe or mines
        """
        for sentence in self.knowledge:
            safeCells = sentence.known_safes()
            mineCells = sentence.known_mines()

            if(not safeCells is None):
                safeCopy = safeCells.copy()
                for cell in safeCopy:
                    self.mark_safe(cell)
            if(not mineCells is None):
                mineCopy = mineCells.copy()
                for cell in mineCopy:
                    self.mark_mine(cell)

            safeCells = None
            mineCells = None

        #removes duplicates in case they were created by inference operations
        self.cleanup_KB()

    def cleanup_KB(self):
        cleanedKnowledgeBase = list(set(self.knowledge))
        self.knowledge = cleanedKnowledgeBase

    def make_inferences(self):
        """
        Checks all of the AI KB sentences,
        determines if subset inferences can be made to create new sentences to add to the KB
        any time we have two sentences set1 = count1 and set2 = count2 where set2 is a subset of set1, 
        then we can construct the new sentence set1 - set2 = count1 - count2. 
        """
        sentences = []

        for sentence1 in self.knowledge:
            for sentence2 in self.knowledge:
                if(sentence1 != sentence2):
                    cells1 = sentence1.cells
                    cells2 = sentence2.cells
                    if(cells2.issubset(cells1)):
                        setDifference = cells1 - cells2
                        countDifference = sentence1.count - sentence2.count
                        newSentence = Sentence(setDifference, countDifference)
                        sentences.append(newSentence)

        self.knowlege = self.knowledge + sentences             

    def make_safe_move(self):
        """
        Returns a safe cell to choose on the Minesweeper board.
        The move must be known to be safe, and not already a move
        that has been made.
        """

        validMoves = self.safes - self.moves_made

        #if there are no safe moves return none
        if(len(validMoves) == 0):
            return None

        else:
            #tuple used to achieve compatability with random.choice
            validMovesTuple = tuple(validMoves)
            safeMove = random.choice(validMovesTuple)
            return safeMove
        
    def make_random_move(self):
        """
        Returns a move to make on the Minesweeper board.
        Should choose randomly among cells that:
            1) have not already been chosen, and
            2) are not known to be mines
        """
        offLimits = set.union(self.mines, self.moves_made)
        possibleMoves = self.allMoves - offLimits

        #any move from this set will do
        possibleMovesTuple = tuple(possibleMoves)
        move = random.choice(possibleMovesTuple)
        return move

    def generateAllMoves(self):
        """
        Generates all possible moves on a minesweeper board with a given height and width
        """
        moves = set()
        for i in range(self.height):
            for j in range(self.width):
                moves.add((i,j))
        return moves

    def printKnowledge(self):
        for sentence in self.knowledge:
            print(sentence)