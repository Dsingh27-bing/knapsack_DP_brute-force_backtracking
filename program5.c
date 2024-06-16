#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include<string.h>

int dp[10000][10000];
int selected[10000][10000];
int X[10];
int maxprofit;
int num;
int include[10];
int bestset[10];



typedef struct {
  int profit;
  int weight;
  double fraction;
  int original;
} Item;
Item items[10000];


int max(int a, int b) {
    return (a > b) ? a : b;
}

int refined_recursive(int n, int W) {
    //printf("i am in refined recursive n = %d, W= %d", n, W);
    // If there are no items or the maximum weight is 0, return 0
    if (n == 0 || W == 0) {
        return 0;
    }

    // If the value for this subproblem has already been calculated, return it
    if (dp[n][W] != -1) {
        return dp[n][W];
    }

    // If the weight of the nth item is more than the maximum weight, ignore it
    if (items[n-1].weight > W) {
        dp[n][W] = refined_recursive(n-1, W);
    }
    else {
        // Calculate the maximum value by either including or excluding the nth item
        int include = items[n-1].profit + refined_recursive(n-1, W-items[n-1].weight);
        int exclude = refined_recursive(n-1, W);

        if(include > exclude){
          selected[n][W] = 1;
        }else{
          selected[n][W] = 0;
        }

        dp[n][W] = max(include, exclude);
    }

    // Return the maximum value
    return dp[n][W];
}

void refined_dynamic(int argc, char *argv[]){
  FILE *fp, *output, *entries = NULL;
    fp = fopen(argv[3], "r");
    output = fopen("output2.txt", "w");
    entries = fopen("entries2.txt","w");
    // arrays to store profit and weight data
    int numItems, knapsackW, i;

    // Read input from file
    fscanf(fp, "%d %d\n", &numItems, &knapsackW);
    printf("\nNumber of items = %d, Capacity of knapsack = %d \n", numItems, knapsackW);


    for (i = 0; i < numItems; i++) {
        int item, itemProfit, itemWeight;
        fscanf(fp, "item%d: %d %d\n", &item, &itemProfit, &itemWeight);
        items[i].profit = itemProfit;
        items[i].weight = itemWeight;

          }
    fclose(fp);


    // Initialize dp and selected tables to -1
    memset(dp, -1, sizeof(dp));
    memset(selected, -1, sizeof(selected));

    fprintf(entries,"Recursive Dynamic Programming\n");
    refined_recursive(numItems, knapsackW);

    for(int i=1; i<numItems+1; ++i){
      for (int j =1; j<knapsackW+1; ++j){
        if (dp[i][j] == -1){
          dp[i][j] = 0;
        }
        printf("%d ", dp[i][j]);
        fprintf(entries,"%d ", dp[i][j]);
        if(j == knapsackW){
          fprintf(entries,"end");
          printf("end");

        }
      }
      fprintf(entries, "\n");
      printf("\n");
    }

    int sum=0;
    //fprintf(output,"Maximum Weight: %d\n", dp[knapsackW]);
    int k= knapsackW;
    int num_final = 0;
    for (i = numItems; i > 0 && k > 0; i--) {
        if (selected[i][k] == 1) {

            sum= sum+ items[i-1].weight;
            k -= items[i-1].weight;
            ++num_final;
        }
    }
    printf("Maximum Profit: %d\n", dp[numItems][knapsackW]);
    printf("Maximum Weight: %d\n",sum);
    fprintf(output,"%d  %d  %d\n", num_final,dp[numItems][knapsackW],sum);

    int j = knapsackW;
    for (i = numItems; i > 0 && j > 0; i--) {
        if (selected[i][j] == 1) {
          printf("item%d: profit %d, weight %d\n", i, items[i-1].profit, items[i-1].weight);

            fprintf(output,"%s%d: profit %d, weight %d\n", "item", i, items[i-1].profit, items[i-1].weight);
            j -= items[i-1].weight;
        }
    }
    fclose(entries);
    fclose(output);
    return;
}

//brute force function
int bruteforce(int argc, char *argv[]) {
    FILE *fp, *output = NULL;
    fp = fopen(argv[3], "r");
    output = fopen("output1.txt", "w");
    // arrays to store profit and weight data
    int numItems, knapsackW, i;

    // Read input from file
    fscanf(fp, " %d  %d\n", &numItems, &knapsackW);
    printf("Number of items = %d, Capacity of knapsack = %d \n", numItems, knapsackW);


    for (i = 0; i < numItems; i++) {
        int item, itemProfit, itemWeight;
        fscanf(fp, "item%d: %d %d\n", &item, &itemProfit, &itemWeight);

        // Store item data in the arrays
        items[i].profit = itemProfit;
        items[i].weight = itemWeight;
    }
    fclose(fp);

    // creating subsets
    int maxBenefit = 0;
    int maxSet = 0;
    int maxWeight = 0;
    for (int i = 0; i < pow(2, numItems); i++) {
        int benefit = 0;
        int weight_set = 0;
        int set = i;
        int subset = 0;

        for (int j = 0; j < numItems; j++) {
            if (set % 2 == 1) {
                weight_set += items[j].weight;
                benefit += items[j].profit;
                subset |= 1 << j;
            }
            set /= 2;
        }

        // Discard subsets whose sum of weights exceed knapsackW
        if (weight_set <= knapsackW) {
            // Select the maximum total benefit
            if (benefit > maxBenefit) {
                maxBenefit = benefit;
                maxSet = subset;
                maxWeight = weight_set;
            }
        }
    }
    int num_final = 0;
    for (int i = 0; i < numItems; i++) {
        if (maxSet & (1 << i)) {
            ++num_final;
        }
    }
    fprintf(output,"%d\t", num_final);
    printf("Maximum Profit: %d\n", maxBenefit);
    printf("Maximum Weight: %d\n", maxWeight);
    fprintf(output,"%d\t", maxBenefit);
    fprintf(output,"%d\t\n", maxWeight);
    for (int i = 0; i < numItems; i++) {
        if (maxSet & (1 << i)) {
          ++num_final;
          printf("Item%d: Profit = %d, Weight = %d\n", i+1, items[i].profit, items[i].weight);
          fprintf(output,"Item %d: Profit = %d, Weight = %d\n", i+1, items[i].profit, items[i].weight);
        }
    }
    fclose(output);

    return 0;
}



// bactracking

float KWF2(int i, int weight, int profit, int n, int W){
    float bound = profit;
    for (int j = i; j<=n; j++) {
        X[j] = 0;
    }
    float sbx;
    while (weight < W && i<=n) {
        if (weight + items[i-1].weight <= W) {
            X[i] = 1;
            weight = weight + items[i-1].weight;
            bound = bound + items[i-1].profit;
        }else{
            sbx = (float)(W-weight)/(float)items[i-1].weight;
            X[i] = sbx;

            weight = W;
            bound = bound + items[i-1].profit * sbx;
        }
        i += 1;
    }
    return bound;
}


int Promising(int i, int profit, int W, int weight, int n){

    float bound = KWF2(i+1, weight, profit, n, W);
    FILE *fp = NULL;
    fp = fopen("entries3.txt", "a+");
    printf("node: index:%d,profit:%d,weight:%d, upperBound:%f\n", i, profit, weight, bound);
    fprintf(fp,"node: index:%d,profit:%d,weight:%d, upperBound:%f\n", i, profit, weight, bound);
    fclose(fp);
    if (weight >= W) {
        return 0;
    }

    return (bound > maxprofit);
}



void knapsack(int i, int profit, int weight, int W,int n){
    if (weight <= W && profit > maxprofit) {
        maxprofit = profit;
        num = i;
        for (int j = 1; j<=n; j++) {
            bestset[j] = include[j];
        }
    }
    if (Promising(i, profit, W, weight, n)) {
        include[i+1] = 1;
        knapsack(i+1, profit + items[i].profit, weight + items[i].weight, W, n);
        include[i+1] = 0;
        knapsack(i+1, profit, weight, W, n);
    }

}


void Knapsack(int argc, char *argv[]){
    FILE *fp, *output, *entries = NULL;
    fp = fopen(argv[3], "r");
    output = fopen("output3.txt", "w");
    entries = fopen("entries3.txt","w");
    // arrays to store profit and weight data
    int numItems, knapsackW, i;
    num = 0;
    maxprofit = 0;
    int tempweight = 0;
    fscanf(fp, "%d %d ", &numItems,&knapsackW);
    int index;
    for (int i = 0; i < numItems; i++) {
        fscanf(fp, "item%d: %d %d\n", &index, &items[i].profit, &items[i].weight);
        items[i].fraction = (double)items[i].profit / (double)items[i].weight;
        items[i].original = i+1;
      }

      for (int i = 0; i < numItems; i++) {
          for (int j = i+1; j < numItems; j++) {
              if (items[i].fraction < items[j].fraction) {
                int tempProfit = items[i].profit;
                items[i].profit = items[j].profit;
                items[j].profit = tempProfit;
                int tempWeight = items[i].weight;
                items[i].weight = items[j].weight;
                items[j].weight = tempWeight;
                float tempFraction = items[i].fraction;
                items[i].fraction = items[j].fraction;
    // sort the items by their fractions in decreasing order

                items[j].fraction = tempFraction;
                int tempIndex = items[i].original;
                items[i].original = items[j].original;
                items[j].original = tempIndex;

               }
           }
       }

       printf("\nBacktracking Algorithm\n");
       for(int j =0; j<numItems; ++j){
         printf("index:%d, profit:%d, weight:%d, pUw:%f\n",items[j].original, items[j].profit, items[j].weight, items[j].fraction);
         fprintf(entries, "index:%d, profit:%d, weight:%d, pUw:%f\n",items[j].original, items[j].profit, items[j].weight, items[j].fraction);
       }
        fclose(entries);
  
        knapsack(0, 0, 0, knapsackW, numItems);
    int sum = 0;
    int num_final=0;
    for (int i = 1; i<=num; i++) {
        if (bestset[i]==1) {
            tempweight += items[i-1].weight;
            ++num_final;
        }
    }
    printf("Max profit: %d\n", maxprofit);

    printf("Max weight : %d\n\n", tempweight);
    fprintf(output, "%d\t%d\t%d\n", num_final, maxprofit, tempweight);
    for (int i = 0; i < numItems; i++) {
      if (bestset[i] == 1) {
        printf("Item%d: Profit %d Weight %d\n", items[i-1].original,items[i-1].profit,  items[i-1].weight);
        fprintf(output,"item%d: profit %d weight %d\n", items[i-1].original,items[i-1].profit, items[i-1].weight);
      }
    }

  fclose(output);
  return;

}



int main(int argc, char *argv[]) {
	if(argc<4)
	{
		printf("\n \nError in number of arguments!!!\n \n");
		exit(0);
	}

    if ((strcmp(argv[1], "createkn01") == 0) && (strcmp(argv[2], "-k") == 0) && (strcmp(argv[3], "knapsack01.txt") == 0))
    {
        FILE *f;
        f = fopen("knapsack01.txt", "w+");
        srand(time(0)); // seed the random number generator

        // randomly select the number between 4 and 8
        int n = rand() % 5 + 4;

        for (int i = 0; i < n; i++)
        {
            items[i].profit = rand() % (21) + 10;
            items[i].weight = rand() % (16) + 5;
        }

        int totalwi = 0;
        for (int i = 0; i < n; i++)
        {
            totalwi = totalwi + items[i].weight;
        }
        int W = floor(0.6 * totalwi);
        fprintf(f, "%d %d \n", n, W);
        printf("randomly selected %d numbers\n", n);
        for (int i = 0; i < n; i++)
        {
                printf("item%d: profit%d, weight%d\n", i + 1, items[i].profit, items[i].weight);
                fprintf(f, "item%d: %d %d\n", i + 1, items[i].profit, items[i].weight);
            }

        printf("the capacity of the knapsack:%d\n", W);


        fclose(f);

    }
    else if ((strcmp(argv[1], "bruteforce") == 0) && (strcmp(argv[2], "-k") == 0) && (strcmp(argv[3], "knapsack01.txt") == 0))
    {
        FILE *fp;
        fp = fopen("knapsack01.txt", "r");
        if (fp == NULL)
        {
            printf("Error opening file\n");
        }
        else
        {
            bruteforce(argc,argv);
            fclose(fp);
        }
    }
    else if ((strcmp(argv[1], "dynpro") == 0) && (strcmp(argv[2], "-k") == 0) && (strcmp(argv[3], "knapsack01.txt") == 0))
    {
        FILE *fp;
        fp = fopen("knapsack01.txt", "r");
        if (fp == NULL)
        {
            printf("Error opening file\n");
        }
        else
        {
            refined_dynamic(argc,argv);
            fclose(fp);
        }
    }

      else if ((strcmp(argv[1], "backtrack") == 0) && (strcmp(argv[2], "-k") == 0) && (strcmp(argv[3], "knapsack01.txt") == 0))
    {
        FILE *fp;
        fp = fopen("knapsack01.txt", "r");
        if (fp == NULL)
        {
            printf("Error opening file\n");
        }
        else
        {
            int n,knapsackW = 0;
            Knapsack(argc,argv);
            fclose(fp);
        }
    }

    else
    {
        printf("\n Error in arguments \n \n");
    }
    return 0;
}
