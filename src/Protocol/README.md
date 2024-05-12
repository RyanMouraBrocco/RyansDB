# What is RyansProtocol

RyansProtocol is the protocol designed to communicate with RyansDB in a normalized way. The protocol itself is based on the HTTP Protocol. ([learn more](https://datatracker.ietf.org/doc/html/rfc2616))

Some questions have been raised about what the protocol should look like:

- [X] Control Version
- [X] A charset config ? yes! always will be utf-8
- [X] A Content-Encoding Such as accepting gzip, compress, etc ? maybe just gzip 
- [X] A Content-Type ? yes! just string
- [X] A custom headers communication ? yes!!!!!

I'm still pondering these questions, but they aren't hindering me from designing the first version of the protocol.

## 1. Request

<p align="center">
  <img src="../../docs/imgs/ryans_protocol_request.png" />
</p>

* **Version:** The protocol version (currently at 1.0.0).
* **CommandType:** The type of the command.
  1. RawQuery
  2. StoredProcedure
* **Headers:** Request headers where we can pass, for example, Content-Encoding.
* **Message:** The actual message, typically containing the SQL query.


## 2. Response
<p align="center">
  <img src="../../docs/imgs/ryans_protocol_response.png" />
</p>
  
* **Version:** The protocol version (currently at 1.0.0).
* **StatusCode:** The status code of the response (Status codes not defined yet).
* **ErrorDescription:** The error description of the status code (when applicable).
* **Headers:** Response headers where we can pass, for example, Content-Encoding.
* **Message:** The response to the query.

## Why aren't you currently using a binary protocol to encapsulate this RyansProtocol?

I haven't implemented it yet because I judged it unnecessary for the current stage. Since we're only transmitting a string (the query) as the message, there wouldn't be a significant improvement from using a binary protocol at this point. 

For me, the biggest advantage of a binary protocol is the ability to transmit smaller representations of integer types using VarInts. This can significantly reduce message length when dealing with integers, booleans, etc. ([learn more about binary encoding](https://protobuf.dev/programming-guides/encoding/))

**However**, this is not a final decision. In the future, I might consider adding binary encoding to improve the transmission of headers, status codes, or even the message itself if it goes beyond just strings.