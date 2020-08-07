class Node():
    def __init__(self, state, parent, action):
        self.state = state
        self.parent = parent
        self.action = action
        
class StackFrontier():
    def __init__(self):
        self.frontier = []
        self.elementSet = set()

    def add(self, node):
        self.frontier.append(node)
        self.elementSet.add(node)

    def containsState(self, state):
        return (state in self.elementSet)

    def empty(self):
        return len(self.frontier) == 0

    def remove(self):
        if self.empty():
            raise Exception("empty frontier")
        else:
            node = self.frontier[-1]
            self.frontier = self.frontier[:-1]
            self.elementSet.remove(node)
            return node

class QueueFrontier(StackFrontier):

    def remove(self):
        if self.empty():
            raise Exception("empty frontier")
        else:
            node = self.frontier[0]
            self.frontier = self.frontier[1:]
            self.elementSet.remove(node)
            return node