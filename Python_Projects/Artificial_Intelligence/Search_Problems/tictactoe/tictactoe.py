"""
Tic Tac Toe Player
"""

import math
import copy
from itertools import chain

X = "X"
O = "O"
EMPTY = None


def initial_state():
    """
    Returns starting state of the board.
    """
    return [[EMPTY, EMPTY, EMPTY],
            [EMPTY, EMPTY, EMPTY],
            [EMPTY, EMPTY, EMPTY]]


def player(board):
    """
    Returns player who has the next turn on a board.
    """
    countX = 0
    countO = 0

    for row in board:
        for element in row:
            if(element == X):
                countX = countX + 1
            elif(element == O):
                countO = countO + 1

    if(countX <= countO):
        return X
    else:
        return O
    


def actions(board):
    """
    Returns set of all possible actions (i, j) available on the board.
    """
    possibleActions = []
    i = 0
    j = 0
    for element in board:
        for item in element:
            if(item == EMPTY):
                possibleActions.append((i,j))
            j = j + 1
        j = 0
        i = i + 1
    return possibleActions

    

def result(board, action):
    """
    Returns the board that results from making move (i, j) on the board.
    """
    tempBoard = copy.deepcopy(board)
    playerValue = player(board)
    i = action[0]
    j = action[1]

    if(tempBoard[i][j] != EMPTY):
        raise Exception
    else:
        tempBoard[i][j] = playerValue
    return tempBoard


def winner(board):
    """
    Returns the winner of the game, if there is one.
    """
    column1 = []
    column2 = []
    column3 = []
    diagonal1 = []
    diagonal2 = []

    #first check rows
    for row in board:
        if(same(row) and row[0] != EMPTY):
            return row[0]
        else:
            #add items to columns along the way
            column1.append(row[0])
            column2.append(row[1])
            column3.append(row[2])
 
    #append columns and check
    columns = list(chain([column1],[column2],[column3]))
    i=0
    j=2
    for column in columns:
        if(same(column) and column[0] != EMPTY):
            return column[0]
        else:
            #add items to diagonals along the way
            diagonal1.append(column[i])
            diagonal2.append(column[j])
        i = i + 1
        j = j - 1

    #append diagonals and check
    diagonals = list(chain([diagonal1], [diagonal2]))
    for diagonal in diagonals:
        if(same(diagonal) and diagonal[0] != EMPTY):
            return diagonal[0]

    #reaching this point means no one has won
    return None
    

  
def same(aList):
    """
    determines if all the elements in a list are the same
    """
    if(aList[1:] == aList[:-1]):
        return True
    else:
        return False       
        


def terminal(board):
    """
    Returns True if game is over, False otherwise.
    """
    if(not winner(board) is None):
        return True
    elif(full(board)):
        return True
    else:
        return False



def full(board):
    """
    determines if a board is full or not
    """
    for row in board:
        for element in row:
            if(element == EMPTY):
                return False

    #if this point is reached there were no empty spaces
    return True


def utility(board):
    """
    Returns 1 if X has won the game, -1 if O has won, 0 otherwise.
    """
    result = winner(board)
    if(result == X):
        return 1
    elif(result == O):
        return -1
    else:
        return 0

def minimax(board):
    """
    Returns the optimal action for the current player on the board.
    """
    if(terminal(board)):
        return None

    currentPlayer = player(board)
    actionList = actions(board)
    bestAction = ()
    currentValue = 0

    #return the optimal max action for x
    if(currentPlayer == X):
        bestValue = -100000
        for action in actionList:
            revisedBoard = result(board, action)
            currentValue = minValue(revisedBoard)
            if(currentValue > bestValue):
                bestValue = currentValue
                bestAction = action
    #return the optimal min action for O
    else:
        bestValue = 100000
        for action in actionList:
            revisedBoard = result(board, action)
            currentValue = maxValue(revisedBoard)
            if(currentValue < bestValue):
                bestValue = currentValue
                bestAction = action
            
    return bestAction

    
def maxValue(board):
    if(terminal(board)):
        return utility(board)
    else:
        v = -100000
        actionList = actions(board)
        for action in actionList:
            v = max(v, minValue(result(board, action)))
        return v


def minValue(board):
    if(terminal(board)):
        return utility(board)
    else:
        v = 100000
        actionList = actions(board)
        for action in actionList:
            v = min(v, maxValue(result(board, action)))
        return v