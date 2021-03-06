
#include "mcbe.h"
#include "readfile.h"

#define NUMTEST 1


int main(int argc, char const * argv[]) {

    if (argc < 1)
    {
        perror("Usage: MCBE datafile");
        exit(-1);
    }

    Set channels[MAX_m];
    int t;     // Number of channels

    readFiles(argv[1], channels, &t);
    printf ("Data file: %s\n", argv[1]);

    MCBE();

    double time1, time2;

    time1 = pbc_get_time();

    Setup();

    time2 = pbc_get_time();

    printf ("Setup (): %fs\n", (time2 - time1)*1000.0 / NUMTEST);


    int i = 1;     // Test with channel j = 0 and user = channel[j][i]
    int j = 0;
    ID ij = {channels[j].elements[i], j};
    skID sk;

    double everage = 0;
    for (int l = 0; l < NUMTEST; ++l) {


        time1 = pbc_get_time();

        Extract(&sk, ij);

        time2 = pbc_get_time();
        everage += (time2 - time1);
    }
    printf ("Extract (): %fms\n", everage*1000.0/NUMTEST);



    element_t Keys[t];
    Header Hdr;

    everage = 0;
    for (int l = 0; l < NUMTEST; ++l) {


        time1 = pbc_get_time();

        Encrypt(Keys, &Hdr, &param, channels, t);

        time2 = pbc_get_time();
        everage += (time2 - time1);
    }

    printf ("Encrypt (): %fms\n", everage*1000.0/NUMTEST);


    element_t Kj;
    element_init_GT(Kj, pairing);

    everage = 0;
    for (int l = 0; l < NUMTEST; ++l) {
        time1 = pbc_get_time();
        Decrypt(Kj, sk, &param, &Hdr, channels, t);
        time2 = pbc_get_time();
        everage += (time2 - time1);

    }
    printf ("Decrypt (): %fms\n", everage*1000.0/NUMTEST);


//    if (fail) {
//        printf("Cannot decrypt Hdr!\n");
//        return 0;
//    }

    /*if (!element_cmp(Keys[j], Kj))
        printf ("Successful!\n");
    else printf("Fail!\n");

    element_printf("Keys[%d]=%B\n", j, Keys[j]);
    element_printf("Kj = %B\n", Kj);*/

    return 0;
}
