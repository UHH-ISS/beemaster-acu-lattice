//
// Created by florian on 12/8/16.
//
#include <rocksdb/db.h>
//#include <rocksdb/merge_operator.h>
#include <acu/acu.h>
#include <acu/threshold.h>
#include <iostream>
#include <cstdio>

int main() {
    acu::Threshold* a = new acu::Threshold(1, "field1", "val1");
    rocksdb::DB* db;
    rocksdb::Options options;
    options.create_if_missing = true;
    rocksdb::Status s = rocksdb::DB::Open(options, "/tmp/test", &db);
    //std::vector<acu::Threshold> v = {}
    auto f = rocksdb::Slice((char*) a, sizeof(acu::Threshold));
    std::string b = "val1";
    std::string c = "val2";
    s = db->Put(rocksdb::WriteOptions() , "key1", b); 
    assert(s.ok());
    //s = db->Put(rocksdb::WriteOptions() , "key2", c);
    s = db->Put(rocksdb::WriteOptions(), "key3", f);
    assert(s.ok());
    //delete a;
    rocksdb::Iterator* it = db->NewIterator(rocksdb::ReadOptions());
    for (it->SeekToFirst(); it->Valid(); it->Next()) {
        std::cout << it->key().ToString() << ": " << it->value().ToString(true) << std::endl;
    }
    delete it;
    std::string val;
    db->Get(rocksdb::ReadOptions(), "key3", &val);
    
    acu::Threshold* t1 = (acu::Threshold*) rocksdb::Slice(val).data();
    printf("%s | %s | %d\n", t1->field_name.c_str(), t1->value.c_str(), t1->count);

}
