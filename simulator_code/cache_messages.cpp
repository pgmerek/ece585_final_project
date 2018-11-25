/* ECE 485/585 Fall Term 2018 Final Project
 * Split L1 Cache simulation, line class functions
 * Members: Patrick Gmerek, Stephen Poanessa, Emma Smith, Amanda Voegtlin
 * 4 November 2018
 */

#include "header.h"

cache_messages::cache_messages()
{
    head = NULL;
}

cache_messages::~cache_messages()
{
    if (head)
    {
        delete_messages(head);
        head = NULL;
    }
}

void cache_messages::delete_messages()
{
    if (head)
    {
        delete_messages(head);
        head = NULL;
    }
}

void cache_messages::delete_messages(node * head)
{
    // Stopping condition, prevents recursively calling too far
    if (!head)
        return;

    // Recursive call
    delete_messages(head->next);
    if (head->message)  // If there is a message stored,
    {
        delete [] head->message;   // Delete it
        head->message = NULL;   // Set to null
    }
    delete head;    // Finally, delete head
    head = NULL;
}

void cache_messages::add_message(char * msg)
{
    if (!head)  // If this is the first message....
    {
        head = new node;    // Make a new node at head
        // Make sure to set the next pointer to null
        head->next = NULL;

        // Copy message into dynamically allocated array
        head->message = new char[strlen(msg) + 1];
        strcpy(head->message, msg);
    }
    else    // If this is not the first message....
    {
        node * temp = new node;     // Make a new node
        temp->next = head;          // Make the next pointer point to head
        head = temp;                // Update head

        // Copy message into dynamically allocated array
        head->message = new char[strlen(msg) + 1];
        strcpy(head->message, msg);
    }
}

void cache_messages::display_messages() const
{
    if (head)   // Only display messages if there are any to print
        display_messages(head);
}

void cache_messages::display_messages(node * head) const
{
    // Stopping condition, prevents recursively calling too far
    if (!head)
        return;

    // Recursive call
    display_messages(head->next);

    // Do this after recursive call since we want the messages to be displayed in reverse order.
    // We want the messages in reverse order because we add at head
    if (head->message)  // If there is a message stored, print it
        printf("%s\n", head->message);
}

