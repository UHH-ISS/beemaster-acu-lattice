class Lattice{
        string sourceIP;
        int sourcePort;
        int destPort;
        string protocol;
        // support value for pattern
        int support;
        // generate all patterns for input
        generatePatterns(IncomingAlert);
    public:
        Lattice(IncomingAlert)
        // Pattern 1
        Lattice(string);
        // Pattern 2
        Lattice(string, int);
        // Pattern 3
        Lattice(string, int);
        // Pattern 4
        Lattice(string, string);
        // Pattern 5
        Lattice(string, int, int);
        // Pattern 6
        Lattice(string, int, string);
        // Pattern 7
        Lattice(string, int, string);
        // Pattern 8
        Lattice(string, int, int, string);
}

Lattice::Lattice(string srcIP){
    sourceIP = srcIP;
}
Lattice::Lattice(string srcIP, int srcPrt){
    sourceIP = srcIP;
    sourcePort = srcPrt;
}
Lattice::Lattice(string srcIP, int dstPrt){
    sourceIP = srcIP;
    destPort = dstPrt;
}
Lattice::Lattice(string srcIP, string protocol){
    sourceIP = srcIP;
    protocol = protocol;
}
Lattice::Lattice(string srcIP, int srcPrt, int dstPrt){
    sourceIP = srcIP;
    sourcePort = srcPrt;
    destPort = dstPrt;
}
Lattice::Lattice(string srcIP, int srcPrt, string protocol){
    sourceIP = srcIP;
    sourcePort = srcPrt;
    protocol = protocol;
}
Lattice::Lattice(string srcIP, int dstPrt, string protocol){
    sourceIP = srcIP;
    destPort = dstPrt;
    protocol = protocol
}
Lattice::Lattice(string srcIP, int srcPrt, int dstPrt, string protocol){
    sourceIP = srcIP;
    sourcePort = srcPrt;
    destPort = dstPrt;
    protocol = protocol;
}
Lattice::generatePatterns(IncomingAlert alert){
    Lattice pattern1 = Lattice(alert.sourceIP);
    Lattice pattern2 = Lattice(alert.sourceIP, alert.sourcePort);
    Lattice pattern3 = Lattice(alert.sourceIP, alert.destPort);
    Lattice pattern4 = Lattice(alert.sourceIP, alert.protocol);
    Lattice pattern5 = Lattice(alert.sourceIP, alert.sourcePort, alert.destPort);
    Lattice pattern6 = Lattice(alert.sourceIP, alert.sourcePort, alert.protocol);
    Lattice pattern7 = Lattice(alert.sourceIP, alert.destPort, alert.protocol);
    Lattice pattern8 = Lattice(alert.sourceIP, alert.sourcePort, alert.destPort, alert.protocol);
}
Lattice::Lattice(IncomingAlert alert){
    generatePatterns(alert)
}
