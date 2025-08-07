#include <iostream>
using namespace std;

int numOfOnes(long long int number);
int countDigits(long long int num);
long long int onesGenerater(int numOfDigits);

int main()
{
    long long int number;
    cin >> number;
    cout << numOfOnes(number);
}


int countDigits(long long int num) {
    int count = 0;
    if(num<0){
        num = -num;
    }
    while (num != 0) {
        num = num / 10;
        count++;
    }
    return count;
}


long long int onesGenerater(int numOfDigits){
    long long int num=1;
    for(int i=1; i<numOfDigits; i++){
        num=(10*num) + 1;
    }
    return num;
}


int numOfOnes(long long int number){
    if(number==0)
        return 0;
    long long int x = onesGenerater(countDigits(number));
    if(number>0){
        number=number - x;
    } else{
        number=number + x;
    }
    return countDigits(x) + numOfOnes(number);
}