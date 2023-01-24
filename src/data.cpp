#include "data.hpp"
#include "exception.hpp"

//lString

//to-do

//dataArray

uint8_t* llcd::data::dataArray::getData(){
    return data;
};
uint32_t llcd::data::dataArray::getSize(){
    return size;
};
llcd::data::dataArray::dataArray(){
    this->size = 0;
    this->data = nullptr;
}
llcd::data::dataArray::dataArray(const char* filename){
    //to-do
};
llcd::data::dataArray::dataArray(uint8_t* data,uint32_t size){
    this->size = size;
    if(this->size==0)this->data = nullptr;
    else{
        this->data = new uint8_t[size];
        for(uint32_t i = 0;i<size;i++){
            this->data[i] = data[i];
        }
    }
    
};
llcd::data::dataArray::dataArray(dataArray& data){
    this->size = data.size;
    if(this->size==0)this->data = nullptr;
    else{
        this->data = new uint8_t[this->size];
        for(uint32_t i = 0;i<this->size;i++){
            this->data[i] = data.data[i];
        }
    }
};
llcd::data::dataArray::~dataArray(){
    if(this->data==0||this->data==nullptr)return;
    else{
        delete [] this->data;
    }
};
void llcd::data::dataArray::save(const char* filename){
    //to-do
};
void llcd::data::dataArray::reSize(uint32_t size){
    if(this->size==size)return;
    
    if(size==0){
        if(this->data==nullptr)return;
        
        delete [] this->data;
        this->data = nullptr;

        return;
    }
    uint8_t* newData = new uint8_t[size];
    for(uint32_t i = 0;i<size;i++){
        if(this->size<i)newData[i] = this->data[i];
        else newData[i] = 0;
    }
    delete [] this->data;
    this->data = newData;
};
void llcd::data::dataArray::push(uint8_t value){
    reSize(this->size+1);
    data[this->size-1] = value;
};
bool llcd::data::dataArray::getBit(uint32_t place,uint8_t bit){
    if(place>=this->size)return false;
    return bool((this->data[place]>>(bit&0b00000111))&0b00000001);
};
uint8_t& llcd::data::dataArray::operator[](uint32_t place){
    if(place>=this->size)throw llcd::exception(e_outFoBounce_error,"you are trying to access value that is not inside this dataArray");
    return this->data[place];
};

//helper function

uint8_t llcd::data::staticDSize(uint32_t value){
    if(value<2)return 1;
    else{
        uint32_t candidate = 2;
        uint32_t possible = 4;
        while (true)
        {
            if(value<possible)return candidate;
            candidate++;
            possible<<=1;
        }
        
    }
}

//PbView

llcd::data::PbView::PbView(dataArray& data):data(data.getData()){
    reset();
};
llcd::data::PbView::PbView(uint8_t* data):data(data){
    reset();
};
void llcd::data::PbView::reset(){
    place = 0;
    bit = 0;
    nowValue = data[place];
};
bool llcd::data::PbView::readBit(){
    bit>>=1;
    if(bit==0){
        place++;
        nowValue = data[place];
        bit = 0b10000000;
    }
    return nowValue&(bit);
};
uint32_t llcd::data::PbView::readDynamic(){
    uint32_t out = 1;
    while (readBit())
    {
        //increment until get 0 bit
        out++;
    }
    return out;
    
};
uint32_t llcd::data::PbView::readStatic(uint8_t size){
    uint32_t out = 0;
    for(uint8_t i = 0;i<size;i++){
        out<<=1;
        out|=readBit();
    }
    return out;
};

//PbReader

llcd::data::PbReader::PbReader(dataArray& data):data(data){
    reset();
};
llcd::data::PbReader::PbReader(dataArray data):data(data){
    reset();
};
void llcd::data::PbReader::reset(){
    place = 0;
    bit = 0;
    nowValue = data[place];
};
bool llcd::data::PbReader::readBit(){
    bit>>=1;
    if(bit==0){
        place++;
        nowValue = data[place];
        bit = 0b10000000;
    }
    return nowValue&(bit);
};
uint32_t llcd::data::PbReader::readDynamic(){
    uint32_t out = 1;
    while (readBit())
    {
        //increment until get 0 bit
        out++;
    }
    return out;
    
};
uint32_t llcd::data::PbReader::readStatic(uint8_t size){
    uint32_t out = 0;
    for(uint8_t i = 0;i<size;i++){
        out<<=1;
        out|=readBit();
    }
    return out;
};

//PbWriter
llcd::data::PbWriter::PbWriter(){
    clear();
};
void llcd::data::PbWriter::clear(){
    data.reSize(0);
    bit=0;
};
void llcd::data::PbWriter::writeBit(bool value){
    bit>>=1;
    if(bit==0){
        bit = 0b10000000;
        data.push(current);
        current = 0;
    }
    if(value)current|=bit;
};
void llcd::data::PbWriter::writeDynamic(uint32_t value){
    for(uint32_t i = 0;i<value-1;i++){
        writeBit(true);
    }
    writeBit(false);
};
void llcd::data::PbWriter::writeStatic(uint32_t value,uint8_t size){
    for(uint8_t i = 0;i<size;i++){
        writeBit((value>>(size-(1+i)))&1);
    }
};
llcd::data::dataArray llcd::data::PbWriter::exportData(){
    dataArray out = data;
    out.push(current);
    return out;
};

//bJson