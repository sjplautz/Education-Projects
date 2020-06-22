import java.util.ArrayList;
import java.util.Collection;
import java.util.List;

class QuickSorter {
    //public interface that allows simpler use of quicksorter
    public static <T extends Comparable <? super T>> void sort(List<T> input){
        sortHelper(input, 0, input.size()-1);
    }
    private static <T extends Comparable <? super T>> void sortHelper(List<T> input, int l, int r){
        if(l < r){
            int pivot = partition(input, l, r);

            sortHelper(input, l, pivot-1);
            sortHelper(input, pivot+1, r);
        }
    }
    private static <T extends Comparable <? super T>> int partition(List<T> input, int l, int r){
        //selecting pivot as end value
        T pivotValue = input.get(r);
        int  i = l-1;

        for(int j = l; j < r; j++){
            if(input.get(j).compareTo(pivotValue) <= 0){
                i++;
                //swap values
                T temp = input.get(i);
                input.set(i, input.get(j));
                input.set(j, temp);
            }
        }

        T temp = input.get(i+1); 
		input.set(i+1, pivotValue); 
		input.set(r, temp);
        
        return i+1;
    }

     // Driver method
     public static void main(String args[]) {
        List<Double> arr = new ArrayList();
        arr.add(12.1);
        arr.add(11.2);
        arr.add(13.4);
        arr.add(5.51);
        arr.add(5.5);
        arr.add(6.11);
        arr.add(7.9);

        System.out.println("Given Array");
        System.out.println(arr.toString());

        QuickSorter.sort(arr);

        System.out.println("\nSorted array");
        System.out.println(arr.toString());
    }
}