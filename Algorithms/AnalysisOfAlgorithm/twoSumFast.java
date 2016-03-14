package AnalysisOfAlgorithm;

import java.util.Arrays;

public class twoSumFast {
    public static int count(int[] a) {
        Arrays.sort(a);
        int N = a.length;
        int cnt = 0;
        for (int i = 0; i != N; ++i) {
            if (BinarySearch.rank(-a[i], a) > i)
                cnt++;
        }
        return cnt;
    }
}
