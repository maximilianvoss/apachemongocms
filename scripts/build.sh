#!/bin/bash
#

cd ..; cmake -G "Unix Makefiles" && sudo make install && sudo service apache2 restart || apachectl configtest