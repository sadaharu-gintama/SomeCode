package symbolTable;

public class SequentialST<Key, Value> {
    private Node first;
    
    private class Node {
        Key key;
        Value val;
        Node next;
        public Node(Key key, Value value, Node next) {
            this.key = key;
            this.val = val;
            this.next = next;
        }
    }
    
    public Value get(Key key) {
        for (Node it = first; it != null; it = it.next) {
            if (key.equals(it.key))
                return it.val;
        }
        return null;
    }
    
    public void put(Key key, Value val) {
        for (Node it = first; it != null; it = it.next) {
            if (key.equals(it.key)) {
                it.val = val;
                return;
            }
        }
        first = new Node(key, val, first);
    }
}
