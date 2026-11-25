class ArraySemErr08 {
    public static void main(String[] args) {
        int[] a = new int[3];
        int[] b = new int[3];
        a++;
        int[] c = a + b;
    }
}