package Sorting;

public class MergeSort {
    private static Comparable[] aux;
    public static void MergeSort(Comparable[] a) {
        aux = new Comparable[a.length];
        sort(a, 0, a.length - 1);
    }
    
    private static void sort(Comparable[] a, int low, int high) {
        if (high <= low) return;
        int middle = (low + high) / 2;
        sort(a, low, middle);
        sort(a, middle, high);
        merge(a, low, middle, high);
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
