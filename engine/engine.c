#include <zmq.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include "search.pb-c.h"
#define MAX_MSG_SIZE 1024

int dsq_engine(void) {

    void *context = zmq_ctx_new ();
    void *responder = zmq_socket (context, ZMQ_REP);
    int rc = zmq_bind (responder, "tcp://*:3773");
    assert (rc == 0);

    while (1) {
        char buffer [MAX_MSG_SIZE];
        int msg_len = zmq_recv (responder, buffer, MAX_MSG_SIZE, 0);
        printf ("Received message of length %s\n", msg_len);

        SearchPosition *msg;

        // Read packed messa
        // Unpack the message using protobuf-c.
        msg = search_position__unpack (NULL, msg_len, buffer);   
        if (msg == NULL)
        {
          fprintf (stderr, "Error unpacking incoming message\n");
          exit (1);
        }

        printf ("Unpacked a message!\n");
        sleep (1);          //  Do some 'work'
        zmq_send (responder, "Got it!", 7, 0);
    }
    return 0;
}