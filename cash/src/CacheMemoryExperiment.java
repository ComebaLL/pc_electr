/* Kuvykin N.D CMC-21
    cash lab
 */
public class CacheMemoryExperiment {

    public static void main(String[] args) {
        // Шаг 1: Генерация массива
        int arraySize = 10000;
        int[] array = new int[arraySize];

        // Шаг 5: Многократное повторение
        int repetitions = 75;
        long totalCacheMemorySize = 0;

        for (int r = 0; r < repetitions; r++) {
            long[] executionTimes = new long[arraySize];

            // Шаг 2-4: Измерение времени выполнения
            for (int i = 0; i < arraySize; i++) {
                long startTime = System.nanoTime();

                // Производим арифметическую операцию (например, сложение с константой)
                array[i] += 5 * i; // Увеличиваем константу для каждой итерации

                long endTime = System.nanoTime();
                executionTimes[i] = endTime - startTime;
            }

            // Шаг 4: Проверка на значительное увеличение времени выполнения
            for (int i = 1; i < arraySize; i++) {
                if (executionTimes[i] > executionTimes[i - 1] * 2) {
                    // Шаг 6: Определение среднего индекса
                    int averageIndex = calculateAverageIndex(executionTimes);

                    // Шаг 7: Определение количества элементов в кэше
                    int cacheSize = calculateCacheSize( averageIndex);

                    // Шаг 8: Расчет объема кэш-памяти
                    int elementSize = calculateElementSize(array,averageIndex);
                    int cacheMemorySize = cacheSize * elementSize;
                    totalCacheMemorySize += cacheMemorySize;

                    break;
                }
            }
        }

        // Вывод общего затраченного объема кэш-памяти после всех итераций
        System.out.println("Total cache memory size across all iterations: " + totalCacheMemorySize + " bytes");
    }

    // Метод для расчета среднего индекса
    private static int calculateAverageIndex(long[] executionTimes) {
        long sum = 0;
        for (long time : executionTimes) {
            sum += time;
        }
        return (int) (sum / executionTimes.length);
    }

    // Метод для расчета количества элементов в кэше
    private static int calculateCacheSize(int averageIndex) {
        // Просто пример: возвращаем половину среднего индекса
        return averageIndex / 2;
    }

    // Метод для расчета размера элемента массива
    private static int calculateElementSize(int[] array, int index) {
        // Просто пример: возвращаем размер элемента на индексе
        return array[index];
    }
}









