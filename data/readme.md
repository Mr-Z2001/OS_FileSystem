## format of userdata.csv
```
each line:
<username>,<password_hashedby_sha224>
```

## format of dir.csv
```
each line:
<directoryID>,<directoryName>,<fileNum>,<fileID1>,<fileID2>,...
```

## format of graph.csv
```
each line:
<parentDirectory>,<childDirectory>
```

## format of file.csv
```
each line:
<fileID>,<fileName>
```

## format of nodeInfo.csv
```
each line:
node_id, fd, name, type, length, link_num, create_time, last_modify, parent_id, group_pr.size(), [group_name, pr]+, user_pr.size(), [user_name, pr]+, location.size(), [location]+, link.size(), [link]+
```

## format of tree.csv:
```
first line: 
node_cnt, edge_count
following each line: 
parent_node, child_node
```
