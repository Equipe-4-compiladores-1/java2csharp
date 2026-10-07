class IfParentesesExplicitos {
    public static void main(String[] args) {
        int x = 3;
        if (x == 1 + 2) {
            System.out.println("sem agrupamento explicito");
        }
        if (x == (1 + 2)) {
            System.out.println("com agrupamento explicito");
        }
    }
}