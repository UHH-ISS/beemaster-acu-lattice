#include <acu/storage.h>
#include <acu/incoming_alert.h>
#include "rocksdb/db.h"
#include "rocksdb/options.h"
#include "rocksdb/slice.h"

using namespace rocksdb;
using namespace std;
using namespace acu;

std::string kDBPath = "/tmp/rocks_lattice";

class latticeStorage: public Storage{
        bool initDB(string path){
            DB* db;
            Options options;
            //optimize options recommended by devs
            options.IncreaseParallelism();
            options.OptimizeLevelStyleCompaction();
            // creates DB if its not present
            options.create_if_missing = true;

            // open DB
            Status status = DB::Open(options, path, &db);
            assert(status.ok());
        }
    public:
        bool persist(acu::IncomingAlert alert){
            // Datenstruktur definieren
            Status status = *this->Put(WriteOptions(), alert.srcPort, alert.sourcePort);
            return assert(status.ok());
        }
        struct query(string query){
            // get Data out of DB
            status = *this->Get(ReadOptions(), query, &data)
            // response struct not final. More thoughts to be made.
            return  struct data {
                string sourceIP;
                int sourcePort;
                string destIP;
                int destPort;
                string IncidentType;
                string protocol;
            } ;
        }
        struct[] queryList(string query){
            // get grouped data
        }
}

int main(){
    latticeStorage rocks;
    rocks.initDB(kDBPath);
    return 0;
}
