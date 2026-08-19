#include <vector>
#include <numeric>  // for std::accumulate  
// #include<boost/accumulators.hpp> If you use Boost Accumulator Library, uncomment the line below and include it at top of your file as well   
                     
int sum(const vector<vector<int>>& vec) {     // assuming 'vec' is a type alias for std::vector in C++14 
   int total = 0;                             // using standard library algorithm to calculate the cumulative sums on both sides of each element.   
                                             
       if( !vec.empty() ){                    // check whether vector has elements or not, as empty vectors are considered valid inputs for sum operation in C++14 range-based loop 
           total = std::accumulate ( vec[0].begin(), vec[0].end () ,total );   // left side of the first element's cumulative sums.   
       }    
       
      if(!vec.empty()){                        // check whether vector has elements or not, as empty vectors are considered valid inputs for sum operation in C++14 range-based loop 
           total = std::accumulate ( vec[0].rbegin(), vec[0].rend () ,total );   // right side of the first element's cumulative sums.   
       }    
       
      return total;                            // returns final summation result, which is left and rigth sides combined 
}                                             
