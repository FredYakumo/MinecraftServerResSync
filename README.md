# ServerResourceSync
A Server fIle resources synchronous tool with a server program and a client program

# How to use
Server load and use `config.yaml` to locate managed file list.

Mange resource config:
class -> file path list

`config.yaml` example:
```yaml
host: 0.0.0.0
port: 23333
thread_count: 8
outbound_speed_limit_kb: 255
res:
  - class_name: "main"
    path:
        - "path/to/file_list"
        - "path/to/another/file_list"
  - class_name: "unused"
    path: "path/to/no_used"
```

## Start manage server with cmd line:
```shell
./server/server -c config.yaml
```

# How does server work
When server starts, server load resources class list and others configuration from config settings, and calculates all resources hash value(sha1) then stored in memory.
with api: /getResourceList, return all resource classes information.
with websocket api: /fetchRes, client send data with `SyncData` protocol to server, server answer with same protocol data.
`SyncData` protocol contains the informations about:
- estimated file list, and hash
- global progress
- current file
- data block
