
#include <iostream> 
#include <thread> 
using namespace std; 
  
//test fn
void fn1(int Z) 
{ 
    for (int i = 0; i < Z; i++) { 
        cout << "Thread with pointer\n"; 
    } 
} 
  
//callable obj 
class thread_obj { 
public: 
    void operator()(int x) 
    { 
        for (int i = 0; i < x; i++) 
            cout << "Thread with object\n"; 
    } 
}; 
  
int main() 
{ 
   // cout << "Threads 1 and 2 and 3 "
   //      "operating at the same time" << endl; 
  
    //thread launched by function pointer as callable, 3 in para input
    thread th1(fn1, 3); 
  
    //thread launched by using function object as callable, 3 in para input 
    thread th2(thread_obj(), 3); 
  
    //lambda expression 
    auto f = [](int x) { 
        for (int i = 0; i < x; i++) 
            cout << "Thread with lambda expression\n"; 
    }; 
  
    //thread launched by lamda expression as callable, 3 in para input
    thread th3(f, 3); 
  
    // Wait for the threads to finish 
    th1.join(); 
    th2.join(); 
    th3.join(); 
    //End
    return 0; 
}