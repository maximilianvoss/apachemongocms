# Remarks

##Assets

**Upload of assets**
```
$ curl -i -X POST -b "tokenId=[token]" --upload-file [filename] http://[host]/dam/
$ curl -i -X PUT -b "tokenId=[token]" --upload-file [filename] http://[host]/dam/
```

**Get image**
```
$ curl -X GET http://[host]/dam/[imageid].[renditionname].png
$ curl -X GET http://[host]/dam/[imageid].[renditionname].jpg
```

**Get image property**
```
$ curl -X GET http://[host]/dam/[imageid].json
```

**Update image property**
```
$ curl -X POST -b "tokenId=[token] -d "[key]=[value]" http://[host]/dam/[imageid].json
```

##User

**Register User**
```
$ curl -i -X POST -d "username=user&password=password" http://[host]/user/register.json
```

**Login**
```
$ curl -i -X POST -d "username=user&password=password" http://[host]/user/login.json
```

**Logout**
```
$ curl -i -X POST -b "tokenId=[token]" http://[host]/user/logout.json
```

**Unregister User**
```
$ curl -i -X POST -b "tokenId=[token]" -d "username=user&password=password" http://[host]/user/unregister.json
```

**Get Profile**
```
$ curl -i -X GET -b "tokenId=[token]" http://[host]/user/profile.json
```

**Set Profile**
```
$ curl -i -X POST -d "key=value" -b "tokenId=[token]" http://[host]/user/profile.json
```
