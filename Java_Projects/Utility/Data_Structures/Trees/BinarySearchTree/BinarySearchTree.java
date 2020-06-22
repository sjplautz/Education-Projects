package trees;

import java.util.LinkedList;
import javax.swing.RowFilter.ComparisonType;
import sun.misc.Queue;

public class BinarySearchTree<T extends Comparable> {
    private Node root;
    private long size;

    public BinarySearchTree() {
        this.root = null;
        this.size = 0;
    }

    public BinarySearchTree(N root) {
        this.root = root;
        this.size = 0;
    }

    public Node getRoot() {
        return this.root;
    }

    public long getSize() {
        return this.size;
    }

    public void insert(T input) {
        if (this.root == null) {
            this.root = new Node(input);
            this.size++;
        } else
            insertHelper(input, this.root);
    }

    private void insertHelper(T input, Node current) {
        int result = input.compareTo(current.getKey());

        // duplicate key
        if (result == 0)
            return;

        // input less than current key goes left
        else if (result < 0) {
            if (current.getLeft() != null)
                insertHelper(input, current.getLeft());
            else {
                current.setLeft(new Node(input, current));
                this.size++;
            }
        }

        // input greater than currrent key goes right
        else {
            if (current.getRight() != null)
                insertHelper(input, current.getRight());
            else {
                current.setRight(new Node(input, current));
                this.size++;
            }
        }
    }

    // public interface into deleting a node from the bst
    public void delete(T input) {
        findNode(input, this.root);
    }

    // searches the binary tree for the given node, delegates deletion to remove()
    private void findNode(T input, Node current) {
        int result = input.compareTo(current.getKey());

        if (result == 0)
            remove(current);
        else if (result < 0) {
            if (current.getLeft() != null)
                findNode(input, current.getLeft());
            else
                System.out.println("Error, value: " + input + " not found");
        } else {
            if (current.getRight() != null)
                findNode(input, current.getRight());
            else
                System.out.println("Error, value: " + input + " not found");
        }
    }

    // handles the actual deletion of the node
    private void remove(Node current) {
        if (isLeaf(current))
            destroy(current);
        else if (isHalf(current)) {
            if (current.getRight() != null) {
                Node balancer = current.getRight();
                Node parent = current.getParent();
                balancer.setParent(parent);
                if(parent.getLeft() != null && parent.getLeft().getKey().compareTo(current.getKey()) == 0)
                    parent.setLeft(balancer);
                else
                    parent.setRight(balancer);
                current = null;

            } else {
                Node balancer = current.getLeft();
                Node parent = current.getParent();
                balancer.setParent(parent);
                if(parent.getLeft() != null && parent.getLeft().getKey().compareTo(current.getKey()) == 0)
                    parent.setLeft(balancer);
                else
                    parent.setRight(balancer);
                current = null;
            }
        } else {
            Node balancer = findPredecessor(current.getLeft());
            Comparable temp = balancer.getKey();
            destroy(balancer);
            current.setKey(temp);
        }
    }

    // removes reference to a node so it can be garbage collected
    private void destroy(Node input) {
        if (input.getParent() == null)
            this.root = null;
        else {
            int result = input.getKey().compareTo(input.getParent().getKey());
            if (result < 0)
                input.getParent().setLeft(null);
            else
                input.getParent().setRight(null);
            input = null;
        }
    }

    // determines whether a node only has one child or not
    private boolean isHalf(Node toDelete) {
        Node left = toDelete.getLeft();
        Node right = toDelete.getRight();
        if (left == null)
            return true;
        else if (right == null)
            return true;
        else
            return false;
    }

    // determines whether a node is a leaf or not
    private boolean isLeaf(Node toDelete) {
        if ((toDelete.getLeft() == null) && (toDelete.getRight() == null))
            return true;
        else
            return false;
    }

    // finds either the right-most node (predecessor) under a node to be deleted
    private Node findPredecessor(Node current) {
        Node temp = current.getRight();

        if (temp == null)
            return current;
        else
            return findPredecessor(temp);
    }

    // prints a single line representing a level-order traversal of the tree
    public void printLevelOrder() {
        if (this.root == null)
            return;

        LinkedList<Node> printQueue = new LinkedList<Node>();
        printQueue.add(this.root);

        while (!printQueue.isEmpty()) {
            Node current = printQueue.poll();
            System.out.print(current.getKey() + " ");
            if (current.getLeft() != null)
                printQueue.add(current.getLeft());
            if (current.getRight() != null)
                printQueue.add(current.getRight());
        }
        System.out.println();
    }

    // prints a level-order traversal of the bst with one level per line
    public void prettyPrintLevelOrder() {
        if (this.root == null)
            return;

        LinkedList<Node> printQueue = new LinkedList<Node>();
        printQueue.add(this.root);

        while (true) {
            // nodeCount (queue size) indicates number of nodes at current level
            int nodeCount = printQueue.size();
            if (nodeCount == 0)
                break;

            while (nodeCount > 0) {
                Node current = printQueue.poll();
                System.out.print(current.getKey() + " ");
                if (current.getLeft() != null)
                    printQueue.add(current.getLeft());
                if (current.getRight() != null)
                    printQueue.add(current.getRight());
                nodeCount--;
            }
            System.out.println();
        }
        System.out.println();
    }

    public static void main(String args[]) {
        BinarySearchTree<Integer> bst = new BinarySearchTree();

        bst.insert(21);
        bst.insert(7);
        bst.insert(35);
        bst.insert(18);
        bst.insert(3);
        bst.insert(97);
        bst.insert(16);
        bst.insert(22);
        bst.insert(27);
        bst.insert(48);
        bst.insert(16);
        bst.insert(4);
        bst.insert(2);
        bst.insert(20);
        bst.insert(25);
        bst.insert(24);
        bst.insert(28);

        bst.prettyPrintLevelOrder();
        bst.delete(22);
        bst.prettyPrintLevelOrder();
        return;
    }
}