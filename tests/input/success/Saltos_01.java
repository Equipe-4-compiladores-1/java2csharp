class Saltos_01 {
    public static void main(String[] args) {
        int soma = 0;
        for (int i = 0; i < 10; i++) {
            if (i % 2 == 0) {
                continue;
            }
            if (i > 7) {
                break;
            }
            soma = soma + i;
        }
        System.out.println(soma);

        int n = 0;
        while (true) {
            n++;
            if (n == 5) {
                break;
            }
        }
        System.out.println(n);

        for (int a = 0; a < 3; a++) {
            int b = 0;
            while (b < 3) {
                b++;
                if (b == 2) {
                    continue;
                }
                System.out.printf("%d %d\n", a, b);
            }
            if (a == 1) {
                break;
            }
        }
    }
}
