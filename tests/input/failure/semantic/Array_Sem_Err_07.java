class ArraySemErr07 {
    public static void main(String[] args) {
        if (true) {
            int[] temp = new int[2];
            temp[0] = 1;
        }
        temp = new int[4];
    }
}