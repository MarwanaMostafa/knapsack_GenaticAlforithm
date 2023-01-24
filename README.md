# knapsack_GenaticAlforithm
Knapsack problem we seek to find best solutions without solve directly problem but modify any random solution to reach best output 

# How to run :
1-file contain input (as knapsack_input.txt in this repo) which inputs :

    i-first line number of test case
 
    ii-second line Size of the knapsack
  
    iii-third line Number of items
  
    iv-fourth line:
    
      • Weight and value for each item separated by space
      
# How to get best solution :
1-Initialize population randomly

2-Calculate fitness to each chromosome through sum values for each chromosome then high summation take high and damage fitness for infeasible chromosome 

3-selection some chromosome using Rank selection 

4-crossover and mutation 

5-replacement which we take best fitness 
