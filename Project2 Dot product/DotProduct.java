import java.util.Random;
public class DotProduct {
 
private static final int VECTOR_SIZE = 1000000;
 
private static final int NUM_EXPERIMENTS = 100;
 
public static void main(String[] args) {
 
float[] vector1 = new float[VECTOR_SIZE];
 
float[] vector2 = new float[VECTOR_SIZE];
 
Random random = new Random();
 
float total_time = 0;
 
for (int j = 0; j < NUM_EXPERIMENTS; j++) {
 
// Initialize vectors with random values
 
for (int i = 0; i < VECTOR_SIZE; i++) {
 
vector1[i] = random.nextFloat();
 
vector2[i] = random.nextFloat();
 
}
 
// Compute dot product
 
float dot_product = 0;
 
long start_time = System.nanoTime();
 
for (int i = 0; i < VECTOR_SIZE; i++) {
 
dot_product += vector1[i] * vector2[i];
 
}
 
long end_time = System.nanoTime();
 
double time_taken = (double) (end_time - start_time) / 1000000000;
 
total_time += time_taken;
 
// Print dot product and time take
 
System.out.printf("Experiment %d: Dot product: %f, Time taken: %f seconds%n", j + 1, dot_product, time_taken);
 
}
 
// Print average time taken
 
System.out.printf("Average time taken: %f seconds%n", total_time / NUM_EXPERIMENTS);
 
}
}
