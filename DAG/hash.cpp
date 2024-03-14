#include <cmath>
#include <iostream>
#include <algorithm>
#include <vector>
using namespace std;

    struct Address
{
	string address;
	int read[1200]={-1};
	int write[1200]={-1};
	int read_count=0;
	int write_count=0;
};
    static vector<Address> Add_list,empty_Add;

class HashTable {
    public:

    static const int PRIME_CONST = 31;
    static const int ARR_SIZE = 5000;
    int arr[ARR_SIZE]={0};
    int count=0;


    public:
        HashTable() {
            fill_n(arr, ARR_SIZE, 0);
        }

        int put(string key, int txnID, int write) {
            int id,i, index = hashFunc(key);
            Address temp_add;
            if(arr[index] == 0){
            Add_list.push_back(temp_add);
            arr[index]=count;
             count++;
            }
            id=arr[index];
            i = Add_list[id].read_count++;
            Add_list[id].read[i]=txnID;
            if(write==1)
            {
            i = Add_list[id].write_count++;
            Add_list[id].write[i]=txnID;
            }
            return arr[index] ;
        }

        static int hashFunc(string key) {
            size_t sum;
            for (int i = 0; i < key.length(); i++) {
                sum += (key[i] * (int)pow(PRIME_CONST, i)) % ARR_SIZE;
            }
            return sum % ARR_SIZE;
        }
        
        bool check(string key, int txn1, int txn2) {
            int id= put( key, txn1, 0);
            if(Add_list[id].write[txn2]==1) { return true; }
            if(Add_list[id].write[txn1]==1 and Add_list[id].read[txn2]==1) { return true; }   
            return false;         
            
        
        }

        void printAll() {
            for (int i = 0; i < ARR_SIZE; i++) {
                if (arr[i]>0) {
                    cout << arr[i] << endl;
                }
            }
            cout << endl;
        }
};


