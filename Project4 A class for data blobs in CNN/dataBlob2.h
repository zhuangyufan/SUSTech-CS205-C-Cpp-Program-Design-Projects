#include <iostream>
#include <cstring>
#include <memory>
using namespace std;

enum class DataType {
    UINT8,
    INT16,
    INT32,
    FLOAT,
    DOUBLE
};

class DataBlob {
public:
    DataBlob(int rows, int cols, int channels, DataType type);
    DataBlob(const DataBlob& other);
    ~DataBlob();

    int getRows() const { return rows_; }
    int getCols() const { return cols_; }
    int getChannels() const { return channels_; }
    int getNumElements() const {return rows_*cols_*channels_; }
    DataType getType() const { return type_; }
    int getByteSize() const { return rows_ * cols_ * channels_ * getDataTypeSize(type_); }

    bool setElement(int index, double value);

    double getElement(int index) const;

    DataBlob& operator=(const DataBlob& other);
    DataBlob operator+(const DataBlob& other) const;
    DataBlob operator-(const DataBlob& other) const;
    DataBlob operator*(const DataBlob& other) const;
    bool operator==(const DataBlob& other) const;
    friend std::ostream & operator<<(std::ostream & os, const DataBlob & d);

private:
    static int getDataTypeSize(DataType type);
    void allocateMemory();
    void deallocateMemory();

    int rows_;
    int cols_;
    int channels_;
    DataType type_;
    shared_ptr<unsigned char[]> data_;
};
