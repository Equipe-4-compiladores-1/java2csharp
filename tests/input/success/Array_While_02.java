class ArrayWhile02 {
    public static void main(String[] args) {
        int n = 4;
        double[] notas = new double[n];
        boolean[] aprovados = new boolean[n];

        int idx = 0;
        while (idx < n) {
            notas[idx] = (idx + 1) * 2.5;
            aprovados[idx] = notas[idx] >= 6.0;
            idx++;
        }

        System.out.println(notas[0]);
        System.out.println(aprovados[3]);
    }
}