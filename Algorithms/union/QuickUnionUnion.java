package union;

public class QuickUnionUnion {
    private int[] id;
    private int count;
    
    public QuickUnionUnion(int N) {
        count = N;
        id = new int[N];
        for (int i = 0; i != N; ++i)
            id[i] = i;
    }
    
    public int count() { return count; }
    
    public boolean connected(int p, int q) { return find(p) == find(q); }
    
    public int find(int p) { 
        while (p != id[p]) p = id[p];
        return p;
    }
    
    public void union(int p, int q) {
        int pId = find(p);
        int qId = find(q);
        if (pId == qId) return;
        id[pId] = qId;
        count --;
    }
}
