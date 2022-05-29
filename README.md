# uttu
---
Written to streamline networking within openconcord/core, with 'raw' sockets.

**a note about compatibility**

uttu has (thus far) only been tested on linux systems. If you wish to contribute, we're always looking to increase our platform to windows and OSx. Any platform specific code should be implemented in `src/api/<platform>`, while includes should be placed in the precompile headers of `uttu.hpp`.

#### dependencies / notes

given that (eventually), all OC libraries aren't going to be hosted on the same platform (git-\*), it makes sense to keep some log within the git repo itself.

- All current **known and library wide** issues are stored in [bounty](docs/bounty.txt) 
- All intended features / the current roadmap are/is stored in [roadmap](docs/roadmap.txt)

uttu relies on the same suite of dependencies as core; ~~nholmann/json and~~ crypto++.
