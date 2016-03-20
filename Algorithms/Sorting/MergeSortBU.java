package Sorting;

public class MergeSortBU {
    private static Comparable[] aux;
    public static void MergeSortBU(Comparable[] a) {
        int N = a.length;
        aux = new Comparable[N];
        for (int sz = 1; sz < N; sz = sz * 2) {
            for (int low = 0; low < N - sz; low += sz * 2) {
                merge(a, low, low + sz - 1, Math.min(low + 2 * sz - 1, N - 1));
            }
        } 
    }
    
    private static void merge(Comparable[] a, int low, int middle, int high) {
        int i = low;
        int j = middle + 1;
        System.arraycopy(a, low, aux, low, high);
        
        for (int k = low; k <= high; k++) {
            if (i > middle)                a[k] = aux[j++];
            else if (j > high)             a[k] = aux[i++];
            else if (less(aux[j], aux[i])) a[k] = aux[j++];
            else                           a[k] = aux[i++];
        }
    }
    
    private static boolean less(Comparable v, Comparable w) {
        return v.compareTo(w) < 0;
    }
    
    private static void exch(Comparable[] a, int i, int j) {
        Comparable t = a[i];
        a[i] = a[j];
        a[j] = t;
    }
    
    public static boolean isSorted(Comparable[] a) {
        for (int i = 1; i < a.length; ++i)
            if (less(a[i], a[i - 1])) return false;
        return true;
    }
}
