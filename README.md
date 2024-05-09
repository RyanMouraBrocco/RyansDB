# RyansDB

## What is RyansDB?

This project was designed to be a relational database and handle all connections using its own protocol (The RyansProtocol ([learn more](./src/Protocol/README.md))).

The main goal of this code is to teach (and primarily learn) about how a database operates behind the scenes. The idea here is to be as simple as possible to convey the idea of teaching about databases.

Of course, I would like to add something more unique into the project, and i this database would be a relational database GPU-based


## What do we have ?

We have abstractions of the socket and TCP connections.

## TO DO

- [X] Ensure connection stability even when something in the network goes wrong.
- [X] Begin using the ResultPattern to handle errors more effectively
- [ ] Create a protocol to receive commands and return results.
- [ ] Create a file of configurations.
- [ ] Apply the rule 0-3-5 in all class.