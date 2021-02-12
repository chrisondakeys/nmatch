# nmatch
Usage: ```nmatch``` _PATTERN_ STRING  
nmatch: a nucleotide sequence naive matching algorithm  
Returns _PATTERN_'s start indices in STRING  
Preprocesing time: 0, matching time: O((n-m+1) m)  
Note: soft-masked sequences are treated equally as standard  
Example: ```nmatch UGAWACGT tsaaacgttgatacgt```  
result: 0 8  
compilation with ```gcc -O3 nmatch.c -o nmatch``` worked on my machine  
