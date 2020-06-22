import java.util.ArrayList;
import java.util.List;

//generic mergesort function, supports any class that implements the list interface and containing comparables
class MergeSorter {
    public static <T extends Comparable<? super T>> List<T> sort(List<T> input) {
        //base case
        if(input.size() <= 1)
            return input;

        else {
            int midIndex = input.size() / 2;

            List<T> left = new ArrayList<T>();
            List<T> right = new ArrayList<T>();

            for (int i = 0; i < midIndex; i++)
                left.add(input.get(i));
            for (int j = midIndex; j < input.size(); j++)
                right.add(input.get(j));

            return merge(sort(left), sort(right));
        }
    }

    private static <T extends Comparable<? super T>> List<T> merge(List<T> left, List<T> right) {
        int i = 0, j = 0;

        List<T> result = new ArrayList<T>();

        while (i < left.size() && j < right.size()) {
            if (left.get(i).compareTo(right.get(j)) <= 0)
                result.add(left.get(i++));
            else
                result.add(right.get(j++));
        }

        // if one subcollection emptied first, fill with remaining elements
        while (i < left.size()) {
            result.add(left.get(i++));
        }
        while (j < right.size()) {
            result.add(right.get(j++));
        }

        return result;
    }

    // Driver method
    public static void main(String args[]) {
        List<Integer> arr = new ArrayList<Integer>();
        arr.add(12);
        arr.add(11);
        arr.add(13);
        arr.add(5);
        arr.add(6);
        arr.add(7);

        System.out.println("Given Array");
        System.out.println(arr.toString());

        arr = MergeSorter.sort(arr);

        System.out.println("\nSorted array");
        System.out.println(arr.toString());
    }
}