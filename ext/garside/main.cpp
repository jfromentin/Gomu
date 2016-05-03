#include "monoid.hpp"
#include "braids.hpp"
#include <list>

using namespace std;

int main(){
  Generator x=generator(4,3);
  cout<<generator(1,3)<<endl;
  cout<<generator(2,3)<<endl;
  cout<<(int)get_i(x)<<','<<(int)get_j(x)<<endl;
}
