# Installation

## Pre-requisite installation

Starting Point is a Ubuntu 16.04 LTS Server version

**Install SSH-Server**
```
apt-get install openssh-server
```

**Install latest MongoRelease**
See: [https://docs.mongodb.com/master/tutorial/install-mongodb-on-ubuntu/?_ga=1.113818695.117372036.1469974004]
```
# apt-key adv --keyserver hkp://keyserver.ubuntu.com:80 --recv EA312927
# echo "deb http://repo.mongodb.org/apt/ubuntu xenial/mongodb-org/3.2 multiverse" | sudo tee /etc/apt/sources.list.d/mongodb-org-3.2.list
# apt-get update
# apt-get install mongodb-org
```

Paste Content to: */lib/systemd/system/mongod.service*
```
[Unit]
Description=High-performance, schema-free document-oriented database
After=network.target
Documentation=https://docs.mongodb.org/manual

[Service]
User=mongodb
Group=mongodb
ExecStart=/usr/bin/mongod --quiet --config /etc/mongod.conf

[Install]
WantedBy=multi-user.target
```

**Start the MongoDB**

```
# service mongod start
```

**Install Apache2 with development dependencies**
```
# apt-get install apache2 apache2-dev
```

**Install CMake**
```
# apt-get install cmake
```

**Include OpenSSL**
```
# apt-get install libssl-dev
```

**Install G++**
```
# apt-get install g++
```

**Install ImageMagick**
```
# apt-get install imagemagick libmagickwand-6-headers libmagick++-6.q16-dev
```

**Install BSON Library**
```
# apt-get install curl
# curl -O -L https://github.com/mongodb/libbson/releases/download/1.3.5/libbson-1.3.5.tar.gz
# tar -xvzf libbson-1.3.5.tar.gz
# cd libbson-1.3.5/
# ./configure
# make
# make install
```

**Install mongo-c-driver**
```
# curl -L -O https://github.com/mongodb/mongo-c-driver/releases/download/1.3.5/mongo-c-driver-1.3.5.tar.gz
# tar -xvzf mongo-c-driver-1.3.5.tar.gz
# cd mongo-c-driver-1.3.5/
# ./configure
# make
# make install
```

## Prepare the module

**Change LDConfig paths to ensure modules can be loaded**

Add following lines to: */etc/ld.so.conf.d/mongocms.conf*
```
# mongo cms modules
/usr/local/mongocms/lib

# ldconfig
```

**Create paths where everything is going to happen**
```
# mkdir -p /var/www/html/assetstore
# mkdir -p /var/www/html/dam
# mkdir -p /var/www/html/user
# chown -R www-data.www-data /var/www/html/*
```

**Add the module to the Apache2**
```
# cd /etc/apache2/mods-available/
# ln -sf ~cms/mongocms/httpconfig/* .
# a2enmod mongocms
# service apache2 restart
```

**Adjustment of apache2.conf**
```
<Directory /var/www/html>
        SetHandler mongocms
</Directory>
```

## Compile Code
```
$ cd ~/mongocms/scripts/
$ ./build.sh
```