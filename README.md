# uttu
*~~boost::asio is bloat~~ literally anything other than bytecode is bloat* ~u2on

As such, you should run our bytecode and not ask questions (docs are also bloat)

---
## -- custom socket wrapper for Concord>Core --
Written to streamline networking within concord>core, with raw sockets.

**Basic usage**

`Session`
  - Represent socket sessions; you use them to connect and accept connections.
  - The two main funcitons you'll interact with are `Accept` and `Connect`. Both return a `std::share_ptr` to a newly created `Peer` class representing the connection.
  - You can set the Criteria for accepting new connections based on IP with a void function pointer taking a string to `Critera`
  - At any time you can `Close` the session, cutting off all Peer connections and closing the Socket.

`Peer`
  - An abstract class for interactions with a connected peer.
  - Provides `Read` and `Write` functions.
  - Can close w/ `Close`.
