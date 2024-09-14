
import java.util.Arrays;

public class Main {

	public static void main(String[] args) {

		int[] number = new int[] { 0, 1, 2, 3, 4 };
		int k = 3;

		int[] perm = Arrays.copyOfRange(number, 0, k);
		for (; perm != null;) {
			System.out.println(Arrays.toString(perm));
			perm = getNextKPermutation(k, number);
		}

	}

	public static int[] getNextKPermutation(int k, int[] number) {
		int n = number.length;
		int i = k;
		for (; i < n && number[i] < number[k - 1];) {
			i = i + 1;
		}
		if (i < n) {
			swap(number, i, k - 1);
		} else {
			reverse(number, k - 1);
			int j = k - 2;
			for (; j >= 0 && number[j] > number[j + 1];) {
				j = j - 1;
			}
			if (j < 0) {
				return null;
			} else {
				i = n - 1;
				for (; i > j;) {
					if (number[i] > number[j]) {
						break;
					}
					i = i - 1;
				}
				swap(number, i, j);
				reverse(number, j);
			}
		}
		return Arrays.copyOfRange(number, 0, k);

	}

	public static void reverse(int[] permutation, int index) {
		int shift = index + 1;
		for (int i = 0; i < (permutation.length - shift) / 2; i++) {
			int temp = permutation[shift + i];
			permutation[shift + i] = permutation[permutation.length - i - 1];
			permutation[permutation.length - i - 1] = temp;
		}
	}

	public static void swap(int[] array, int i, int j) {
		int temp = array[i];
		array[i] = array[j];
		array[j] = temp;
	}

}
