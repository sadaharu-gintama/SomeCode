package AnalysisOfAlgorithm;

import java.util.Arrays;

public class threeSumFast {
    public static int count(int[] a) {
        Arrays.sort(a);
        int N = a.length;
        int cnt = 0;
        for (int i = 0; i != N; ++i) {
            for (int j = 0; j != N; ++j) {
                if (BinarySearch.rank(-a[i], a) > i)
                    cnt++;
            }
        }
        return cnt;
    }
}
