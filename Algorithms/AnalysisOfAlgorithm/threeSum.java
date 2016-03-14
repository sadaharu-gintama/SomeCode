package AnalysisOfAlgorithm;

public class threeSum {
    public static int count(int[] a) {
        int N = a.length;
        int cnt = 0;
        for (int i = 0; i != N; ++i)
            for (int j = 0; j != N; ++j)
                for (int k = 0; k != N; ++k)
                    if (a[i] + a[j] + a[k] == 0)
                        cnt ++;
        return cnt;
    }
}