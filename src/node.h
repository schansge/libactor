#ifndef NODE_H
#define NODE_H

#include <dispatch/dispatch.h>
#include "message.h"

// size
typedef unsigned int actor_size_t;

// node id
typedef unsigned int actor_node_id_t;

// message queue usage
typedef bool actor_node_message_queue_usage_t;

// node struct
typedef struct {
    actor_node_id_t nid;
    actor_message_queue_t message_queues;
    actor_size_t message_queue_count;
    actor_size_t message_queue_pos;
    actor_node_message_queue_usage_t* message_queue_usage;
    dispatch_semaphore_t process_semaphore;
} actor_node_struct;
typedef actor_node_struct* actor_node_t;

// create node
actor_node_t actor_node_create(actor_node_id_t id, actor_size_t size);

// include process here, to make shure node struct is defined
#include "process.h"

// spawn new process
actor_process_id_t actor_process_spawn(actor_node_t node,
    actor_process_function_t function);

// get free message queue
actor_message_queue_t actor_node_message_queue_get_free(
    actor_node_t node, actor_process_id_t* id);

// get message queue for id
actor_message_queue_t actor_node_message_queue_get(actor_node_t node,
    actor_process_id_t pid);

// release message queue
void actor_node_message_queue_release(actor_node_t node, actor_process_id_t pid);

// cleanup
void actor_node_cleanup(actor_node_t node);
void actor_node_release(actor_node_t node);

#endif
