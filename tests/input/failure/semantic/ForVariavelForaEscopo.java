class ForVariavelForaEscopo {
    public static void main(String[] args) {
        for (int i = 0; i < 10; i = i + 1) {
            int x = i;
        }
        int y = i;
    }
}

