import java.util.Scanner;

public class matrixproduct_java {
    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);
        System.out.println("1. Multiplication");
        System.out.println("2. Line Multiplication");
        System.out.println("Selection?: ");

        String selection = scanner.nextLine();

        System.out.println("Dimensions: lins=cols ?");

        String userInput = scanner.nextLine();

        try {
            int intValue = Integer.parseInt(userInput);
            if (selection.equals("1"))
                OnMult(intValue, intValue);
            else if (selection.equals("2"))
                OnMultLine(intValue, intValue);
            else
                System.out.println("Invalid selection.");
        } catch (NumberFormatException e) {
            System.out.println("Invalid input. Please enter a valid integer.");
        }
        finally {
            scanner.close();
        }
    }

    public static void OnMult(int a, int b) {
        double temp;
        int i, j, k;

        double[][] pha = new double[a][a];
        double[][] phb = new double[a][a];
        double[][] phc = new double[a][b];

        for(i=0; i<a; i++){
            for(j=0; j<a; j++){
                pha[i][j] = 1.0;
            }
        }

        for (i=0; i<a; i++){
            for(j=0; j<a; j++){
                phb[i][j] = i + 1.0;
            }
        }

        long startTime = System.nanoTime();

        for(i=0; i<a; i++){
            for(j=0; j<b; j++){
                temp = 0.0;
                for(k=0; k<a; k++){
                    temp += pha[i][k] * phb[k][j];
                }
                phc[i][j] = temp;
            }
        }
        long endTime = System.nanoTime();
        long duration = (endTime - startTime);

        System.out.println("\nResult: ");
        for(i=0; i<1; i++){
            for(j=0; j<Math.min(10, b); j++){
                System.out.print(phc[i][j] + " ");
            }
        }
        System.out.println("\n\nTime: " + String.format("%.3f", duration/1_000_000_000.0) + " second(s)");
    }

    public static void OnMultLine(int a, int b){

        int i, j, k;

        double[][] pha = new double[a][a];
        double[][] phb = new double[a][a];
        double[][] phc = new double[a][b];

        for(i=0; i<a; i++){
            for(j=0; j<a; j++){
                pha[i][j] = 1.0;
            }
        }

        for (i=0; i<a; i++){
            for(j=0; j<a; j++){
                phb[i][j] = i + 1.0;
            }
        }

        long startTime = System.nanoTime();

        for (i = 0; i < a; i++) {
            for (j = 0; j < a; j++) {
                for (k = 0; k < b; k++) {
                    phc[i][k] += pha[i][j] * phb[j][k];
                }
            }
        }

        long endTime = System.nanoTime();

        long duration = (endTime - startTime);

        System.out.println("\nResult: ");
        for(i=0; i<1; i++){
            for(j=0; j<Math.min(10, b); j++){
                System.out.print(phc[i][j] + " ");
            }
        }
        System.out.println("\n\nTime: " + String.format("%.3f", duration/1_000_000_000.0) + " second(s)");
    }
}