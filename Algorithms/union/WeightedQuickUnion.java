package union;

public class WeightedQuickUnion {
    private int[] id;
    private int[] sz;
    private int count;
    
    public WeightedQuickUnion(int N) {
        count = N;
        id = new int[N];
        for (int i = 0; i != N; ++i) id[i] = i;
        sz = new int[N];
        for (int i = 0; i != N; ++i) sz[i] = 1;
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
        if (sz[pId] < sz[qId]) { id[pId] = qId; sz[qId] += sz[pId]; }
        else { id[qId] = pId; sz[pId] += sz[qId]; }
        count --;
    }
}
