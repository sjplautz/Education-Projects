import java.util.ArrayList;
import java.util.Collection;
import java.util.List;

class binarySearcher {
    public static <T extends Comparable<? super T>> T search(List<T> input, T key){
        return searchHelper(input, key, 0, input.size()-1);
    }
    
    private static <T extends Comparable<? super T>> T searchHelper(List<T> input, T key, int l, int r){
        if(r >= l){
            int midIndex = (l+r)/2;

            int result = input.get(midIndex).compareTo(key);

            if(result == 0)
                return input.get(midIndex);
            else if(result < 0)
                return searchHelper(input, key, midIndex+1, r);
            else
                return searchHelper(input, key, l, midIndex-1);
        }
        return null;
    }

       // Driver method
       public static void main(String args[]) {
        List<Double> arr = new ArrayList();
        arr.add(1.1);
        arr.add(4.34);
        arr.add(6.99);
        arr.add(9.1);
        arr.add(2.89);
        arr.add(5.66);

        System.out.println("Given Array");
        System.out.println(arr.toString());

        QuickSorter.sort(arr);
        System.out.println(arr.toString());

        Double result = binarySearcher.search(arr, 9.1);
        System.out.println(result.toString());
    }
}