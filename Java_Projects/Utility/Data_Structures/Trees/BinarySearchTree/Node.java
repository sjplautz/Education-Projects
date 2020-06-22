package trees;

public class Node <T extends Comparable>{
    private Node parent;
    private Node left;
    private Node right;
    private T key;

    public Node(){
        this.parent = null;
        this.left = null;
        this.right = null;
        this.key = null;
    }

    public Node(T key){
        this.parent = null;
        this.left = null;
        this.right = null;
        this.key = key;
	}
	
	public Node(T key, Node parent){
		this.parent = parent;
		this.left = null;
		this.right = null;
		this.key = key;
	}

    public Node(T key, Node parent, Node left, Node right){
        this.parent = parent;
        this.left = left;
        this.right = right;
        this.key = key;
    }

	public Node getParent() {
		return this.parent;
	}

	public void setParent(Node parent) {
		this.parent = parent;
	}

	public Node getLeft() {
		return this.left;
	}

	public void setLeft(Node left) {
		this.left = left;
	}

	public Node getRight() {
		return this.right;
	}

	public void setRight(Node right) {
		this.right = right;
	}

	public T getKey() {
		return this.key;
	}

	public void setKey(T key) {
		this.key = key;
	}  
}