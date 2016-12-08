//
// Created by florian on 12/1/16.
//

#include "acu/correlation.h"
#include "acu/lattice.h"
struct lattice {}
struct pattern {}

class LatticeCorrelation: public Correlation{
        int thresholds[] = [10];
        string topic = "/acu/scans";
        struct dbInfos = 0;
        compress_lattice(lattice, threshold);
    public:
        void Invoke(struct alerts){
            // create returned list
            patterns = [];
            // Alle lattices indiziert nach sourceIP
            lattice_ip = Aggregation.get()
                for(IncomingAlert alert: alerts){
                    ip = IncomingAlert.sourceIP;
                    if(!lattice_ip.find(ip)){
                        // create lattice with ip
                        lattice_ip = Lattice(ip);
                    }
                    for(int i = 0; i < 8; i++){
                        pattern_ip = Lattice(alert) ;
                    }
                }
            // filtering process
            // mining significant pattern instances
            for(all lattice_ip in Lattice){
                for(each pattern in lattice_ip){
                    if(pattern.support < thresholds[0]){
                        pattern.delete;
                    }
                }
            }
            // filtering redundant pattern instances
            // init non-redundant significant pattern instance set
            for(all lattice_ip in Lattice){
                // compress revised Lattice lattice_ip using threshold
                // ???
                patterns += compress_lattice(lattice_ip, thresholds[0]);
            }
        return patterns;
        }
        
}
