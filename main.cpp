#include "CStack.h"
#include "Factory.h"
#include "Functions.h"

#include <fstream>
#include <chrono>
#include <omp.h>


int fillInputFile(int); // filling input file

int fillInputFile(int numberOfLines) {

    ofstream File("input.txt");

    for (int i = 0; i < numberOfLines; ++++i) {
        File << "0 file" << i << " 0" << endl;
        File << "1 file" << i + 1 << " 1" << endl;
    }

    File.close();

    return 0;
}


int main() {

    CData0 a(7);

    fillInputFile(10);

    Factory* f[] = {new FactoryCData0, new FactoryCData1};
    CStack** arr = nullptr;
    size_t n = 0;
    size_t LMax = 2;

    FILE *in = fopen("input.txt", "r");
    char* str = nullptr;

    try {
        while (mygets(str, LMax, in)) {
            CStack** tmp = new CStack*[n + 1];
            if (arr) {
                memmove(tmp, arr, sizeof(void *) * n);
                delete [] arr;
            }
            arr = tmp;
            arr[++n - 1] = CStack::create_by_Factory(str, f);
        }
        cout << "--------------------" << endl;
        for (size_t i = 0; i < n; ++i)
            arr[i]->output();

        cout << "The output to output-files was made" << endl;
        // cyclic shift testing
        cout << "--------------------" << endl;
        cout << "Cyclic shift testing" << endl;
        cout << "Before cyclic shift:" << endl;
        for (size_t i = 0; i < n; ++i)
            cout << *arr[i] << endl;

        // cyclic shift without omp
        CData0 tmpObj1(*arr[0]);

        auto start = chrono::system_clock::now();

        for (size_t i = 0; i < n - 1; ++i)
            *arr[i] = *arr[i + 1];
        *arr[n - 1] = tmpObj1;
        
        cout << "After cyclic shift w/o omp:" << endl;
        for (size_t i = 0; i < n; ++i)
            cout << *arr[i] << endl;

        auto end = chrono::system_clock::now();
        int elapsed_ms = static_cast<int>
        (chrono::duration_cast<chrono::milliseconds>(end - start).count() );
        cout << "\nTime for cyclic shift without omp constructions = " << elapsed_ms <<" ms\n";

        // cyclic shift WITH omp
        CData0 tmpObj11(*arr[0]), tmpObj2(*arr[n / 4]), tmpObj3(*arr[n / 2]), tmpObj4(*arr[(3 * n) / 4]); // last elements of subArrays

	cout << "tmpObj11 = " << tmpObj11 << endl;
	cout << "tmpObj2 = " << tmpObj2 << endl; 
	cout << "tmpObj3 = " << tmpObj3 << endl; 
	cout << "tmpObj4 = " << tmpObj4 << endl; 

        start = chrono::system_clock::now();

#pragma omp parallel sections
        {
#pragma omp section
            {
                for (size_t i = 0; i < n / 4 - 1; ++i)
                    *arr[i] = *arr[i + 1];
                *arr[n / 4 - 1] = tmpObj2;
            }
#pragma omp section
            {
                for (size_t i = n / 4; i < n / 2 - 1; ++i)
                    *arr[i] = *arr[i + 1];
                *arr[n / 2 - 1] = tmpObj3;
            }
#pragma omp section
            {
                for (size_t i = n / 2; i < (3 * n) / 4 - 1; ++i)
                    *arr[i] = *arr[i + 1];
                *arr[(3 * n) / 4 - 1] = tmpObj4;
            }
#pragma omp section
            {
                for (size_t i = (3 * n) / 4; i < n - 1; ++i)
                    *arr[i] = *arr[i + 1];
                *arr[n - 1] = tmpObj11;
            }
        }

        end = chrono::system_clock::now();
        elapsed_ms = static_cast<int>
        (chrono::duration_cast<chrono::milliseconds>(end - start).count() );
        cout << "Time cyclic shift WITH omp constructions = " << elapsed_ms <<" ms\n";
        cout << "--------------------" << endl;

//        cout << "After:" << endl;
//        for (size_t i = 0; i < n; ++i)
//            cout << *arr[i] << endl;
//        cout << "--------------------" << endl;

//        for (size_t i = 0; i < n; ++i)
//            arr[i]->output();
//        cout << "The output to output-files was made" << endl;
//        cout << "--------------------" << endl;

        // Decrementing testing
//        cout << "Decrementing testing" << endl;
//        cout << "Original object: " << *arr[4] << endl;
//        cout << "Object after prefix decrementing: " << --(*arr[4]) << endl;

//        cout << "Original object: " << (*arr[4])-- << endl;
//        cout << "Object after postfix decrementing: " << *arr[4] << endl;

        // Incrementing testing
//        cout << "\nIncrementing testing" << endl;
//        cout << "Original object: " << *arr[4] << endl;
//        cout << "Object after prefix incrementing: " << ++(*arr[4]) << endl;

//        cout << "Original object: " << (*arr[4])++ << endl;
//        cout << "Object after postfix incrementing: " << *arr[4] << endl;

        // operator + testing
//        cout << "--------------------" << endl;
//        cout << "Operator + testing" << endl;
//        cout << (*arr[4]) << " + " <<  (*arr[1]) << " = ";
//        cout << (*arr[4]) + (*arr[1]) << endl;
//        cout << (*arr[0]) << " + " <<  (*arr[2]) << " = ";
//        cout << (*arr[0]) + (*arr[2]) << endl;
//        cout << (*arr[3]) << " + " <<  (*arr[4]) << " = ";
//        cout << (*arr[3]) + (*arr[4]) << endl;
//        cout << "--------------------" << endl;

        // adding objects in sum
        cout << "Sum searching testing\n\n";

        // sum searching without omp construction
        CData0 sum(0);

        start = chrono::system_clock::now();

        for (size_t i = 0; i < n; ++i)
            sum = sum + *arr[i];

        end = chrono::system_clock::now();
        elapsed_ms = static_cast<int>
        (chrono::duration_cast<chrono::milliseconds>(end - start).count() );
        cout << "Time for searching the sum without omp constructions = " << elapsed_ms <<" ms\n";
        cout << "sum afer adding WO omp = " << sum << endl;

        // sum searching WITH omp construction
        CData0 result_sum_W_omp(0);

        CData0 sum1(0);
        CData0 sum2(0);
        CData0 sum3(0);
        CData0 sum4(0);

        start = chrono::system_clock::now();

#pragma omp parallel sections
        {
#pragma omp section
            {
                for (size_t i = 0; i < n / 4; ++i)
                    sum1 = sum1 + *arr[i];
            }
#pragma omp section
            {
                for (size_t i = n / 4; i < n / 2 ; ++i)
                    sum2 = sum2 + *arr[i];
            }
#pragma omp section
            {
                for (size_t i = n/2; i < (3 * n) / 4; ++i)
                    sum3 = sum3 + *arr[i];
            }
#pragma omp section
            {
                for (size_t i = (3 * n) / 4; i < n; ++i)
                    sum4 = sum4 + *arr[i];
            }
        }
        result_sum_W_omp = sum1 + sum2 + sum3 + sum4;

        end = chrono::system_clock::now();
        elapsed_ms = static_cast<int>
        (chrono::duration_cast<chrono::milliseconds>(end - start).count() );
        cout << "Time for searching the sum WITH omp constructions = " << elapsed_ms <<" ms\n";
        cout << "sum afer adding WITH omp = " << result_sum_W_omp << endl;
        cout << "--------------------" << endl;
        
        cout << "After cyclic shift:" << endl;
        for (size_t i = 0; i < n; ++i)
            cout << *arr[i] << endl;

    }
    catch (int i) {
        if (i == -1) {
            cout << "Incorrect type(string " << n << ")\n";
        }
        if (i == -2) {
            cout << "Incorrect data(string " << n << ")\n";
        }
        arr[n - 1] = nullptr;
        delete [] str;
    }

    for (size_t i = 0; i < n; ++i)
        delete arr[i];
    delete [] arr;
    for (size_t i = 0; i < 2; ++i)
        delete f[i];

    fclose(in);
    return 0;
}
