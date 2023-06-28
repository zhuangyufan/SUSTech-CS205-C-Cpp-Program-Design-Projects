#include "dataBlob2.h"


DataBlob::DataBlob(int rows, int cols, int channels, DataType type) :
    rows_(rows),
    cols_(cols),
    channels_(channels),
    type_(type),
    data_(nullptr) {
    allocateMemory();
}

DataBlob::DataBlob(const DataBlob& other) :
    rows_(other.rows_),
    cols_(other.cols_),
    channels_(other.channels_),
    type_(other.type_),
    data_(other.data_) {}

DataBlob::~DataBlob() {
    // cout << "Destructor()" << endl << endl;
}

int DataBlob::getDataTypeSize(DataType type) {
    switch (type) {
        case DataType::UINT8:
            return 1;
        case DataType::INT16:
            return 2;
        case DataType::INT32:
        case DataType::FLOAT:
            return 4;
        case DataType::DOUBLE:
            return 8;
        default:
            return 0;
    }
}

void DataBlob::allocateMemory() {
    int byteSize = getByteSize();
    if (byteSize > 0) {
        data_ = std::shared_ptr<unsigned char[]>(new unsigned char[byteSize]{});
    }
}

void DataBlob::deallocateMemory() {
    if (data_ != nullptr) {
        delete[] data_.get();
        data_ = nullptr;
    }
}

bool DataBlob::setElement(int index, double value) {
    if (index >= rows_ * cols_ * channels_){
        cout << "Error: index out of bound." << endl;
        return false;
    }
    if (data_.use_count() > 1){ // allocate new memory
        std::shared_ptr<unsigned char[]> data_tmp(new unsigned char[getByteSize()]{});              
        memcpy(data_tmp.get(), data_.get(), getByteSize());
        data_ = data_tmp;
        data_tmp = NULL;
    }
    switch (type_) {
        case DataType::UINT8:
            data_[index] = static_cast<unsigned char>(value);
            break;
        case DataType::INT16:
            *reinterpret_cast<short*>(data_.get() + index * 2) = static_cast<short>(value);
            break;
        case DataType::INT32:
            *reinterpret_cast<int*>(data_.get() + index * 4) = static_cast<int>(value);
            break;
        case DataType::FLOAT:
            *reinterpret_cast<float*>(data_.get() + index * 4) = static_cast<float>(value);
            break;
        case DataType::DOUBLE:
            *reinterpret_cast<double*>(data_.get() + index * 8) = value;
            break;
        default:
            break;
    }
    return true;
}

double DataBlob::getElement(int index) const {
    if (index >= rows_ * cols_ * channels_){
        cout << "Error: index out of bound." << endl;
        return 0;
    }
        switch (type_) {
        case DataType::UINT8:
            return data_[index];
        case DataType ::INT16:
            return *reinterpret_cast<short*>(data_.get() + index * 2);
        case DataType::INT32:
            return *reinterpret_cast<int*>(data_.get() + index * 4);
        case DataType::FLOAT:
            return *reinterpret_cast<float*>(data_.get() + index * 4);
        case DataType::DOUBLE:
            return *reinterpret_cast<double*>(data_.get() + index * 8);
        default:
            return 0;
    }
}

DataBlob& DataBlob::operator=(const DataBlob& other) {
    if (this != &other) {
        rows_ = other.rows_;
        cols_ = other.cols_;
        channels_ = other.channels_;
        type_ = other.type_;
        data_ = other.data_;
    }
    return *this;
}

DataBlob DataBlob::operator+(const DataBlob& other) const {
    if (rows_ != other.rows_ || cols_ != other.cols_ || channels_ != other.channels_) {
        cout << "Error: unequal size." << endl;
        return DataBlob(0, 0, 0, type_);
    }
    if (type_ != other.type_){
        cout << "Warning: Forced type conversion. Possible loss of precision." << endl;
    }
    DataBlob result(rows_, cols_, channels_, type_);
    int numElements = rows_ * cols_ * channels_;
    for (int i = 0; i < numElements; ++i) {
        result.setElement(i, getElement(i) + other.getElement(i));
    }
    return result;
}

DataBlob DataBlob::operator-(const DataBlob& other) const {
    if (rows_ != other.rows_ || cols_ != other.cols_ || channels_ != other.channels_) {
        cout << "Error: unequal size." << endl;
        return DataBlob(0, 0, 0, type_);
    }
    if (type_ != other.type_){
        cout << "Warning: Forced type conversion." << endl;
    }
    DataBlob result(rows_, cols_, channels_, type_);
    int numElements = rows_ * cols_ * channels_;
    for (int i = 0; i < numElements; ++i) {
        result.setElement(i, getElement(i) - other.getElement(i));
    }
    return result;
}

DataBlob DataBlob::operator*(const DataBlob& other) const {
    if (rows_ != other.rows_ || cols_ != other.cols_ || channels_ != other.channels_) {
        cout << "Error: unequal size." << endl;
        return DataBlob(0, 0, 0, type_);
    }
    if (type_ != other.type_){
        cout << "Warning: Forced type conversion." << endl;
    }
    DataBlob result(rows_, cols_, channels_, type_);
    int numElements = rows_ * cols_ * channels_;
    for (int i = 0; i < numElements; ++i) {
        result.setElement(i, getElement(i) * other.getElement(i));
    }
    return result;
}

bool DataBlob::operator==(const DataBlob& other) const {
    if (rows_ != other.rows_ || cols_ != other.cols_ || channels_ != other.channels_ || type_ != other.type_) {
        return false;
    }
    int numElements = rows_ * cols_ * channels_;
    for (int i = 0; i < numElements; ++i) {
        if (getElement(i) != other.getElement(i)) {
            return false;
        }
    }
    return true;
}

std::ostream & operator<<(std::ostream & os, const DataBlob & d){
        os << "size (" << d.rows_ << "x" << d.cols_ << ")" << std::endl;
        for (size_t r = 0; r < d.rows_; r++)
        {
            for(size_t c = 0; c < d.cols_; c++)
                os << d.getElement(r * d.cols_ + c) << ", ";
            os << std::endl;
        }
        os << "use_count = " << d.data_.use_count() << endl;
        return os;
}
