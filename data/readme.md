## format of userdata.csv
```
bid: 0
each line:
<userID>,<username>,<password_hashedby_sha224>
```

## format of userGroupData.csdv
```
bid: 1
each line:
<userGroupID>,<userGroupName>,<userCount>,<userID0>,<userID1>...
```

## format of nodeInfo.csv
```
bid: 2
each line:
node_id, fd, name, type, length, link_num, create_time, last_modify, parent_id, group_pr.size(), [group_name, pr]+, user_pr.size(), [user_name, pr]+, location.size(), [location]+, link.size(), [link]+
```

## format of tree.csv:
```
bid: 3
first line: 
node_cnt, edge_count
following each line: 
parent_node, child_node
```

## format of commands.txt
```
bid: 4
each line:
<command>,<commandID>
```