#include "dataBlob2.h"


int main() {
    // Create a DataBlob object with 2 rows, 3 columns, and 1 channel of type float
    DataBlob blob1(2, 3, 1, DataType::FLOAT);

    // Fill the blob with some values
    for (int i = 0; i < blob1.getNumElements(); ++i) {
        blob1.setElement(i, i * 0.5f);
    }

    // Print the values of the blob
    std::cout << "Blob 1:\n";
    std::cout << blob1 << endl;

    // Create another DataBlob object with the same dimensions and type
    DataBlob blob2(2, 3, 1, DataType::FLOAT);

    // Fill the blob with some values
    for (int i = 0; i < blob2.getNumElements(); ++i) {
        blob2.setElement(i, i * 1.5f);
    }

    // Print the values of the blob
    std::cout << "Blob 2:\n";
    std::cout << blob2 << endl;

    // Test the assignment operator
    blob1 = blob2;
    std::cout << "Blob 1 after assignment:\n";
    std::cout << blob1 << endl;

    // Test changing the value
    blob1.setElement(0,1);
    std::cout << "Blob 1 after changing one value:\n";
    std::cout << blob1 << endl;
    std::cout << "Blob 2 after blob1 changed one value:\n";
    std::cout << blob2 << endl;

    // Test the addition operator
    DataBlob blob3 = blob1 + blob2;
    std::cout << "Blob 3 (sum of Blob 1 and Blob 2):\n";
    std::cout << blob3 << endl;

    // Test the subtraction operator
    DataBlob blob4 = blob3 - blob2;
    std::cout << "Blob 4 (difference between Blob 3 and Blob 2):\n";
    std::cout << blob4 << endl;

    // Test the multiplication operator
    DataBlob blob5 = blob4 * blob1;
    std::cout << "Blob 5 (product of Blob 4 and Blob 1):\n";
    std::cout << blob5 << endl;

    // Test the comparison operator
    bool isEqual = blob5 == blob5;
    std::cout << "Blob 5 is equal to itself: " << isEqual << "\n" << endl;

    // Create a DataBlob object with 2 rows, 3 columns, and 1 channel of type int16
    DataBlob blob6(2, 3, 1, DataType::INT16);
    
    // Fill the blob with some values
    for (int i = 0; i < blob6.getNumElements(); ++i) {
        blob6.setElement(i, i * 2);
    }
    
    // Print the values of the blob
    std::cout << "Blob 6:\n";
    std::cout << blob6 << endl;

    // Test the addition operator (int + float)
    std::cout << "Blob 7 (sum of Blob 6 and Blob 1):\n";
    DataBlob blob7 = blob6 + blob1;
    std::cout << blob7 << endl;

    // Test the addition operator (float + int)
    std::cout << "Blob 8 (sum of Blob 1 and Blob 6):\n";
    DataBlob blob8 = blob1 + blob6;
    std::cout << blob8 << endl;

    return 0;
}
