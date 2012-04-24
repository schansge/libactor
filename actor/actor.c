#include "actor.h"

// spawn process
actor_process_id_t actor_process_spawn(actor_node_t node,
    actor_process_function_t function) {
    // check for valid node
    if (node == NULL) {
        return -1;
    }

    // create process
    actor_process_t process = actor_process_create(node);

    // check for success
    if (process == NULL) {
        return -1;
    }

    // get dispatch queue
    dispatch_queue_t dispatch_queue = dispatch_get_global_queue(
        DISPATCH_QUEUE_PRIORITY_HIGH, 0);

    // check for success
    if (dispatch_queue == NULL) {
        // cleanup
        actor_process_release(process);

        return -1;
    }

    // invoke new procces
    dispatch_async(dispatch_queue, ^ {
            // call process kernel
            function(process);

            // cleanup process
            actor_process_release(process);
        });

    return process->pid;
}

// message sendig
actor_message_t actor_message_send(actor_process_t process, actor_node_id_t node_id,
    actor_process_id_t dest_id, actor_message_data_t const data,
    actor_size_t size) {
    // check input
    if ((process == NULL) || (data == NULL)) {
        return NULL;
    }

    // create message
    actor_message_t message = actor_message_create(data, size);

    // check success
    if (message == NULL) {
        return NULL;
    }

    // set message destination
    message->destination = dest_id;

    // destination message queue
    actor_message_queue_t queue = NULL;

    // check node if
    if (node_id == process->node->nid) {
        // get message queue
        queue = actor_node_message_queue_get(process->node, dest_id);

        // check success
        if (queue == NULL) {
            // release message
            actor_message_release(message);

            return NULL;
        }
    }
    else {
        // get remote node message queue
        queue = actor_node_message_queue_get(process->node,
            process->node->remote_nodes[node_id]);

        // check success
        if (queue == NULL) {
            // release message
            actor_message_release(message);

            return NULL;
        }
    }

    // enqueue message
    return actor_message_queue_put(queue, message);
}

// message receive
actor_message_t actor_message_receive(actor_process_t process,
    actor_time_t timeout) {
    // check for correct input
    if ((process == NULL) || (timeout < 0.0)) {
        return NULL;
    }

    // get message
    actor_message_t message = actor_message_queue_get(process->message_queue, timeout);

    // check success
    if (message == NULL) {
        return NULL;
    }

    return message;
}
